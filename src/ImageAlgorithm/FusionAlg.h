/*
 * =====================================================================================
 *
 *       Filename:  FusionAlg.h
 *        Created:  2014/8/7 11:10:42
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  图像算法类,每个算法debug大概30ms。CLAHES暂未确定
 *
 * =====================================================================================
 */

#include "OpHelp.h"

#include "imagealgorithm_global.h"

#include <qmutex.h>
#include <qwaitcondition.h>

class IMAGEALGORITHM_EXPORT FusionAlg
{
public:
	FusionAlg(int width, int height, int circleX, int circleY, float gainR, float gainB);
	~FusionAlg(void);

public:
	//通用
	//添加R,G,B图像到队列
	void AddRGB(unsigned char* r, unsigned char* g, unsigned char* b);
	//判断队列是否有R,G,B图像,并保存到成员变量
	bool IsRGB();
	//添加图像到队列
	void AddImage(unsigned char* img);
	//获取队列中的最终图像
	bool GetImage(unsigned char** img);
	//释放队列当前图像
	void ReleaseImage();

	//启动图像算法线程
	void fusioning();
	void fusioningS();

	//1.高斯+边缘算法
	//高斯算法:去噪，滤波
	void gaussianFilter(BGRbmp rgbBmp);  
	//边缘算法:增强边缘显示
	void EdgeE(BGRbmp bgrBmp);

	//判断是否有高斯算法的队列
	bool IsGaussianRGB();
	void MallocGaussian();
	void MallocImage();

	//2.直方图+高斯算法
	//直方图:对比度增强
	void CLAHES(BGRbmp bgrBmp);
	//高斯算法
	void gaussianFilterS(BGRbmp rgbBmp);  

	//是否有直方图队列
	bool IsClaheRGBS();
	//处理算法后的内存
	void MallocClaheS();
	void MallocImageS();

private:
	//图像大小
	unsigned int ImageSize()
	{
		return miWidth*miHeight;	
	}

	//start 直方内算法
	void MakeLut (unsigned char * pLUT, unsigned char Min, unsigned char Max, unsigned int uiNrBins);
	void ClipHistogram (unsigned long* pulHistogram, unsigned int   
                    uiNrGreylevels, unsigned long ulClipLimit);
	void MakeHistogram (unsigned char* pImage, unsigned int uiXRes,   
		                unsigned int uiSizeX, unsigned int uiSizeY,   
			            unsigned long* pulHistogram,   
				        unsigned int uiNrGreylevels, unsigned char* pLookupTable);
	void MapHistogram (unsigned long* pulHistogram, unsigned char Min, unsigned char Max,   
		               unsigned int uiNrGreylevels, unsigned long ulNrOfPixels);
	void Interpolate (unsigned char * pImage, int uiXRes, unsigned long * pulMapLU,   
		             unsigned long * pulMapRU, unsigned long * pulMapLB,  unsigned long * pulMapRB,   
			          unsigned int uiXSize, unsigned int uiYSize, unsigned char * pLUT); 
	//-end

private:
	//最终图像
	unsigned char* mImage;

	//保存R,G,B图像
	unsigned char* mR;
	unsigned char* mG;
	unsigned char* mB;

	//1高斯和边缘
	unsigned char* mGaussianR;
	unsigned char* mGaussianG;
	unsigned char* mGaussianB;

	unsigned char* mEdgeeR;
	unsigned char* mEdgeeG;
	unsigned char* mEdgeeB;

	//2直方和高斯
	unsigned char* mClaheRS;
	unsigned char* mClaheGS;
	unsigned char* mClaheBS;

	unsigned char* mGaussianRS;
	unsigned char* mGaussianGS;
	unsigned char* mGaussianBS;

	//保存大小
	unsigned int miWidth;
	unsigned int miHeight;
	int miCircleX;	
	int miCircleY;	
	float mfGainR;
	float mfGainB;

	enum {uiMAX_REG_X=16, uiMAX_REG_Y = 16, uiNR_OF_GREY = 256 };
};
