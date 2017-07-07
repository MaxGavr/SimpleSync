#pragma once
#include "PreviewListCtrl.h"

class SyncManager;

class CMainDlg : public CDialogEx
{
// Создание
public:
	CMainDlg(SyncManager* syncManager, CWnd* pParent = NULL);

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLESYNC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

// Реализация
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
private:
    SyncManager* m_syncManager;

    CPreviewListCtrl m_previewList;
public:
    afx_msg void OnSourceFolderChange();
private:
    CString m_sourcePath;
    CString m_destinationPath;
public:
    afx_msg void OnDestinationFolderChange();
    afx_msg void OnPreviewButtonClicked();
};
