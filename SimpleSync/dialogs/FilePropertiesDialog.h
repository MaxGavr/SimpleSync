#pragma once

#include "sync/SyncManager.h"
#include "afxwin.h"



// Incapsulates the minimum set of controls,
// that are required to show all file properties
// Tip: don't forget to connect variables with controls via DDX_* functions
class FilePropertiesControls
{
public:
    BOOL initialize(const FileProperties& file, const SyncManager* syncManager);

    const LPCTSTR m_timeFormat = _T("%d %b %Y ã., %X");

    CString m_filePath;
    CString m_fileSize;

    CString m_fileCreationTime;
    CString m_fileWriteTime;
    CString m_fileAccessTime;

    CButton m_hiddenControl;
    CButton m_readOnlyControl;
    CButton m_archivedControl;
    CButton m_systemControl;
};



class CFilePropertiesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFilePropertiesDialog)

public:
	CFilePropertiesDialog(const FileProperties& file,
                          const SyncManager* syncManager,
                          CWnd* pParent = NULL);
	virtual ~CFilePropertiesDialog();

    virtual BOOL OnInitDialog();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_PROPERTIES_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
    FileProperties m_file;
    const SyncManager* m_syncManager;

    FilePropertiesControls m_controls;
};
