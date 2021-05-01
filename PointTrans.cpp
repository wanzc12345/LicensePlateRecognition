
/**************************************************************************
 *  �ļ�����PointTrans.cpp
 *
 *  ͼ�������API�����⣺
 *
 *  LinerTrans()		- ͼ�����Ա任
 *	WindowTrans()		- ͼ�񴰿ڱ任
 *  GrayStretch()		- ͼ��Ҷ�����
 *  InteEqualize()		��ֱ��ͼ����
 *
 *************************************************************************/
#include <syslib.h>
#include <stdlib.h>
#include "stdafx.h"
#include "PointTrans.h"
//#include "DIBAPI.h"
#include <math.h>


#include <direct.h>

/*************************************************************************
 *
 * �������ƣ�
 *   ThresholdTrans()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   BYTE  bThre	    - ��ֵ
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ�������ֵ�任�����ڻҶ�ֵС����ֵ������ֱ������
 * �Ҷ�ֵΪ0���Ҷ�ֵ������ֵ������ֱ������Ϊ255��
 *
 ************************************************************************/
BOOL WINAPI ThresholdTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �ж��Ƿ�С����ֵ
			if ((*lpSrc) < bThre)
			{
				// ֱ�Ӹ�ֵΪ0
				*lpSrc = 0;
			}
			else
			{
				// ֱ�Ӹ�ֵΪ255
				*lpSrc = 255;
			}
		}
	}
	
	// ����
	return TRUE;
}
/*************************************************************************
 *
 * �������ƣ�
 *   GetMedianNum()
 *
 * ����:
 *   unsigned char * bpArray	- ָ��Ҫ��ȡ��ֵ������ָ��
 *   int   iFilterLen			- ���鳤��
 *
 * ����ֵ:
 *   unsigned char      - ����ָ���������ֵ��
 *
 * ˵��:
 *   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
 *
 ************************************************************************/

unsigned char WINAPI GetMedianNum(unsigned char * bArray, int iFilterLen)
{
	// ѭ������
	int		i;
	int		j;
	
	// �м����
	unsigned char bTemp;
	
	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// ����
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}
	
	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}
	
	// ������ֵ
	return bTemp;
}

/*************************************************************************
 *
 * �������ƣ�
 *   GrayStretch()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   BYTE bX1			- �Ҷ������һ�����X����
 *   BYTE bY1			- �Ҷ������һ�����Y����
 *   BYTE bX2			- �Ҷ�����ڶ������X����
 *   BYTE bY2			- �Ҷ�����ڶ������Y����
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ����лҶ����졣
 *
 ************************************************************************/
BOOL WINAPI GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// �Ҷ�ӳ���
	BYTE	bMap[256];
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ����Ҷ�ӳ���
	for (i = 0; i <= bX1; i++)
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			bMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			bMap[i] = 255;
		}
	}
	
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �����µĻҶ�ֵ
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   ReplaceColorPal()
 *
 * ����:
 *   LPSTR lpDIB			- ָ��ԴDIBͼ��ָ��
 *   BYTE * bpColorsTable	- α��ɫ�����
 *
 * ����ֵ:
 *   BOOL					- �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����ָ����α��ɫ��������滻ͼ��ĵ��԰壬����bpColorsTable
 * ָ��Ҫ�滻��α��ɫ�����
 *
 ************************************************************************/
BOOL WINAPI ReplaceColorPal(LPSTR lpDIB, BYTE * bpColorsTable)
{
	
	// ѭ������
	int i;
	
	// ��ɫ���е���ɫ��Ŀ
	WORD wNumColors;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;
	
	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWinStyleDIB;
	
	// �������
	BOOL bResult = FALSE;
	
	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// ��ȡDIB����ɫ���е���ɫ��Ŀ
	wNumColors = ::DIBNumColors(lpDIB);
		
	// �ж���ɫ��Ŀ�Ƿ���256ɫ
	if (wNumColors == 256)
	{
		
		// �ж��Ƿ���WIN3.0��DIB
		bWinStyleDIB = IS_WIN30_DIB(lpDIB);
		
		// ��ȡα��ɫ���룬����DIB��ɫ��
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// ����DIB��ɫ���ɫ����
				lpbmi->bmiColors[i].rgbRed = bpColorsTable[i * 4];
				
				// ����DIB��ɫ����ɫ����
				lpbmi->bmiColors[i].rgbGreen = bpColorsTable[i * 4 + 1];
				
				// ����DIB��ɫ����ɫ����
				lpbmi->bmiColors[i].rgbBlue = bpColorsTable[i * 4 + 2];
				
				// ����DIB��ɫ�屣��λ
				lpbmi->bmiColors[i].rgbReserved = 0;
			}
			else
			{
				// ����DIB��ɫ���ɫ����
				lpbmc->bmciColors[i].rgbtRed = bpColorsTable[i * 4];
				
				// ����DIB��ɫ����ɫ����
				lpbmc->bmciColors[i].rgbtGreen = bpColorsTable[i * 4 + 1];
				
				// ����DIB��ɫ����ɫ����
				lpbmc->bmciColors[i].rgbtBlue = bpColorsTable[i * 4 + 2];
			}
		}
	}
	
	// ����
	return bResult;	

}
/*************************************************************************
 *
 * �������ƣ�
 *   MedianFilter()
 *
 * ����:
 *   LPSTR lpDIBBits		- ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth			- Դͼ���ȣ���������
 *   LONG  lHeight			- Դͼ��߶ȣ���������
 *   int   iFilterH			- �˲����ĸ߶�
 *   int   iFilterW			- �˲����Ŀ��
 *   int   iFilterMX		- �˲���������Ԫ��X����
 *   int   iFilterMY		- �˲���������Ԫ��Y����
 *
 * ����ֵ:
 *   BOOL					- �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����DIBͼ�������ֵ�˲���
 *
 ************************************************************************/

BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int iFilterH, int iFilterW, 
						 int iFilterMX, int iFilterMY)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ָ��Ҫ���������ָ��
	unsigned char*	lpDst;
	
	// ָ����ͼ���ָ��
	LPSTR			lpNewDIBBits;
	HLOCAL			hNewDIBBits;
	
	// ָ���˲��������ָ��
	unsigned char	* aValue;
	HLOCAL			hArray;
	
	// ѭ������
	LONG			i;
	LONG			j;
	LONG			k;
	LONG			l;
	
	// ͼ��ÿ�е��ֽ���
	LONG			lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	// ��ʱ�����ڴ棬�Ա����˲�������
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hArray == NULL)
	{
		// �ͷ��ڴ�
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);
		
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	aValue = (unsigned char * )LocalLock(hArray);
	
	// ��ʼ��ֵ�˲�
	// ��(��ȥ��Ե����)
	for(i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// ��ȡ�˲�������
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// ָ��DIB��i - iFilterMY + k�У���j - iFilterMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;
					
					// ��������ֵ
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}
			
			// ��ȡ��ֵ
			* lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
		}
	}
	
	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);
	
	// ����
	return TRUE;
}
/*************************************************************************
 *
 * �������ƣ�
 *   HoughDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 * ����ֵ:
 *   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶԼ��ͼ���е�ƽ��ֱ�ߡ����ͼ����������ƽ�е�ֱ�ߣ���������ƽ��ֱ��
 * ��ȡ������
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

float HoughDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	int Anglenumber;
	#define pi 3.1415926
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;
	
	// ָ��任���ָ��
	LPSTR   lpTrans;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ָ��任���ָ��
	LPSTR	lpTransArea;
	HLOCAL	hTransArea;

	//�任��ĳߴ�
	int iMaxDist;
	int iMaxAngleNumber;

	//�任�������
	int iDist;
	int iAngleNumber;

	//ѭ������
	long i;
	long j;

	//����ֵ
	unsigned char pixel;

	//�洢�任���е��������ֵ
	MaxValue MaxValue1;
	MaxValue MaxValue2;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	//����任��ĳߴ�
	//������
	iMaxDist = (int) sqrt(lWidth*lWidth + lHeight*lHeight);

	//�Ƕȴ�0��180��ÿ��2��
	iMaxAngleNumber = 180;

	//Ϊ�任������ڴ�
	hTransArea = LocalAlloc(LHND, lWidth * lHeight * sizeof(int));

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpTransArea = (char * )LocalLock(hTransArea);
		
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpTrans = (char *)lpTransArea;
	memset(lpTrans, 0, lWidth * lHeight * sizeof(int));

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{

			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;

			//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel = (unsigned char)*lpSrc;

			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && *lpSrc != 0)
				return FALSE;

			//����Ǻڵ㣬���ڱ任��Ķ�Ӧ�����ϼ�1
			if(pixel == 0)
			{
				//ע�ⲽ����2��
				for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
				{
					iDist = (int) fabs(i*cos(iAngleNumber*pi/180.0) + \
						j*sin(iAngleNumber*pi/180.0));
				
					//�任��Ķ�Ӧ���ϼ�1
					*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber) = \
						*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber) +1;
				}
			}
		
		}
	}
				
	//�ҵ��任���е��������ֵ��
	MaxValue1.Value=0;
	MaxValue2.Value=0;
	
	//�ҵ���һ�����ֵ��
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

	//����һ�����ֵ�㸽������
	for (iDist = -9;iDist < 10;iDist++)
	{
		for(iAngleNumber=-1; iAngleNumber<2; iAngleNumber++)
		{
			if(iDist+MaxValue1.Dist>=0 && iDist+MaxValue1.Dist<iMaxDist \
				&& iAngleNumber+MaxValue1.AngleNumber>=0 && iAngleNumber+MaxValue1.AngleNumber<=iMaxAngleNumber)
			{
				*(lpTransArea+(iDist+MaxValue1.Dist)*iMaxAngleNumber+\
					(iAngleNumber+MaxValue1.AngleNumber))=0;
			}
		}
	}

	//�ҵ��ڶ������ֵ��
	for (iDist=0; iDist<iMaxDist;iDist++)
	{
		for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
		{
			if((int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber)>MaxValue2.Value)
			{
				MaxValue2.Value = (int)*(lpTransArea+iDist*iMaxAngleNumber+iAngleNumber);
				MaxValue2.Dist = iDist;
				MaxValue2.AngleNumber = iAngleNumber;
			
			}

		}
	}


	Anglenumber=MaxValue1.AngleNumber;
		
//	}

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// �ͷ��ڴ�
	LocalUnlock(hTransArea);
	LocalFree(hTransArea);
   if(Anglenumber>0)
   {
	// ����
	   //MessageBox("ok",0,0);
	   return Anglenumber+90;
   }
   else
   {
   	   return 90-abs(Anglenumber);
   }
}


/*************************************************
*
* �������ƣ�
*       CharSegment()
*
*  ������
*      HDIB  hDIB      ��ԭͼ��ľ��
*
*  ����ֵ��
*     CRectLink        ����ű��ָ�ĸ����ַ�λ����Ϣ������
*
* ���ܣ�
*    ��ͼ���д�ʶ����ַ����������������ش�Ÿ����ַ���λ����Ϣ������
*
*  ˵����
*    �˺���ֻ�ܶ�2ֵ�����ͼ����д���
*
*********************************************************/


CRectLink CharSegment(HANDLE hDIB)
{
	
	//�����������ÿ���ַ����������
	CRectLink charRect1,charRect2;
	charRect1.clear();
	charRect2.clear();

	// ָ��DIB��ָ��
	LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL)hDIB);
	
	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;	

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	//ָ�����صĵ�ָ��
	BYTE* lpSrc;

	//ͼ��ĳ��ȺͿ��
	int height,width;

	//��ȡͼ��Ŀ��
	width=(int)::DIBWidth(lpDIB);

	//��ȡͼ��ĳ���
	height=(int)::DIBHeight(lpDIB);

	//����ͼ��ÿ�е��ֽ���
	LONG	lLineBytes = WIDTHBYTES(width * 8);

	//�������±߽���������
	int top,bottom;

    //���صĻҶ�ֵ
    int gray; 

	//����ѭ������
	int i,j;

	//����ͳ��ͼ�����ַ������ļ�����
    int	digicount=0;


	//��������ɨ�裬�ҵ��ϱ߽�

	//��
	for (i=0;i<height;i++)
	{
         //��
  		for (j=0;j<width;j++)
		{
            // ָ��ͼ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

			//��øõ�ĻҶ�ֵ
			gray = *(lpSrc);

			//���Ƿ�Ϊ�ڵ�
			if (gray == 0)
			{   
               //��Ϊ�ڵ㣬�Ѵ˵���Ϊ�ַ����µ���ߵ�
				top = i;

				//��iǿ�и�ֵ���ж�ѭ��
				i=height;

				//����ѭ��
				break;
			}

        //����õ㲻�Ǻڵ㣬����ѭ��
		}
	}


    //��������ɨ�裬���±߽�

	//��
	for (i = height-1;i>=0;i--)
    {

		//��
		for (j=0;j<width;j++)
		{
			// ָ��ͼ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

			//��ȡ�õ�ĻҶ�ֵ
			gray = *(lpSrc);

			//�ж��Ƿ�Ϊ�ڵ�
			if (gray == 0)
			{
				//��Ϊ�ڵ㣬�Ѵ˵���Ϊ�ַ����µ���͵�
				bottom = i;

				//��iǿ�и�ֵ���ж�ѭ��
				i=-1;

				//����ѭ��
				break;
			}

          //����õ㲻�Ǻڵ㣬����ѭ��
		}
	
	}

	//lab �����Ƿ����һ���ַ��ָ�ı�־
	bool lab = false;

	//����ɨ��һ�����Ƿ��ֺ�ɫ��
	bool black = false;

    //���λ����Ϣ�Ľṹ��
	CRect rect;

	//����������
	digicount=0;
   
	//��
	for (i=0;i<width;i++)
	{
		//��ʼɨ��һ��
		black=false;

		for (j=0;j<height;j++)
			{	
				// ָ��ͼ���i�У���j�����ص�ָ��
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;

				//��ȡ�õ�ĻҶ�ֵ
				gray = *(lpSrc);

                //�ж��Ƿ�Ϊ�ڵ�
				if (gray == 0)
				{
					//������ֺڵ㣬���ñ�־λ
					black=true;

					//�����û�н���һ���ַ��ķָ�
					if(lab==false)
					{	
						//�������߽�
						rect.left = i;

						//�ַ��ָʼ
						lab = true;
					}

					//����ַ��ָ��Ѿ���ʼ��
					else

                      //����ѭ��
						break;
				}		
			}

		//����Ѿ�ɨ�������ұ����У�˵������ͼ��ɨ����ϡ��˳�
           if(i==(width-1))
			   
		 //�˳�����ѭ��	   
		   break;

		//�������black��Ϊfalse��˵��ɨ����һ�У���û�з��ֺڵ㡣������ǰ�ַ��ָ����
		if(lab==true&&black==false)
		{   
           //��λ����Ϣ����ṹ����

           //�����ұ߽�
			rect.right =i;

			//�����ϱ߽�
			rect.top =top;

			//�����±߽�
			rect.bottom =bottom;

			//��������һ�����أ�����ѹ���ַ�
			rect.InflateRect (1,1);

            //������ṹ�������λ����Ϣ������1�ĺ���
			charRect1.push_back (rect);

			//���ñ�־λ����ʼ��һ�ε��ַ��ָ�
			lab=false;

			//�ַ�����ͳ�Ƽ�������1
			digicount++;
			
		}

		//������һ�е�ɨ��

	}

   //�ٽ������������ε�top��bottom��ȷ��

	//������1��ֵ������2
	charRect2=charRect1;

    //������2���������
    charRect2.clear ();

	//����һ���µĴ��λ����Ϣ�Ľṹ��
	CRect rectnew;

	//��������1��ͷ��β�������ɨ��
	while(!charRect1.empty())
	{    
		//������1ͷ�ϵõ�һ������
		rect= charRect1.front();

		//������1ͷ����ɾ��һ��
		charRect1.pop_front();

		//������Ӿ�ȷ�ľ�������

		//��þ�ȷ����߽�
		rectnew.left =rect.left-1 ;

		//��þ�ȷ���ұ߽�
		rectnew.right =rect.right+1 ;

		//ͨ����õľ�ȷ���ұ߽�����±߾����½��о�ȷ��λ

		// ���¶���ɨ������ϱ߽�
		
		//��
		for(i=rect.top ;i<rect.bottom ;i++)
		{   
          //��
			for(j=rect.left ;j<rect.right ;j++)
			{   
                 // ָ��ͼ���i�У���j�����ص�ָ��
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

				//�����������Ǻڵ�
				if (*lpSrc == 0)
				{	
					//�����ϱ߽�
					rectnew.top = i-1;
					
					//��i����ǿ�ƶ���������ѭ��
					i=rect.bottom  ;

					//����ѭ��
					break;
				}	
			}
		}

		//���¶���ɨ������±߽�
   
        //��
		for(i=rect.bottom-1 ;i>=rect.top  ;i--)
		{
			//��
			for(j=rect.left ;j<rect.right ;j++)
			{
				// ָ��ͼ���i�У���j�����ص�ָ��
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;

				//�õ����Ϊ�ڵ�
				if (*lpSrc == 0)
				{	
					//�����±߽�
					rectnew.bottom = i+1;

                    //��i����ǿ�ƶ���������ѭ��
					i=-1;
                    //����ѭ��
					break;
				}	
			}
		}

		//���õ����µ�׼ȷ��λ����Ϣ�Ӻ���嵽����2��β��
		charRect2.push_back (rectnew);
	}

	//������2 ���ݸ�����1
	charRect1=charRect2;
	
	//�������
	::GlobalUnlock(hDIB);

	//������1����
	return charRect1;
}

/*********************************** ************************************
�������ƣ�DisplayDIB
������
	CDC* pDC			-ָ��ǰ�豸�����ģ�Divice Context����ָ��
	HDIB hDIB			-Ҫ��ʾ��λͼ�ľ��
**********************************************************************/

void DisplayDIB(CDC* pDC,HDIB hDIB)
{
	BYTE* lpDIB=(BYTE*)::GlobalLock((HGLOBAL)hDIB);
	// ��ȡDIB��Ⱥ͸߶�
	int cxDIB =  ::DIBWidth((char*) lpDIB);
	int cyDIB =  ::DIBHeight((char*)lpDIB);
	CRect rcDIB,rcDest;
	rcDIB.top = rcDIB.left = 0;
	rcDIB.right = cxDIB;
	rcDIB.bottom = cyDIB;
	//����Ŀ��ͻ��������С�ߴ�
	rcDest = rcDIB;
	//CDC* pDC=GetDC();
	ClearAll(pDC);
	//�ڿͻ�����ʾͼ��
	//for(int ii=0;ii<10;ii++)
	::PaintDIB(pDC->m_hDC,rcDest,hDIB,rcDIB,NULL);
	::GlobalUnlock((HGLOBAL)hDIB);
}
/*****************���������ַ�����ľ��ο�*******************/
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
		
		//�ӱ�ͷ�ϵõ�һ������
		rect2=rect= charRect.front();
		//������ͷ����ɾ��һ��
		charRect.pop_front();
		//ע�⣬����ԭ�ȵ�rect�������ͼ��ԭ��(���½�)�ģ�
		//������Ļ�ϻ�ͼʱ��Ҫת���Կͻ���Ϊԭ�������
		rect.top =::DIBHeight ((char*)lpDIB)-rect2.bottom;
		rect.bottom =::DIBHeight ((char*)lpDIB)-rect2.top ;
		pDC->Rectangle (&rect);
	}
	::GlobalUnlock ((HGLOBAL)hDIB);
}
/////////////////////////////////
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
