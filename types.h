#ifndef _TYPES_H
#define _TYPES_H

struct Prototype
{
	char  text;
	float *features;	
	char   fullpath[256];   //该数据用于测试和保存图像，在实际应用中需要删除
};

#endif