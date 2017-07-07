#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"



class CSyncApp : public CWinApp
{
public:
	CSyncApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	DECLARE_MESSAGE_MAP()
};

extern CSyncApp theApp;
