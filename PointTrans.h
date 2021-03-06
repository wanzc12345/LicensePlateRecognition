// PointTrans.h
//#include <syslib.h>
#include <stdlib.h>
#include "stdafx.h"
#include "DIBAPI.h"
#include <math.h>
#include <direct.h>
#include <iostream>
#include <deque>
using namespace std;

#ifndef _INC_PointTransAPI
#define _INC_PointTransAPI



typedef deque<CRect> CRectLink;
typedef deque<HDIB>  HDIBLink;
// 函数原型
int digicount;
//HDIB m_hDIB;
CRectLink m_charRect;
CRectLink m_charRectCopy;
HDIB AutoAlign(HDIB hDIB);
BOOL WINAPI ThresholdTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre);
BOOL WINAPI GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2);
BOOL WINAPI ReplaceColorPal(LPSTR lpDIB, BYTE * bpColorsTable);
BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int iFilterH, int iFilterW, 
						 int iFilterMX, int iFilterMY);
//HDIB WINAPI NewDIB(long width, long height,unsigned short biBitCount); 
unsigned char WINAPI GetMedianNum(unsigned char * bArray, int iFilterLen);
float HoughDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
//对位图进行分割.返回一个存储着每块分割区域的链表
CRectLink CharSegment(HANDLE hDIB);
void DisplayDIB(CDC* pDC,HDIB hDIB);
//画框
void DrawFrame(CDC* pDC,HDIB hDIB, CRectLink charRect,unsigned int linewidth,COLORREF color);
void DisplayDIB(CDC* pDC,HDIB hDIB);
void ClearAll(CDC *pDC);
void Framecut1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
void Framecut2(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
#endif //!_INC_PointTransAPI


typedef struct{
	int Value;
	int Dist;
	int AngleNumber;
}	MaxValue;





/*************************************************************************
 *
 * 函数名称：
 *   ThresholdTrans()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   BYTE  bThre	    - 阈值
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行阈值变换。对于灰度值小于阈值的象素直接设置
 * 灰度值为0；灰度值大于阈值的象素直接设置为255。
 *
 ************************************************************************/
BOOL WINAPI ThresholdTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 判断是否小于阈值
			if ((*lpSrc) < bThre)
			{
				// 直接赋值为0
				*lpSrc = 255;
			}
			else
			{
				// 直接赋值为255
				*lpSrc = 0;
			}
		}
	}
	
	// 返回
	return TRUE;
}
//////////////////////////边框切除
void Framecut1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

		// 指向源图像的指针
	unsigned char*	lpSrc;
	int* Num1=new int[lHeight];
	//int* Num2=new int[lWidth];
	int lheight;
	//int lwidth;

	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	for(i=0;i<=lHeight;i++)//统计每行黑点个数
	{
		int temp=0;//黑点个数
		for(j=0;j<=lWidth;j++)
		{
           // 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			if(*lpSrc==0)
			{
				temp++;
			}
		}
		*(Num1+i)=temp;
	}
	for(i=0;i<=lHeight;i++)
	{
		if(*(Num1+i)<35)
		{
			*(Num1+i)=0;
		}
	}
	for(i=2*lHeight/5;i>=0;i--)
	{
	   if(*(Num1+i)!=0&&*(Num1+i-1)==0)
	   {
		   lheight=i-1;
		   break;
	   }
	}
	for(i=lheight;i>=0;i--)
	{
		for(j=0;j<=lWidth;j++)
		   {
			   // 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			*lpSrc=255;
		   }
	}

	for(i=3*lHeight/5;i<=lHeight;i++)
	{
	   if(*(Num1+i)!=0&&*(Num1+i+1)==0)
	   {
		   lheight=i+1;
		   break;
	   }
	}
	for(i=lheight;i<=lHeight;i++)
	{
		for(j=0;j<=lWidth;j++)
		   {
			   // 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			*lpSrc=255;
		   }
	}
}

void Framecut2(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
		// 指向源图像的指针
	unsigned char*	lpSrc;
	int* Num2=new int[lWidth];
	int lwidth;

	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);	


    for(i=0;i<lWidth;i++)//统计每列黑点个数
	{
		int temp=0;//黑点个数
		for(j=0;j<lHeight;j++)
		{
           // 指向DIB第i列，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
			if(*lpSrc==0)
			{
				temp++;
			}
		}
		*(Num2+i)=temp;
	}
    for(i=0;i<=lWidth;i++)
	{
		if(*(Num2+i)<5)
		{
			*(Num2+i)=0;
		}
	}
	for(i=lWidth/15;i>=0;i--)
	{
		if(*(Num2+i)!=0&&*(Num2+i-1)==0)
		{
			lwidth=i-1;
			break;
		}
	}

    for(i=lwidth;i<=lWidth;i++)
	{
		if(*(Num2+i)==0&&*(Num2+i+1)!=0)
		{
			lwidth=i;
			break;
		}
	}
	for(i=lwidth;i>=0;i--)
	{
		for(j=0;j<=lHeight;j++)
		   {
			   // 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
			*lpSrc=255;
		   }
	}

	for(i=14*lWidth/15;i<=lWidth;i++)
	{
	   if(*(Num2+i)!=0&&*(Num2+i+1)==0)
	   {
		   lwidth=i+1;
		   break;
	   }
	}
	for(i=lwidth;i<=lWidth;i++)
	{
		for(j=0;j<=lHeight;j++)
		   {
			   // 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
			*lpSrc=255;
		   }
	}
	for(i=2*lWidth/7;i<=3*lWidth/7;i++)
	{
		if(*(Num2+i)==0)
		{
    		for(j=0;j<=lHeight;j++)
			{
               lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
		       *lpSrc=255;
			}
		}
	}
}
	






/*************************************************************************
 *
 * 函数名称：
 *   GetMedianNum()
 *
 * 参数:
 *   unsigned char * bpArray	- 指向要获取中值的数组指针
 *   int   iFilterLen			- 数组长度
 *
 * 返回值:
 *   unsigned char      - 返回指定数组的中值。
 *
 * 说明:
 *   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
 *
 ************************************************************************/

unsigned char WINAPI GetMedianNum(unsigned char * bArray, int iFilterLen)
{
	// 循环变量
	int		i;
	int		j;
	
	// 中间变量
	unsigned char bTemp;
	
	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// 互换
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}
	
	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}
	
	// 返回中值
	return bTemp;
}

/*************************************************************************
 *
 * 函数名称：
 *   GrayStretch()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   BYTE bX1			- 灰度拉伸第一个点的X坐标
 *   BYTE bY1			- 灰度拉伸第一个点的Y坐标
 *   BYTE bX2			- 灰度拉伸第二个点的X坐标
 *   BYTE bY2			- 灰度拉伸第二个点的Y坐标
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行灰度拉伸。
 *
 ************************************************************************/
BOOL WINAPI GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 灰度映射表
	BYTE	bMap[256];
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 计算灰度映射表
	for (i = 0; i <= bX1; i++)
	{
		// 判断bX1是否大于0（防止分母为0）
		if (bX1 > 0)
		{
			// 线性变换
			bMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// 直接赋值为0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// 判断bX1是否等于bX2（防止分母为0）
		if (bX2 != bX1)
		{
			// 线性变换
			bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// 直接赋值为bY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)
	{
		// 判断bX2是否等于255（防止分母为0）
		if (bX2 != 255)
		{
			// 线性变换
			bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// 直接赋值为255
			bMap[i] = 255;
		}
	}
	
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 计算新的灰度值
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   ReplaceColorPal()
 *
 * 参数:
 *   LPSTR lpDIB			- 指向源DIB图像指针
 *   BYTE * bpColorsTable	- 伪彩色编码表
 *
 * 返回值:
 *   BOOL					- 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用指定的伪彩色编码表来替换图像的调试板，参数bpColorsTable
 * 指向要替换的伪彩色编码表。
 *
 ************************************************************************/
BOOL WINAPI ReplaceColorPal(LPSTR lpDIB, BYTE * bpColorsTable)
{
	
	// 循环变量
	int i;
	
	// 颜色表中的颜色数目
	WORD wNumColors;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO lpbmc;
	
	// 表明是否是Win3.0 DIB的标记
	BOOL bWinStyleDIB;
	
	// 创建结果
	BOOL bResult = FALSE;
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// 获取DIB中颜色表中的颜色数目
	wNumColors = ::DIBNumColors(lpDIB);
		
	// 判断颜色数目是否是256色
	if (wNumColors == 256)
	{
		
		// 判断是否是WIN3.0的DIB
		bWinStyleDIB = IS_WIN30_DIB(lpDIB);
		
		// 读取伪彩色编码，更新DIB调色板
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// 更新DIB调色板红色分量
				lpbmi->bmiColors[i].rgbRed = bpColorsTable[i * 4];
				
				// 更新DIB调色板绿色分量
				lpbmi->bmiColors[i].rgbGreen = bpColorsTable[i * 4 + 1];
				
				// 更新DIB调色板蓝色分量
				lpbmi->bmiColors[i].rgbBlue = bpColorsTable[i * 4 + 2];
				
				// 更新DIB调色板保留位
				lpbmi->bmiColors[i].rgbReserved = 0;
			}
			else
			{
				// 更新DIB调色板红色分量
				lpbmc->bmciColors[i].rgbtRed = bpColorsTable[i * 4];
				
				// 更新DIB调色板绿色分量
				lpbmc->bmciColors[i].rgbtGreen = bpColorsTable[i * 4 + 1];
				
				// 更新DIB调色板蓝色分量
				lpbmc->bmciColors[i].rgbtBlue = bpColorsTable[i * 4 + 2];
			}
		}
	}
	
	// 返回
	return bResult;	

}
/*************************************************************************
 *
 * 函数名称：
 *   MedianFilter()
 *
 * 参数:
 *   LPSTR lpDIBBits		- 指向源DIB图像指针
 *   LONG  lWidth			- 源图像宽度（象素数）
 *   LONG  lHeight			- 源图像高度（象素数）
 *   int   iFilterH			- 滤波器的高度
 *   int   iFilterW			- 滤波器的宽度
 *   int   iFilterMX		- 滤波器的中心元素X坐标
 *   int   iFilterMY		- 滤波器的中心元素Y坐标
 *
 * 返回值:
 *   BOOL					- 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数对DIB图像进行中值滤波。
 *
 ************************************************************************/

BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int iFilterH, int iFilterW, 
						 int iFilterMX, int iFilterMY)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 指向要复制区域的指针
	unsigned char*	lpDst;
	
	// 指向复制图像的指针
	LPSTR			lpNewDIBBits;
	HLOCAL			hNewDIBBits;
	
	// 指向滤波器数组的指针
	unsigned char	* aValue;
	HLOCAL			hArray;
	
	// 循环变量
	LONG			i;
	LONG			j;
	LONG			k;
	LONG			l;
	
	// 图像每行的字节数
	LONG			lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	// 暂时分配内存，以保存滤波器数组
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);
	
	// 判断是否内存分配失败
	if (hArray == NULL)
	{
		// 释放内存
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);
		
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	aValue = (unsigned char * )LocalLock(hArray);
	
	// 开始中值滤波
	// 行(除去边缘几行)
	for(i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// 列(除去边缘几列)
		for(j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 读取滤波器数组
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// 指向DIB第i - iFilterMY + k行，第j - iFilterMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;
					
					// 保存象素值
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}
			
			// 获取中值
			* lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
		}
	}
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);
	
	// 返回
	return TRUE;
}
/*************************************************************************
 *
 * 函数名称：
 *   HoughDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 * 返回值:
 *   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对检测图像中的平行直线。如果图像中有两条平行的直线，则将这两条平行直线
 * 提取出来。
 * 
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

float HoughDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	float Anglenumber;
	float Anglenumber1;
	int Anglenumber2;
	#define pi 3.1415926
	// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向变换域的指针
	LPSTR   lpTrans;

	// 图像每行的字节数
	LONG lLineBytes;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//指向变换域的指针
	LPSTR	lpTransArea;
	HLOCAL	hTransArea;

	//变换域的尺寸
	int iMaxDist;
	int iMaxAngleNumber;

	//变换域的坐标
	int iDist;
	int iAngleNumber;
	
	//循环变量
	long i;
	long j;

	//像素值
	unsigned char pixel;

	//存储变换域中的最大值
	MaxValue MaxValue1;
	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	//计算变换域的尺寸
	//最大距离
	iMaxDist = (int) sqrt((double)(lWidth*lWidth + lHeight*lHeight));

	//角度从0－180，每格1度
	iMaxAngleNumber = 180;

	//为变换域分配内存
	hTransArea = LocalAlloc(LHND, lWidth * lHeight * sizeof(int));

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpTransArea = (char * )LocalLock(hTransArea);
		
	// 初始化新分配的内存，设定初始值为0
	lpTrans = (char *)lpTransArea;
	memset(lpTrans, 0, lWidth * lHeight * sizeof(int));

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && *lpSrc != 0)
				return FALSE;

			//如果是黑点，则在变换域的对应各点上加1
			if(pixel == 0)
			{
				
				for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
				{
					iDist = (int) fabs(i*cos(iAngleNumber*pi/180.0) + \
						j*sin(iAngleNumber*pi/180.0));
				
					//变换域的对应点上加1
					*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber) = \
						*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber) +1;
				}
			}
		
		}
	}
				
	//找到变换域中的两个最大值点
	MaxValue1.Value=0;
	//找到第一个最大值点
	for (iDist=0; iDist<iMaxDist;iDist++)
	{
		for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
		{
			if((int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber)>MaxValue1.Value)
			{
				MaxValue1.Value = (int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber);
				MaxValue1.Dist = iDist;
				MaxValue1.AngleNumber = iAngleNumber;
			
			}

		}
	}

	Anglenumber=MaxValue1.AngleNumber+1;
	Anglenumber1=MaxValue1.AngleNumber-1;
	/////////////////////////////////////////////////////////////////////////////////
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
			if(pixel != 255 && *lpSrc != 0)
				return FALSE;

			//如果是黑点，则在变换域的对应各点上加1
			if(pixel == 0)
			{
				while(Anglenumber1<=Anglenumber)
				{
                    Anglenumber2=10*Anglenumber1;
					iDist = (int) fabs(i*cos(Anglenumber1*pi/180.0) + \
						j*sin(Anglenumber1*pi/180.0));
				
					//变换域的对应点上加1
					*(lpTransArea+iDist*iMaxAngleNumber+Anglenumber2) = \
						*(lpTransArea+iDist*iMaxAngleNumber+Anglenumber2) +1;
					Anglenumber1=Anglenumber1+0.1;
				}
			}
		
		}
	}
	Anglenumber1=MaxValue1.AngleNumber-1;			
	//找到变换域中的两个最大值点
	MaxValue1.Value=0;
	//找到第一个最大值点
	for (iDist=0; iDist<iMaxDist;iDist++)
	{
		while(Anglenumber1<=Anglenumber)
		{
			Anglenumber2=10*Anglenumber1;
			if((int)*(lpTransArea+iDist*iMaxAngleNumber+Anglenumber2)>MaxValue1.Value)
			{
				MaxValue1.Value = (int)*(lpTransArea+iDist*iMaxAngleNumber+Anglenumber2);
				MaxValue1.Dist = iDist;
				MaxValue1.AngleNumber = Anglenumber2;
			
			}
        Anglenumber1=Anglenumber1+0.1;

		}
	}

	Anglenumber=float(MaxValue1.AngleNumber)/10;	
//	}

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 释放内存
	LocalUnlock(hTransArea);
	LocalFree(hTransArea);
   if(Anglenumber<90)
   {
	// 返回
	   return 90+Anglenumber;
   }
   else
   {
   	   return Anglenumber-90;
   }
}


/*************************************************
*
* 函数名称：
*       CharSegment()
*
*  参数：
*      HDIB  hDIB      －原图像的句柄
*
*  返回值：
*     CRectLink        －存放被分割的各个字符位置信息的链表
*
* 功能：
*    将图像中待识别的字符逐个分离出来并返回存放各个字符的位置信息的链表
*
*  说明：
*    此函数只能对2值化后的图像进行处理
*
*********************************************************/


CRectLink CharSegment(HANDLE hDIB)
{
	
	//清空用来保存每个字符区域的链表
	CRectLink charRect1,charRect2;
	charRect1.clear();
	charRect2.clear();

	// 指向DIB的指针
	LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL)hDIB);
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;	

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	//指向象素的的指针
	BYTE* lpSrc;

	//图像的长度和宽度
	int height,width;

	//获取图像的宽度
	width=(int)::DIBWidth(lpDIB);

	//获取图像的长度
	height=(int)::DIBHeight(lpDIB);

	//计算图像每行的字节数
	LONG	lLineBytes = WIDTHBYTES(width * 8);

	//定义上下边界两个变量
	int top,bottom;

    //象素的灰度值
    int gray; 

	//设置循环变量
	int i,j;

	//用来统计图像中字符个数的计数器
    digicount=0;


	//从上往下扫描，找到上边界

	//行
	for (i=0;i<height;i++)
	{
         //列
  		for (j=0;j<width;j++)
		{
            // 指向图像第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

			//获得该点的灰度值
			gray = *(lpSrc);

			//看是否为黑点
			if (gray == 0)
			{   
               //若为黑点，把此点作为字符大致的最高点
				top = i;

				//对i强行赋值以中断循环
				i=height;

				//跳出循环
				break;
			}

        //如果该点不是黑点，继续循环
		}
	}


    //从下往上扫描，找下边界

	//行
	for (i = height-1;i>=0;i--)
    {

		//列
		for (j=0;j<width;j++)
		{
			// 指向图像第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

			//获取该点的灰度值
			gray = *(lpSrc);

			//判断是否为黑点
			if (gray == 0)
			{
				//若为黑点，把此点作为字符大致的最低点
				bottom = i;

				//对i强行赋值以中断循环
				i=-1;

				//跳出循环
				break;
			}

          //如果该点不是黑点，继续循环
		}
	
	}

	//lab 用作是否进入一个字符分割的标志
	bool lab = false;

	//表明扫描一列中是否发现黑色点
	bool black = false;

    //存放位置信息的结构体
	CRect rect;

	//计数器置零
	digicount=0;
   
	//行
	for (i=0;i<width;i++)
	{
		//开始扫描一列
		black=false;

		for (j=0;j<height;j++)
			{	
				// 指向图像第i行，第j个象素的指针
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;

				//获取该点的灰度值
				gray = *(lpSrc);

                //判断是否为黑点
				if (gray == 0)
				{
					//如果发现黑点，设置标志位
					black=true;

					//如果还没有进入一个字符的分割
					if(lab==false)
					{	
						//设置左侧边界
						rect.left = i;

						//字符分割开始
						lab = true;
					}

					//如果字符分割已经开始了
					else

                      //跳出循环
						break;
				}		
			}

		//如果已经扫到了最右边那列，说明整副图像扫描完毕。退出
           if(i==(width-1))
			   
		 //退出整个循环	   
		   break;

		//如果到此black仍为false，说明扫描了一列，都没有发现黑点。表明当前字符分割结束
		if(lab==true&&black==false)
		{   
           //将位置信息存入结构体中

           //设置右边界
			rect.right =i;

			//设置上边界
			rect.top =top;

			//设置下边界
			rect.bottom =bottom;

			//将框外括一个象素，以免压到字符
			rect.InflateRect (1,1);

            //将这个结构体插入存放位置信息的链表1的后面
			charRect1.push_back (rect);

			//设置标志位，开始下一次的字符分割
			lab=false;

			//字符个数统计计数器加1
			digicount++;
			
		}

		//进入下一列的扫描

	}

   //再将矩形轮廓矩形的top和bottom精确化

	//将链表1赋值给链表2
	charRect2=charRect1;

    //将链表2的内容清空
    charRect2.clear ();

	//建立一个新的存放位置信息的结构体
	CRect rectnew;

	//对于链表1从头至尾逐个进行扫描
	while(!charRect1.empty())
	{    
		//从链表1头上得到一个矩形
		rect= charRect1.front();

		//从链表1头上面删掉一个
		charRect1.pop_front();

		//计算更加精确的矩形区域

		//获得精确的左边界
		rectnew.left =rect.left-1 ;

		//获得精确的右边界
		rectnew.right =rect.right+1 ;

		//通过获得的精确左右边界对上下边境重新进行精确定位

		// 由下而上扫描计算上边界
		
		//行
		for(i=rect.top ;i<rect.bottom ;i++)
		{   
          //列
			for(j=rect.left ;j<rect.right ;j++)
			{   
                 // 指向图像第i行，第j个象素的指针
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

				//如果这个象素是黑点
				if (*lpSrc == 0)
				{	
					//设置上边界
					rectnew.top = i-1;
					
					//对i进行强制定义以跳出循环
					i=rect.bottom  ;

					//跳出循环
					break;
				}	
			}
		}

		//由下而上扫描计算下边界
   
        //行
		for(i=rect.bottom-1 ;i>=rect.top  ;i--)
		{
			//列
			for(j=rect.left ;j<rect.right ;j++)
			{
				// 指向图像第i行，第j个象素的指针
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

				//该点如果为黑点
				if (*lpSrc == 0)
				{	
					//设置下边界
					rectnew.bottom = i+1;

                    //对i进行强制定义以跳出循环
					i=-1;
                    //跳出循环
					break;
				}	
			}
		}

		//将得到的新的准确的位置信息从后面插到链表2的尾上
		charRect2.push_back (rectnew);
	}

	//将链表2 传递给链表1
	charRect1=charRect2;
	
	//解除锁定
	::GlobalUnlock(hDIB);

	//将链表1返回
	return charRect1;
}

/*********************************** ************************************
函数名称：DisplayDIB
参数：
	CDC* pDC			-指向当前设备上下文（Divice Context）的指针
	HDIB hDIB			-要显示的位图的句柄
**********************************************************************/

void DisplayDIB(CDC* pDC,HDIB hDIB)
{
	BYTE* lpDIB=(BYTE*)::GlobalLock((HGLOBAL)hDIB);
	// 获取DIB宽度和高度
	int cxDIB =  ::DIBWidth((char*) lpDIB);
	int cyDIB =  ::DIBHeight((char*)lpDIB);
	CRect rcDIB,rcDest;
	rcDIB.top = rcDIB.left = 0;
	rcDIB.right = cxDIB;
	rcDIB.bottom = cyDIB;
	//设置目标客户区输出大小尺寸
	rcDest = rcDIB;
	//CDC* pDC=GetDC();
	ClearAll(pDC);
	//在客户区显示图像
	//for(int ii=0;ii<10;ii++)

	::PaintDIB(pDC->m_hDC,rcDest,hDIB,rcDIB,NULL);
	
	
	::GlobalUnlock((HGLOBAL)hDIB);
}


////////////////////////
void ClearAll(CDC *pDC)
{
	CRect rect;
	//GetClientRect(&rect);
	rect.left =0;rect.top =0;rect.right =2000;rect.bottom =1000;
	CPen pen;
	pen.CreatePen (PS_SOLID,1,RGB(255,255,255));
	pDC->SelectObject (&pen);
	pDC->Rectangle (&rect);
	::DeleteObject (pen);
}

/*****************绘制数字字符外面的矩形框*******************/
void DrawFrame(CDC* pDC,HDIB hDIB, CRectLink charRect,unsigned int linewidth,COLORREF color)
{	
	CPen pen;
	pen.CreatePen (PS_SOLID,linewidth,color);
	pDC->SelectObject (&pen);
	::SelectObject (*pDC,GetStockObject(NULL_BRUSH));
	CRect rect,rect2;
	BYTE* lpDIB=(BYTE*)::GlobalLock ((HGLOBAL)hDIB);
	while(!charRect.empty())
	{
		
		//从表头上得到一个矩形
		rect2=rect= charRect.front();
		//从链表头上面删掉一个
		charRect.pop_front();
		//注意，这里原先的rect是相对于图像原点(左下角)的，
		//而在屏幕上绘图时，要转换以客户区为原点的坐标
		rect.top =::DIBHeight ((char*)lpDIB)-rect2.bottom;
		rect.bottom =::DIBHeight ((char*)lpDIB)-rect2.top ;
		pDC->Rectangle (&rect);
	}
	::GlobalUnlock ((HGLOBAL)hDIB);
}
/////////////////////////////////
/******************************************************************
*
*  函数名称：
*      StdDIBbyRect()
*
*  参数：
*     HDIB  hDIB          －图像的句柄
*     int   tarWidth      －标准化的宽度
*     int   tarHeight     －标准化的高度
*
*  返回值：
*         无
*
*  功能：
*     将经过分割的字符，进行缩放处理使他们的宽和高一致，以方便特征的提取
*
*  说明：
*     函数中用到了，每个字符的位置信息，所以必须在执行完分割操作之后才能执行标准化操作
*
******************************************************************/
void StdDIBbyRect(HDIB hDIB, int tarWidth, int tarHeight)
{	

	//指向图像的指针
	LPSTR lpDIB=(LPSTR)::GlobalLock ((HGLOBAL)hDIB);

	//指向象素起始位置的指针
    LPSTR lpDIBBits=::FindDIBBits (lpDIB);
	
	//指向象素的指针
//	BYTE* lpSrc;

	//获取图像的的宽度
	LONG lWidth=::DIBWidth ((char*)lpDIB);

	//获取图像的高度
	LONG lHeight=::DIBHeight ((char*)lpDIB);

	// 循环变量
	int	i;
	int	j;
	
	// 图像每行的字节数
	LONG	lLineBytes = WIDTHBYTES(lWidth * 8);

	//宽度、高度方向上的缩放因子
	double wscale,hscale;

	//开辟一块临时缓存区,来存放变化后的图像信息
	//LPSTR lpNewDIBBits;
//	LPSTR lpDst;
	BYTE* lpNewDIBBits;
	BYTE* lpDst;
 
    //缓存区的大小和原图像的数据区大小一样
	HLOCAL nNewDIBBits=LocalAlloc(LHND,lLineBytes*lHeight);

	//指向缓存区开始位置的指针
	lpNewDIBBits=(unsigned char*)LocalLock(nNewDIBBits);

	//指向缓存内信息的指针
	lpDst=(unsigned char*)lpNewDIBBits;

	//将缓存区的内容赋初始值
	memset(lpDst,(BYTE)255,lLineBytes*lHeight);

	//进行映射操作的坐标变量
//	int i_src,j_src;

	//存放字符位置信息的结构体
	CRect rect;
	CRect rectnew;

	//先清空一个新的矩形区域链表以便存储标准化后的矩形区域链表
	m_charRectCopy.clear ();

	//从头到尾逐个扫描各个结点
	while(!m_charRect.empty())
	{
		//从表头上得到一个矩形
		rect= m_charRect.front();

		//从链表头上面删掉一个
		m_charRect.pop_front();

		//计算缩放因子

		//横坐标方向的缩放因子
		wscale=(double)tarWidth/rect.Width ();

		//纵坐标方向的缩放因子
		hscale=(double)tarHeight/rect.Height ();

		//计算标准化矩形

		//上边界
		rectnew.top =rect.top ;

		//下边界
		rectnew.bottom =rect.top +tarHeight;

		//左边界
		rectnew.left =rect.left ;

		//右边界
		rectnew.right =rectnew.left +tarWidth;
	   int m,n;
       double x,y,p,q;
	   int d;


		//将原矩形框内的象素映射到新的矩形框内
		for(i=rectnew.top ;i<rectnew.bottom ;i++)
		{
			for(j=rectnew.left ;j<rectnew.right ;j++)
			{   

			/*	//计算映射坐标
				i_src=rectnew.top +int((i-rectnew.top )/hscale);
				j_src=rectnew.left +int((j-rectnew.left )/wscale);

				//将相对应的象素点进行映射操作
				lpSrc=(unsigned char *)lpDIBBits + lLineBytes *  i_src + j_src;
				lpDst = (char *)lpNewDIBBits + lLineBytes * i + j;
				*lpDst=*lpSrc;    */
				y=(i-rectnew.top)/hscale;
				x=(j-rectnew.left)/wscale;
				if(y>0) m=(int)y;
				else m=(int)(y-1);

				if(x>0) n=(int)x;
				else n=(int)(x-1);
				q=y-m;
				p=x-n;
				if(q==1) {q=0;m=m+1;}
				if(p==1) {p=0;n=n+1;}
				int u=(int)(tarHeight/hscale);
				int v=(int)(tarWidth/wscale);
				if((m>=0)&&(m<(u+1))&&(n>=0)&&(n<(v+1)))
			d=(int)((1.0-q)*((1.0-p)*(*((unsigned char *)lpDIBBits + lLineBytes *(rectnew.top+m) + rectnew.left+n))
				+p*(*((unsigned char *)lpDIBBits + lLineBytes *(rectnew.top+m) + rectnew.left+n+1)))
				+q*((1.0-p)*(*((unsigned char *)lpDIBBits + lLineBytes *(rectnew.top+m+1) + rectnew.left+n))
				+p*(*((unsigned char *)lpDIBBits + lLineBytes *(rectnew.top+m+1) + rectnew.left+n+1))));
				else d=255;
				if(d>180) *((unsigned char *)lpNewDIBBits + lLineBytes * i + j)=(BYTE)255;
				else *((unsigned char *)lpNewDIBBits + lLineBytes * i + j)=(BYTE)0;
			lpDst= (unsigned char *)lpNewDIBBits + lLineBytes * i + j;
				
			}
		}
		//将标准化后的矩形区域插入新的链表
		m_charRectCopy.push_back (rectnew);

	
	}

	//存储标准化后新的rect区域
    m_charRect=m_charRectCopy;

	//将缓存区的内容拷贝到图像的数据区内
	memcpy(lpDIBBits,lpNewDIBBits,lLineBytes*lHeight);

	//解除锁定
	::GlobalUnlock ((HGLOBAL)hDIB);
}

/*******************************************
*
*  函数名称：
*  AutoAlign()
*
*  参数：
*    HDIB   hDIB        －原图像的句柄
*
*  返回值   
*    HDIB               －紧缩排列后的新图像的句柄
*
*  功能：
*     将经过了标准化处理的字符进行规整的排列，以方便下一步的处理
*
*  说明：
*     紧缩排列的操作必须在标准化操作之后进行
*
********************************************************/


HDIB AutoAlign(HDIB hDIB)
{   
	
	//指向图像的指针
 	BYTE* lpDIB=(BYTE*)::GlobalLock ((HGLOBAL)hDIB);

	//指向象素起始位置的指针
	BYTE* lpDIBBits=(BYTE*)::FindDIBBits ((char*)lpDIB);

	//指向象素的指针
	BYTE* lpSrc;

	//获取图像的宽度
	LONG lWidth=::DIBWidth ((char*)lpDIB);

	//获取图像的高度
	LONG lHeight=::DIBHeight ((char*)lpDIB);

	//获取标准化的宽度
	int w=m_charRect.front ().Width() ;

	//获取标准化的高度
	int h=m_charRect.front ().Height() ;

	//建立一个新的图像正好能够将标准化的字符并排放置
	HDIB hNewDIB=::NewDIB (digicount*w,h,8);
	
	//指向新的图像的指针
	BYTE* lpNewDIB=(BYTE*) ::GlobalLock((HGLOBAL)hNewDIB);
	
	//指向象素起始位置的指针
	BYTE* lpNewDIBBits=(BYTE*)::FindDIBBits((char*)lpNewDIB);

	//指向象素的指针
	BYTE* lpDst=lpNewDIBBits;

	//计算原图像每行的字节数
	LONG lLineBytes=(lWidth+3)/4*4;

	//计算新图像每行的字节数
	LONG lLineBytesnew =(digicount*w+3)/4*4;

	//将新的图像初始化为白色
	memset(lpDst,(BYTE)255,lLineBytesnew * h);

	//映射操作的坐标变量
	int i_src,j_src;

	//循环变量
	int i,j;

	//统计字符个数的变量
	int counts=0;

	//存放位置信息的结构体
	CRect rect,rectnew;

	//清空一个新的链表来存放新的字符位置信息
	m_charRectCopy.clear ();

	//从头至尾逐个扫描原链表的各个结点
	while(!m_charRect.empty() )
	{   
		//从表头上得到一个矩形框
		rect=m_charRect.front ();

		//将这个矩形框从链表上删除
		m_charRect.pop_front ();

		//计算新的矩形框的位置信息

		//左边界
		rectnew.left =counts*w;

		//右边界
		rectnew.right =(counts+1)*w;

		//上边界
		rectnew.top =0;

		//下边界
		rectnew.bottom =h;

		//将获得的新的矩形框插入到新的链表中
		m_charRectCopy.push_back (rectnew);

		//将原矩形框内的象素映射到新的矩形框中
		for(i=0;i<h;i++)
		{  
			for(j=counts*w;j<(counts+1)*w;j++)
			{   

				//计算映射坐标
	          	i_src=rect.top +i;
				j_src=rect.left +j-counts*w;

				//进行象素的映射
				lpSrc=(BYTE *)lpDIBBits + lLineBytes *  i_src + j_src;
				lpDst=(BYTE *)lpNewDIBBits + lLineBytesnew * i + j;
				*lpDst=*lpSrc;
			}
		}

		//字符个数加1
		counts++;
		
	}

	//将获得的新的链表复制到原链表中，以方便下一次的调用
	m_charRect=m_charRectCopy;

	//解除锁定
	::GlobalUnlock (hDIB);
   

	::GlobalUnlock (hNewDIB);
	
	return hNewDIB;
}

