// PhotodealView.h : interface of the CPhotodealView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHOTODEALVIEW_H__94B32159_9851_4DE2_8B70_B8F12421D8C7__INCLUDED_)
#define AFX_PHOTODEALVIEW_H__94B32159_9851_4DE2_8B70_B8F12421D8C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPhotodealView : public CView
{
protected: // create from serialization only
	CPhotodealView();
	DECLARE_DYNCREATE(CPhotodealView)

// Attributes
public:
	CPhotodealDoc* GetDocument();
	void Rectinication(HDIB hDIB,float fAngel);
	bool gyhinfoinput;
	bool gyhfinished;
	int  w_sample;
	int  h_sample;
	HDIB m_hDIBtemp;
	HDIB m_hDIBt;
    
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotodealView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPhotodealView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPhotodealView)
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);  // user message
	afx_msg void OnEnhance();
	afx_msg void OnFILE256ToGray();
	afx_msg void OnENHAMidianF();
	afx_msg void OnPointThre();
	afx_msg void OnEmendation();
	afx_msg void OnRestart();
	afx_msg void OnReload();
	afx_msg void OnInputInfo();
	afx_msg void OnPhotoCut();
	afx_msg void OnStandard();
	afx_msg void OnTie();
	afx_msg void OnOnonlyOnce();
	afx_msg void OnRecognize();
	afx_msg void OnDistill();
	afx_msg void OnChracTrain();
	afx_msg void OnNumTrain();
	afx_msg void OnLetterTrain();
	afx_msg void OnFramecut();
	afx_msg void OnFramecutx();
	afx_msg void OnFramecuty();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNumTrain2();
	afx_msg void OnLetterTrain2();
	afx_msg void OnChracTrain2();
	afx_msg void OnDistill2();
	afx_msg void OnRecognize2();
	afx_msg void OnRecognize3();
};

#ifndef _DEBUG  // debug version in PhotodealView.cpp
inline CPhotodealDoc* CPhotodealView::GetDocument()
   { return (CPhotodealDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTODEALVIEW_H__94B32159_9851_4DE2_8B70_B8F12421D8C7__INCLUDED_)
