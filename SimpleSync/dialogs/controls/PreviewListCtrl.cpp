#include "stdafx.h"
#include "SimpleSync.h"
#include "PreviewListCtrl.h"
#include "dialogs/FilePropertiesDialog.h"
#include "dialogs/CompareDialog.h"

#include "sync/SyncManager.h"




IMPLEMENT_DYNAMIC(CPreviewListCtrl, CMFCListCtrl)

CPreviewListCtrl::CPreviewListCtrl(SyncManager* syncManager)
    : m_syncManager(syncManager)
{
    // Loading order must match ICON enum
    m_rightArrowImageSmall.Load(IDB_RIGHT_ARROW_SMALL); // ICON::RIGHT_ARROW
    m_equalImageSmall.Load(IDB_EQUAL_SMALL); // ICON::EQUAL
    m_leftArrowImageSmall.Load(IDB_LEFT_ARROW_SMALL); // ICON::LEFT_ARROW
    m_folderImageSmall.Load(IDB_FOLDER_SMALL); // ICON::FOLDER
    m_removeImageSmall.Load(IDB_REMOVE_SMALL); // ICON::REMOVE
    m_questionImageSmall.Load(IDB_QUESTION_SMALL); // ICON::QUESTION

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



void CPreviewListCtrl::setupPreviewList()
{
    SetExtendedStyle(GetExtendedStyle() |
                     LVS_EX_FULLROWSELECT |
                     LVS_EX_SUBITEMIMAGES);
    SetImageList(&m_imageList, LVSIL_SMALL);

    InsertColumn(LIST_COLUMN::INDEX, _T("№"),
                 LVCFMT_RIGHT);
    InsertColumn(LIST_COLUMN::SOURCE_FILE, _T("Исходная директория"),
                 LVCFMT_LEFT);
    InsertColumn(LIST_COLUMN::ACTION, _T(""),
                 LVCFMT_CENTER | LVCFMT_FIXED_WIDTH,
                 PREVIEW_LIST_IMAGE_SIZE);
    InsertColumn(LIST_COLUMN::DESTINATION_FILE, _T("Конечная директория"),
                 LVCFMT_LEFT);

    adjustColumnsWidth();
}

void CPreviewListCtrl::adjustColumnsWidth()
{
    SetRedraw(FALSE);

    LPRECT rect = new RECT();
    GetClientRect(rect);
    LONG listWidth = rect->right - rect->left;

    LONG fraction = (listWidth - PREVIEW_LIST_IMAGE_SIZE) / 9;

    SetColumnWidth(LIST_COLUMN::INDEX, fraction * 1);
    SetColumnWidth(LIST_COLUMN::SOURCE_FILE, fraction * 4);
    SetColumnWidth(LIST_COLUMN::DESTINATION_FILE, fraction * 4);

    SetRedraw(TRUE);
}

void CPreviewListCtrl::showPreview()
{
    clearPreview();
    
    SyncManager::OperationQueue operations = m_syncManager->getOperationQueue();
    sortOperationsByFolders(operations);
    
    for (auto& operation : m_sortedOperations)
        printSyncOperation(operation.get());

    adjustColumnsWidth();
}

void CPreviewListCtrl::clearPreview()
{
    DeleteAllItems();
    m_sortedOperations.clear();
}



void CPreviewListCtrl::printSyncOperation(SyncOperation* op,
                                          int index)
{
    printOperationIndex(op, index);

    switch (op->getType())
    {
    case SyncOperation::TYPE::COPY:
        printCopyOperation(dynamic_cast<CopyOperation *>(op), index);
        break;
    case SyncOperation::TYPE::REPLACE:
        printReplaceOperation(dynamic_cast<ReplaceOperation *>(op), index);
        break;
    case SyncOperation::TYPE::REMOVE:
        printRemoveOperation(dynamic_cast<RemoveOperation *>(op), index);
        break;
    case SyncOperation::TYPE::CREATE:
        printCreateOperation(dynamic_cast<CreateFolderOperation *>(op), index);
        break;
    case SyncOperation::TYPE::EMPTY:
        printEmptyOperation(dynamic_cast<EmptyOperation *>(op), index);
        break;
    }
}

void CPreviewListCtrl::printFile(const FileProperties& file,
                                 int index,
                                 LIST_COLUMN column)
{
    CString printedString;
    if (file.isFolder())
        printedString = m_syncManager->getFileRelativePath(file, TRUE);
    else
        printedString = file.getFileName();

    SetItemText(index, column, printedString);
}

void CPreviewListCtrl::printOperationIndex(const SyncOperation* operation,
                                           int& index)
{
    int listItemCount = GetItemCount();

    BOOL itemExists = (index >= 0) && (index < listItemCount);

    if (!itemExists)
        index = listItemCount;

    LVITEM listItem;
    listItem.mask = LVIF_TEXT | LVIF_IMAGE;
    listItem.iItem = index;
    listItem.iSubItem = LIST_COLUMN::INDEX;

    CString indexStr;
    indexStr.Format(_T("%d"), index + 1);
    listItem.pszText = indexStr.GetBuffer();

    
    if (operation->getFile().isFolder())
        listItem.iImage = ICON::FOLDER;
    else
        listItem.iImage = -1;

    if (itemExists)
        SetItem(&listItem);
    else
        InsertItem(&listItem);
}

void CPreviewListCtrl::printOperationIcon(ICON icon, int index)
{
    LVITEM listItem;
    listItem.mask = LVIF_IMAGE;
    listItem.iItem = index;
    listItem.iSubItem = LIST_COLUMN::ACTION;
    listItem.iImage = icon;

    SetItem(&listItem);
}



void CPreviewListCtrl::printCopyOperation(CopyOperation* operation,
                                          int index)
{
    ICON icon;

    FileProperties file = operation->getFile();
    LIST_COLUMN fileColumn;

    if (m_syncManager->isFileInSourceFolder(file))
    {
        fileColumn = LIST_COLUMN::SOURCE_FILE;
        icon = ICON::RIGHT_ARROW;
    }
    else
    {
        fileColumn = LIST_COLUMN::DESTINATION_FILE;
        icon = ICON::LEFT_ARROW;
    }

    printFile(file, index, fileColumn);
    printOperationIcon(icon, index);
}

void CPreviewListCtrl::printRemoveOperation(RemoveOperation* operation,
                                            int index)
{
    FileProperties file = operation->getFile();
    LIST_COLUMN fileColumn;

    if (m_syncManager->isFileInSourceFolder(file))
        fileColumn = LIST_COLUMN::SOURCE_FILE;
    else
        fileColumn = LIST_COLUMN::DESTINATION_FILE;

    printFile(file, index, fileColumn);
    printOperationIcon(ICON::REMOVE, index);
}

void CPreviewListCtrl::printReplaceOperation(ReplaceOperation* operation,
                                             int index)
{
    ICON icon;

    FileProperties originalFile = operation->getFile();
    FileProperties fileToReplace = operation->getFileToReplace();
    LIST_COLUMN originalFileColumn;
    LIST_COLUMN replacedFileColumn;

    if (m_syncManager->isFileInSourceFolder(originalFile))
    {
        originalFileColumn = LIST_COLUMN::SOURCE_FILE;
        replacedFileColumn = LIST_COLUMN::DESTINATION_FILE;
        icon = ICON::RIGHT_ARROW;
    }
    else
    {
        originalFileColumn = LIST_COLUMN::DESTINATION_FILE;
        replacedFileColumn = LIST_COLUMN::SOURCE_FILE;
        icon = ICON::LEFT_ARROW;
    }

    if (operation->isAmbiguous())
        icon = ICON::QUESTION;

    printFile(originalFile, index, originalFileColumn);
    printFile(fileToReplace, index, replacedFileColumn);
    printOperationIcon(icon, index);
}

void CPreviewListCtrl::printEmptyOperation(EmptyOperation* operation,
                                           int index)
{
    FileProperties file = operation->getFile();
    FileProperties equalFile = operation->getEqualFile();
    LIST_COLUMN fileColumn;
    LIST_COLUMN equalFileColumn;
    
    if (m_syncManager->isFileInSourceFolder(file))
    {
        fileColumn = LIST_COLUMN::SOURCE_FILE;
        equalFileColumn = LIST_COLUMN::DESTINATION_FILE;
    }
    else
    {
        fileColumn = LIST_COLUMN::DESTINATION_FILE;
        equalFileColumn = LIST_COLUMN::SOURCE_FILE;
    }
    
    printFile(file, index, fileColumn);
    printFile(equalFile, index, equalFileColumn);
    printOperationIcon(ICON::EQUAL, index);
}

void CPreviewListCtrl::printCreateOperation(CreateFolderOperation* operation,
                                            int index)
{
    ICON icon;

    FileProperties originalFolder = operation->getFile();
    FileProperties folderToCreate = operation->getFolderToCreate();
    LIST_COLUMN originalFolderColumn;
    LIST_COLUMN folderToCreateColumn;

    if (m_syncManager->isFileInSourceFolder(originalFolder))
    {
        originalFolderColumn = LIST_COLUMN::SOURCE_FILE;
        folderToCreateColumn = LIST_COLUMN::DESTINATION_FILE;
        icon = ICON::RIGHT_ARROW;
    }
    else
    {
        originalFolderColumn= LIST_COLUMN::DESTINATION_FILE;
        folderToCreateColumn = LIST_COLUMN::SOURCE_FILE;
        icon = ICON::LEFT_ARROW;
    }

    printFile(originalFolder, index, originalFolderColumn);
    printFile(folderToCreate, index, folderToCreateColumn);
    printOperationIcon(icon, index);
}



int CPreviewListCtrl::forbidOperation(int index)
{
    SyncOperation* operation = m_sortedOperations[index].get();

    if (operation->getType() == SyncOperation::TYPE::EMPTY)
        return -1;

    operation->forbid(TRUE);
    printSyncOperation(operation, index);

    for (size_t i = index + 1; i < m_sortedOperations.size(); ++i)
    {
        auto nextOperation = m_sortedOperations[i];
        if (nextOperation->dependsOn(operation))
        {
            int nextPositionToCheck = forbidOperation(i);
            if (nextPositionToCheck != -1)
                i = nextPositionToCheck - 1;
        }
        else
            return i;
    }
}

void CPreviewListCtrl::sortOperationsByFolders(SyncManager::OperationQueue& operations)
{
    auto notInvolveFolder = [](SyncOperation::ptr& op) -> bool {
        return (bool)!op->getFile().isFolder();
    };

    auto it = std::stable_partition(operations.begin(),
                                    operations.end(),
                                    notInvolveFolder);
    m_sortedOperations.assign(it, operations.end());
    operations.erase(it, operations.end());

    // TODO: optimize
    for (const auto& operation : operations)
    {
        auto dependsOn = [&operation](SyncOperation::ptr op) -> bool {
            return operation->dependsOn(op.get());
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



BEGIN_MESSAGE_MAP(CPreviewListCtrl, CMFCListCtrl)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CPreviewListCtrl::OnDoubleClick)
    ON_NOTIFY_REFLECT(NM_RCLICK, &CPreviewListCtrl::OnRightClick)
    ON_NOTIFY(HDN_ENDTRACKA, 0, &CPreviewListCtrl::OnColumnResizeDragEnd)
    ON_NOTIFY(HDN_ENDTRACKW, 0, &CPreviewListCtrl::OnColumnResizeDragEnd)
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
    
    auto clickedOperation = m_sortedOperations[hitTestInfo.iItem].get();
    
    BOOL filePropertiesShown = showFilePropertiesDialog(clickedOperation);
    if (!filePropertiesShown)
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


    int operationIndex = hitTestInfo.iItem;
    auto clickedOperation = m_sortedOperations[operationIndex].get();
        
    if (clickedOperation->getType() == SyncOperation::TYPE::REPLACE)
    {
        auto op = dynamic_cast<ReplaceOperation*>(clickedOperation);
    
        if (op->isAmbiguous())
        {
            op->removeAmbiguity();
            printSyncOperation(clickedOperation, operationIndex);
            return;
        }
    }
    
    if (clickedOperation->isForbidden())
    {
        clickedOperation->forbid(FALSE);
        printSyncOperation(clickedOperation, operationIndex);
    }
    else
        forbidOperation(operationIndex);
}

LRESULT CPreviewListCtrl::OnAdjustColumns(WPARAM wParam, LPARAM lParam)
{
    adjustColumnsWidth();
    return 1;
}

void CPreviewListCtrl::OnColumnResizeDragEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
    adjustColumnsWidth();
    *pResult = 1;
}

void CPreviewListCtrl::OnSize(UINT nType, int cx, int cy)
{
    CMFCListCtrl::OnSize(nType, cx, cy);

    PostMessage(WM_ADJUST_COLUMNS);
}

COLORREF CPreviewListCtrl::OnGetCellTextColor(int nRow, int nColumn)
{
    LIST_COLUMN col = (LIST_COLUMN)nColumn;
    
    BOOL properColumn = (col == LIST_COLUMN::SOURCE_FILE ||
                         col == LIST_COLUMN::DESTINATION_FILE);
    BOOL operationExists = nRow < m_sortedOperations.size();
    
    if (properColumn && operationExists)
    {
        auto operation = m_sortedOperations[nRow].get();
        return chooseOperationTextColor(operation);
    }

    return CMFCListCtrl::OnGetCellTextColor(nRow, nColumn);
}

COLORREF CPreviewListCtrl::OnGetCellBkColor(int nRow, int nColumn)
{
    LIST_COLUMN col = (LIST_COLUMN)nColumn;

    BOOL properColumn = (col == LIST_COLUMN::SOURCE_FILE ||
                         col == LIST_COLUMN::DESTINATION_FILE);
    BOOL operationExists = nRow < m_sortedOperations.size();

    if (properColumn && operationExists)
    {
        auto operation = m_sortedOperations[nRow].get();
        return chooseOperationBkColor(operation);
    }

    return CMFCListCtrl::OnGetCellBkColor(nRow, nColumn);
}



COLORREF CPreviewListCtrl::chooseOperationTextColor(const SyncOperation* operation) const
{
    using TYPE = SyncOperation::TYPE;
    TYPE type = operation->getType();

    if (type == TYPE::REMOVE)
        return m_colors.REMOVE_TEXT_COLOR;

    if (type == TYPE::COPY || type == TYPE::CREATE || type == TYPE::REPLACE)
    {
        COLORREF color;
        FileProperties file = operation->getFile();

        if (m_syncManager->isFileInSourceFolder(file))
            color = m_colors.SOURCE_TO_DESTINATION_TEXT_COLOR;
        else
            color = m_colors.DESTINATION_TO_SOURCE_TEXT_COLOR;

        if (type == TYPE::REPLACE)
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
    
    FileProperties file = operation->getFile();
    BOOL isForbidden = operation->isForbidden();

    if (isForbidden)
        color = m_colors.FORBIDDEN_BACK_COLOR;

    if (file.isFolder())
    {
        if (isForbidden)
            color = m_colors.FORBIDDEN_FOLDER_BACK_COLOR;
        else
            color = m_colors.FOLDER_BACK_COLOR;
    }

    return color;
}
