/*
 * =====================================================================================
 *
 *       Filename:  ShowThread.h
 *        Created:  2014/7/30 10:52:05
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  显示图像
 *
 * =====================================================================================
 */
#ifndef  SHOWTHREAD_INC
#define  SHOWTHREAD_INC

#include <qthread.h>
#include <qobject.h>

class FusionAlg;
class ShowThread : public QThread
{
	Q_OBJECT
public:
	ShowThread(FusionAlg *fusion);
	~ShowThread(void);

signals:
	/* 
	 *功能: 当获取到图像后发送该信号.主窗体会接收该信号.
	 *参数: buffer in bmp图像
	        iSize in bmp大小
	 *返回: void
	 */
	void ImageSignal(unsigned char *buffer, int iSize);

//继承方法
protected:
	void run();

private:
	FusionAlg *mFusionAlg;
};

#endif   /* ----- #ifndef SHOWTHREAD_INC  ----- */
