
#include <stdio.h>
#include <math.h> 
#include <time.h>
#include <stdlib.h>
#define BIGRND 32767

double drnd();
double dpn1();
double squash(double x);
double *alloc_1d_dbl(int n);
double *alloc_1d_dbl(int n);
double **alloc_2d_dbl(int m, int n);
int ndigicount; //多次提取样本特征总个数
double ** all_data_in; // 多次提取特征矩阵
void bpnn_initialize(int seed);
void bpnn_randomize_weights(double **w, int m, int n);
void bpnn_zero_weights(double **w, int m, int n);
void bpnn_layerforward(double *l1, double *l2, double **conn, int n1, int n2);
void bpnn_output_error(double *delta, double *target, double *output, int nj);
void bpnn_hidden_error(double* delta_h, int nh, double *delta_o, int no, double **who, double *hidden);
void bpnn_adjust_weights(double *delta, int ndelta, double *ly, int nly, double** w, double **oldw, double eta, double momentum);
void w_weight(double **w,int n1,int n2,char*name);
bool r_weight(double **w,int n1,int n2,char *name);
void w_num(int n1,int n2,int n3,char*name);
bool r_num(int *n,char *name);

double **TZTQ_24(BYTE*image ,int p,int w);
//double **code(BYTE*image ,int *p,int w,int h,int dw);
void BpTrain1(HDIB hDIB,int n_hidden,double min_ex,double momentum,double eta ,int width,int height);
void BpTrain2(HDIB hDIB,int n_hidden,double min_ex,double momentum,double eta ,int width,int height);
void BpTrain3(HDIB hDIB,int n_hidden,double min_ex,double momentum,double eta ,int width,int height);
CString* CodeRecognize(HDIB hDIB,int width ,int height ,int n_in ,int n_hidden,int n_out);


/*** 返回0－1的双精度随机数 ***/
double drnd()
{
	return ((double) rand() / (double) BIGRND);
}

/*** 返回-1.0到1.0之间的双精度随机数 ***/
double dpn1()
{
	return ((drnd() * 2.0) - 1.0);
}

double squash(double x)
{
	return (1.0 / (1.0 + exp(-x)));
}
/*** 申请1维双精度实数数组 ***/

double *alloc_1d_dbl(int n)
{
	double *new1;
	
	new1 = (double *) malloc ((unsigned) (n * sizeof (double)));
	if (new1 == NULL) {
		printf("ALLOC_1D_DBL: Couldn't allocate array of doubles\n");
		return (NULL);
	}
	return (new1);
}

/*** 申请2维双精度实数数组 ***/

double **alloc_2d_dbl(int m, int n)
{
	int i;
	double **new1;
	
	new1 = (double **) malloc ((unsigned) (m * sizeof (double *)));
	if (new1 == NULL) {
		//	printf("ALLOC_2D_DBL: Couldn't allocate array of dbl ptrs\n");
		return (NULL);
	}
	
	for (i = 0; i < m; i++) {
		new1[i] = alloc_1d_dbl(n);
	}
	
	return (new1);
}

/*** 设置随机数种子 ***/
void bpnn_initialize(int seed)
{
	//printf("Random number generator seed: %d\n", seed);
	srand(seed);
}

/*** 随机初始化权值 ***/
void bpnn_randomize_weights(double **w, int m, int n)
{
	int i, j;
	
	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			
			w[i][j] = dpn1();
		}
	}
}
/*** 0初始化权值 ***/
void bpnn_zero_weights(double **w, int m, int n)
{
	int i, j;
	
	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			w[i][j] = 0.0;
		}
	}
}

/*********前向传输*********/
void bpnn_layerforward(double *l1, double *l2, double **conn, int n1, int n2)
{
	double sum;
	int j, k;
	
	/*** 设置阈值 ***/
	l1[0] = 1.0;
	
	/*** 对于第二层的每个神经元 ***/
	for (j = 1; j <= n2; j++) {
		
		/*** 计算输入的加权总和 ***/
		sum = 0.0;
		for (k = 0; k <= n1; k++) {
			sum += conn[k][j] * l1[k];
		}
		l2[j] = squash(sum);
	}
}

/* 输出误差 */

void bpnn_output_error(double *delta, double *target, double *output, int nj)
{
	int j;
	double o, t, errsum;
	
	errsum = 0.0;
	for (j = 1; j <= nj; j++) {
		o = output[j];
		t = target[j];
		delta[j] = o * (1.0 - o) * (t - o);
		
	}
	
}

/* 隐含层误差 */

void bpnn_hidden_error(double* delta_h, int nh, double *delta_o, int no, double **who, double *hidden)
{
	int j, k;
	double h, sum, errsum;
	
	errsum = 0.0;
	for (j = 1; j <= nh; j++) {
		h = hidden[j];
		sum = 0.0;
		for (k = 1; k <= no; k++) {
			sum += delta_o[k] * who[j][k];
		}
		delta_h[j] = h * (1.0 - h) * sum;
		
	}
	
}

/* 调整权值 */
void bpnn_adjust_weights(double *delta, int ndelta, double *ly, int nly, double** w, double **oldw, double eta, double momentum)
{
	double new_dw;
	int k, j;
	
	ly[0] = 1.0;
	for (j = 1; j <= ndelta; j++) {
		for (k = 0; k <= nly; k++) {
			new_dw = ((eta * delta[j] * ly[k]) + (momentum * oldw[k][j]));
			w[k][j] += new_dw;
			oldw[k][j] = new_dw;
		}
	}
}
/*******保存权值**********/
void w_weight(double **w,int n1,int n2,char*name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	fp=fopen(name,"wb+");
	buffer=(double*)malloc((n1+1)*(n2+1)*sizeof(double));
	for(i=0;i<=n1;i++)
	{
		for(j=0;j<=n2;j++)
			buffer[i*(n2+1)+j]=w[i][j];
	}
	fwrite((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);
	fclose(fp);
	free(buffer);
}



/************读取权值*************/
bool  r_weight(double **w,int n1,int n2,char *name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"无法读取权值信息",NULL,MB_ICONSTOP);
		return (false);
	}
	buffer=(double*)malloc((n1+1)*(n2+1)*sizeof(double));
	fread((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);
	
	for(i=0;i<=n1;i++)
	{
		for(j=0;j<=n2;j++)
			w[i][j]=buffer[i*(n2+1)+j];
	}
	fclose(fp);
	free(buffer);
	return(true);
}

/*****保存各层结点的数目******/
void w_num(int n1,int n2,int n3,char*name)
{
	FILE *fp;
	fp=fopen(name,"wb+");
	int *buffer;
	buffer=(int*)malloc(3*sizeof(int));
	buffer[0]=n1;
	buffer[1]=n2;
	buffer[2]=n3;
	fwrite((char*)buffer,sizeof(int),3,fp);
	fclose(fp);
	free(buffer);
}

/********读取各层结点数目*********/

bool r_num(int *n,char *name)
{
	int *buffer;
	FILE *fp;
	buffer=(int *)malloc(3*sizeof(int));
	if((fp=fopen(name,"rb"))==NULL)
	{
		::MessageBox(NULL,"结点参数",NULL,MB_ICONSTOP);
		return (false);
	}
	fread((char*)buffer,sizeof(int),3,fp);
	n[0]=buffer[0];
	n[1]=buffer[1];
	n[2]=buffer[2];
	fclose(fp);
	free(buffer);
	return(true);
}
/********************************************************
* 函数名称 VerticalCode()
*
* 参量：
*   BYTE* lpDIBBits   －指向输入图像的象素其实位置的指针 
*   int  num          －图片中样本的个数
*   LONG lLineByte    －输入图片每行的字节数
*   LONG lSwidth      －预处理时归一化的宽度
*   LONG lSheight     －预处理时归一化的长度
*
* 返回值：
*   double**           －特征向量矩阵
*
*  函数功能 ：
*      对于输入样本提取特征向量，在这里把归一化样本的
*	   水平和竖直方向的统计特征作为特征提取出来
***************************************************************/

double** VerticalCode(BYTE* lpDIBBits,int num, LONG lLineByte,LONG lSwidth,LONG lSheight)
{   
   //循环变量
    int i,j,k;
    BYTE* lpSrc; 
    
	//统计变量
	int sum;

    //  建立保存特征向量的二维数组
   double **data;
   
   // 为这个数组申请二维存储空间
   data = alloc_2d_dbl(num,lSwidth+lSheight);
 
   // 将归一化的样本的每个象素作为一个特征点提取出来

   //逐个数据扫描
   for(k=0;k<num;k++)  
   { 
     //统计每行的象素点个数
      for(i=0;i<lSheight;i++)
	  {    
        //对统计变量初始化
	 	  sum=0;

          //对每个数据逐列扫描
          for(j=k*lSwidth;j<(k+1)*lSwidth;j++)
		  {
            // 指向图像第i行第j列个象素的指针
            lpSrc = lpDIBBits + i*lLineByte + j;

             //如果这个象素是黑色的
             if(*(lpSrc)==0)

             //统计变量加1
                sum++;
		  }
         data[k][i]=sum;
	  }

    //统计每列的象素点个数
    for(j=k*lSwidth;j<(k+1)*lSwidth;j++)
	{    
        //对统计变量初始化
	 	  sum=0;

          //对每个数据逐行扫描
         for(i=0;i<lSheight;i++)
          {
            // 指向图像第i行第j列个象素的指针
            lpSrc = lpDIBBits + i*lLineByte + j;

             //如果这个象素是黑色的
             if(*(lpSrc)==0)

             //统计变量加1
                sum++;
		  }
         data[k][j-k*lSwidth+lSheight]=sum;
	  }

   }
 
//返回特征向量矩阵
   return(data);  
}

//新增TZTQ_24
double ** TZTQ_24(BYTE* lpDIBBits,int num, LONG lLineBytes,LONG lSwidth)
{	
int i,j,k,m;

	//分配一个内存空间并得到二维指针
	double ** tezheng=alloc_2d_dbl(num,39);

	BYTE* lpSrc;	

	int b;

	//存储临时的特征
	double * tz=new double[9];
	double * p=new double[6];
    double * f=new double[39];

	for(k=0;k<num;k++)
	{	
		for(i=0;i<9;i++) tz[i]=0;

		//提取前8个特征
		for(m=0;m<9;m++)
		{	for(i=int(m/3)*5;i<(int(m/3)+1)*5+1;i++)
				for(j=m%3*10+k*lSwidth;j<(m%3+1)*10+2+k*lSwidth;j++)
				{	lpSrc=lpDIBBits + lLineBytes *  i + j;
					b=(*lpSrc==255)?0:1;
				    tz[m]+=b;
				}
				if(tz[m]==0) tz[m]=1;
		}

			for (i=0;i<3;i++)
			{
				p[i]=0;
				p[i+3]=0;
				for(j=0;j<3;j++)
				{
					f[i*3+j]=tz[i*3+(j+1)%3]/tz[i*3+j];
					f[(i+3)*3+j]=tz[i+(j+1)%3*3]/tz[i+j%3*3];
					p[i]+=tz[i*3+j];
					p[i+3]+=tz[i+j*3];
				}
			}
			for (i=0;i<2;i++)
				for(j=0;j<3;j++)
					f[18+i*3+j]=p[i*3+(j+1)%3]/p[i*3+j];
				double c=f[0];
				for(i=1;i<24;i++)
					if(f[i]>c) c=f[i];
					for(i=0;i<24;i++)
						f[i]=f[i]/c;
     //新增六个特征
					for(i=0;i<6;i++)
						f[24+i]=0;
					int x=0;
					int y=0;
					for(m=1;m<=3;m++)
					{
						for(i=0;i<15;i++)

						{
							lpSrc=(unsigned char*)lpDIBBits+k*lSwidth+8*m+lLineBytes*i;
							x=*lpSrc;
      lpSrc=(unsigned char*)lpDIBBits+k*lSwidth+8*m+lLineBytes*(i+1);
							y=*lpSrc;
							if((x==0) && (y==255) ) f[23+m]+=1;
							x=y=0;
						}
                  for(i=0;i<29;i++)
				  {
	lpSrc=(unsigned char*)lpDIBBits+k*lSwidth+4*m*lLineBytes+i;
							x=*lpSrc;
      lpSrc=(unsigned char*)lpDIBBits+k*lSwidth+4*m*lLineBytes+(i+1);
							y=*lpSrc;
							if((x==0) && (y==255) ) f[26+m]+=1;
							x=y=0;
						}
   lpSrc=(unsigned char*)lpDIBBits+k*lSwidth+8*m+lLineBytes*15;
							x=*lpSrc;
      lpSrc=(unsigned char*)lpDIBBits+k*lSwidth+4*m*lLineBytes+29;
							y=*lpSrc;
							if(!x)f[23+m]+=1;
							if(!y) f[26+m]+=1;
							x=y=0;
					}
                c=f[24];
				for(i=25;i<30;i++)
					if(f[i]>c) c=f[i];
					for(i=24;i<30;i++)
						f[i]=f[i]/c;  
					//再新增九个特征

      for(m=0;m<9;m++)
		  f[30+m]=tz[m];

                c=f[30];
				for(i=30;i<39;i++)
					if(f[i]>c) c=f[i];
					for(i=30;i<39;i++)
						f[i]=f[i]/c;  
		//存储特征
		for(i=0;i<39;i++)
			tezheng[k][i]=f[i];
	}
	

//	::GlobalUnlock ((HGLOBAL)hDIB);
	//返回特征向量矩阵的指针
	return tezheng;
	delete []f;
	delete []p;
	delete []tz;
}

/****************************************************
* 函数名称 BpTrain()
* 
* 参数：
*   double **data_in    -指向输入的特征向量数组的指针    
*	double **data_out   -指向理想输出数组的指针
int n_in            -输入层结点的个数 
*   int n_hidden        -BP网络隐层结点的数目
*   double min_ex       -训练时允许的最大均方误差
*   double momentum     -BP网络的相关系数
*   double eta          -BP网络的训练步长
*   int num             -输入样本的个数
*
* 函数功能：
*     根据输入的特征向量和期望的理想输出对BP网络尽行训练
*     训练结束后将权值保存并将训练的结果显示出来
********************************************************/
void BpTrain1(double ** data_in, double** data_out,int n_in,int n_hidden,double min_ex,double momentum,double eta ,int num)
{
	
	
	
	//循环变量   
	int i,k,l;
	
	//输出层结点数目
	int  n_out=4;   
	//指向输入层数据的指针
	double* input_unites; 
	//指向隐层数据的指针
	double* hidden_unites;
	//指向输出层数据的指针
	double* output_unites; 
	//指向隐层误差数据的指针
	double* hidden_deltas;
	//指向输出层误差数剧的指针
	double* output_deltas;  
	//指向理想目标输出的指针
	double* target;    
	//指向输入层于隐层之间权值的指针
	double** input_weights;
	//指向隐层与输出层之间的权值的指针
	double** hidden_weights;
	//指向上一此输入层于隐层之间权值的指针
	double** input_prev_weights ;
	//指向上一此隐层与输出层之间的权值的指针
	double** hidden_prev_weights;
	//每次循环后的均方误差误差值 
	double ex;
	
	//为各个数据结构申请内存空间
	input_unites= alloc_1d_dbl(n_in + 1);
	hidden_unites=alloc_1d_dbl(n_hidden + 1);
	output_unites=alloc_1d_dbl(n_out + 1);
	hidden_deltas = alloc_1d_dbl(n_hidden + 1);
	output_deltas = alloc_1d_dbl(n_out + 1);
	target = alloc_1d_dbl(n_out + 1);
	input_weights=alloc_2d_dbl(n_in + 1, n_hidden + 1);
	input_prev_weights = alloc_2d_dbl(n_in + 1, n_hidden + 1);
	hidden_prev_weights = alloc_2d_dbl(n_hidden + 1, n_out + 1);
	hidden_weights = alloc_2d_dbl(n_hidden + 1, n_out + 1);
	
	//为产生随机序列撒种
	time_t t; 
	bpnn_initialize((unsigned)time(&t));
	
	//对各种权值进行初始化初始化
	bpnn_randomize_weights( input_weights,n_in,n_hidden);
	bpnn_randomize_weights( hidden_weights,n_hidden,n_out);
	bpnn_zero_weights(input_prev_weights, n_in,n_hidden );
	bpnn_zero_weights(hidden_prev_weights,n_hidden,n_out );
	
	//开始进行BP网络训练
	//这里设定最大的迭代次数为15000次
	for(l=0;l<15000;l++)  
	{ 
		//对均方误差置零
		ex=0;
		//对样本进行逐个的扫描
		for(k=0;k<num;k++)  
		{ 
			//将提取的样本的特征向量输送到输入层上
			for(i=1;i<=n_in;i++)
				input_unites[i] = data_in[k][i-1];
			
			//将预定的理想输出输送到BP网络的理想输出单元
			for(i=1;i<=n_out;i++)
				target[i]=data_out[k][i-1];
			
			//前向传输激活
			
			//将数据由输入层传到隐层 
			bpnn_layerforward(input_unites,hidden_unites,
				input_weights, n_in,n_hidden);
			//将隐层的输出传到输出层
			bpnn_layerforward(hidden_unites, output_unites,
				hidden_weights,n_hidden,n_out);
			
			//误差计算
			
			//将输出层的输出与理想输出比较计算输出层每个结点上的误差
			bpnn_output_error(output_deltas,target,output_unites,n_out);
			//根据输出层结点上的误差计算隐层每个节点上的误差
			bpnn_hidden_error(hidden_deltas,n_hidden, output_deltas, n_out,hidden_weights, hidden_unites);
			
			//权值调整
			//根据输出层每个节点上的误差来调整隐层与输出层之间的权值    
			bpnn_adjust_weights(output_deltas,n_out, hidden_unites,n_hidden,
				hidden_weights, hidden_prev_weights, eta, momentum); 
			//根据隐层每个节点上的误差来调整隐层与输入层之间的权值    	
			bpnn_adjust_weights(hidden_deltas, n_hidden, input_unites, n_in,
				input_weights, input_prev_weights, eta, momentum);  
			
			//误差统计		
			for(i=1;i<=n_out;i++)
				ex+=(output_unites[i]-data_out[k][i-1])*(output_unites[i]-data_out[k][i-1]);
		}
		//计算均方误差
		ex=ex/double(num*n_out);
		//如果均方误差已经足够的小，跳出循环，训练完毕  
		if(ex<min_ex)break;
	}
	
	//相关保存
	
	//保存输入层与隐层之间的权值
	w_weight(input_weights,n_in,n_hidden,"win1.dat");
	//保存隐层与输出层之间的权值
	w_weight(hidden_weights,n_hidden,n_out,"whi1.dat");
	
	//保存各层结点的个数
	w_num(n_in,n_hidden,n_out,"num1");
	
	//显示训练结果
	
	CString str;
	if(ex<=min_ex)
	{
		str.Format ("迭代%d次，\n平均误差%.4f",l,ex);
		
		::MessageBox(NULL,str,"训练结果",NULL);
	}
	
	if(ex>min_ex)
	{
		
		str.Format("迭代%d次，平均误差%.4f\n请调整参数重新训练吧！",l,ex);
		::MessageBox(NULL,str,"训练结果",NULL);
	}
	
	//释放内存空间
	
	free(input_unites);
	free(hidden_unites);
	free(output_unites);
	free(hidden_deltas);
	free(output_deltas);
	free(target);
	free(input_weights);
	free(hidden_weights);
	free(input_prev_weights);
	free(hidden_prev_weights);
}

/////////////////////////////
void BpTrain2(double ** data_in, double** data_out,int n_in,int n_hidden,double min_ex,double momentum,double eta ,int num)
{
	
	
	
	//循环变量   
	int i,k,l;
	
	//输出层结点数目
	int  n_out=4;   
	//指向输入层数据的指针
	double* input_unites; 
	//指向隐层数据的指针
	double* hidden_unites;
	//指向输出层数据的指针
	double* output_unites; 
	//指向隐层误差数据的指针
	double* hidden_deltas;
	//指向输出层误差数剧的指针
	double* output_deltas;  
	//指向理想目标输出的指针
	double* target;    
	//指向输入层于隐层之间权值的指针
	double** input_weights;
	//指向隐层与输出层之间的权值的指针
	double** hidden_weights;
	//指向上一此输入层于隐层之间权值的指针
	double** input_prev_weights ;
	//指向上一此隐层与输出层之间的权值的指针
	double** hidden_prev_weights;
	//每次循环后的均方误差误差值 
	double ex;
	
	//为各个数据结构申请内存空间
	input_unites= alloc_1d_dbl(n_in + 1);
	hidden_unites=alloc_1d_dbl(n_hidden + 1);
	output_unites=alloc_1d_dbl(n_out + 1);
	hidden_deltas = alloc_1d_dbl(n_hidden + 1);
	output_deltas = alloc_1d_dbl(n_out + 1);
	target = alloc_1d_dbl(n_out + 1);
	input_weights=alloc_2d_dbl(n_in + 1, n_hidden + 1);
	input_prev_weights = alloc_2d_dbl(n_in + 1, n_hidden + 1);
	hidden_prev_weights = alloc_2d_dbl(n_hidden + 1, n_out + 1);
	hidden_weights = alloc_2d_dbl(n_hidden + 1, n_out + 1);
	
	//为产生随机序列撒种
	time_t t; 
	bpnn_initialize((unsigned)time(&t));
	
	//对各种权值进行初始化初始化
	bpnn_randomize_weights( input_weights,n_in,n_hidden);
	bpnn_randomize_weights( hidden_weights,n_hidden,n_out);
	bpnn_zero_weights(input_prev_weights, n_in,n_hidden );
	bpnn_zero_weights(hidden_prev_weights,n_hidden,n_out );
	
	//开始进行BP网络训练
	//这里设定最大的迭代次数为15000次
	for(l=0;l<15000;l++)  
	{ 
		//对均方误差置零
		ex=0;
		//对样本进行逐个的扫描
		for(k=0;k<num;k++)  
		{ 
			//将提取的样本的特征向量输送到输入层上
			for(i=1;i<=n_in;i++)
				input_unites[i] = data_in[k][i-1];
			
			//将预定的理想输出输送到BP网络的理想输出单元
			for(i=1;i<=n_out;i++)
				target[i]=data_out[k][i-1];
			
			//前向传输激活
			
			//将数据由输入层传到隐层 
			bpnn_layerforward(input_unites,hidden_unites,
				input_weights, n_in,n_hidden);
			//将隐层的输出传到输出层
			bpnn_layerforward(hidden_unites, output_unites,
				hidden_weights,n_hidden,n_out);
			
			//误差计算
			
			//将输出层的输出与理想输出比较计算输出层每个结点上的误差
			bpnn_output_error(output_deltas,target,output_unites,n_out);
			//根据输出层结点上的误差计算隐层每个节点上的误差
			bpnn_hidden_error(hidden_deltas,n_hidden, output_deltas, n_out,hidden_weights, hidden_unites);
			
			//权值调整
			//根据输出层每个节点上的误差来调整隐层与输出层之间的权值    
			bpnn_adjust_weights(output_deltas,n_out, hidden_unites,n_hidden,
				hidden_weights, hidden_prev_weights, eta, momentum); 
			//根据隐层每个节点上的误差来调整隐层与输入层之间的权值    	
			bpnn_adjust_weights(hidden_deltas, n_hidden, input_unites, n_in,
				input_weights, input_prev_weights, eta, momentum);  
			
			//误差统计		
			for(i=1;i<=n_out;i++)
				ex+=(output_unites[i]-data_out[k][i-1])*(output_unites[i]-data_out[k][i-1]);
		}
		//计算均方误差
		ex=ex/double(num*n_out);
		//如果均方误差已经足够的小，跳出循环，训练完毕  
		if(ex<min_ex)break;
	}
	
	//相关保存
	
	//保存输入层与隐层之间的权值
	w_weight(input_weights,n_in,n_hidden,"win2.dat");
	//保存隐层与输出层之间的权值
	w_weight(hidden_weights,n_hidden,n_out,"whi2.dat");
	
	//保存各层结点的个数
	w_num(n_in,n_hidden,n_out,"num2");
	
	//显示训练结果
	
	CString str;
	if(ex<=min_ex)
	{
		str.Format ("迭代%d次，\n平均误差%.4f",l,ex);
		
		::MessageBox(NULL,str,"训练结果",NULL);
	}
	
	if(ex>min_ex)
	{
		
		str.Format("迭代%d次，平均误差%.4f\n请调整参数重新训练吧！",l,ex);
		::MessageBox(NULL,str,"训练结果",NULL);
	}
	
	//释放内存空间
	
	free(input_unites);
	free(hidden_unites);
	free(output_unites);
	free(hidden_deltas);
	free(output_deltas);
	free(target);
	free(input_weights);
	free(hidden_weights);
	free(input_prev_weights);
	free(hidden_prev_weights);
}
////////////////////////////////////////
void BpTrain3(double ** data_in, double** data_out,int n_in,int n_hidden,double min_ex,double momentum,double eta ,int num)
{
	
	
	
	//循环变量   
	int i,k,l;
	
	//输出层结点数目
	int  n_out=4;   
	//指向输入层数据的指针
	double* input_unites; 
	//指向隐层数据的指针
	double* hidden_unites;
	//指向输出层数据的指针
	double* output_unites; 
	//指向隐层误差数据的指针
	double* hidden_deltas;
	//指向输出层误差数剧的指针
	double* output_deltas;  
	//指向理想目标输出的指针
	double* target;    
	//指向输入层于隐层之间权值的指针
	double** input_weights;
	//指向隐层与输出层之间的权值的指针
	double** hidden_weights;
	//指向上一此输入层于隐层之间权值的指针
	double** input_prev_weights ;
	//指向上一此隐层与输出层之间的权值的指针
	double** hidden_prev_weights;
	//每次循环后的均方误差误差值 
	double ex;
	
	//为各个数据结构申请内存空间
	input_unites= alloc_1d_dbl(n_in + 1);
	hidden_unites=alloc_1d_dbl(n_hidden + 1);
	output_unites=alloc_1d_dbl(n_out + 1);
	hidden_deltas = alloc_1d_dbl(n_hidden + 1);
	output_deltas = alloc_1d_dbl(n_out + 1);
	target = alloc_1d_dbl(n_out + 1);
	input_weights=alloc_2d_dbl(n_in + 1, n_hidden + 1);
	input_prev_weights = alloc_2d_dbl(n_in + 1, n_hidden + 1);
	hidden_prev_weights = alloc_2d_dbl(n_hidden + 1, n_out + 1);
	hidden_weights = alloc_2d_dbl(n_hidden + 1, n_out + 1);
	
	//为产生随机序列撒种
	time_t t; 
	bpnn_initialize((unsigned)time(&t));
	
	//对各种权值进行初始化初始化
	bpnn_randomize_weights( input_weights,n_in,n_hidden);
	bpnn_randomize_weights( hidden_weights,n_hidden,n_out);
	bpnn_zero_weights(input_prev_weights, n_in,n_hidden );
	bpnn_zero_weights(hidden_prev_weights,n_hidden,n_out );
	
	//开始进行BP网络训练
	//这里设定最大的迭代次数为15000次
	for(l=0;l<15000;l++)  
	{ 
		//对均方误差置零
		ex=0;
		//对样本进行逐个的扫描
		for(k=0;k<num;k++)  
		{ 
			//将提取的样本的特征向量输送到输入层上
			for(i=1;i<=n_in;i++)
				input_unites[i] = data_in[k][i-1];
			
			//将预定的理想输出输送到BP网络的理想输出单元
			for(i=1;i<=n_out;i++)
				target[i]=data_out[k][i-1];
			
			//前向传输激活
			
			//将数据由输入层传到隐层 
			bpnn_layerforward(input_unites,hidden_unites,
				input_weights, n_in,n_hidden);
			//将隐层的输出传到输出层
			bpnn_layerforward(hidden_unites, output_unites,
				hidden_weights,n_hidden,n_out);
			
			//误差计算
			
			//将输出层的输出与理想输出比较计算输出层每个结点上的误差
			bpnn_output_error(output_deltas,target,output_unites,n_out);
			//根据输出层结点上的误差计算隐层每个节点上的误差
			bpnn_hidden_error(hidden_deltas,n_hidden, output_deltas, n_out,hidden_weights, hidden_unites);
			
			//权值调整
			//根据输出层每个节点上的误差来调整隐层与输出层之间的权值    
			bpnn_adjust_weights(output_deltas,n_out, hidden_unites,n_hidden,
				hidden_weights, hidden_prev_weights, eta, momentum); 
			//根据隐层每个节点上的误差来调整隐层与输入层之间的权值    	
			bpnn_adjust_weights(hidden_deltas, n_hidden, input_unites, n_in,
				input_weights, input_prev_weights, eta, momentum);  
			
			//误差统计		
			for(i=1;i<=n_out;i++)
				ex+=(output_unites[i]-data_out[k][i-1])*(output_unites[i]-data_out[k][i-1]);
		}
		//计算均方误差
		ex=ex/double(num*n_out);
		//如果均方误差已经足够的小，跳出循环，训练完毕  
		if(ex<min_ex)break;
	}
	
	//相关保存
	
	//保存输入层与隐层之间的权值
	w_weight(input_weights,n_in,n_hidden,"win3.dat");
	//保存隐层与输出层之间的权值
	w_weight(hidden_weights,n_hidden,n_out,"whi3.dat");
	
	//保存各层结点的个数
	w_num(n_in,n_hidden,n_out,"num3");
	
	//显示训练结果
	
	CString str;
	if(ex<=min_ex)
	{
		str.Format ("迭代%d次，\n平均误差%.4f",l,ex);
		
		::MessageBox(NULL,str,"训练结果",NULL);
	}
	
	if(ex>min_ex)
	{
		
		str.Format("迭代%d次，平均误差%.4f\n请调整参数重新训练吧！",l,ex);
		::MessageBox(NULL,str,"训练结果",NULL);
	}
	
	//释放内存空间
	
	free(input_unites);
	free(hidden_unites);
	free(output_unites);
	free(hidden_deltas);
	free(output_deltas);
	free(target);
	free(input_weights);
	free(hidden_weights);
	free(input_prev_weights);
	free(hidden_prev_weights);
}

/*******************************************
* 函数名称
* CodeRecognize()
* 参量
*  double **data_in     -指向待识别样本特征向量的指针
*  int num              -待识别的样本的个数 
*  int n_in             -Bp网络输入层结点的个数              
*  int n_hidden         -Bp网络隐层结点的个数
*  int n_out            -Bp网络输出层结点的个数
* 函数功能：  
*    读入输入样本的特征相量并根据训练所得的权值 
*    进行识别，将识别的结果写入result.txt 
****************************************/

CString* CodeRecognize(double **data_in, int num ,int n_in,int n_hidden,int n_out)
{
		//循环变量
	int i,k;
	// 指向识别结果的指针 
	int *recognize;
	//为存放识别的结果申请存储空间
	recognize=(int*)malloc(num*sizeof(int));
	CString Charac[4]={"京","晋","粤",""};
	CString Num[3]={"0","2","4"};
	CString Letter[3]={"K","E","A"};
	//指向输入层数据的指针
	double* input_unites;  
	//指向隐层数据的指针
	double* hidden_unites;
	//指向输出层数据的指针
	double* output_unites; 
	//指向输入层于隐层之间权值的指针
	double** input_weights;
	//指向隐层与输出层之间的权值的指针
	double** hidden_weights;
	//为各个数据结构申请内存空间
	input_unites= alloc_1d_dbl(n_in + 1);
	hidden_unites=alloc_1d_dbl(n_hidden + 1);
	output_unites=alloc_1d_dbl(n_out + 1);
	input_weights=alloc_2d_dbl(n_in + 1, n_hidden + 1);
	hidden_weights = alloc_2d_dbl(n_hidden + 1, n_out + 1);
	
	

	//逐个样本扫描
	for(k=0;k<num;k++)
	{ 
	if(k==0)
		{
           if( r_weight(input_weights,n_in,n_hidden,"win1.dat")==false)
		   return 0 ;
	       if(r_weight(hidden_weights,n_hidden,n_out,"whi1.dat")==false)
		   return 0;
		}
	   else if(k==1)
	   {
	if( r_weight(input_weights,n_in,n_hidden,"win3.dat")==false)
		    return 0;
	        if(r_weight(hidden_weights,n_hidden,n_out,"whi3.dat")==false)
		  return 0;
		}
		else
		{
			if( r_weight(input_weights,n_in,n_hidden,"win2.dat")==false)
		   return 0;
	        if(r_weight(hidden_weights,n_hidden,n_out,"whi2.dat")==false)
	 return 0;
		}

		//将提取的样本的特征向量输送到输入层上
		for(i=1;i<=n_in;i++)
			input_unites[i]=data_in[k][i-1];
		
		//前向输入激活
        bpnn_layerforward(input_unites,hidden_unites,
			input_weights, n_in,n_hidden);
        bpnn_layerforward(hidden_unites, output_unites,
			hidden_weights,n_hidden,n_out);
		
		//根据输出结果进行识别
		int result=0 ;
		//考察每一位的输出
		for(i=1;i<=n_out;i++)
		{
			//如果大于0.5判为1
			if(k==0)
			{
			if(output_unites[i]>0.55)
				
				result+=(int)pow(2,double(4-i));
			}
			else if(k==1)
			{
                if(output_unites[i]>0.36)
				
				result+=(int)pow(2,double(4-i));
			}
			else
			{
               if(output_unites[i]>0.4)
				
				result+=(int)pow(2,double(4-i));
			}
		}
		
		//如果判定的结果小于等于9，认为合理
		if(result<=9)
			recognize[k]=result;
		//如果判定的结果大于9，认为不合理将结果定位为一个特殊值20
		if(result>9)
			recognize[k]=20;
	}
	

	
	//将识别的结果显示出来
	CString str;
	CString* str1=new CString;
	for(i=0;i<num;i++)
	{
		if(recognize[i]!=20)
		{
			if(i==0)
			{
				str=Charac[recognize[i]];
			}
			else if(i==1||i==2)
			{
				str=Letter[recognize[i]];
			}
			else
			{
				str=Num[recognize[i]];
			}
		}
		
		else
		{
			str=Charac[3];
		}
		(*str1)+=str;
	}
	
	//通知用户训练完成
	
    
	//::MessageBox(NULL,str1.c_str(),"识别结果",NULL);
	
	//释放存储空间
	free(input_unites);
	free(hidden_unites);
	free(output_unites);
	free(input_weights);
	free(hidden_weights);

	return str1;
	
	
}


