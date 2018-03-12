
// Huro_Competition.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHuro_CompetitionApp:
// See Huro_Competition.cpp for the implementation of this class
//

class CHuro_CompetitionApp : public CWinApp
{
public:
	CHuro_CompetitionApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHuro_CompetitionApp theApp;