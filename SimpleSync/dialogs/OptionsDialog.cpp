#include "stdafx.h"
#include "../SimpleSync.h"
#include "OptionsDialog.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CSyncOptionsDialog, CDialogEx)

CSyncOptionsDialog::CSyncOptionsDialog(const SyncManager* syncManager,
                                       CWnd* pParent)
	: CDialogEx(IDD_OPTIONS_DIALOG, pParent)
{
    m_syncOptions = syncManager->getOptions();

    m_recursiveOption = m_syncOptions.recursive;
    m_deleteFilesOption = m_syncOptions.deleteFiles;
    m_copyMissingFilesOption = m_syncOptions.copyMissingFiles;
    m_syncHiddenFilesOption = m_syncOptions.syncHiddenFiles;
    m_createEmptyFoldersOption = m_syncOptions.createEmptyFolders;
}

CSyncOptionsDialog::~CSyncOptionsDialog()
{
}

SyncManagerOptions CSyncOptionsDialog::getOptions() const
{
    return m_syncOptions;
}

void CSyncOptionsDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_RECURSIVE_CHECK, m_recursiveOption);
    DDX_Check(pDX, IDC_DELETE_FILES_CHECK, m_deleteFilesOption);
    DDX_Check(pDX, IDC_COPY_MISSING_CHECK, m_copyMissingFilesOption);
    DDX_Check(pDX, IDC_HIDDEN_FILES_CHECK, m_syncHiddenFilesOption);
    DDX_Check(pDX, IDC_EMPTY_FOLDERS_CHECK, m_createEmptyFoldersOption);
}


BEGIN_MESSAGE_MAP(CSyncOptionsDialog, CDialogEx)
    ON_CONTROL_RANGE(BN_CLICKED,
                     IDC_RECURSIVE_CHECK,
                     IDC_EMPTY_FOLDERS_CHECK,
                     &CSyncOptionsDialog::OnOptionClicked)
END_MESSAGE_MAP()


void CSyncOptionsDialog::OnOptionClicked(UINT id)
{
    UpdateData(TRUE);

    switch (id)
    {
    case IDC_RECURSIVE_CHECK:
        m_syncOptions.recursive = m_recursiveOption;
        break;
    case IDC_DELETE_FILES_CHECK:
        m_syncOptions.deleteFiles = m_deleteFilesOption;
        break;
    case IDC_COPY_MISSING_CHECK:
        m_syncOptions.copyMissingFiles = m_copyMissingFilesOption;
        break;
    case IDC_HIDDEN_FILES_CHECK:
        m_syncOptions.syncHiddenFiles = m_syncHiddenFilesOption;
        break;
    case IDC_EMPTY_FOLDERS_CHECK:
        m_syncOptions.createEmptyFolders = m_createEmptyFoldersOption;
        break;
    }
}
