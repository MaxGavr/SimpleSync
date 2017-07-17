#pragma once
#include "afxcmn.h"

#define WM_SHOW_SCAN_PROGRESS (WM_USER + 200)
#define WM_SCAN_COMPLETED (WM_USER + 201)

class SyncManager;



class CScanProgressDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CScanProgressDialog)

public:
	CScanProgressDialog(SyncManager* syncManager, CWnd* pParent = NULL);
	virtual ~CScanProgressDialog();

    // Run in separate worker thread
    static UINT runScan(LPVOID pParam);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCAN_PROGRESS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
    // Called in callback that is passed to SyncManager::scan()
    void showScanProgress(const CString& folder);

    SyncManager* m_syncManager;
    BOOL m_scanResult;

    CString m_currentFolderTitle;
    CProgressCtrl m_scanProgressBar;

public:
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnShowScanProgress(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnScanCompleted(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCancelCommand();
};
