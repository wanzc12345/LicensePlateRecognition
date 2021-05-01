// Photodeal.h : main header file for the PHOTODEAL application
//

#if !defined(AFX_PHOTODEAL_H__2A71C0E2_2984_4351_8454_F3110C508ECF__INCLUDED_)
#define AFX_PHOTODEAL_H__2A71C0E2_2984_4351_8454_F3110C508ECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPhotodealApp:
// See Photodeal.cpp for the implementation of this class
//

class CPhotodealApp : public CWinApp
{
public:
	CPhotodealApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotodealApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPhotodealApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTODEAL_H__2A71C0E2_2984_4351_8454_F3110C508ECF__INCLUDED_)
