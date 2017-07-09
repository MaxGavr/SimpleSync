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
    InsertColumn(LIST_COLUMNS::INDEX, L"№", LVCFMT_LEFT, 30);
    InsertColumn(LIST_COLUMNS::SOURCE_FILE, L"Исходная директория", LVCFMT_LEFT, 100);
    InsertColumn(LIST_COLUMNS::ACTION, L"Действие", LVCFMT_CENTER, 70);
    InsertColumn(LIST_COLUMNS::DESTINATION_FILE, L"Конечная директория", LVCFMT_LEFT, 100);
}

void CPreviewListCtrl::showPreview()
{
    SyncManager::OperationQueue syncActions = m_syncManager->scan();

    for (auto& action : syncActions)
    {
        printSyncAction(action);
    }
}

void CPreviewListCtrl::printSyncAction(SyncOperation* operation)
{
    CString itemIndexStr;
    itemIndexStr.Format(_T("%d"), GetItemCount() + 1);
    
    int itemIndex = InsertItem(GetItemCount(), itemIndexStr);
    CString action;
    
    FileProperties file = operation->getFile();

    switch (operation->getType())
    {
    case SyncOperation::TYPE::COPY:
    {
        if (m_syncManager->isFileInSourceFolder(file))
        {
            SetItemText(itemIndex, LIST_COLUMNS::SOURCE_FILE, file.getFileName());
            action = L"->";
        }
        else
        {
            SetItemText(itemIndex, LIST_COLUMNS::DESTINATION_FILE, file.getFileName());
            action = L"<-";
        }

        break;
    }
    case SyncOperation::TYPE::REMOVE:
    {
        action = L"X";
        if (m_syncManager->isFileInSourceFolder(file))
        {
            SetItemText(itemIndex, LIST_COLUMNS::SOURCE_FILE, file.getFileName());
        }
        else
        {
            SetItemText(itemIndex, LIST_COLUMNS::DESTINATION_FILE, file.getFileName());
        }
        break;
    }
    case SyncOperation::TYPE::EMPTY:
    {
        auto emptyOperation = dynamic_cast<EmptyOperation *>(operation);
        FileProperties equalFile = emptyOperation->getEqualFile();

        action = L"=";
        if (m_syncManager->isFileInSourceFolder(file))
        {
            SetItemText(itemIndex, LIST_COLUMNS::SOURCE_FILE, file.getFileName());
            SetItemText(itemIndex, LIST_COLUMNS::DESTINATION_FILE, equalFile.getFileName());
        }
        else
        {
            SetItemText(itemIndex, LIST_COLUMNS::SOURCE_FILE, equalFile.getFileName());
            SetItemText(itemIndex, LIST_COLUMNS::DESTINATION_FILE, file.getFileName());
        }

        break;
    }
    }
    SetItemText(itemIndex, LIST_COLUMNS::ACTION, action);
}


BEGIN_MESSAGE_MAP(CPreviewListCtrl, CListCtrl)
END_MESSAGE_MAP()
