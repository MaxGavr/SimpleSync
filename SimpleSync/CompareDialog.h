#pragma once

#include "afxwin.h"
#include "FileProperties.h"
#include "FilePropertiesDialog.h"

class SyncManager;



class CCompareFilesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareFilesDialog)

public:
	CCompareFilesDialog(const FileProperties& leftFile, const FileProperties& rightFile,
                        const SyncManager* syncManager,
                        CWnd* pParent = NULL);
	virtual ~CCompareFilesDialog();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_FILES_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
    const SyncManager* m_syncManager;

    FileProperties m_leftFile;
    FileProperties m_rightFile;

    FilePropertiesControls m_leftControls;
    FilePropertiesControls m_rightControls;

public:
    virtual BOOL OnInitDialog();
};
