#include "stdafx.h"
#include "../SimpleSync.h"
#include "FilePropertiesDialog.h"
#include "../sync/SyncManager.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CFilePropertiesDialog, CDialogEx)

CFilePropertiesDialog::CFilePropertiesDialog(const FileProperties& file,
                                             const SyncManager* syncManager,
                                             CWnd* pParent)
    : CDialogEx(IDD_FILE_PROPERTIES_DIALOG, pParent),
      m_file(file),
      m_syncManager(syncManager)
{
}

CFilePropertiesDialog::~CFilePropertiesDialog()
{
}

void CFilePropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_RELATIVE_PATH_EDIT, m_controls.m_filePath);
    DDX_Text(pDX, IDC_FILE_SIZE_EDIT, m_controls.m_fileSize);
    DDX_Text(pDX, IDC_CREATION_TIME_EDIT, m_controls.m_fileCreationTime);
    DDX_Text(pDX, IDC_WRITE_TIME_EDIT, m_controls.m_fileWriteTime);
    DDX_Text(pDX, IDC_ACCESS_TIME_EDIT, m_controls.m_fileAccessTime);

    DDX_Control(pDX, IDC_HIDDEN_CHECK, m_controls.m_hiddenControl);
    DDX_Control(pDX, IDC_READONLY_CHECK, m_controls.m_readOnlyControl);
    DDX_Control(pDX, IDC_ARCHIVED_CHECK, m_controls.m_archivedControl);
    DDX_Control(pDX, IDC_SYSTEM_CHECK, m_controls.m_systemControl);
}


BEGIN_MESSAGE_MAP(CFilePropertiesDialog, CDialogEx)
END_MESSAGE_MAP()


BOOL CFilePropertiesDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (m_controls.initialize(m_file, m_syncManager))
        UpdateData(FALSE);

    return TRUE;
}

BOOL FilePropertiesControls::initialize(const FileProperties& file, const SyncManager* syncManager)
{
    if (!syncManager)
        return FALSE;

    m_filePath = syncManager->getFileRelativePath(file, TRUE);
    
    WCHAR fileSizeStr[255];
    StrFormatByteSize(file.getSize(), fileSizeStr, 255);
    m_fileSize = fileSizeStr;

    m_fileCreationTime = file.getCreationTime().Format(m_timeFormat);
    m_fileWriteTime = file.getLastWriteTime().Format(m_timeFormat);
    m_fileAccessTime = file.getLastAccessTime().Format(m_timeFormat);

    m_archivedControl.SetCheck(file.isArchived());
    m_hiddenControl.SetCheck(file.isHidden());
    m_systemControl.SetCheck(file.isSystem());
    m_readOnlyControl.SetCheck(file.isReadOnly());

    return TRUE;
}
