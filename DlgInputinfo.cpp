// DlgInputinfo.cpp : implementation file
//

#include "stdafx.h"
#include "Photodeal.h"
#include "DlgInputinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgInputinfo dialog


DlgInputinfo::DlgInputinfo(CWnd* pParent /*=NULL*/)
	: CDialog(DlgInputinfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgInputinfo)
	m_width = 0;
	m_height = 0;
	//}}AFX_DATA_INIT
}


void DlgInputinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgInputinfo)
	DDX_Text(pDX, IDC_EDIT1, m_width);
	DDX_Text(pDX, IDC_EDIT2, m_height);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgInputinfo, CDialog)
	//{{AFX_MSG_MAP(DlgInputinfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgInputinfo message handlers
