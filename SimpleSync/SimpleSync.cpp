#include "stdafx.h"
#include "SimpleSync.h"
#include "dialogs/MainDlg.h"
#include "sync/SyncManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CSyncApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CSyncApp::CSyncApp()
{
}


CSyncApp theApp;


BOOL CSyncApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    SetRegistryKey(_T("SimpleSync"));


    SyncManager syncManager;

	CMainDialog dialog(&syncManager);
	m_pMainWnd = &dialog;
	INT_PTR nResponse = dialog.DoModal();

	if (pShellManager != NULL)
		delete pShellManager;

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

