#if !defined(AFX_DLGWHITEANDBLACK_H__3310B9ED_740E_4ED8_9701_A023D06830E6__INCLUDED_)
#define AFX_DLGWHITEANDBLACK_H__3310B9ED_740E_4ED8_9701_A023D06830E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlgwhiteandblack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dlgwhiteandblack dialog

class Dlgwhiteandblack : public CDialog
{
// Construction
public:
	CRect m_MouseRect;
	BOOL m_bIsDraging;
	Dlgwhiteandblack(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dlgwhiteandblack)
	enum { IDD = IDD_DIALOG2 };
	BYTE	m_bwb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dlgwhiteandblack)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dlgwhiteandblack)
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWHITEANDBLACK_H__3310B9ED_740E_4ED8_9701_A023D06830E6__INCLUDED_)
