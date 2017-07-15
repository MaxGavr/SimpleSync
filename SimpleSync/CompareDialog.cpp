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
    m_equalImage.Load(IDB_EQUAL);
    m_greaterImage.Load(IDB_GREATER);
    m_lessImage.Load(IDB_LESS);
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


    DDX_Control(pDX, IDC_SIZE_BITMAP, m_sizeBitmap);
    DDX_Control(pDX, IDC_CREATION_TIME_BITMAP, m_creationTimeBitmap);
    DDX_Control(pDX, IDC_WRITE_TIME_BITMAP, m_writeTimeBitmap);
    DDX_Control(pDX, IDC_ACCESS_TIME_BITMAP, m_accessTimeBitmap);


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

    if (!m_syncManager->isFileInSourceFolder(m_leftFile))
        std::swap(m_leftFile, m_rightFile);

    if (m_leftControls.initialize(m_leftFile, m_syncManager) &&
        m_rightControls.initialize(m_rightFile, m_syncManager))
        UpdateData(FALSE);

    using File = FileProperties;
    File::COMPARISON_RESULT result;

    result = File::compareProperty(m_leftFile.getSize(),
                                   m_rightFile.getSize());
    m_sizeBitmap.SetBitmap(chooseImage(result));

    result = File::compareProperty(m_leftFile.getCreationTime(),
                                   m_rightFile.getCreationTime());
    m_creationTimeBitmap.SetBitmap(chooseImage(result));

    result = File::compareProperty(m_leftFile.getLastWriteTime(),
                                   m_rightFile.getLastWriteTime());
    m_writeTimeBitmap.SetBitmap(chooseImage(result));

    result = File::compareProperty(m_leftFile.getLastAccessTime(),
                                   m_rightFile.getLastAccessTime());
    m_accessTimeBitmap.SetBitmap(chooseImage(result));

    return TRUE;
}

CPngImage& CCompareFilesDialog::chooseImage(FileProperties::COMPARISON_RESULT result)
{
    switch (result)
    {
    case FileProperties::COMPARISON_RESULT::EQUAL:
        return m_equalImage;
    case FileProperties::COMPARISON_RESULT::PREFERABLE:
        return m_greaterImage;
    case FileProperties::COMPARISON_RESULT::NON_PREFERABLE:
        return m_lessImage;
    default:
        return m_equalImage;
    }
}
