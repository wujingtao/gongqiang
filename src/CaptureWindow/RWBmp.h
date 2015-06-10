#ifndef  RWBMP_INC
#define  RWBMP_INC

#include <windows.h>  

class RWBmp
{
public:
	RWBmp(void);
	~RWBmp(void);

public:
	/* 
	 *功能: 获取8位bmp图
	 *参数: cBuffer in 图形字节
	        iWidth in 宽度
					iHeight in 高度
					iSize out bmp图像大小
	 *返回: bmp格式的字节大小
	 */
	static unsigned char* Get8BitBmp(unsigned char* cBuffer, int iWidth, int iHeight, int &iSize);

	/* 
	 *功能: 获取24位bmp图
	 *参数: cBuffer in 图像字节
	        iWidth in 宽度
					iHeight in 高度
					iSize out bmp图像大小
	 *返回: bmp格式的字节大小
	 */
	static unsigned char* Get24BitBmp(unsigned char* cBuffer, int iWidth, int iHeight, int &iSize);


	//写8位
	static bool Write8BitBmp(unsigned char *cBuffer, int iWidth, int iHeight, const char *cName);
	//写24位
	static bool Write24BitBmp(unsigned char *cBuffer, int iWidth, int iHeight, const char *cName);

private:
	static unsigned char* bmpBuffer;
};

#endif   /* ----- #ifndef RWBMP_INC  ----- */
