#if !defined(AFX_DLGEHANCE_H__4EDD2172_21AF_4316_920F_D71FB02E018E__INCLUDED_)
#define AFX_DLGEHANCE_H__4EDD2172_21AF_4316_920F_D71FB02E018E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEhance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgEhance dialog

class DlgEhance : public CDialog
{
// Construction
public:
	CPoint m_p2;
	CPoint m_p1;
	CRect m_MouseRect;
	int m_iIsDraging;
	BOOL m_bDrawed;
	DlgEhance(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgEhance)
	enum { IDD = IDD_DIALOG1 };
	BYTE	m_bX1;
	BYTE	m_bX2;
	BYTE	m_bY2;
	BYTE	m_bY1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEhance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgEhance)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit2();
	afx_msg void OnKillfocusEdit3();
	afx_msg void OnKillfocusEdit4();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEHANCE_H__4EDD2172_21AF_4316_920F_D71FB02E018E__INCLUDED_)
