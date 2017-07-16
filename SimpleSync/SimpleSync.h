#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ���#endif

#include "resource.h"



class CSyncApp : public CWinApp
{
public:
	CSyncApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CSyncApp theApp;
