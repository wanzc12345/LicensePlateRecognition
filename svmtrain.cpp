// svmtest.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <io.h>
#include <string.h>
#include <windows.h>
#include "types.h"
#include "../multsvm/multsvm.h"
#include "args.h"
#include <fstream>
#include <vector>

Prototype* ExtractFeature(int&, char*, int, char*);

int main(int argc, char* argv[])
{
	std::vector<char> simCharSet;
	int totalSamples;
//	char templatePath[256];// simCharSet[256];
//	char saveFile[256];
//	FILE *fp;
//	int  simCharGroup, tempNum=0,flag=1,count=1,recogID,corrID, matchCount, totalSamples, i,j, errCount;
//	float THRESHOLD=0.9999, lastrate, rate=0;
		
	Prototype *prototype, *trainSet;
	
	//请选择相似字符组别
	printf("请选择相似字符组别: \n");
	printf("[0] 0D\n[1] 0Q\n[2] 8B\n[3] 5S\n[4] 6G\n[5] MHWN\n[6] 1IT\n[7] 2Z\n[8] DQ\n");
//	scanf("%d", &simCharGroup);
//	simCharGroup = 5;
	{
		simCharSet.push_back('0');
		simCharSet.push_back('1');
		simCharSet.push_back('2');
		simCharSet.push_back('3');
		simCharSet.push_back('4');
		simCharSet.push_back('5');
		simCharSet.push_back('6');
		simCharSet.push_back('7');
		simCharSet.push_back('8');
		simCharSet.push_back('9');

		simCharSet.push_back('A');
		simCharSet.push_back('B');
		simCharSet.push_back('C');
		simCharSet.push_back('D');
		simCharSet.push_back('E');
		simCharSet.push_back('F');
		simCharSet.push_back('G');

		simCharSet.push_back('H');
		simCharSet.push_back('J');
		simCharSet.push_back('K');
		simCharSet.push_back('L');
		simCharSet.push_back('M');
		simCharSet.push_back('N');

		simCharSet.push_back('P');
		simCharSet.push_back('Q');
		simCharSet.push_back('R');
		simCharSet.push_back('S');
		simCharSet.push_back('T');

		simCharSet.push_back('U');
		simCharSet.push_back('V');
		simCharSet.push_back('W');
		simCharSet.push_back('X');
		simCharSet.push_back('Y');
		simCharSet.push_back('Z');
		simCharSet.push_back('\0');
	}

//	sprintf(saveFile, "e:\\%s.svm", &simCharSet[0]);
//	fp = fopen(saveFile, "w");

	//提取所有要训练样本的特征和样本数目
	printf("正在提取所有训练集合的特征.....\n");
	trainSet = ExtractFeature(totalSamples, trainPath, d, &simCharSet[0]);
	printf("训练……\n");
	multsvm l_svm;
	int numClass = strlen(&simCharSet[0]);
	std::ofstream arg;
	arg.open(trainedarg,std::ios_base::out|std::ios_base::binary);
	l_svm.loadTrainData_Prototype(trainSet,totalSamples,d,numClass,&simCharSet[0]);
	l_svm.storeBinArg(arg);
	arg.close();

	return 0;
}

#define Row 20
#define Col 10

Prototype* allocMemForFeature(int dimension, int templateCount)
{
	Prototype* prototype=NULL;
	int i;

	prototype = new Prototype[templateCount];

	for(i=0;i<templateCount;i++)
	{
		prototype[i].features = new float[dimension];
		memset(prototype[i].features,0,sizeof(float)*dimension);
	}

	return prototype;
}

bool PanReadBmp(char *filename,unsigned char image[Row][Col])
{
	FILE *InFile;

    BITMAPFILEHEADER FileHeader;
    BITMAPINFOHEADER InfoHeader;
	unsigned char rgb[3];

    int i,j;
    int width, depth;
	
    InFile = fopen(filename,"rb");
    if(NULL == InFile)    //文件打开失败
    {
        printf("file open error! %s\n",filename);		
        return FALSE;
    }
        
    if(fread((char*)&FileHeader,1,sizeof(BITMAPFILEHEADER),InFile)
        != sizeof(BITMAPFILEHEADER))
    {
        printf("%s read error! \n",filename); 
        return FALSE;
    }

    if(fread((char*)&InfoHeader,1,sizeof(BITMAPINFOHEADER),InFile)
        != sizeof(BITMAPINFOHEADER))
    {
        printf("%s read error! \n",filename);                                
        return FALSE;
    }

	width = (unsigned)InfoHeader.biWidth;
    depth = (unsigned)InfoHeader.biHeight;     
	if(Col != width || Row != depth)
	{
		printf("width or depth do not match.\n");
		return FALSE;
	}

    fseek(InFile,1078,SEEK_SET);
    if(8 == InfoHeader.biBitCount)
    {
		unsigned char buf[32];
		for(i=0;i<depth;i++) {
			fread(image[i],sizeof(unsigned char),width,InFile); 
			fread(buf,sizeof(unsigned char),2,InFile);
		}
	}
	else
	{	
		fseek(InFile,sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER),SEEK_SET);
		if(24 == InfoHeader.biBitCount)
		{
			for(i=0;i<depth;i++)
			{
				for(j=0;j<width;j++)
				{
					fread(rgb,sizeof(unsigned char),3,InFile); 	
					image[i][j] = rgb[0];
				}
			}
		}		
			
	}

	fclose(InFile);
	return TRUE;
}

BOOL PanPreProcess(unsigned char image[Row][Col])
{
    //unsigned char image[Row][Col];
	int i, j;
    for(i=0; i<Row; i++)
    {
        for(j=0; j<Col; j++)
        {    
            if(image[i][j] <= 0)
                image[i][j] = 0;
            else
                image[i][j] = 1;
        }
    }

    return TRUE;
}    

BOOL ZoningWMHN(float vector[],unsigned char  image[Row][Col])
{
	int i,j;
	unsigned char charImage[Row][Col];
//	for ( i=0 ; i<d ; ++i )
//		vector[i] = 0;
	long total = 0;
	for ( j=0 ; j<Row ; ++j ) {
		for ( i=0 ; i<Col ; ++i ) {
//			vector[(j>>1)*(Col/2)+(i>>1)] += image[j][i];
			vector[j*Col+i] = image[j][i];
			total += image[j][i];
		}
	}
	float avg = (float)total/d;
	float k = 128/avg;
	for ( i=0 ; i<d ; ++i )
		vector[i] *= k;
//	for ( i=0 ; i<d ; ++i )
//		vector[i] /=4;
/*
	memcpy(charImage[0], image[0], Row*Col);

	for(i=0;i<Row;i++)
	{
		for(j=0;j<Col;j++)
		{
			if(j < 5 || j>18)
				charImage[i][j] = 0;
		}
	}
    float total=0;
    int   number,zonRow=4,zonCol=4;
	for(i=0; i<Row; i++)
    {
        for(j=0; j<Col; j++)
        {
            number = i/zonRow*(Col/zonCol) + j/zonCol;	//区域位置
            if(number > d)
            {
                //printf("\nzoning error");
				return FALSE;
            }
            vector[number] = vector[number] + (float)image[i][j];
			total = total + image[i][j];
        }
    }
*/
	return TRUE;
}

Prototype* ExtractFeature(int& sampleCount, char* trainPath, int dimension, char *simSet)
{
	Prototype* prototype=NULL;
	
	_finddata_t  imageFile;
	long H_File;  //文件和路径
	int total, i, j,bNext=0, length;
	char fullPath[256];
	unsigned char image[Row][Col];
	
	length = strlen(simSet);

	_chdir(trainPath);

	if((H_File = _findfirst( "*.bmp", &imageFile)) == -1L)
		return NULL;
	
	//样本总数
    total = 0;
   	while(bNext == 0) 
	{	
		for(i=0;i<length;i++)
		{
			if(imageFile.name[5] == simSet[i])
				break;
		}
		if(i!=length)
			total++;
		bNext = _findnext(H_File, &imageFile);
	}
	if(total == 0)
		return NULL;

	//根据样本总数分配内存
	prototype = allocMemForFeature(dimension, total);

	H_File = _findfirst( "*.bmp", &imageFile);
	bNext = 0;
	i = 0;
	while(bNext == 0) 
	{		
		sprintf(fullPath, "%s%s", trainPath, imageFile.name);

		for(j=0;j<length;j++)
		{
			if(imageFile.name[5] == simSet[j])
				break;
		}
		if(j!=length)	
		{
			//此处加入特征提取函数
			PanReadBmp(fullPath, image);
			
			//预处理为01图像
//			PanPreProcess(image);
			
			prototype[i].text = imageFile.name[5];   //正确ID

			//特征提取(对不同的特征提取算法不一样)
		//	EveryHeight(prototype[i].features, image);
	 		ZoningWMHN(prototype[i].features, image);
			
			strcpy(prototype[i].fullpath, fullPath);

			i++;
		}

		bNext = _findnext(H_File, &imageFile);	
	}
	sampleCount = total;
	
	return prototype;
}
