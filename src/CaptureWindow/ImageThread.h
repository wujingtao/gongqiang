/*
 * =====================================================================================
 *
 *       Filename:  ImageThread.h
 *        Created:  2014/7/8 10:07:37
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  线程类，处理图像
 *
 * =====================================================================================
 */

#ifndef  IMAGETHREAD_INC
#define  IMAGETHREAD_INC


#include "OpDefs.h"
//qt
#include <qthread.h>
#include <qwaitcondition.h>
#include <qmutex.h>

//flycapture
#include "Error.h"
namespace FlyCapture2
{
	class Camera;
}

//class Fusion;
class FusionAlg;
class ImageThread :
	public QThread
{
public:
	ImageThread(FlyCapture2::Camera *pCamera, FusionAlg *fusionAlg);
	~ImageThread(void);

public:
	//设置线程暂停标志
	void SetPause(bool pause);

	//获取当前rgb,用于保存图片
	bool GetRGB(unsigned char** r, unsigned char** g, unsigned char** b);

private:

	/* 
	 *功能: 显示8位图像
	 *参数: null
	 *返回: void
	 */
	void showCamImage();

	/* 
	 *功能: 显示合成的24位图像
	 *参数: null
	 *返回: void
	 */
	void showImage24();


	//继承方法
protected:
	void run();

private:
	FlyCapture2::Camera* mpCamera;
	FusionAlg* mFusionAlg;
	unsigned char *mpImageR;
	unsigned char *mpImageG;
	unsigned char *mpImageB;

	FlyCapture2::Error mErr;
	//暂停图像线程
	QWaitCondition mWaitCondition;
	QMutex mMutex;
	//默认为0,3次为1为一周期
	bool mbR;
	bool mbB;
	bool mbG;


	bool mbPause;

	//当前图像合成模式
	static unsigned int s_mCurrentMode;
};

#endif   /* ----- #ifndef IMAGETHREAD_INC  ----- */
