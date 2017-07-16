#include "stdafx.h"
#include "SimpleSync.h"
#include "ProgressDialog.h"
#include "SyncManager.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CSyncProgressDialog, CDialogEx)

CSyncProgressDialog::CSyncProgressDialog(SyncManager* syncManager,
                                         CWnd* pParent)
	: CDialogEx(IDD_SYNC_DIALOG, pParent),
      m_syncManager(syncManager)
{
}

CSyncProgressDialog::~CSyncProgressDialog()
{
}

UINT CSyncProgressDialog::runSync(LPVOID pParam)
{
    auto dialog = (CSyncProgressDialog*)pParam;

    SyncManager::SyncCallback callback = [dialog](const SyncOperation* op) {
        dialog->showOperationProgress(op);
    };

    dialog->m_syncManager->sync(&callback);

    dialog->PostMessage(WM_SYNC_COMPLETED);

    return 0;
}

void CSyncProgressDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SYNC_STATIC, m_currentOperationTitle);
    DDX_Control(pDX, IDC_SYNC_PROGRESS, m_syncProgressBar);
}

void CSyncProgressDialog::showOperationProgress(const SyncOperation* operation)
{
    SyncOperation::TYPE type = operation->getType();
    CString title;
    CString* fullTitle = new CString(_T(""));

    FileProperties file = operation->getFile();
    CString fileRelativePath = m_syncManager->getFileRelativePath(file, TRUE);

    switch (type)
    {
    case SyncOperation::TYPE::COPY:
        title = _T("Копирование %s...");
        break;
    case SyncOperation::TYPE::REPLACE:
        title = _T("Замена %s...");
        break;
    case SyncOperation::TYPE::REMOVE:
        title = _T("Удаление %s...");
        break;
    case SyncOperation::TYPE::CREATE:
        title = _T("Создание папки %s...");
        break;
    default:
        title = _T("");
    }

    fullTitle->Format(title, fileRelativePath);

    PostMessage(WM_SHOW_SYNC_PROGRESS, (WPARAM)fullTitle);
}


BEGIN_MESSAGE_MAP(CSyncProgressDialog, CDialogEx)
    ON_MESSAGE(WM_SHOW_SYNC_PROGRESS, OnShowSyncProgress)
    ON_MESSAGE(WM_SYNC_COMPLETED, OnSyncCompleted)
END_MESSAGE_MAP()


BOOL CSyncProgressDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    int operationsCount = m_syncManager->getOperations().size();
    m_syncProgressBar.SetRange(0, operationsCount);
    m_syncProgressBar.SetPos(0);
    m_syncProgressBar.SetStep(1);

    AfxBeginThread(runSync, this);

    return TRUE;
}

LRESULT CSyncProgressDialog::OnSyncCompleted(WPARAM wParam, LPARAM lParam)
{
    m_currentOperationTitle = CString("Синхронизация завершена");
    UpdateData(FALSE);

    auto okButton = (CButton *)GetDlgItem(IDOK);
    okButton->EnableWindow(TRUE);

    return 1;
}

LRESULT CSyncProgressDialog::OnShowSyncProgress(WPARAM wParam, LPARAM lParam)
{
    m_syncProgressBar.StepIt();
    m_currentOperationTitle = *((CString *)wParam);
    UpdateData(FALSE);

    return 1;
}
