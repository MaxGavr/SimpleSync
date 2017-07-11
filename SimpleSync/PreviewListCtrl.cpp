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
    DeleteAllItems();

    SyncManager::OperationQueue syncActions = m_syncManager->getOperations();
    for (auto& action : syncActions)
        printSyncAction(action);
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
void CPreviewListCtrl::optimizeColumnsWidth()
{
    for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
    {
        SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
    }
}


BEGIN_MESSAGE_MAP(CPreviewListCtrl, CListCtrl)
END_MESSAGE_MAP()
