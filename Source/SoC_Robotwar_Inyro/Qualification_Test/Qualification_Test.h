
// Qualification_Test.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CQualification_TestApp:
// �� Ŭ������ ������ ���ؼ��� Qualification_Test.cpp�� �����Ͻʽÿ�.
//

class CQualification_TestApp : public CWinApp
{
public:
	CQualification_TestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CQualification_TestApp theApp;