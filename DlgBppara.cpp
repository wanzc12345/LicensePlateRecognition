// DlgBppara.cpp : implementation file
//

#include "stdafx.h"
#include "Photodeal.h"
#include "DlgBppara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgBppara dialog


DlgBppara::DlgBppara(CWnd* pParent /*=NULL*/)
	: CDialog(DlgBppara::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgBppara)
	m_hiddennum = 0;
	m_error = 0.0;
	m_relatpara = 0.0;
	m_pmeter = 0.0;
	//}}AFX_DATA_INIT
}


void DlgBppara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgBppara)
	DDX_Text(pDX, IDC_EDIT1, m_hiddennum);
	DDX_Text(pDX, IDC_EDIT2, m_error);
	DDX_Text(pDX, IDC_EDIT3, m_relatpara);
	DDX_Text(pDX, IDC_EDIT4, m_pmeter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgBppara, CDialog)
	//{{AFX_MSG_MAP(DlgBppara)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgBppara message handlers
