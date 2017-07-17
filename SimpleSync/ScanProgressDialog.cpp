#include "stdafx.h"
#include "SimpleSync.h"
#include "ScanProgressDialog.h"
#include "SyncManager.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CScanProgressDialog, CDialogEx)

CScanProgressDialog::CScanProgressDialog(SyncManager* syncManager,
                                         CWnd* pParent)
	: CDialogEx(IDD_SCAN_PROGRESS_DIALOG, pParent),
      m_syncManager(syncManager)
{
}

CScanProgressDialog::~CScanProgressDialog()
{
}

UINT CScanProgressDialog::runScan(LPVOID pParam)
{
    auto dialog = (CScanProgressDialog*)pParam;

    SyncManager::ScanCallback callback = [dialog](const CString& folder) {
        dialog->showScanProgress(folder);
    };

    BOOL result = dialog->m_syncManager->scan(&callback);
    dialog->m_scanResult = result;

    dialog->PostMessage(WM_SCAN_COMPLETED);

    return 0;
}

void CScanProgressDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SCAN_STATUS_STATIC, m_currentFolderTitle);
    DDX_Control(pDX, IDC_SCAN_PROGRESS, m_scanProgressBar);
}

void CScanProgressDialog::showScanProgress(const CString& folder)
{
    CString* title = new CString(_T(""));
    
    title->Format(_T("Сканируется %s"), folder);

    PostMessage(WM_SHOW_SCAN_PROGRESS, (WPARAM)title);
}

BOOL CScanProgressDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // thread creation
    AfxBeginThread(runScan, this);

    return TRUE;
}

LRESULT CScanProgressDialog::OnShowScanProgress(WPARAM wParam, LPARAM lParam)
{
    m_scanProgressBar.SetMarquee(TRUE, 10);

    m_currentFolderTitle = *((CString *)wParam);
    UpdateData(FALSE);
    delete (CString *)wParam;

    return 1;
}

LRESULT CScanProgressDialog::OnScanCompleted(WPARAM wParam, LPARAM lParam)
{
    m_currentFolderTitle = CString("Cканирование завершено");
    UpdateData(FALSE);

    if (!m_scanResult)
    {
        MessageBox(_T("Невозможно провести сканирование!\n"
                      "Убедитесь, что обе директории выбраны."),
                   _T("Ошибка"), MB_ICONERROR | MB_OK);
    }
    else
    {
        int operationsCount = m_syncManager->getOperations().size();
        if (operationsCount == 0)
        {
            MessageBox(_T("Сканирование не выявило никаких возможных операций.\n"),
                       _T("Синхронизация"), MB_ICONINFORMATION | MB_OK);
        }
    }

    SendMessage(WM_COMMAND, IDCANCEL);


    return 1;
}

void CScanProgressDialog::OnCancelCommand()
{
    if ((GetKeyState(VK_ESCAPE) & 0x8000) == 0)
        CDialogEx::OnCancel();
}


BEGIN_MESSAGE_MAP(CScanProgressDialog, CDialogEx)
    ON_MESSAGE(WM_SHOW_SCAN_PROGRESS, OnShowScanProgress)
    ON_MESSAGE(WM_SCAN_COMPLETED, OnScanCompleted)
    ON_COMMAND(IDCANCEL, &CScanProgressDialog::OnCancelCommand)
END_MESSAGE_MAP()


