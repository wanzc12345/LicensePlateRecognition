#if !defined(AFX_DLGINPUTINFO_H__D906B13B_F6EA_48AF_B9C3_BE06D2D8B9D0__INCLUDED_)
#define AFX_DLGINPUTINFO_H__D906B13B_F6EA_48AF_B9C3_BE06D2D8B9D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputinfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgInputinfo dialog

class DlgInputinfo : public CDialog
{
// Construction
public:
	DlgInputinfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgInputinfo)
	enum { IDD = IDD_DIALOG3 };
	int		m_width;
	int		m_height;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgInputinfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgInputinfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTINFO_H__D906B13B_F6EA_48AF_B9C3_BE06D2D8B9D0__INCLUDED_)
