#include "stdafx.h"
#include "SimpleSync.h"
#include "ProgressDialog.h"
#include "sync/SyncManager.h"
#include "operations/SyncOperation.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CSyncProgressDialog, CDialogEx)

CSyncProgressDialog::CSyncProgressDialog(SyncManager* syncManager,
                                         CWnd* pParent)
	: CDialogEx(IDD_SYNC_PROGRESS_DIALOG, pParent),
      m_syncManager(syncManager)
{
}

CSyncProgressDialog::~CSyncProgressDialog()
{
}

UINT CSyncProgressDialog::runSync(LPVOID pParam)
{
    auto dialog = (CSyncProgressDialog*)pParam;

    SyncManager::SyncCallback callback = [dialog](SyncOperation::ptr& op) {
        dialog->showOperationProgress(op.get());
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
    CString title;
    CString* fullTitle = new CString(_T(""));

    SyncOperation::TYPE type = operation->getType();
    switch (type)
    {
    case SyncOperation::TYPE::COPY:
        title = _T("����������� %s");
        break;
    case SyncOperation::TYPE::REPLACE:
        title = _T("������ %s");
        break;
    case SyncOperation::TYPE::REMOVE:
        title = _T("�������� %s");
        break;
    case SyncOperation::TYPE::CREATE:
        title = _T("�������� ����� %s");
        break;
    default:
        title = _T("...");
    }

    FileProperties file = operation->getFile();
    CString filePath = m_syncManager->getFileRelativePath(file, TRUE);

    fullTitle->Format(title, filePath);

    PostMessage(WM_SHOW_SYNC_PROGRESS, (WPARAM)fullTitle);
}


BOOL CSyncProgressDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SyncManager::OperationQueue operations = m_syncManager->getOperationQueue();

    auto notForbidden = [](SyncOperation::ptr& op) {
        return !op->isForbidden();
    };

    int operationCount = std::count_if(operations.begin(), operations.end(), notForbidden);

    m_syncProgressBar.SetRange(0, operationCount);
    m_syncProgressBar.SetPos(0);
    m_syncProgressBar.SetStep(1);

    // thread creation
    AfxBeginThread(runSync, this);

    return TRUE;
}

LRESULT CSyncProgressDialog::OnSyncCompleted(WPARAM wParam, LPARAM lParam)
{
    m_currentOperationTitle = CString("������������� ���������");
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
    delete (CString *)wParam;

    return 1;
}

void CSyncProgressDialog::OnCancelCommand()
{
    if ((GetKeyState(VK_ESCAPE) & 0x8000) == 0)
        CDialogEx::OnCancel();
}


BEGIN_MESSAGE_MAP(CSyncProgressDialog, CDialogEx)
    ON_MESSAGE(WM_SHOW_SYNC_PROGRESS, OnShowSyncProgress)
    ON_MESSAGE(WM_SYNC_COMPLETED, OnSyncCompleted)
    ON_COMMAND(IDCANCEL, &CSyncProgressDialog::OnCancelCommand)
END_MESSAGE_MAP()
