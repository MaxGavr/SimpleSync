#include "stdafx.h"
#include "SimpleSync.h"
#include "PreviewListCtrl.h"
#include "dialogs/FilePropertiesDialog.h"
#include "dialogs/CompareDialog.h"


IMPLEMENT_DYNAMIC(CPreviewListCtrl, CMFCListCtrl)

CPreviewListCtrl::CPreviewListCtrl(SyncManager* syncManager)
    : m_syncManager(syncManager)
{
    m_rightArrowImageSmall.Load(IDB_RIGHT_ARROW_SMALL); // ICONS::RIGHT_ARROW
    m_equalImageSmall.Load(IDB_EQUAL_SMALL); // ICONS::EQUAL
    m_leftArrowImageSmall.Load(IDB_LEFT_ARROW_SMALL); // ICONS::LEFT_ARROW
    m_folderImageSmall.Load(IDB_FOLDER_SMALL); // ICONS::FOLDER
    m_removeImageSmall.Load(IDB_REMOVE_SMALL); // ICONS::REMOVE
    m_questionImageSmall.Load(IDB_QUESTION_SMALL); // ICONS::QUESTION

    m_imageList.Create(PREVIEW_LIST_IMAGE_SIZE, PREVIEW_LIST_IMAGE_SIZE,
                       ILC_COLOR32, 0, PREVIEW_LIST_IMAGE_COUNT);
    COLORREF defaultMask = RGB(0, 0, 0);
    m_imageList.Add(&m_rightArrowImageSmall, defaultMask);
    m_imageList.Add(&m_equalImageSmall, defaultMask);
    m_imageList.Add(&m_leftArrowImageSmall, defaultMask);
    m_imageList.Add(&m_folderImageSmall, defaultMask);
    m_imageList.Add(&m_removeImageSmall, defaultMask);
    m_imageList.Add(&m_questionImageSmall, defaultMask);
}

CPreviewListCtrl::~CPreviewListCtrl()
{
}

void CPreviewListCtrl::setupColumns()
{
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT |
                     LVS_EX_SUBITEMIMAGES);
    
    SetImageList(&m_imageList, LVSIL_SMALL);

    InsertColumn(LIST_COLUMNS::INDEX, _T("№"),
                 LVCFMT_RIGHT,
                 PREVIEW_LIST_IMAGE_SIZE);
    InsertColumn(LIST_COLUMNS::SOURCE_FILE, _T("Исходная директория"),
                 LVCFMT_LEFT);
    InsertColumn(LIST_COLUMNS::ACTION, _T(""),
                 LVCFMT_CENTER | LVCFMT_FIXED_WIDTH,
                 PREVIEW_LIST_IMAGE_SIZE);
    InsertColumn(LIST_COLUMNS::DESTINATION_FILE, _T("Конечная директория"),
                 LVCFMT_LEFT);

    adjustColumnsWidth();
}

void CPreviewListCtrl::showPreview()
{
    clearPreview();
    
    SyncManager::OperationQueue operations = m_syncManager->getOperations();
    sortOperationsByFolders(operations);
    
    for (auto& operation : m_sortedOperations)
        printSyncAction(operation);

    adjustColumnsWidth();
}

void CPreviewListCtrl::clearPreview()
{
    DeleteAllItems();
    m_sortedOperations.clear();
}

void CPreviewListCtrl::printSyncAction(SyncOperation* operation, int index)
{
    printOperationIndex(operation, index);

    switch (operation->getType())
    {
    case SyncOperation::TYPE::COPY:
        printCopyOperation(dynamic_cast<CopyOperation *>(operation), index);
        break;
    case SyncOperation::TYPE::REPLACE:
        printReplaceOperation(dynamic_cast<ReplaceOperation *>(operation), index);
        break;
    case SyncOperation::TYPE::REMOVE:
        printRemoveOperation(dynamic_cast<RemoveOperation *>(operation), index);
        break;
    case SyncOperation::TYPE::CREATE:
        printCreateOperation(dynamic_cast<CreateFolderOperation *>(operation), index);
        break;
    case SyncOperation::TYPE::EMPTY:
        printEmptyOperation(dynamic_cast<EmptyOperation *>(operation), index);
        break;
    }
}

void CPreviewListCtrl::printFile(const FileProperties& file, int index, LIST_COLUMNS column)
{
    CString str;
    if (file.isFolder())
        str = m_syncManager->getFileRelativePath(file, TRUE);
    else
        str = file.getFileName();

    SetItemText(index, column, str);
}

void CPreviewListCtrl::printCopyOperation(CopyOperation* operation, int index)
{
    FileProperties file = operation->getFile();
    ICONS icon;

    if (m_syncManager->isFileInSourceFolder(file))
    {
        printFile(file, index, LIST_COLUMNS::SOURCE_FILE);
        icon = ICONS::RIGHT_ARROW;
    }
    else
    {
        printFile(file, index, LIST_COLUMNS::DESTINATION_FILE);
        icon = ICONS::LEFT_ARROW;
    }

    printOperationIcon(icon, index);
}

void CPreviewListCtrl::printRemoveOperation(RemoveOperation* operation, int index)
{
    FileProperties file = operation->getFile();

    if (m_syncManager->isFileInSourceFolder(file))
        printFile(file, index, LIST_COLUMNS::SOURCE_FILE);
    else
        printFile(file, index, LIST_COLUMNS::DESTINATION_FILE);

    printOperationIcon(ICONS::REMOVE, index);
}

void CPreviewListCtrl::printReplaceOperation(ReplaceOperation* operation, int index)
{
    ICONS icon;

    FileProperties originalFile = operation->getFile();
    FileProperties fileToReplace = operation->getFileToReplace();

    if (m_syncManager->isFileInSourceFolder(originalFile))
    {
        printFile(originalFile, index, LIST_COLUMNS::SOURCE_FILE);
        printFile(fileToReplace, index, LIST_COLUMNS::DESTINATION_FILE);
        icon = ICONS::RIGHT_ARROW;
    }
    else
    {
        printFile(fileToReplace, index, LIST_COLUMNS::SOURCE_FILE);
        printFile(originalFile, index, LIST_COLUMNS::DESTINATION_FILE);
        icon = ICONS::LEFT_ARROW;
    }

    if (operation->isAmbiguous())
        icon = ICONS::QUESTION;

    printOperationIcon(icon, index);
}

void CPreviewListCtrl::printEmptyOperation(EmptyOperation* operation, int index)
{
    FileProperties file = operation->getFile();
    FileProperties equalFile = operation->getEqualFile();
    
    if (m_syncManager->isFileInSourceFolder(file))
    {
        printFile(file, index, LIST_COLUMNS::SOURCE_FILE);
        printFile(equalFile, index, LIST_COLUMNS::DESTINATION_FILE);
    }
    else
    {
        printFile(equalFile, index, LIST_COLUMNS::SOURCE_FILE);
        printFile(file, index, LIST_COLUMNS::DESTINATION_FILE);
    }
    
    printOperationIcon(ICONS::EQUAL, index);
}

void CPreviewListCtrl::printCreateOperation(CreateFolderOperation* operation, int index)
{
    ICONS icon;

    FileProperties originalFolder = operation->getFile();
    FileProperties folderToCreate = operation->getFolder();

    if (m_syncManager->isFileInSourceFolder(originalFolder))
    {
        printFile(originalFolder, index, LIST_COLUMNS::SOURCE_FILE);
        printFile(folderToCreate, index, LIST_COLUMNS::DESTINATION_FILE);
        icon = ICONS::RIGHT_ARROW;
    }
    else
    {
        printFile(folderToCreate, index, LIST_COLUMNS::SOURCE_FILE);
        printFile(originalFolder, index, LIST_COLUMNS::DESTINATION_FILE);
        icon = ICONS::LEFT_ARROW;
    }

    printOperationIcon(icon, index);
}

void CPreviewListCtrl::forbidOperation(int index)
{
    SyncOperation* operation = m_sortedOperations[index];

    if (operation->getType() == SyncOperation::TYPE::EMPTY)
        return;

    operation->forbid(TRUE);

    for (size_t i = index + 1; i < m_sortedOperations.size(); ++i)
    {
        if (m_sortedOperations[i]->dependsOn(operation))
            forbidOperation(i);
        else
            break;
    }
    printSyncAction(operation, index);
}

void CPreviewListCtrl::sortOperationsByFolders(SyncManager::OperationQueue& operations)
{
    auto notInvolveFolder = [](const SyncOperation* op) -> bool {
        return (bool)!op->getFile().isFolder();
    };

    auto it = std::stable_partition(operations.begin(), operations.end(), notInvolveFolder);
    m_sortedOperations.assign(it, operations.end());
    operations.erase(it, operations.end());

    // TODO: optimize
    for (const auto operation : operations)
    {
        auto dependsOn = [&operation](const SyncOperation* op) -> bool {
            return operation->dependsOn(op);
        };

        auto iter = std::find_if(m_sortedOperations.begin(),
                                 m_sortedOperations.end(),
                                 dependsOn);
        if (iter != m_sortedOperations.end())
            m_sortedOperations.insert(++iter, operation);
        else
            m_sortedOperations.push_front(operation);
    }
}

void CPreviewListCtrl::adjustColumnsWidth()
{
    SetRedraw(FALSE);

    LPRECT rect = new RECT();
    GetClientRect(rect);
    LONG listWidth = rect->right - rect->left;

    LONG part = (listWidth - PREVIEW_LIST_IMAGE_SIZE) / 9;

    SetColumnWidth(LIST_COLUMNS::INDEX, part * 1);
    SetColumnWidth(LIST_COLUMNS::SOURCE_FILE, part * 4);
    SetColumnWidth(LIST_COLUMNS::DESTINATION_FILE, part * 4);
    
    SetRedraw(TRUE);
}

BEGIN_MESSAGE_MAP(CPreviewListCtrl, CMFCListCtrl)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CPreviewListCtrl::OnDoubleClick)
    ON_NOTIFY_REFLECT(NM_RCLICK, &CPreviewListCtrl::OnRightClick)
    ON_NOTIFY(HDN_ENDTRACKA, 0, &CPreviewListCtrl::OnColumnDragEnd)
    ON_NOTIFY(HDN_ENDTRACKW, 0, &CPreviewListCtrl::OnColumnDragEnd)
    ON_MESSAGE(WM_ADJUST_COLUMNS, OnAdjustColumns)
    ON_WM_SIZE()
END_MESSAGE_MAP()


void CPreviewListCtrl::OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    *pResult = 0;
    
    LVHITTESTINFO hitTestInfo;
    hitTestInfo.pt = pNMItemActivate->ptAction;

    if (SubItemHitTest(&hitTestInfo) == -1)
        return;
    
    const SyncOperation* clickedOperation = m_sortedOperations[hitTestInfo.iItem];
        
    if (!showFilePropertiesDialog(clickedOperation))
        showFilesComparisonDialog(clickedOperation);
}

BOOL CPreviewListCtrl::showFilePropertiesDialog(const SyncOperation* singleFileOperation)
{
    FileProperties file = singleFileOperation->getFile();
    SyncOperation::TYPE type = singleFileOperation->getType();
    
    if (file.isFolder())
        return FALSE;

    if (type == SyncOperation::TYPE::COPY || type == SyncOperation::TYPE::REMOVE)
    {
        CFilePropertiesDialog dialog(file, m_syncManager);
        dialog.DoModal();
        
        return TRUE;
    }
    else
        return FALSE;
}

BOOL CPreviewListCtrl::showFilesComparisonDialog(const SyncOperation* twoFilesOperation)
{
    FileProperties firstFile = twoFilesOperation->getFile();
    SyncOperation::TYPE type = twoFilesOperation->getType();

    if (firstFile.isFolder())
        return FALSE;

    FileProperties secondFile;
    
    if (type == SyncOperation::TYPE::REPLACE || type == SyncOperation::TYPE::EMPTY)
    {
        if (type == SyncOperation::TYPE::REPLACE)
        {
            auto op = dynamic_cast<const ReplaceOperation*>(twoFilesOperation);
            secondFile = op->getFileToReplace();
        }

        if (type == SyncOperation::TYPE::EMPTY)
        {
            auto op = dynamic_cast<const EmptyOperation*>(twoFilesOperation);
            secondFile = op->getEqualFile();
        }

        CCompareFilesDialog dialog(firstFile, secondFile, m_syncManager);
        dialog.DoModal();
    }
    return TRUE;
}

void CPreviewListCtrl::OnRightClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    *pResult = 0;

    LVHITTESTINFO hitTestInfo;
    hitTestInfo.pt = pNMItemActivate->ptAction;

    if (SubItemHitTest(&hitTestInfo) == -1)
        return;

    SyncOperation* clickedOperation = m_sortedOperations[hitTestInfo.iItem];
        
    if (clickedOperation->getType() == SyncOperation::TYPE::REPLACE)
    {
        auto op = dynamic_cast<ReplaceOperation*>(clickedOperation);
    
        if (op->isAmbiguous())
        {
            op->removeAmbiguity();
            printSyncAction(clickedOperation, hitTestInfo.iItem);
            return;
        }
    }
    
    if (clickedOperation->isForbidden())
    {
        clickedOperation->forbid(FALSE);
        printSyncAction(clickedOperation, hitTestInfo.iItem);
    }
    else
        forbidOperation(hitTestInfo.iItem);
}

LRESULT CPreviewListCtrl::OnAdjustColumns(WPARAM wParam, LPARAM lParam)
{
    adjustColumnsWidth();
    return 1;
}

COLORREF CPreviewListCtrl::OnGetCellTextColor(int nRow, int nColumn)
{
    LIST_COLUMNS col = (LIST_COLUMNS)nColumn;
    
    BOOL properColumn = (col == LIST_COLUMNS::SOURCE_FILE ||
                         col == LIST_COLUMNS::DESTINATION_FILE);
    BOOL operationExists = nRow < m_sortedOperations.size();
    
    if (properColumn && operationExists)
        return chooseOperationTextColor(m_sortedOperations[nRow]);

    return CMFCListCtrl::OnGetCellTextColor(nRow, nColumn);
}

COLORREF CPreviewListCtrl::OnGetCellBkColor(int nRow, int nColumn)
{
    LIST_COLUMNS col = (LIST_COLUMNS)nColumn;

    BOOL properColumn = (col == LIST_COLUMNS::SOURCE_FILE ||
                         col == LIST_COLUMNS::DESTINATION_FILE);
    BOOL operationExists = nRow < m_sortedOperations.size();

    if (properColumn && operationExists)
        return chooseOperationBkColor(m_sortedOperations[nRow]);

    return CMFCListCtrl::OnGetCellBkColor(nRow, nColumn);
}

void CPreviewListCtrl::printOperationIndex(const SyncOperation* operation, int& index)
{
    int listItemCount = GetItemCount();

    BOOL itemExists = (index >= 0) && (index < listItemCount);

    if (!itemExists)
        index = listItemCount;

    LVITEM listItem;
    listItem.mask = LVIF_TEXT | LVIF_IMAGE;
    listItem.iItem = index;
    listItem.iSubItem = LIST_COLUMNS::INDEX;
    
    CString indexStr;
    indexStr.Format(_T("%d"), index + 1);
    listItem.pszText = indexStr.GetBuffer();
    
    if (operation->getFile().isFolder())
        listItem.iImage = ICONS::FOLDER;
    else
        listItem.iImage = -1;
    
    if (itemExists)
        SetItem(&listItem);
    else
        InsertItem(&listItem);
}

void CPreviewListCtrl::printOperationIcon(ICONS icon, int index)
{
    LVITEM listItem;
    listItem.mask = LVIF_IMAGE;
    listItem.iItem = index;
    listItem.iSubItem = LIST_COLUMNS::ACTION;
    listItem.iImage = icon;

    SetItem(&listItem);
}

COLORREF CPreviewListCtrl::chooseOperationTextColor(const SyncOperation* operation) const
{
    SyncOperation::TYPE type = operation->getType();

    if (type == SyncOperation::TYPE::REMOVE)
        return m_colors.REMOVE_TEXT_COLOR;

    if (type == SyncOperation::TYPE::COPY ||
        type == SyncOperation::TYPE::CREATE ||
        type == SyncOperation::TYPE::REPLACE)
    {
        COLORREF color;
        FileProperties file = operation->getFile();

        if (m_syncManager->isFileInSourceFolder(file))
            color = m_colors.SOURCE_TO_DESTINATION_TEXT_COLOR;
        else
            color = m_colors.DESTINATION_TO_SOURCE_TEXT_COLOR;

        if (type == SyncOperation::TYPE::REPLACE)
        {
            auto op = dynamic_cast<const ReplaceOperation*>(operation);
            color = op->isAmbiguous() ? m_colors.AMBIGUOUS_TEXT_COLOR : color;
        }

        return color;
    }
    
    return m_colors.DEFAULT_TEXT_COLOR;
}

COLORREF CPreviewListCtrl::chooseOperationBkColor(const SyncOperation* operation) const
{
    COLORREF color = m_colors.DEFAULT_BACK_COLOR;

    if (operation->isForbidden())
        color = m_colors.FORBIDDEN_BACK_COLOR;

    if (operation->getFile().isFolder())
    {
        if (operation->isForbidden())
            color = m_colors.FORBIDDEN_FOLDER_BACK_COLOR;
        else
            color = m_colors.FOLDER_BACK_COLOR;
    }

    return color;
}

void CPreviewListCtrl::OnColumnDragEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
    adjustColumnsWidth();
    *pResult = 1;
}

void CPreviewListCtrl::OnSize(UINT nType, int cx, int cy)
{
    CMFCListCtrl::OnSize(nType, cx, cy);

    PostMessage(WM_ADJUST_COLUMNS);
}
