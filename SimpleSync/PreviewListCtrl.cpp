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
    InsertColumn(COLUMNS::INDEX, L"№", LVCFMT_LEFT, 30);
    InsertColumn(COLUMNS::SOURCE_FILE, L"Исходная директория", LVCFMT_LEFT, 100);
    InsertColumn(COLUMNS::ACTION, L"Действие", LVCFMT_LEFT, 70);
    InsertColumn(COLUMNS::DESTINATION_FILE, L"Конечная директория", LVCFMT_LEFT, 100);
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
    CString listItemText;
    listItemText.Format(_T("%d"), GetItemCount() + 1);
    
    int index = InsertItem(GetItemCount(), listItemText);
    FileProperties file = operation->getFile();

    switch (operation->getType())
    {
    case SyncOperation::TYPE::COPY :
    {
        if (file.getFileFolder() == m_syncManager->getSourceFolder())
        {
            SetItemText(index, COLUMNS::SOURCE_FILE, file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L">>>");
        }
        else
        {
            SetItemText(index, COLUMNS::DESTINATION_FILE, file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L"<<<");
        }

        break;
    }
    case SyncOperation::TYPE::REMOVE :
    {
        if (file.getFileFolder() == m_syncManager->getSourceFolder())
        {
            SetItemText(index, COLUMNS::SOURCE_FILE, file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L"X");
        }
        else
        {
            SetItemText(index, COLUMNS::DESTINATION_FILE, file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L"X");
        }
    }
    }
}


BEGIN_MESSAGE_MAP(CPreviewListCtrl, CListCtrl)
END_MESSAGE_MAP()
