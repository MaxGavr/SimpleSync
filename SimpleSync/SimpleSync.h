
// SimpleSync.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CSyncApp:
// � ���������� ������� ������ ��. SimpleSync.cpp
//

class CSyncApp : public CWinApp
{
public:
	CSyncApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CSyncApp theApp;
