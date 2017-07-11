#include "stdafx.h"
#include "SimpleSync.h"
#include "MainDlg.h"
#include "SyncManager.h"
#include "OptionsDialog.h"
#include "ParamsDialog.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CMainDlg::CMainDlg(SyncManager* syncManager, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SIMPLESYNC_DIALOG, pParent)
    , m_syncManager(syncManager)
    , m_sourcePath(_T(""))
    , m_destinationPath(_T(""))
    , m_previewList(syncManager)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PREVIEW_LIST, m_previewList);
    DDX_Text(pDX, IDC_SOURCE_PATH_BROWSE, m_sourcePath);
    DDX_Text(pDX, IDC_DESTINATION_FOLDER_BROWSE, m_destinationPath);
    DDX_Radio(pDX, IDC_DIRECTION_TO_RIGHT_BUTTON, m_directionRadioBox);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_EN_CHANGE(IDC_SOURCE_PATH_BROWSE, &CMainDlg::OnSourceFolderChange)
    ON_EN_CHANGE(IDC_DESTINATION_FOLDER_BROWSE, &CMainDlg::OnDestinationFolderChange)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_DIRECTION_TO_RIGHT_BUTTON, IDC_DIRECTION_TO_LEFT_BUTTON, &CMainDlg::OnDirectionButtonClicked)
    ON_BN_CLICKED(IDC_PREVIEW_BUTTON, &CMainDlg::OnPreviewButtonClicked)
    ON_BN_CLICKED(IDC_SYNC_BUTTON, &CMainDlg::OnSyncButtonClicked)
    ON_BN_CLICKED(IDC_OPTIONS_BUTTON, &CMainDlg::OnOptionsButtonClicked)
    ON_BN_CLICKED(IDC_PARAMETERS_BUTTON, &CMainDlg::OnParametersButtonClicked)
END_MESSAGE_MAP()


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_directionRadioBox = 0;
    UpdateData(FALSE);

    m_previewList.setupColumns();

	return TRUE;
}

void CMainDlg::OnPaint()
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
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMainDlg::OnSourceFolderChange()
{
    UpdateData(TRUE);
    m_syncManager->setSourceFolder(m_sourcePath);
}


void CMainDlg::OnDestinationFolderChange()
{
    UpdateData(TRUE);
    m_syncManager->setDestinationFolder(m_destinationPath);
}


void CMainDlg::OnPreviewButtonClicked()
{
    m_syncManager->scan();
    m_previewList.showPreview();
}


void CMainDlg::OnSyncButtonClicked()
{
    m_syncManager->sync();
}

void CMainDlg::OnDirectionButtonClicked(UINT nID)
{
    UpdateData(TRUE);
    m_syncManager->setSyncDirection((SyncManager::SYNC_DIRECTION)m_directionRadioBox);
}

void CMainDlg::OnOptionsButtonClicked()
{
    CSyncOptionsDialog optionsDialog(m_syncManager);

    if (optionsDialog.DoModal() == IDOK)
    {
        m_syncManager->setOptions(optionsDialog.getOptions());
    }
}


void CMainDlg::OnParametersButtonClicked()
{
    CCompareParamsDialog parametersDialog(m_syncManager);

    if (parametersDialog.DoModal() == IDOK)
    {
        m_syncManager->setComparisonParameters(parametersDialog.getParameters());
    }
}
