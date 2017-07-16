#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define WM_SHOW_SYNC_PROGRESS (WM_USER + 100)
#define WM_SYNC_COMPLETED (WM_USER + 101)

class SyncManager;
class SyncOperation;

class CSyncProgressDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSyncProgressDialog)

public:
	CSyncProgressDialog(SyncManager* syncManager, CWnd* pParent = NULL);
	virtual ~CSyncProgressDialog();

    static UINT runSync(LPVOID pParam);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYNC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
    void showOperationProgress(const SyncOperation* operation);

    SyncManager* m_syncManager;
    CString m_currentOperationTitle;
    CProgressCtrl m_syncProgressBar;

public:
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnShowSyncProgress(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSyncCompleted(WPARAM wParam, LPARAM lParam);
};
