#include "stdafx.h"
#include "SimpleSync.h"
#include "PreviewListCtrl.h"
#include "FilePropertiesDialog.h"
#include "CompareDialog.h"


IMPLEMENT_DYNAMIC(CPreviewListCtrl, CMFCListCtrl)

CPreviewListCtrl::CPreviewListCtrl(SyncManager* syncManager)
    : m_syncManager(syncManager)
{
}

CPreviewListCtrl::~CPreviewListCtrl()
{
}

void CPreviewListCtrl::setupColumns()
{
    InsertColumn(LIST_COLUMNS::INDEX, L"�", LVCFMT_LEFT, 30)    InsertColumn(LIST_COLUMNS::SOURCE_FILE, L"�������� ������    InsertColumn(LIST_COLUMNS::ACTION, L"��������", LVCF    InsertColumn(LIST_COLUMNS::DESTINATION_FILE, L"�������� ������}
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_AUTOSIZECOLUMNS |
        LVS_EX_FULLROWSELECT);

    optimizeColumnsWidth();
}

void CPreviewListCtrl::showPreview()
{
    sortOperationsByFolders(m_syncManager->getOperations());
    
    DeleteAllItems();
    for (auto& action : m_sortedOperations)
        printSyncAction(action);

    optimizeColumnsWidth();
}

void CPreviewListCtrl::printSyncAction(SyncOperation* operation, int index)
{
    CString itemIndexStr;
    if (index < 0 || index >= GetItemCount())
    {
        itemIndexStr.Format(_T("%d"), GetItemCount() + 1);
        index = InsertItem(GetItemCount(), itemIndexStr);
    }
    else
    {
        itemIndexStr.Format(_T("%d"), index + 1);
        SetItemText(index, 0, itemIndexStr);
    }

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

void CPreviewListCtrl::printCopyOperation(CopyOperation* operation, int index)
{
    FileProperties file = operation->getFile();
    CString action;

    if (m_syncManager->isFileInSourceFolder(file))
    {
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, file.getFileName());
        action = L"->";
    }
    else
    {
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, file.getFileName());
        action = L"<-";
    }

    SetItemText(index, LIST_COLUMNS::ACTION, action);
}

void CPreviewListCtrl::printRemoveOperation(RemoveOperation* operation, int index)
{
    FileProperties file = operation->getFile();

    if (m_syncManager->isFileInSourceFolder(file))
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, file.getFileName());
    else
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, file.getFileName());

    SetItemText(index, LIST_COLUMNS::ACTION, L"X");
}

void CPreviewListCtrl::printReplaceOperation(ReplaceOperation* operation, int index)
{
    CString action;

    FileProperties originalFile = operation->getFile();
    FileProperties fileToReplace = operation->getFileToReplace();

    if (m_syncManager->isFileInSourceFolder(originalFile))
    {
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, originalFile.getFileName());
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, fileToReplace.getFileName());
        action = L"-->";
    }
    else
    {
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, fileToReplace.getFileName());
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, originalFile.getFileName());
        action = L"<--";
    }

    if (operation->isAmbiguous())
        action = L"?";

    SetItemText(index, LIST_COLUMNS::ACTION, action);
}

void CPreviewListCtrl::printEmptyOperation(EmptyOperation* operation, int index)
{
    FileProperties file = operation->getFile();
    FileProperties equalFile = operation->getEqualFile();

    if (m_syncManager->isFileInSourceFolder(file))
    {
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, file.getFileName());
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, equalFile.getFileName());
    }
    else
    {
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, equalFile.getFileName());
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, file.getFileName());
    }

    SetItemText(index, LIST_COLUMNS::ACTION, L"=");
}

void CPreviewListCtrl::printCreateOperation(CreateFolderOperation* operation, int index)
{
    CString action;

    FileProperties originalFolder = operation->getFile();
    FileProperties folderToCreate = operation->getFolder();

    if (m_syncManager->isFileInSourceFolder(originalFolder))
    {
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, originalFolder.getFileName());
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, folderToCreate.getFileName());
        action = L"-->";
    }
    else
    {
        SetItemText(index, LIST_COLUMNS::SOURCE_FILE, folderToCreate.getFileName());
        SetItemText(index, LIST_COLUMNS::DESTINATION_FILE, originalFolder.getFileName());
        action = L"<--";
    }

    SetItemText(index, LIST_COLUMNS::ACTION, action);
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

void CPreviewListCtrl::optimizeColumnsWidth()
{
    for (int i = 0; i < GetHeaderCtrl().GetItemCount(); ++i)
    {
        SetColumnWidth(i, LVSCW_AUTOSIZE);
        int contentWidth = GetColumnWidth(i);
        SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
        int headerWidth = GetColumnWidth(i);

        SetColumnWidth(i, max(contentWidth, headerWidth));
    }
}

BEGIN_MESSAGE_MAP(CPreviewListCtrl, CMFCListCtrl)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CPreviewListCtrl::OnDoubleClick)
    ON_NOTIFY_REFLECT(NM_RCLICK, &CPreviewListCtrl::OnRightClick)
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

COLORREF CPreviewListCtrl::chooseOperationTextColor(const SyncOperation* operation) const
{
    SyncOperation::TYPE type = operation->getType();

    if (type == SyncOperation::TYPE::REMOVE)
        return m_colors.DELETION_COLOR;

    if (type == SyncOperation::TYPE::COPY ||
        type == SyncOperation::TYPE::CREATE ||
        type == SyncOperation::TYPE::REPLACE)
    {
        COLORREF color;
        FileProperties file = operation->getFile();

        if (m_syncManager->isFileInSourceFolder(file))
            color = m_colors.SOURCE_TO_DESTINATION_COLOR;
        else
            color = m_colors.DESTINATION_TO_SOURCE_COLOR;

        if (type == SyncOperation::TYPE::REPLACE)
        {
            auto op = dynamic_cast<const ReplaceOperation*>(operation);
            color = op->isAmbiguous() ? m_colors.AMBIGUOUS_COLOR : color;
        }

        return color;
    }
    
    return m_colors.DEFAULT_COLOR;
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
