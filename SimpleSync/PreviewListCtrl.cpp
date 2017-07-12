#include "stdafx.h"
#include "SimpleSync.h"
#include "PreviewListCtrl.h"



IMPLEMENT_DYNAMIC(CPreviewListCtrl, CListCtrl)

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
    SetExtendedStyle(LVS_EX_AUTOSIZECOLUMNS);

    optimizeColumnsWidth();
}

void CPreviewListCtrl::showPreview()
{
    sortOperationsByFolders(m_syncManager->getOperations());
    
    DeleteAllItems();
    for (auto& action : m_sortedOperations)
        printSyncAction(action);
}

void CPreviewListCtrl::printSyncAction(SyncOperation* operation)
{
    CString itemIndexStr;
    itemIndexStr.Format(_T("%d"), GetItemCount() + 1);
    
    int itemIndex = InsertItem(GetItemCount(), itemIndexStr);

    switch (operation->getType())
    {
    case SyncOperation::TYPE::COPY:
        printCopyOperation(dynamic_cast<CopyOperation *>(operation), itemIndex);
        break;
    case SyncOperation::TYPE::REPLACE:
        printReplaceOperation(dynamic_cast<ReplaceOperation *>(operation), itemIndex);
        break;
    case SyncOperation::TYPE::REMOVE:
        printRemoveOperation(dynamic_cast<RemoveOperation *>(operation), itemIndex);
        break;
    case SyncOperation::TYPE::CREATE:
        printCreateOperation(dynamic_cast<CreateFolderOperation *>(operation), itemIndex);
        break;
    case SyncOperation::TYPE::EMPTY:
        printEmptyOperation(dynamic_cast<EmptyOperation *>(operation), itemIndex);
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

void CPreviewListCtrl::sortOperationsByFolders(SyncManager::OperationQueue& operations)
{
    auto notInvolveFolder = [](const SyncOperation* op) -> bool {
        return (bool)!op->getFile().isDirectory();
    };

    auto it = std::stable_partition(operations.begin(), operations.end(), notInvolveFolder);
    m_sortedOperations.assign(it, operations.end());
    operations.erase(it, operations.end());

    // TODO: optimize
    for (const auto operation : operations)
    {
        FileProperties file = operation->getFile();

        auto l = [&file, this](const SyncOperation* op) -> bool {
            return m_syncManager->getFileRelativePath(file, FALSE) ==
                m_syncManager->getFileRelativePath(op->getFile(), TRUE);
        };

        auto iter = std::find_if(m_sortedOperations.begin(), m_sortedOperations.end(), l);
        if (iter != m_sortedOperations.end())
            m_sortedOperations.insert(++iter, operation);
        else
            m_sortedOperations.push_front(operation);
    }
}

void CPreviewListCtrl::optimizeColumnsWidth()
{
    for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
    {
        SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
    }
}


BEGIN_MESSAGE_MAP(CPreviewListCtrl, CListCtrl)
END_MESSAGE_MAP()
