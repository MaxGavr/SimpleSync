#include "stdafx.h"
#include "SimpleSync.h"
#include "MainDlg.h"
#include "OptionsDialog.h"
#include "ParamsDialog.h"
#include "ProgressDialog.h"
#include "ScanProgressDialog.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CMainDialog::CMainDialog(SyncManager* syncManager, CWnd* pParent)
	: CDialogEx(IDD_SIMPLESYNC_DIALOG, pParent),
      m_syncManager(syncManager),
      m_sourcePath(_T("")),
      m_destinationPath(_T("")),
      m_previewList(syncManager),
      m_directionRadioBox((int)syncManager->getSyncDirection())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_SIMPLE_SYNC_ICON);
}



void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PREVIEW_LIST, m_previewList);
    DDX_Text(pDX, IDC_SOURCE_PATH_BROWSE, m_sourcePath);
    DDX_Text(pDX, IDC_DESTINATION_FOLDER_BROWSE, m_destinationPath);
    DDX_Radio(pDX, IDC_DIRECTION_TO_RIGHT_BUTTON, m_directionRadioBox);
}



BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_EN_CHANGE(IDC_SOURCE_PATH_BROWSE, &CMainDialog::OnSourceFolderChange)
    ON_EN_CHANGE(IDC_DESTINATION_FOLDER_BROWSE, &CMainDialog::OnDestinationFolderChange)
    ON_CONTROL_RANGE(BN_CLICKED,
                     IDC_DIRECTION_TO_RIGHT_BUTTON,
                     IDC_DIRECTION_TO_LEFT_BUTTON,
                     &CMainDialog::OnDirectionButtonClicked)
    ON_BN_CLICKED(IDC_PREVIEW_BUTTON, &CMainDialog::OnPreviewButtonClicked)
    ON_BN_CLICKED(IDC_SYNC_BUTTON, &CMainDialog::OnSyncButtonClicked)
    ON_BN_CLICKED(IDC_OPTIONS_BUTTON, &CMainDialog::OnOptionsButtonClicked)
    ON_BN_CLICKED(IDC_PARAMETERS_BUTTON, &CMainDialog::OnParametersButtonClicked)
    ON_BN_CLICKED(IDC_HELP_BUTTON, &CMainDialog::OnHelpButtonClicked)
END_MESSAGE_MAP()



BOOL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_directionRightImage.Load(IDB_RIGHT_ARROW);
    m_directionBothImage.Load(IDB_LEFT_RIGHT_ARROW);
    m_directionLeftImage.Load(IDB_LEFT_ARROW);
    m_helpImage.Load(IDB_HELP);

    auto directionRight = (CButton*)GetDlgItem(IDC_DIRECTION_TO_RIGHT_BUTTON);
    directionRight->SetBitmap(m_directionRightImage);

    auto directionBoth = (CButton*)GetDlgItem(IDC_DIRECTION_BOTH_BUTTON);
    directionBoth->SetBitmap(m_directionBothImage);

    auto directionLeft = (CButton*)GetDlgItem(IDC_DIRECTION_TO_LEFT_BUTTON);
    directionLeft->SetBitmap(m_directionLeftImage);

    auto help = (CButton*)GetDlgItem(IDC_HELP_BUTTON);
    help->SetBitmap(m_helpImage);

    m_previewList.setupPreviewList();

	return TRUE;
}

void CMainDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialogEx::OnPaint();
}

HCURSOR CMainDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMainDialog::OnSourceFolderChange()
{
    UpdateData(TRUE);
    m_syncManager->setSourceFolder(m_sourcePath);
    m_previewList.clearPreview();
}

void CMainDialog::OnDestinationFolderChange()
{
    UpdateData(TRUE);
    m_syncManager->setDestinationFolder(m_destinationPath);
    m_previewList.clearPreview();
}

void CMainDialog::OnPreviewButtonClicked()
{
    CScanProgressDialog dialog(m_syncManager);
    dialog.DoModal();

    m_previewList.showPreview();
}

void CMainDialog::OnSyncButtonClicked()
{
    SyncManager::OperationQueue operations = m_syncManager->getOperationQueue();
    int operationsCount = operations.size();

    if (operationsCount == 0)
    {
        MessageBox(_T("Нечего синхронизировать!"),
                   _T("Отмена"), MB_ICONINFORMATION | MB_OK);
        return;
    }

    auto isAmbiguous = [](SyncOperation::ptr& op) -> BOOL {
        auto rOp = std::dynamic_pointer_cast<ReplaceOperation>(op);
        if (rOp)
            return rOp->isAmbiguous();
        return FALSE;
    };
    BOOL hasAmbiguous = std::any_of(operations.begin(), operations.end(), isAmbiguous);

    if (hasAmbiguous)
    {
        LPCTSTR msg = _T("Некоторые операции (помечены \"?\") "
                         "требуют Вашего решения, в противном случае  "
                         "они не будут выполнены. "
                         "Желаете начать синхронизацию?");
        LPCTSTR title = _T("Неоднозначные операции");
        int response = MessageBox(msg, title, MB_ICONEXCLAMATION | MB_YESNO);
        if (response == IDNO)
            return;
    }

    CSyncProgressDialog dialog(m_syncManager);
    dialog.DoModal();
}

void CMainDialog::OnDirectionButtonClicked(UINT nID)
{
    UpdateData(TRUE);
    m_syncManager->setSyncDirection((SyncManager::SYNC_DIRECTION)m_directionRadioBox);
}

void CMainDialog::OnOptionsButtonClicked()
{
    CSyncOptionsDialog optionsDialog(m_syncManager);

    if (optionsDialog.DoModal() == IDOK)
        m_syncManager->setOptions(optionsDialog.getOptions());
}

void CMainDialog::OnParametersButtonClicked()
{
    CCompParametersDialog parametersDialog(m_syncManager);

    if (parametersDialog.DoModal() == IDOK)
        m_syncManager->setComparisonParameters(parametersDialog.getParameters());
}


void CMainDialog::OnHelpButtonClicked()
{
    LPCWSTR msg = _T("Двойной клик на элементе списка позволяет "
                     "просмотреть свойства файла/ов.\n"
                     "Клик правой кнопкой - отмена операции.");
    LPWSTR title = _T("Помощь");
    MessageBox(msg, title, MB_ICONINFORMATION | MB_OK);
}
