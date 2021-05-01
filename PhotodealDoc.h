// PhotodealDoc.h : interface of the CPhotodealDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "DIBAPI.H"



class CPhotodealDoc : public CDocument
{
protected: // create from serialization only
	CPhotodealDoc();
	DECLARE_DYNCREATE(CPhotodealDoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotodealDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nColorIndex;
	void ReplaceHDIB(HDIB hDIB);
	void InitDIBData();
	virtual ~CPhotodealDoc();
	COLORREF m_refColorBKG;		// ±³¾°É«
	HDIB m_hDIB;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;


// Generated message map functions
protected:
	//{{AFX_MSG(CPhotodealDoc)
	afx_msg void OnReload();
	afx_msg void OnReloadMagine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//#endif // !defined(AFX_PHOTODEALDOC_H__FB0C29E3_3498_4F0A_85E6_BF875F52B9D7__INCLUDED_)
