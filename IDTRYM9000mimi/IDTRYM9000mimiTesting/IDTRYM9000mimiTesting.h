
// IDTRYM9000mimiTesting.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIDTRYM9000mimiTestingApp:
// �йش����ʵ�֣������ IDTRYM9000mimiTesting.cpp
//

class CIDTRYM9000mimiTestingApp : public CWinApp
{
public:
	CIDTRYM9000mimiTestingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIDTRYM9000mimiTestingApp theApp;