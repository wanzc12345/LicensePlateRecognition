// PhotodealView.cpp : implementation of the CPhotodealView class
//
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "Photodeal.h"
#include "DlgEhance.h"
#include "PointTrans.h"
#include "ColorTable.h"
#include "mainfrm.h"
#include "PhotodealDoc.h"
#include "PhotodealView.h"
#include "Dlgwhiteandblack.h"
#include "math.h"
#include "DlgInputinfo.h"
#include "DlgBppara.h"
#include "DlgSvmpara.h"
#include "Bp.h"
//#include "Cstring.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhotodealView

IMPLEMENT_DYNCREATE(CPhotodealView, CView)

BEGIN_MESSAGE_MAP(CPhotodealView, CView)
	//{{AFX_MSG_MAP(CPhotodealView)
	ON_COMMAND(ID_Enhance, OnEnhance)
	ON_COMMAND(ID_FILE_256ToGray, OnFILE256ToGray)
	ON_COMMAND(ID_ENHA_MidianF, OnENHAMidianF)
	ON_COMMAND(ID_POINT_THRE, OnPointThre)
	ON_COMMAND(ID_Emendation, OnEmendation)
	ON_COMMAND(ID_InputInfo, OnInputInfo)
	ON_COMMAND(ID_PhotoCut, OnPhotoCut)
	ON_COMMAND(ID_Standard, OnStandard)
	ON_COMMAND(ID_Tie, OnTie)
	ON_COMMAND(ID_OnonlyOnce, OnOnonlyOnce)
	ON_COMMAND(ID_Recognize, OnRecognize)
	ON_COMMAND(ID_Distill, OnDistill)
	ON_COMMAND(ID_ChracTrain, OnChracTrain)
	ON_COMMAND(ID_NumTrain, OnNumTrain)
	ON_COMMAND(ID_LetterTrain, OnLetterTrain)
	ON_COMMAND(ID_Framecutx, OnFramecutx)
	ON_COMMAND(ID_Framecuty, OnFramecuty)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID__32796, &CPhotodealView::OnNumTrain2)
	ON_COMMAND(ID__32797, &CPhotodealView::OnLetterTrain2)
	ON_COMMAND(ID__32800, &CPhotodealView::OnChracTrain2)
	ON_COMMAND(ID_32798, &CPhotodealView::OnDistill2)
	ON_COMMAND(ID_32799, &CPhotodealView::OnRecognize2)
	ON_COMMAND(ID_32801, &CPhotodealView::OnRecognize3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhotodealView construction/destruction

CPhotodealView::CPhotodealView()
{
	 gyhinfoinput=false;
     gyhfinished=false;
}

CPhotodealView::~CPhotodealView()
{
}

BOOL CPhotodealView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CPhotodealView::Rectinication(HDIB hDIB,float fAngel)
{
   #define PI 3.1415926
	//ѭ������
   long i0;
   long j0;
   long i1;
   long j1;
   float ffAngle;

   LONG lWidth;
   LONG lHeight;
   LONG lLineBytes;
   
   
   CPhotodealDoc* pDoc = GetDocument();
   // ָ��DIB��ָ��
	LPSTR lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;
   //����DIB
   	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
   //�ҵ�DIBͼ��������ʼλ��
   lpDIBBits = ::FindDIBBits(lpDIB);

   // �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧�ֲ鿴256ɫλͼ�Ҷ�ֱ��ͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
   // ���Ĺ����״
	BeginWaitCursor();
    //DIB�Ŀ��
	lWidth =::DIBWidth(lpDIB);
    //DIB�ĸ߶�
    lHeight =::DIBHeight(lpDIB);
    //����ͼ��ÿ�е��ֽ���
	lLineBytes =WIDTHBYTES(lWidth * 8);

	if(fAngel>90)
	{
		fAngel=180-fAngel;
	}
    //��ת�ǶȵĻ���
	ffAngle=(fAngel*PI/180);
    //�ǵ�������
	float fSinAngle=(float)sin(ffAngle);
    float fCosAngle=(float)cos(ffAngle);
	float fTgAngle=fSinAngle/fCosAngle;
	unsigned char* m_temp;
	m_temp=new unsigned char[lLineBytes*lHeight];

    //���ƿհ����ݵ��м仺��
	for (i0=0;i0<lLineBytes*lHeight;i0++)
		 m_temp[i0]=255;
    //�ȶ�X�������У������
    for (i0=0;i0<lHeight;i0++)
	{
		for(j0=0;j0<lWidth;j0++)
		{
            //����У���������λ��
			   //MessageBox("ok");
				j1=(LONG)((j0-lWidth/2)*fCosAngle+(i0-lHeight/2)*fSinAngle+lWidth/2+0.5f);
		    	i1=(LONG)(-(j0-lWidth/2)*fSinAngle+(i0-lHeight/2)*fCosAngle+lHeight/2+0.5f);
			

            //��ԭʼ���ظ��Ƶ�Ŀ��λ��000
			if(i1>=0&&i1<lHeight&&j1>=0&&j1<lWidth)
			{
				m_temp[lLineBytes*(lHeight-i1-1)+j1]=*(lpDIBBits+lLineBytes*(lHeight-i0-1)+j0);
            }
		}
	}
    //�ش洦������DIB
	for(i0=0;i0<lLineBytes*lHeight;i0++)
		*(lpDIBBits+i0)=m_temp[i0];

	/////////////////////////////////////////////////////////////////
    //������ֵ�˲�����ƽ������

    // ����DIB
/*	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ֵ�˲��������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ֵ�˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����MedianFilter()������ֵ�˲�
	if (::MedianFilter(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		  3, 1, 0, 1))
	{
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}*/
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	//EndWaitCursor();
	delete[]m_temp;
    //�ָ����
	EndWaitCursor();
}
	

/////////////////////////////////////////////////////////////////////////////
// CPhotodealView drawing

void CPhotodealView::OnDraw(CDC* pDC)
{
	// ��ʾ�ȴ����
	BeginWaitCursor();
	
	// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    
	
	// ��ȡDIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		
		// ��ȡDIB���
		int cxDIB = (int) ::DIBWidth(lpDIB);
		
		// ��ȡDIB�߶�
		int cyDIB = (int) ::DIBHeight(lpDIB);

		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// �ж��Ƿ��Ǵ�ӡ
		if (pDC->IsPrinting())
		{
			// �Ǵ�ӡ���������ͼ���λ�úʹ�С���Ա����ҳ��
			
			// ��ȡ��ӡҳ���ˮƽ���(����)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// ��ȡ��ӡҳ��Ĵ�ֱ�߶�(����)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// ��ȡ��ӡ��ÿӢ��������
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// �����ӡͼ���С�����ţ�����ҳ���ȵ���ͼ���С��
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// �����ӡͼ��λ�ã���ֱ���У�
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;

		}
		else   
		// �Ǵ�ӡ
		{
			// ��������ͼ��
			rcDest = rcDIB;
		}
		
		// ���DIB
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	
	// �ָ��������
	EndWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////
// CPhotodealView printing

BOOL CPhotodealView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPhotodealView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPhotodealView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPhotodealView diagnostics

#ifdef _DEBUG
void CPhotodealView::AssertValid() const
{
	CView::AssertValid();
}

void CPhotodealView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPhotodealDoc* CPhotodealView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhotodealDoc)));
	return (CPhotodealDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhotodealView message handlers
LRESULT CPhotodealView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);

	// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (pDoc->GetHDIB() == NULL)
	{
		// ֱ�ӷ���
		return 0L;
	}
	
	// ��ȡPalette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// ��ȡMainFrame
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);
		
		CClientDC appDC(pAppFrame);

		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		
		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tCCPhotodealView::OnPaletteChanged�е���SelectPalette()ʧ�ܣ�\n");
		}
	}
	
	return 0L;
}
//////////////////////////////////////////////////////
void CPhotodealView::OnEnhance() 
{
// �Ҷ�����
	
	// ��ȡ�ĵ�
    CPhotodealDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// �����Ի���
	DlgEhance dlgPara;
	
	// ��1����
	BYTE	bX1;
	BYTE	bY1;
	
	// ��2����
	BYTE	bX2;
	BYTE	bY2;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�ĻҶ����죬�����Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�ĻҶ����죡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ��ʼ������ֵ
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;
	
	// ��ʾ�Ի�����ʾ�û��趨����λ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	bX1 = dlgPara.m_bX1;
	bY1 = dlgPara.m_bY1;
	bX2 = dlgPara.m_bX2;
	bY2 = dlgPara.m_bY2;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����GrayStretch()�������лҶ�����
	GrayStretch(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bX1, bY1, bX2, bY2);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();	
}

void CPhotodealView::OnFILE256ToGray() 
{
// ��256ɫλͼת���ɻҶ�ͼ
	
	// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// ��ɫ���е���ɫ��Ŀ
	WORD wNumColors;
	
	// ��ȡDIB����ɫ���е���ɫ��Ŀ
	wNumColors = ::DIBNumColors(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ
	if (wNumColors != 256)
	{
		// ��ʾ�û�
		MessageBox("��256ɫλͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ָ��DIB���ص�ָ��
	BYTE *	lpSrc;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ͼ����
	LONG	lWidth;
	
	// ͼ��߶�
	LONG	lHeight;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;
	
	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWinStyleDIB;
	
	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// �Ҷ�ӳ���
	BYTE bMap[256];
	
	// �ж��Ƿ���WIN3.0��DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);
	
	// ����Ҷ�ӳ������������ɫ�ĻҶ�ֵ����������DIB��ɫ��
	for (i = 0; i < 256; i ++)
	{
		if (bWinStyleDIB)
		{
			// �������ɫ��Ӧ�ĻҶ�ֵ
			bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +
						     0.587 * lpbmi->bmiColors[i].rgbGreen +
					         0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);
			
			// ����DIB��ɫ���ɫ����
			lpbmi->bmiColors[i].rgbRed = i;
			
			// ����DIB��ɫ����ɫ����
			lpbmi->bmiColors[i].rgbGreen = i;
			
			// ����DIB��ɫ����ɫ���� 
			lpbmi->bmiColors[i].rgbBlue = i;
			
			// ����DIB��ɫ�屣��λ
			lpbmi->bmiColors[i].rgbReserved = 0;
		}
		else
		{
			// �������ɫ��Ӧ�ĻҶ�ֵ
			bMap[i] = (BYTE)(0.299 * lpbmc->bmciColors[i].rgbtRed +
						     0.587 * lpbmc->bmciColors[i].rgbtGreen +
					         0.114 * lpbmc->bmciColors[i].rgbtBlue + 0.5);
			
			// ����DIB��ɫ���ɫ����
			lpbmc->bmciColors[i].rgbtRed = i;
			
			// ����DIB��ɫ����ɫ����
			lpbmc->bmciColors[i].rgbtGreen = i;
			
			// ����DIB��ɫ����ɫ����
			lpbmc->bmciColors[i].rgbtBlue = i;
		}
	}

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// ��ȡͼ����
	lWidth = ::DIBWidth(lpDIB);
	
	// ��ȡͼ��߶�
	lHeight = ::DIBHeight(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ����ÿ�����ص���ɫ�����������ջҶ�ӳ����ɻҶ�ֵ��
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �任
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// �滻��ǰ��ɫ��Ϊ�Ҷȵ�ɫ��
	pDoc->GetDocPalette()->SetPaletteEntries(0, 256, (LPPALETTEENTRY) ColorsTable[0]);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ʵ���µĵ�ɫ��
	OnDoRealize((WPARAM)m_hWnd,0);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();	
}


void CPhotodealView::OnENHAMidianF() 
{
// ��ֵ�˲�
	
	// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ֵ�˲��������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ֵ�˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	
    // ���Ĺ����״
	BeginWaitCursor();
	
	// ����MedianFilter()������ֵ�˲�
	if (::MedianFilter(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		  3, 1, 0, 1))
	{
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();		
}

void CPhotodealView::OnPointThre() 
{
		// ��ֵ�任
	
	// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// �����Ի���
	Dlgwhiteandblack  dlgPara;
	
	// ��ֵ
	BYTE	bThre;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ֵ�任�������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ֵ�任��", "ϵͳ��ʾ" , 
			MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ��ʼ������ֵ
	dlgPara.m_bwb = 128;
	
	// ��ʾ�Ի�����ʾ�û��趨��ֵ
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨����ֵ
	bThre = dlgPara.m_bwb;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����ThresholdTrans()����������ֵ�任
	ThresholdTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bThre);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// �ָ����
	EndWaitCursor();
}

void CPhotodealView::OnEmendation() 
{
		//Hough����
    float angle;
    //char *str=new char ;
	// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();

	// ��ȡDIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;

	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��Hough�任�������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����㣡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����HoughDIB()������DIB
	if (HoughDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		/////������бУ����������У��
		angle=HoughDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB));//�����б�Ƕ�
		//itoa(angle,str,10);
		//MessageBox(str);
	   Rectinication(pDoc->GetHDIB(),angle);//������бУ������
		//}
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܻ���ͼ���к���0��255֮�������ֵ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
	
	
}

void CPhotodealView::OnRestart() 
{
	// TODO: Add your command handler code here
	
}


void CPhotodealView::OnInputInfo() 
{
	DlgInputinfo inputinfo;
	inputinfo.m_width =19;
	inputinfo.m_height =30;
	if(inputinfo.DoModal ()!=IDOK) return;
	w_sample=inputinfo.m_width;
	h_sample=inputinfo.m_height;
	gyhinfoinput=true;	
}
////�ַ��ָ�
void CPhotodealView::OnPhotoCut() 
{
	CPhotodealDoc* pDoc = GetDocument();
	m_charRect=CharSegment(pDoc->GetHDIB());
	//����Ļ����ʾλͼ
	CDC* pDC=GetDC();
	DisplayDIB(pDC,pDoc->GetHDIB());	
	DrawFrame(pDC,pDoc->GetHDIB(),m_charRect,2,RGB(20,60,200));
}

void CPhotodealView::OnStandard() 
{
	CPhotodealDoc* pDoc = GetDocument();
	StdDIBbyRect(pDoc->GetHDIB(),w_sample,h_sample);
	//����Ļ����ʾλͼ
	CDC* pDC=GetDC();
    HDIB m_hDIB=pDoc->GetHDIB();
    m_hDIBtemp=pDoc->GetHDIB();
	DisplayDIB(pDC,m_hDIB);	
	DrawFrame(pDC,m_hDIB,m_charRect,2,RGB(21,255,25));
	gyhfinished=true;
	
}

void CPhotodealView::OnTie() 
{
    CPhotodealDoc* pDoc = GetDocument();
     m_hDIBtemp=AutoAlign(m_hDIBtemp);
	//����Ļ����ʾλͼ

	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	DrawFrame(pDC,m_hDIBtemp,m_charRect,1,RGB(252,115,27));
		
    	
}

void CPhotodealView::OnOnonlyOnce() 
{
 	//OnEnhance();
    //OnFILE256ToGray();
    //OnENHAMidianF();
    //OnPointThre();
    //OnEmendation();
    OnFramecutx();
	OnFramecuty();
    OnInputInfo();
    OnPhotoCut();
    OnStandard();
    OnTie();
	
}

void CPhotodealView::OnRecognize() 
{
	
	
	CString str;
		//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	}
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���256ɫͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	
	// ��ȡ�����Ϣ
	int n[3];
	if(r_num(n,"num1")==false)
		return;
	//������������Ŀ
	int  n_in=n[0];
	//�����������Ŀ
	int  n_hidden=n[1];
	//������������Ŀ
	int  n_out=n[2];  
	
	//�жϴ�ʶ�������Ĺ�һ����Ϣ�Ƿ���ѵ��ʱ��ͬ
	if(n_in!=39)
	{
		//�������ͬ��ʾ���󲢷���
		::MessageBox(NULL,"��һ���ߴ�����һ��ѵ��ʱ��һ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//ָ����������������������ָ��  
	double **data_in;
	//�������ѵ����������ȡ��������
	data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	
	//������ȡ��������������ʶ��
    str=*CodeRecognize(data_in, digicount,n_in,n_hidden,n_out);
	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	CRect r;
	GetClientRect(&r);

	int x=0,y=r.bottom/2;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,"�������ַ�Ϊ��");
	x=100;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,str);

	
	
		
}

void CPhotodealView::OnDistill() 
{
	//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	}
	
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���Ҷ�ͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	all_data_in=alloc_2d_dbl(60,39);
	all_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	//��������������Ŀ
	
	ndigicount=digicount;
		double ** next_data_in;
	next_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	double ** n1all_data_in;
	n1all_data_in=alloc_2d_dbl(ndigicount+digicount,39);
 int i,j;
	for( i=0;i<ndigicount;i++)
		for( j=0;j<39;j++)
			n1all_data_in[i][j]=all_data_in[i][j];
for( i=0;i<digicount;i++)
		for( j=0;j<39;j++)
			n1all_data_in[i+ndigicount][j]=next_data_in[i][j];
		all_data_in=n1all_data_in;

	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	ndigicount+=digicount;
}

/*void CPhotodealView::OnDistill2() 
{

	//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	}
	
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���Ҷ�ͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	
	double ** next_data_in;
	next_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	double ** n1all_data_in;
	n1all_data_in=alloc_2d_dbl(ndigicount+digicount,39);
 int i,j;
	for( i=0;i<ndigicount;i++)
		for( j=0;j<39;j++)
			n1all_data_in[i][j]=all_data_in[i][j];
for( i=0;i<digicount;i++)
		for( j=0;j<39;j++)
			n1all_data_in[i+ndigicount][j]=next_data_in[i][j];
		all_data_in=n1all_data_in;

	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	ndigicount+=digicount;	
}*/

void CPhotodealView::OnChracTrain() 
{
	//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	} 
	
	//����BP����ѵ�������Ի���
	
	DlgBppara BpPa;
	
	//��ʼ������
	BpPa.m_relatpara=0;
	BpPa.m_pmeter=0.015;
	BpPa.m_error=0.0001;
	BpPa.m_hiddennum=15;
	
	// ��ʾ�Ի���
	if(BpPa.DoModal()!=IDOK)
	{
		//����
		return;
	}
	//�û���ò�����Ϣ
	
	//���ϵ��
	double  momentum=BpPa.m_relatpara; 
	//��С�������
	double  min_ex=BpPa.m_error; 
	//������Ŀ
	int  n_hidden=BpPa.m_hiddennum; 
	//ѵ������
	double eta=BpPa.m_pmeter;  
	
	
	
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL)m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���Ҷ�ͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	
	//ָ����������������������ָ��  
	double ** next2_data_in;
	next2_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	double ** n2all_data_in;
	n2all_data_in=alloc_2d_dbl(ndigicount+digicount,39);
    int i,j;
	for( i=0;i<ndigicount;i++)
		for( j=0;j<39;j++)
			n2all_data_in[i][j]=all_data_in[i][j];
for( i=0;i<digicount;i++)
		for( j=0;j<39;j++)
			n2all_data_in[i+ndigicount][j]=next2_data_in[i][j];
		all_data_in=n2all_data_in;
		ndigicount+=digicount;

	int n_in=39;
		double out[][4]={0.1,0.1,0.1,0.1,
		0.1,0.1,0.1,0.9,
		0.1,0.1,0.9,0.1,
		0.1,0.1,0.9,0.9,
		0.1,0.9,0.1,0.1,
		0.1,0.9,0.1,0.9,
		0.1,0.9,0.9,0.1,
		0.1,0.9,0.9,0.9,
		0.9,0.1,0.1,0.1,
		0.9,0.1,0.1,0.9};
	
	
	
	double **data_out;
	
	data_out = alloc_2d_dbl(ndigicount,4);
	
	for( i=0;i<ndigicount;i++)
	{
		for( j=0;j<4;j++)
			data_out[i][j]=out[i%3][j];
		
	}
	
	BpTrain1( all_data_in, data_out, n_in,n_hidden,min_ex,momentum,eta,ndigicount);
	

	ndigicount=0;
	free(data_out);
	
	
		
}

void CPhotodealView::OnNumTrain() 
{
	//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	} 
	
	//����BP����ѵ�������Ի���
	
	DlgBppara BpPa;
	
	//��ʼ������
	BpPa.m_relatpara=0;
	BpPa.m_pmeter=0.015;
	BpPa.m_error=0.0001;
	BpPa.m_hiddennum=15;
	
	// ��ʾ�Ի���
	if(BpPa.DoModal()!=IDOK)
	{
		//����
		return;
	}
	//�û���ò�����Ϣ
	
	//���ϵ��
	double  momentum=BpPa.m_relatpara; 
	//��С�������
	double  min_ex=BpPa.m_error; 
	//������Ŀ
	int  n_hidden=BpPa.m_hiddennum; 
	//ѵ������
	double eta=BpPa.m_pmeter;  
	
	
	
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL)m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���Ҷ�ͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	
	//ָ����������������������ָ��  
	double ** next2_data_in;
	next2_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	double ** n2all_data_in;
	n2all_data_in=alloc_2d_dbl(ndigicount+digicount,39);
    int i,j;
	for( i=0;i<ndigicount;i++)
		for( j=0;j<39;j++)
			n2all_data_in[i][j]=all_data_in[i][j];
   for( i=0;i<digicount;i++)
		for( j=0;j<39;j++)
			n2all_data_in[i+ndigicount][j]=next2_data_in[i][j];
		all_data_in=n2all_data_in;
		ndigicount+=digicount;

	int n_in=39;
	double out[][4]={0.1,0.1,0.1,0.1,
		0.1,0.1,0.1,0.9,
		0.1,0.1,0.9,0.1,
		0.1,0.1,0.9,0.9,
		0.1,0.9,0.1,0.1,
		0.1,0.9,0.1,0.9,
		0.1,0.9,0.9,0.1,
		0.1,0.9,0.9,0.9,
		0.9,0.1,0.1,0.1,
		0.9,0.1,0.1,0.9};
	
	
	
	double **data_out;
	
	data_out = alloc_2d_dbl(ndigicount,4);
	
	for( i=0;i<ndigicount;i++)
	{
		for( j=0;j<4;j++)
			data_out[i][j]=out[i%4][j];
		
	}
	
	BpTrain2( all_data_in, data_out, n_in,n_hidden,min_ex,momentum,eta,ndigicount);
	

	ndigicount=0;
	free(data_out);
	
	
	
	
}

void CPhotodealView::OnLetterTrain() 
{
		//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	} 
	
	//����BP����ѵ�������Ի���
	
	DlgBppara BpPa;
	
	//��ʼ������
	BpPa.m_relatpara=0;
	BpPa.m_pmeter=0.015;
	BpPa.m_error=0.0001;
	BpPa.m_hiddennum=15;
	
	// ��ʾ�Ի���
	if(BpPa.DoModal()!=IDOK)
	{
		//����
		return;
	}
	//�û���ò�����Ϣ
	
	//���ϵ��
	double  momentum=BpPa.m_relatpara; 
	//��С�������
	double  min_ex=BpPa.m_error; 
	//������Ŀ
	int  n_hidden=BpPa.m_hiddennum; 
	//ѵ������
	double eta=BpPa.m_pmeter;  
	
	
	
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL)m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���Ҷ�ͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	
	//ָ����������������������ָ��  
	double ** next2_data_in;
	next2_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	double ** n2all_data_in;
	n2all_data_in=alloc_2d_dbl(ndigicount+digicount,39);
    int i,j;
	for( i=0;i<ndigicount;i++)
		for( j=0;j<39;j++)
			n2all_data_in[i][j]=all_data_in[i][j];
for( i=0;i<digicount;i++)
		for( j=0;j<39;j++)
			n2all_data_in[i+ndigicount][j]=next2_data_in[i][j];
		all_data_in=n2all_data_in;
		ndigicount+=digicount;

	int n_in=39;
	double out[][4]={0.1,0.1,0.1,0.1,
		0.1,0.1,0.1,0.9,
		0.1,0.1,0.9,0.1,
		0.1,0.1,0.9,0.9,
		0.1,0.9,0.1,0.1,
		0.1,0.9,0.1,0.9,
		0.1,0.9,0.9,0.1,
		0.1,0.9,0.9,0.9,
		0.9,0.1,0.1,0.1,
		0.9,0.1,0.1,0.9};
	double **data_out;
	
	data_out = alloc_2d_dbl(ndigicount,4);
	
	for( i=0;i<ndigicount;i++)
	{
		for( j=0;j<4;j++)
			data_out[i][j]=out[i%4][j];
		
	}
	
	BpTrain3( all_data_in, data_out, n_in,n_hidden,min_ex,momentum,eta,ndigicount);
	

	ndigicount=0;
	free(data_out);
	
	
	
	
}

void CPhotodealView::OnFramecutx() 
{
	// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();
	HDIB m_hDIB;
	m_hDIB=pDoc->GetHDIB();
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
		
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ֵ�˲��������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ֵ�˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	
    // ���Ĺ����״
	BeginWaitCursor();
	
	Framecut1(lpDIBBits,lWidth ,lHeight);
	::GlobalUnlock(m_hDIB);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIB);
	
}

void CPhotodealView::OnFramecuty() 
{
// ��ȡ�ĵ�
	CPhotodealDoc* pDoc = GetDocument();
	HDIB m_hDIB;
	m_hDIB=pDoc->GetHDIB();
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
		
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ֵ�˲��������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ֵ�˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	
    // ���Ĺ����״
	BeginWaitCursor();
	
	Framecut2(lpDIBBits,lWidth ,lHeight);
	::GlobalUnlock(m_hDIB);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIB);	
}


void CPhotodealView::OnNumTrain2()
{
	// TODO:
	DlgSvmpara SvmPa;
	
	//��ʼ������
	SvmPa.m_sigma=0.1;
	SvmPa.m_punish=100;
	SvmPa.m_trainSvm=1;
	SvmPa.m_number=1;
	
	// ��ʾ�Ի���
	if(SvmPa.DoModal()!=IDOK)
	{
		//����
		return;
	}
}


void CPhotodealView::OnLetterTrain2()
{
	// TODO:
	DlgSvmpara SvmPa;
	
	//��ʼ������
	SvmPa.m_sigma=0.1;
	SvmPa.m_punish=100;
	SvmPa.m_trainSvm=1;
	SvmPa.m_number=1;
	
	// ��ʾ�Ի���
	if(SvmPa.DoModal()!=IDOK)
	{
		//����
		return;
	}
}


void CPhotodealView::OnChracTrain2()
{
	// TODO:
	DlgSvmpara SvmPa;
	
	//��ʼ������
	SvmPa.m_sigma=0.1;
	SvmPa.m_punish=100;
	SvmPa.m_trainSvm=1;
	SvmPa.m_number=1;
	
	// ��ʾ�Ի���
	if(SvmPa.DoModal()!=IDOK)
	{
		//����
		return;
	}
}


void CPhotodealView::OnDistill2()
{
	// TODO:
	//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	}
	
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���Ҷ�ͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	all_data_in=alloc_2d_dbl(60,39);
	all_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	//��������������Ŀ
	
	ndigicount=digicount;
		double ** next_data_in;
	next_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	double ** n1all_data_in;
	n1all_data_in=alloc_2d_dbl(ndigicount+digicount,39);
 int i,j;
	for( i=0;i<ndigicount;i++)
		for( j=0;j<39;j++)
			n1all_data_in[i][j]=all_data_in[i][j];
for( i=0;i<digicount;i++)
		for( j=0;j<39;j++)
			n1all_data_in[i+ndigicount][j]=next_data_in[i][j];
		all_data_in=n1all_data_in;

	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	ndigicount+=digicount;
}


void CPhotodealView::OnRecognize2()
{
	// TODO:
	CString str;
		//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	}
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���256ɫͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	
	// ��ȡ�����Ϣ
	int n[3];
	if(r_num(n,"num1")==false)
		return;
	//������������Ŀ
	int  n_in=n[0];
	//�����������Ŀ
	int  n_hidden=n[1];
	//������������Ŀ
	int  n_out=n[2];  
	
	//�жϴ�ʶ�������Ĺ�һ����Ϣ�Ƿ���ѵ��ʱ��ͬ
	if(n_in!=39)
	{
		//�������ͬ��ʾ���󲢷���
		::MessageBox(NULL,"��һ���ߴ�����һ��ѵ��ʱ��һ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//ָ����������������������ָ��  
	double **data_in;
	//�������ѵ����������ȡ��������
	data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	
	//������ȡ��������������ʶ��
    str=*CodeRecognize(data_in, digicount,n_in,n_hidden,n_out);
	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	CRect r;
	GetClientRect(&r);

	int x=0,y=r.bottom/2;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,"�������ַ�Ϊ��");
	x=100;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,str);

	
}


void CPhotodealView::OnRecognize3()
{
	// TODO:
	CString str;
		//�ж��Ƿ񾭹��˹�һ���Ĵ���
	if(gyhfinished==false)
	{
		//���û�н�����ʾ���󲢷���
		::MessageBox(NULL,"û�н��й�һ��Ԥ����",NULL,MB_ICONSTOP);
		return;
	}
	//���ָ��DIB��ָ��
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//���ָ��DIB���ص�ָ�룬��ָ�����ص���ʼλ��
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//�����ɫ��Ϣ
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//���ǻҶ�ͼ����
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"ֻ�ܴ���256ɫͼ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//��ȡͼ��Ŀ��
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//��ȡͼ��ĸ߶�
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//��һ���Ŀ��
	LONG lSwidth = w_sample;
	
	//��һ���ĸ߶�
	LONG LSheight = h_sample;
	
	// ��ȡ�����Ϣ
	int n[3];
	if(r_num(n,"num1")==false)
		return;
	//������������Ŀ
	int  n_in=n[0];
	//�����������Ŀ
	int  n_hidden=n[1];
	//������������Ŀ
	int  n_out=n[2];  
	
	//�жϴ�ʶ�������Ĺ�һ����Ϣ�Ƿ���ѵ��ʱ��ͬ
	if(n_in!=39)
	{
		//�������ͬ��ʾ���󲢷���
		::MessageBox(NULL,"��һ���ߴ�����һ��ѵ��ʱ��һ��",NULL,MB_ICONSTOP);
		return;
	}
	
	//ָ����������������������ָ��  
	double **data_in;
	//�������ѵ����������ȡ��������
	data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	
	//������ȡ��������������ʶ��
    str=*CodeRecognize(data_in, digicount,n_in,n_hidden,n_out);
	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	CRect r;
	GetClientRect(&r);

	int x=0,y=r.bottom/2;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,"�������ַ�Ϊ��");
	x=100;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,str);

	
}