#include "stdafx.h"
#include "SimpleSync.h"
#include "CompareDialog.h"
#include "SyncManager.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CCompareFilesDialog, CDialogEx)

CCompareFilesDialog::CCompareFilesDialog(const FileProperties& leftFile,
                                         const FileProperties& rightFile,
                                         const SyncManager* syncManager,
                                         CWnd* pParent)
	: CDialogEx(IDD_COMPARE_FILES_DIALOG, pParent),
      m_leftFile(leftFile),
      m_rightFile(rightFile),
      m_syncManager(syncManager)
{
}

CCompareFilesDialog::~CCompareFilesDialog()
{
}

void CCompareFilesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_PATH_TO_FILES_EDIT, m_leftControls.m_filePath);
    DDX_Text(pDX, IDC_LEFT_SIZE_EDIT, m_leftControls.m_fileSize);
    DDX_Text(pDX, IDC_LEFT_CREATION_TIME_EDIT, m_leftControls.m_fileCreationTime);
    DDX_Text(pDX, IDC_LEFT_WRITE_TIME_EDIT, m_leftControls.m_fileWriteTime);
    DDX_Text(pDX, IDC_LEFT_ACCESS_TIME_EDIT, m_leftControls.m_fileAccessTime);

    DDX_Control(pDX, IDC_LEFT_HIDDEN_CHECK, m_leftControls.m_hiddenControl);
    DDX_Control(pDX, IDC_LEFT_READONLY_CHECK, m_leftControls.m_readOnlyControl);
    DDX_Control(pDX, IDC_LEFT_ARCHIVED_CHECK, m_leftControls.m_archivedControl);
    DDX_Control(pDX, IDC_LEFT_SYSTEM_CHECK, m_leftControls.m_systemControl);


    DDX_Text(pDX, IDC_RIGHT_SIZE_EDIT, m_rightControls.m_fileSize);
    DDX_Text(pDX, IDC_RIGHT_CREATION_TIME_EDIT, m_rightControls.m_fileCreationTime);
    DDX_Text(pDX, IDC_RIGHT_WRITE_TIME_EDIT, m_rightControls.m_fileWriteTime);
    DDX_Text(pDX, IDC_RIGHT_ACCESS_TIME_EDIT, m_rightControls.m_fileAccessTime);

    DDX_Control(pDX, IDC_RIGHT_HIDDEN_CHECK, m_rightControls.m_hiddenControl);
    DDX_Control(pDX, IDC_RIGHT_READONLY_CHECK, m_rightControls.m_readOnlyControl);
    DDX_Control(pDX, IDC_RIGHT_ARCHIVED_CHECK, m_rightControls.m_archivedControl);
    DDX_Control(pDX, IDC_RIGHT_SYSTEM_CHECK, m_rightControls.m_systemControl);
}


BEGIN_MESSAGE_MAP(CCompareFilesDialog, CDialogEx)
END_MESSAGE_MAP()


BOOL CCompareFilesDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (m_leftControls.initialize(m_leftFile, m_syncManager) &&
        m_rightControls.initialize(m_rightFile, m_syncManager))
        UpdateData(FALSE);

    return TRUE;
}
