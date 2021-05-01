// DlgBppara.cpp : implementation file
//

#include "stdafx.h"
#include "Photodeal.h"
#include "DlgSvmpara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgBppara dialog


DlgSvmpara::DlgSvmpara(CWnd* pParent /*=NULL*/)
	: CDialog(DlgSvmpara::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgBppara)
	m_sigma = 0;
	m_punish = 0;
	m_trainSvm = 0.0;
	m_number = 0;
	//}}AFX_DATA_INIT
}


void DlgSvmpara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgBppara)
	DDX_Text(pDX, IDC_EDIT1, m_sigma);
	DDX_Text(pDX, IDC_EDIT2, m_punish);
	DDX_Text(pDX, IDC_EDIT3, m_trainSvm);
	DDX_Text(pDX, IDC_EDIT4, m_number);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgSvmpara, CDialog)
	//{{AFX_MSG_MAP(DlgBppara)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgBppara message handlers
