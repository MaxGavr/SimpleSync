
// SimpleSync.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CSyncApp:
// О реализации данного класса см. SimpleSync.cpp
//

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
