#pragma once

#include "FileProperties.h"
#include "afxwin.h"

class SyncManager;



class FilePropertiesControls
{
public:
    BOOL initialize(const FileProperties& file, const SyncManager* syncManager);

    const LPCTSTR m_timeFormat = _T("%d %b %Y ã., %X");

    CString m_filePath;
    ULONGLONG m_fileSize;

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

public:
    virtual BOOL OnInitDialog();
};
