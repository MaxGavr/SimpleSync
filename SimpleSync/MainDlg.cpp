#include "stdafx.h"
#include "SimpleSync.h"
#include "MainDlg.h"
#include "SyncManager.h"
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
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_EN_CHANGE(IDC_SOURCE_PATH_BROWSE, &CMainDlg::OnSourceFolderChange)
    ON_EN_CHANGE(IDC_DESTINATION_FOLDER_BROWSE, &CMainDlg::OnDestinationFolderChange)
    ON_BN_CLICKED(IDC_PREVIEW_BUTTON, &CMainDlg::OnPreviewButtonClicked)
    ON_BN_CLICKED(IDC_SYNC_BUTTON, &CMainDlg::OnSyncButtonClicked)
END_MESSAGE_MAP()


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

	// TODO: �������� �������������� �������������
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
