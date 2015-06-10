#include "RWBmp.h"
#include <assert.h>
#include <stdio.h>



unsigned char* RWBmp::bmpBuffer = NULL;

RWBmp::RWBmp(void)
{
}


RWBmp::~RWBmp(void)
{
	if(bmpBuffer != NULL)
	{
		delete[] bmpBuffer;	
		bmpBuffer = NULL;
	}
}

unsigned char* RWBmp::Get8BitBmp(unsigned char* cBuffer, int iWidth, int iHeight, int &iSize)
{
	int iBitCount = 8;   
    int iColorSize= 1024;  
    int iLineByte = (iWidth * iBitCount / 8 + 3) / 4 * 4;

    BITMAPFILEHEADER fileHead;  
    fileHead.bfType = 0x4D42;   
    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iColorSize + iLineByte*iHeight;  
    fileHead.bfReserved1 = 0;  
    fileHead.bfReserved2 = 0;  
    fileHead.bfOffBits = 54 + iColorSize;  

    BITMAPINFOHEADER infoHead;  
    infoHead.biSize = 40;    //本结构的长度  
    infoHead.biWidth = iWidth; //位图的宽度，以像素为单位  
    infoHead.biHeight  = iHeight; //位图的宽度，以像素为单位  
    infoHead.biPlanes  = 1; //目标设备的级别，必须是1  
    infoHead.biBitCount = 8; //每个像素所占的位数(bit)，其值必须为1（黑白图像），4（16色图），8（256色），24（真彩色图）  
    infoHead.biCompression = BI_RGB; //位图压缩类型，有效的值为BI_RGB（未经压缩）、BI_RLE4，BI_RLE8，BI_BITFIEDS（均为Windows定义常量）。  
    infoHead.biSizeImage  = iLineByte*iHeight;  //实际的位图数据占用的字节数  
    infoHead.biXPelsPerMeter = 0; //指定目标数据的水平分辨率，单位是像素/米。  
    infoHead.biYPelsPerMeter = 0; //指定目标数据的垂直分辨率，单位是像素/米。  
    infoHead.biClrUsed = 0; //位图实际用到的颜色数，如果该值为零，则用到的颜色数为2的biBitCount次幂  
    infoHead.biClrImportant  = 0; //位图显示过程中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。  

    RGBQUAD *pColorTable = new RGBQUAD[256];  
    for (int i=0; i<256; i++)  
    {  
        pColorTable[i].rgbRed = i;  
        pColorTable[i].rgbGreen = i;  
        pColorTable[i].rgbBlue = i;  
        pColorTable[i].rgbReserved = 0;  
    }  

	iSize = fileHead.bfSize;

	static int s_iSize = 0;
	unsigned int curSize = iWidth * iHeight;
	if(s_iSize != curSize)
	{
		s_iSize = curSize;
		if(bmpBuffer != NULL)
		{
			delete[] bmpBuffer;	
			bmpBuffer = NULL;
		}
		bmpBuffer = new unsigned char[iSize];
	}
	int iFileHeadSize = sizeof(BITMAPFILEHEADER);
	memcpy(bmpBuffer, (char*)&fileHead, iFileHeadSize);
	int iInfoHead = sizeof(BITMAPINFOHEADER);
	memcpy(bmpBuffer+iFileHeadSize, (char*)&infoHead, iInfoHead);
	int iRgbGuad = sizeof(RGBQUAD);
	memcpy(bmpBuffer+iFileHeadSize+iInfoHead, (char*)pColorTable, iRgbGuad);
	memcpy(bmpBuffer+iFileHeadSize+iInfoHead+iRgbGuad, cBuffer, iLineByte*iHeight);
	

    delete pColorTable;  

	return bmpBuffer;
}

unsigned char* RWBmp::Get24BitBmp(unsigned char* cBuffer, int iWidth, int iHeight, int &iSize)
{
	int iBitCount = 24;
	int iColorSize = 0;
	//必须是4的倍数
	int iLineByte = (iWidth * iBitCount /8 + 3) /4 * 4;

	//图像文件头
    BITMAPFILEHEADER fileHead;  
    fileHead.bfType = 0x4D42;   
    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iColorSize + iLineByte*iHeight;  
    fileHead.bfReserved1 = 0;  
    fileHead.bfReserved2 = 0;  
    fileHead.bfOffBits = 54 + iColorSize;  

	//图像结构图
    BITMAPINFOHEADER infoHead;  
    infoHead.biSize = 40;    //本结构的长度  
    infoHead.biWidth = iWidth; //位图的宽度，以像素为单位  
    infoHead.biHeight  = -iHeight; //位图的宽度，以像素为单位  
    infoHead.biPlanes  = 1; //目标设备的级别，必须是1  
    infoHead.biBitCount = iBitCount; //每个像素所占的位数(bit)，其值必须为1（黑白图像），4（16色图），8（256色），24（真彩色图）  
    infoHead.biCompression = BI_RGB; //位图压缩类型，有效的值为BI_RGB（未经压缩）、BI_RLE4，BI_RLE8，BI_BITFIEDS（均为Windows定义常量）。  
    infoHead.biSizeImage  = iLineByte*iHeight;  //实际的位图数据占用的字节数  
    infoHead.biXPelsPerMeter = 0; //指定目标数据的水平分辨率，单位是像素/米。  
    infoHead.biYPelsPerMeter = 0; //指定目标数据的垂直分辨率，单位是像素/米。  
    infoHead.biClrUsed = 0; //位图实际用到的颜色数，如果该值为零，则用到的颜色数为2的biBitCount次幂  
    infoHead.biClrImportant  = 0; //位图显示过程中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。  

	iSize = fileHead.bfSize;

	static int s_iSize = 0;
	unsigned int curSize = iWidth * iHeight;
	if(s_iSize != curSize)
	{
		s_iSize = curSize;
		if(bmpBuffer != NULL)
		{
			delete[] bmpBuffer;	
			bmpBuffer = NULL;
		}
		bmpBuffer = new unsigned char[iSize];
	}
	int iFileHeadSize = sizeof(BITMAPFILEHEADER);
	memcpy(bmpBuffer, (char*)&fileHead, iFileHeadSize);
	int iInfoHead = sizeof(BITMAPINFOHEADER);
	memcpy(bmpBuffer+iFileHeadSize, (char*)&infoHead, iInfoHead);
	memcpy(bmpBuffer+iFileHeadSize+iInfoHead, cBuffer, iLineByte*iHeight);
	
	return bmpBuffer;
}

bool RWBmp::Write8BitBmp(unsigned char *cBuffer, int iWidth, int iHeight, const char *cName)   
{  
    //对参数的有效性进行检查  
    assert((cBuffer != NULL) && (iWidth > 0) && (iHeight > 0) && (cName != NULL));  
  
    //每个像素所占的位数(bit)  
    int biBitCount = 8;   
  
    //颜色表大小，以字节为单位，灰度图像颜色表为1024字节  
    int colorTablesize = 1024;  
  
    //待存储图像数据每行字节数为4的倍数  
    int lineByte = (iWidth * biBitCount / 8 + 3) / 4 * 4;  
  
    //以二进制写的方式打开文件  
    FILE *fp = fopen(cName, "wb");  
    if(fp == NULL)  
    {  
        return false;  
    }  
  
    //申请位图文件头结构变量，填写文件头信息  
    BITMAPFILEHEADER fileHead;  
    fileHead.bfType = 0x4D42;   //bmp类型  
  
    //bfSize是图像文件4个组成部分之和  
    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*iHeight;  
    fileHead.bfReserved1 = 0;  
    fileHead.bfReserved2 = 0;  
  
    //bfOffBits是图像文件前3个部分所需空间之和  
    fileHead.bfOffBits = 54 + colorTablesize;  
  
    //写文件头进文件  
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);  
  
    //申请位图信息头结构变量，填写信息头信息  
    BITMAPINFOHEADER head;  
    head.biSize = 40;    //本结构的长度  
    head.biWidth = iWidth; //位图的宽度，以像素为单位  
    head.biHeight  = -iHeight; //位图的宽度，以像素为单位  
    head.biPlanes  = 1; //目标设备的级别，必须是1  
    head.biBitCount = 8; //每个像素所占的位数(bit)，其值必须为1（黑白图像），4（16色图），8（256色），24（真彩色图）  
    head.biCompression = BI_RGB; //位图压缩类型，有效的值为BI_RGB（未经压缩）、BI_RLE4，BI_RLE8，BI_BITFIEDS（均为Windows定义常量）。  
    head.biSizeImage  = lineByte*iHeight;  //实际的位图数据占用的字节数  
    head.biXPelsPerMeter = 0; //指定目标数据的水平分辨率，单位是像素/米。  
    head.biYPelsPerMeter = 0; //指定目标数据的垂直分辨率，单位是像素/米。  
    head.biClrUsed = 0; //位图实际用到的颜色数，如果该值为零，则用到的颜色数为2的biBitCount次幂  
    head.biClrImportant  = 0; //位图显示过程中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。  
  
    //写位图信息头进内存  
    fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);  
  
    //申请颜色表所需要的空间，写颜色表进文件  
    RGBQUAD *pColorTable = new RGBQUAD[256];  
    for (int i=0; i<256; i++)  
    {  
        pColorTable[i].rgbRed = i;  
        pColorTable[i].rgbGreen = i;  
        pColorTable[i].rgbBlue = i;  
        pColorTable[i].rgbReserved = 0;  
    }  
    fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);  
    delete pColorTable;  
  
    //写格式图像数据写进文件  
    fwrite(cBuffer, iHeight*lineByte, 1, fp);  
  
    //关闭文件  
    fclose(fp);   
  
    return true;  
}

bool RWBmp::Write24BitBmp(unsigned char *cBuffer, int iWidth, int iHeight, const char *cName)
{
	assert((cBuffer != NULL) && (iWidth > 0) && (iHeight > 0) && (cName != NULL));  
	
	//以二进制写的方式打开文件  
    FILE *fp = fopen(cName, "wb");  
    if(fp == NULL)  
    {  
		fclose(fp);
        return false;  
    }  
	//ofstream ofs(cName, ofstream::trunc | ofstream::binary);
	//if(!ofs)
	//{
	//	return false;	
	//}

    int iBitCount = 24;   
    int iColorSize= 0;  
    int iLineByte = (iWidth * iBitCount / 8 + 3) / 4 * 4;  

    BITMAPFILEHEADER fileHead;  
    fileHead.bfType = 0x4D42;   
    //bfSize是图像文件4个组成部分之和  
    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iColorSize + iLineByte*iHeight;  
    fileHead.bfReserved1 = 0;  
    fileHead.bfReserved2 = 0;  
    //bfOffBits是图像文件前3个部分所需空间之和  
    fileHead.bfOffBits = 54 + iColorSize;  

	//ofs.write((char*)&fileHead, sizeof(BITMAPFILEHEADER));
	//ofs.flush();
	//写文件头进文件  
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);  

    BITMAPINFOHEADER infoHead;  
    infoHead.biSize = 40;    //本结构的长度  
    infoHead.biWidth = iWidth; //位图的宽度，以像素为单位  
    infoHead.biHeight  = -iHeight; //位图的宽度，以像素为单位  
    infoHead.biPlanes  = 1; //目标设备的级别，必须是1  
    infoHead.biBitCount = iBitCount; //每个像素所占的位数(bit)，其值必须为1（黑白图像），4（16色图），8（256色），24（真彩色图）  
    infoHead.biCompression = BI_RGB; //位图压缩类型，有效的值为BI_RGB（未经压缩）、BI_RLE4，BI_RLE8，BI_BITFIEDS（均为Windows定义常量）。  
    infoHead.biSizeImage  = iLineByte*iHeight;  //实际的位图数据占用的字节数  
    infoHead.biXPelsPerMeter = 0; //指定目标数据的水平分辨率，单位是像素/米。  
    infoHead.biYPelsPerMeter = 0; //指定目标数据的垂直分辨率，单位是像素/米。  
    infoHead.biClrUsed = 0; //位图实际用到的颜色数，如果该值为零，则用到的颜色数为2的biBitCount次幂  
    infoHead.biClrImportant  = 0; //位图显示过程中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。  

	//写位图信息头进内存  
    fwrite(&infoHead, sizeof(BITMAPINFOHEADER), 1, fp);  

	//ofs.write((char*)&infoHead, sizeof(BITMAPINFOHEADER));
	//ofs.flush();


	//ofs.write((char*)cBuffer, iLineByte*iHeight);
	//ofs.flush();
	//ofs.close();

	//写格式图像数据写进文件  
    fwrite(cBuffer, iHeight*iLineByte, 1, fp);  
  
    //关闭文件  
    fclose(fp);   

	return true;
}