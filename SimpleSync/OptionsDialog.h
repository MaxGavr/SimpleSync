#pragma once

#include "SyncManager.h"




class CSyncOptionsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSyncOptionsDialog)

public:
	CSyncOptionsDialog(const SyncManager* syncManager, CWnd* pParent = NULL);
	virtual ~CSyncOptionsDialog();

    SyncManagerOptions getOptions() const;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnOptionClicked(UINT id);

private:
    BOOL m_recursiveOption;
    BOOL m_deleteFilesOption;
    BOOL m_copyMissingFilesOption;
    BOOL m_syncHiddenFilesOption;
    BOOL m_createEmptyFoldersOption;

    SyncManagerOptions m_syncOptions;
};
