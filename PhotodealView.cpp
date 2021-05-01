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
	//循环变量
   long i0;
   long j0;
   long i1;
   long j1;
   float ffAngle;

   LONG lWidth;
   LONG lHeight;
   LONG lLineBytes;
   
   
   CPhotodealDoc* pDoc = GetDocument();
   // 指向DIB的指针
	LPSTR lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
   //锁定DIB
   	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
   //找到DIB图像象素起始位置
   lpDIBBits = ::FindDIBBits(lpDIB);

   // 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持查看256色位图灰度直方图！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
   // 更改光标形状
	BeginWaitCursor();
    //DIB的宽度
	lWidth =::DIBWidth(lpDIB);
    //DIB的高度
    lHeight =::DIBHeight(lpDIB);
    //计算图像每行的字节数
	lLineBytes =WIDTHBYTES(lWidth * 8);

	if(fAngel>90)
	{
		fAngel=180-fAngel;
	}
    //旋转角度的弧度
	ffAngle=(fAngel*PI/180);
    //角的正余旋
	float fSinAngle=(float)sin(ffAngle);
    float fCosAngle=(float)cos(ffAngle);
	float fTgAngle=fSinAngle/fCosAngle;
	unsigned char* m_temp;
	m_temp=new unsigned char[lLineBytes*lHeight];

    //复制空白数据到中间缓存
	for (i0=0;i0<lLineBytes*lHeight;i0++)
		 m_temp[i0]=255;
    //先对X方向进行校正处理
    for (i0=0;i0<lHeight;i0++)
	{
		for(j0=0;j0<lWidth;j0++)
		{
            //计算校正后的坐标位置
			   //MessageBox("ok");
				j1=(LONG)((j0-lWidth/2)*fCosAngle+(i0-lHeight/2)*fSinAngle+lWidth/2+0.5f);
		    	i1=(LONG)(-(j0-lWidth/2)*fSinAngle+(i0-lHeight/2)*fCosAngle+lHeight/2+0.5f);
			

            //将原始象素复制到目标位置000
			if(i1>=0&&i1<lHeight&&j1>=0&&j1<lWidth)
			{
				m_temp[lLineBytes*(lHeight-i1-1)+j1]=*(lpDIBBits+lLineBytes*(lHeight-i0-1)+j0);
            }
		}
	}
    //回存处理结果到DIB
	for(i0=0;i0<lLineBytes*lHeight;i0++)
		*(lpDIBBits+i0)=m_temp[i0];

	/////////////////////////////////////////////////////////////////
    //调用中值滤波进行平滑处理

    // 锁定DIB
/*	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的中值滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用MedianFilter()函数中值滤波
	if (::MedianFilter(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		  3, 1, 0, 1))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}*/
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	//EndWaitCursor();
	delete[]m_temp;
    //恢复光标
	EndWaitCursor();
}
	

/////////////////////////////////////////////////////////////////////////////
// CPhotodealView drawing

void CPhotodealView::OnDraw(CDC* pDC)
{
	// 显示等待光标
	BeginWaitCursor();
	
	// 获取文档
	CPhotodealDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    
	
	// 获取DIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		
		// 获取DIB宽度
		int cxDIB = (int) ::DIBWidth(lpDIB);
		
		// 获取DIB高度
		int cyDIB = (int) ::DIBHeight(lpDIB);

		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// 判断是否是打印
		if (pDC->IsPrinting())
		{
			// 是打印，计算输出图像的位置和大小，以便符合页面
			
			// 获取打印页面的水平宽度(象素)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// 获取打印页面的垂直高度(象素)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// 获取打印机每英寸象素数
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// 计算打印图像大小（缩放，根据页面宽度调整图像大小）
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// 计算打印图像位置（垂直居中）
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;

		}
		else   
		// 非打印
		{
			// 不必缩放图像
			rcDest = rcDIB;
		}
		
		// 输出DIB
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	
	// 恢复正常光标
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

	// 获取文档
	CPhotodealDoc* pDoc = GetDocument();
	
	// 判断DIB是否为空
	if (pDoc->GetHDIB() == NULL)
	{
		// 直接返回
		return 0L;
	}
	
	// 获取Palette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// 获取MainFrame
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
			TRACE0("\tCCPhotodealView::OnPaletteChanged中调用SelectPalette()失败！\n");
		}
	}
	
	return 0L;
}
//////////////////////////////////////////////////////
void CPhotodealView::OnEnhance() 
{
// 灰度拉伸
	
	// 获取文档
    CPhotodealDoc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 创建对话框
	DlgEhance dlgPara;
	
	// 点1坐标
	BYTE	bX1;
	BYTE	bY1;
	
	// 点2坐标
	BYTE	bX2;
	BYTE	bY2;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的灰度拉伸，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的灰度拉伸！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 初始化变量值
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;
	
	// 显示对话框，提示用户设定拉伸位置
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	bX1 = dlgPara.m_bX1;
	bY1 = dlgPara.m_bY1;
	bX2 = dlgPara.m_bX2;
	bY2 = dlgPara.m_bY2;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用GrayStretch()函数进行灰度拉伸
	GrayStretch(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bX1, bY1, bX2, bY2);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	
}

void CPhotodealView::OnFILE256ToGray() 
{
// 将256色位图转换成灰度图
	
	// 获取文档
	CPhotodealDoc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 颜色表中的颜色数目
	WORD wNumColors;
	
	// 获取DIB中颜色表中的颜色数目
	wNumColors = ::DIBNumColors(lpDIB);
	
	// 判断是否是8-bpp位图
	if (wNumColors != 256)
	{
		// 提示用户
		MessageBox("非256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 指向DIB象素的指针
	BYTE *	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像宽度
	LONG	lWidth;
	
	// 图像高度
	LONG	lHeight;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO lpbmc;
	
	// 表明是否是Win3.0 DIB的标记
	BOOL bWinStyleDIB;
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// 灰度映射表
	BYTE bMap[256];
	
	// 判断是否是WIN3.0的DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);
	
	// 计算灰度映射表（保存各个颜色的灰度值），并更新DIB调色板
	for (i = 0; i < 256; i ++)
	{
		if (bWinStyleDIB)
		{
			// 计算该颜色对应的灰度值
			bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +
						     0.587 * lpbmi->bmiColors[i].rgbGreen +
					         0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);
			
			// 更新DIB调色板红色分量
			lpbmi->bmiColors[i].rgbRed = i;
			
			// 更新DIB调色板绿色分量
			lpbmi->bmiColors[i].rgbGreen = i;
			
			// 更新DIB调色板蓝色分量 
			lpbmi->bmiColors[i].rgbBlue = i;
			
			// 更新DIB调色板保留位
			lpbmi->bmiColors[i].rgbReserved = 0;
		}
		else
		{
			// 计算该颜色对应的灰度值
			bMap[i] = (BYTE)(0.299 * lpbmc->bmciColors[i].rgbtRed +
						     0.587 * lpbmc->bmciColors[i].rgbtGreen +
					         0.114 * lpbmc->bmciColors[i].rgbtBlue + 0.5);
			
			// 更新DIB调色板红色分量
			lpbmc->bmciColors[i].rgbtRed = i;
			
			// 更新DIB调色板绿色分量
			lpbmc->bmciColors[i].rgbtGreen = i;
			
			// 更新DIB调色板蓝色分量
			lpbmc->bmciColors[i].rgbtBlue = i;
		}
	}

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// 获取图像宽度
	lWidth = ::DIBWidth(lpDIB);
	
	// 获取图像高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 更换每个象素的颜色索引（即按照灰度映射表换成灰度值）
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 变换
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// 替换当前调色板为灰度调色板
	pDoc->GetDocPalette()->SetPaletteEntries(0, 256, (LPPALETTEENTRY) ColorsTable[0]);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 实现新的调色板
	OnDoRealize((WPARAM)m_hWnd,0);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	
}


void CPhotodealView::OnENHAMidianF() 
{
// 中值滤波
	
	// 获取文档
	CPhotodealDoc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的中值滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
    // 更改光标形状
	BeginWaitCursor();
	
	// 调用MedianFilter()函数中值滤波
	if (::MedianFilter(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		  3, 1, 0, 1))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();		
}

void CPhotodealView::OnPointThre() 
{
		// 阈值变换
	
	// 获取文档
	CPhotodealDoc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 参数对话框
	Dlgwhiteandblack  dlgPara;
	
	// 阈值
	BYTE	bThre;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的阈值变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的阈值变换！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 初始化变量值
	dlgPara.m_bwb = 128;
	
	// 显示对话框，提示用户设定阈值
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的阈值
	bThre = dlgPara.m_bwb;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用ThresholdTrans()函数进行阈值变换
	ThresholdTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bThre);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
}

void CPhotodealView::OnEmendation() 
{
		//Hough运算
    float angle;
    //char *str=new char ;
	// 获取文档
	CPhotodealDoc* pDoc = GetDocument();

	// 获取DIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的Hough变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用HoughDIB()函数对DIB
	if (HoughDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		/////调用倾斜校正函数进行校正
		angle=HoughDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB));//获得倾斜角度
		//itoa(angle,str,10);
		//MessageBox(str);
	   Rectinication(pDoc->GetHDIB(),angle);//调用倾斜校正函数
		//}
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
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
////字符分割
void CPhotodealView::OnPhotoCut() 
{
	CPhotodealDoc* pDoc = GetDocument();
	m_charRect=CharSegment(pDoc->GetHDIB());
	//在屏幕上显示位图
	CDC* pDC=GetDC();
	DisplayDIB(pDC,pDoc->GetHDIB());	
	DrawFrame(pDC,pDoc->GetHDIB(),m_charRect,2,RGB(20,60,200));
}

void CPhotodealView::OnStandard() 
{
	CPhotodealDoc* pDoc = GetDocument();
	StdDIBbyRect(pDoc->GetHDIB(),w_sample,h_sample);
	//在屏幕上显示位图
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
	//在屏幕上显示位图

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
		//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	}
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理256色图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	
	// 读取结点信息
	int n[3];
	if(r_num(n,"num1")==false)
		return;
	//获得输入层结点数目
	int  n_in=n[0];
	//获得隐层结点数目
	int  n_hidden=n[1];
	//获得输出层结点数目
	int  n_out=n[2];  
	
	//判断待识别样本的归一划信息是否与训练时相同
	if(n_in!=39)
	{
		//如果不相同提示错误并返回
		::MessageBox(NULL,"归一划尺寸与上一次训练时不一致",NULL,MB_ICONSTOP);
		return;
	}
	
	//指向输入样本的特征向量的指针  
	double **data_in;
	//从输入的训练样本中提取特征向量
	data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	
	//根据提取的特征进行样本识别
    str=*CodeRecognize(data_in, digicount,n_in,n_hidden,n_out);
	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	CRect r;
	GetClientRect(&r);

	int x=0,y=r.bottom/2;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,"■车牌字符为：");
	x=100;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,str);

	
	
		
}

void CPhotodealView::OnDistill() 
{
	//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	}
	
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	all_data_in=alloc_2d_dbl(60,39);
	all_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	//计算输入层结点的数目
	
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

	//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	}
	
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
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
	//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	} 
	
	//建立BP网络训练参数对话框
	
	DlgBppara BpPa;
	
	//初始化变量
	BpPa.m_relatpara=0;
	BpPa.m_pmeter=0.015;
	BpPa.m_error=0.0001;
	BpPa.m_hiddennum=15;
	
	// 显示对话框
	if(BpPa.DoModal()!=IDOK)
	{
		//返回
		return;
	}
	//用户获得参数信息
	
	//相关系数
	double  momentum=BpPa.m_relatpara; 
	//最小均方误差
	double  min_ex=BpPa.m_error; 
	//隐层数目
	int  n_hidden=BpPa.m_hiddennum; 
	//训练步长
	double eta=BpPa.m_pmeter;  
	
	
	
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL)m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	
	//指向输入样本的特征向量的指针  
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
	//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	} 
	
	//建立BP网络训练参数对话框
	
	DlgBppara BpPa;
	
	//初始化变量
	BpPa.m_relatpara=0;
	BpPa.m_pmeter=0.015;
	BpPa.m_error=0.0001;
	BpPa.m_hiddennum=15;
	
	// 显示对话框
	if(BpPa.DoModal()!=IDOK)
	{
		//返回
		return;
	}
	//用户获得参数信息
	
	//相关系数
	double  momentum=BpPa.m_relatpara; 
	//最小均方误差
	double  min_ex=BpPa.m_error; 
	//隐层数目
	int  n_hidden=BpPa.m_hiddennum; 
	//训练步长
	double eta=BpPa.m_pmeter;  
	
	
	
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL)m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	
	//指向输入样本的特征向量的指针  
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
		//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	} 
	
	//建立BP网络训练参数对话框
	
	DlgBppara BpPa;
	
	//初始化变量
	BpPa.m_relatpara=0;
	BpPa.m_pmeter=0.015;
	BpPa.m_error=0.0001;
	BpPa.m_hiddennum=15;
	
	// 显示对话框
	if(BpPa.DoModal()!=IDOK)
	{
		//返回
		return;
	}
	//用户获得参数信息
	
	//相关系数
	double  momentum=BpPa.m_relatpara; 
	//最小均方误差
	double  min_ex=BpPa.m_error; 
	//隐层数目
	int  n_hidden=BpPa.m_hiddennum; 
	//训练步长
	double eta=BpPa.m_pmeter;  
	
	
	
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL)m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	
	//指向输入样本的特征向量的指针  
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
	// 获取文档
	CPhotodealDoc* pDoc = GetDocument();
	HDIB m_hDIB;
	m_hDIB=pDoc->GetHDIB();
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
		
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的中值滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
    // 更改光标形状
	BeginWaitCursor();
	
	Framecut1(lpDIBBits,lWidth ,lHeight);
	::GlobalUnlock(m_hDIB);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIB);
	
}

void CPhotodealView::OnFramecuty() 
{
// 获取文档
	CPhotodealDoc* pDoc = GetDocument();
	HDIB m_hDIB;
	m_hDIB=pDoc->GetHDIB();
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
		
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的中值滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
    // 更改光标形状
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
	
	//初始化变量
	SvmPa.m_sigma=0.1;
	SvmPa.m_punish=100;
	SvmPa.m_trainSvm=1;
	SvmPa.m_number=1;
	
	// 显示对话框
	if(SvmPa.DoModal()!=IDOK)
	{
		//返回
		return;
	}
}


void CPhotodealView::OnLetterTrain2()
{
	// TODO:
	DlgSvmpara SvmPa;
	
	//初始化变量
	SvmPa.m_sigma=0.1;
	SvmPa.m_punish=100;
	SvmPa.m_trainSvm=1;
	SvmPa.m_number=1;
	
	// 显示对话框
	if(SvmPa.DoModal()!=IDOK)
	{
		//返回
		return;
	}
}


void CPhotodealView::OnChracTrain2()
{
	// TODO:
	DlgSvmpara SvmPa;
	
	//初始化变量
	SvmPa.m_sigma=0.1;
	SvmPa.m_punish=100;
	SvmPa.m_trainSvm=1;
	SvmPa.m_number=1;
	
	// 显示对话框
	if(SvmPa.DoModal()!=IDOK)
	{
		//返回
		return;
	}
}


void CPhotodealView::OnDistill2()
{
	// TODO:
	//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	}
	
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理灰度图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	all_data_in=alloc_2d_dbl(60,39);
	all_data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	//计算输入层结点的数目
	
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
		//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	}
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理256色图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	
	// 读取结点信息
	int n[3];
	if(r_num(n,"num1")==false)
		return;
	//获得输入层结点数目
	int  n_in=n[0];
	//获得隐层结点数目
	int  n_hidden=n[1];
	//获得输出层结点数目
	int  n_out=n[2];  
	
	//判断待识别样本的归一划信息是否与训练时相同
	if(n_in!=39)
	{
		//如果不相同提示错误并返回
		::MessageBox(NULL,"归一划尺寸与上一次训练时不一致",NULL,MB_ICONSTOP);
		return;
	}
	
	//指向输入样本的特征向量的指针  
	double **data_in;
	//从输入的训练样本中提取特征向量
	data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	
	//根据提取的特征进行样本识别
    str=*CodeRecognize(data_in, digicount,n_in,n_hidden,n_out);
	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	CRect r;
	GetClientRect(&r);

	int x=0,y=r.bottom/2;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,"■车牌字符为：");
	x=100;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,str);

	
}


void CPhotodealView::OnRecognize3()
{
	// TODO:
	CString str;
		//判断是否经过了归一划的处理
	if(gyhfinished==false)
	{
		//如果没有进行提示错误并返回
		::MessageBox(NULL,"没有进行归一划预处理",NULL,MB_ICONSTOP);
		return;
	}
	//获得指向DIB的指针
	BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) m_hDIBtemp);
	
	//获得指向DIB象素的指针，并指向象素的起始位置
	BYTE *lpDIBBits =(BYTE*)::FindDIBBits((char *)lpDIB);
	
	//获得颜色信息
	int numColors=(int) ::DIBNumColors((char *)lpDIB);
	//不是灰度图返回
    if (numColors!=256) 
	{
		::GlobalUnlock((HGLOBAL) m_hDIBtemp);
		::MessageBox(NULL,"只能处理256色图像",NULL,MB_ICONSTOP);
		return;
	}
	
	//获取图像的宽度
    LONG lWidth = (LONG) ::DIBWidth((char *)lpDIB); 
	
	//获取图像的高度
	LONG lHeight = (LONG) ::DIBHeight((char *)lpDIB);
	
	//计算图像每行的字节数
	LONG lLineByte = (lWidth+3)/4*4; 
	
	//归一化的宽度
	LONG lSwidth = w_sample;
	
	//归一化的高度
	LONG LSheight = h_sample;
	
	// 读取结点信息
	int n[3];
	if(r_num(n,"num1")==false)
		return;
	//获得输入层结点数目
	int  n_in=n[0];
	//获得隐层结点数目
	int  n_hidden=n[1];
	//获得输出层结点数目
	int  n_out=n[2];  
	
	//判断待识别样本的归一划信息是否与训练时相同
	if(n_in!=39)
	{
		//如果不相同提示错误并返回
		::MessageBox(NULL,"归一划尺寸与上一次训练时不一致",NULL,MB_ICONSTOP);
		return;
	}
	
	//指向输入样本的特征向量的指针  
	double **data_in;
	//从输入的训练样本中提取特征向量
	data_in = TZTQ_24( lpDIBBits, digicount,  lLineByte, lSwidth);
	
	//根据提取的特征进行样本识别
    str=*CodeRecognize(data_in, digicount,n_in,n_hidden,n_out);
	::GlobalUnlock(m_hDIBtemp);
	
	CDC* pDC=GetDC();
	DisplayDIB(pDC,m_hDIBtemp);
	CRect r;
	GetClientRect(&r);

	int x=0,y=r.bottom/2;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,"■车牌字符为：");
	x=100;
	//pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
	pDC->TextOut(x,y,str);

	
}