#pragma once
#include "controls/PreviewListCtrl.h"

class SyncManager;

class CMainDlg : public CDialogEx
{
public:
	CMainDlg(SyncManager* syncManager, CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLESYNC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

private:
    SyncManager* m_syncManager;
    CString m_sourcePath;
    CString m_destinationPath;

    int m_directionRadioBox;
    CPreviewListCtrl m_previewList;

    CPngImage m_directionRightImage;
    CPngImage m_directionBothImage;
    CPngImage m_directionLeftImage;

    CPngImage m_helpImage;

public:
    afx_msg void OnSourceFolderChange();
    afx_msg void OnDestinationFolderChange();

    afx_msg void OnPreviewButtonClicked();
    afx_msg void OnSyncButtonClicked();

    afx_msg void OnDirectionButtonClicked(UINT nID);
    afx_msg void OnHelpButtonClicked();

    afx_msg void OnOptionsButtonClicked();
    afx_msg void OnParametersButtonClicked();
};
