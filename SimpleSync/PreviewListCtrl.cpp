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
    SyncManager::ActionQueue syncActions = m_syncManager->scan();

    for (const auto& action : syncActions)
    {
        printSyncAction(action);
    }
}

void CPreviewListCtrl::printSyncAction(const SyncAction& action)
{
    CString listItemText;
    listItemText.Format(_T("%d"), GetItemCount() + 1);
    
    int index = InsertItem(GetItemCount(), listItemText);

    switch (action.m_type)
    {
    case SyncAction::TYPE::COPY :
    {
        if (action.m_file.getFileFolder() == m_syncManager->getSourceFolder())
        {
            SetItemText(index, COLUMNS::SOURCE_FILE, action.m_file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L">>>");
        }
        else
        {
            SetItemText(index, COLUMNS::DESTINATION_FILE, action.m_file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L"<<<");
        }

        break;
    }
    case SyncAction::TYPE::REMOVE :
    {
        if (action.m_file.getFileFolder() == m_syncManager->getSourceFolder())
        {
            SetItemText(index, COLUMNS::SOURCE_FILE, action.m_file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L"X");
        }
        else
        {
            SetItemText(index, COLUMNS::DESTINATION_FILE, action.m_file.getFileName());
            SetItemText(index, COLUMNS::ACTION, L"X");
        }
    }
    }
}


BEGIN_MESSAGE_MAP(CPreviewListCtrl, CListCtrl)
END_MESSAGE_MAP()
