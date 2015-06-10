/*
 * =====================================================================================
 *
 *       Filename:  CaptureWindow.h
 *        Created:  2014/7/8 9:13:12
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  主窗体
 *
 * =====================================================================================
 */

#ifndef  CAPTUREWINDOW_INC
#define  CAPTUREWINDOW_INC




//qt
#include <QMainWindow>

#include "ui_capture.h"

class ViewWidget;
class FusionAlg;
class ImageThread;
class ShowThread;
class Win_QextSerialPort;
class CaptureWindow :
	public QMainWindow
{
	Q_OBJECT
public:
	CaptureWindow(void);
	~CaptureWindow(void);

public:

  //窗体运行顺序
	void Begin();

public slots:
	//暂停处理
	void PauseSlot(bool ok);
	//保存按钮
	void SaveSlot();
	  /* 
	 *功能: 发送图片信号后触发该函数
	 *参数: buffer in bitmap图像字节
	        iSize in 字节大小
	 *返回: void
	  */

	//关闭软件
	void CancelSlot();

	void ImageSlot(unsigned char* buffer, int iSize);
	/* 
	*功能: 当串口收到消息后触发该函数
	*参数: null
	*返回: void
	*/
	void MyComSlot();

	//继承
protected:
	//鼠标滚轮事件
	void wheelEvent(QWheelEvent *event);

private:
	/* 
	 *功能: 串口收到信息为滤波盘后设置
	 *参数: null
	 *返回: void
	 */
	void SetFilter();

	/* 
	 *功能: 串口收到信息为操作键后设置
	 *参数: null
	 *返回: void
	 */
	void SetKey();

private:
	//串口
	Win_QextSerialPort *mMyCom;
	//串口接收消息4次为一周期
	int miVal;
	//保存串口4次消息
	char mcRs232[4];

	//算法类
	FusionAlg* mFusionAlg;
	//图像处理类
	ImageThread *mImgTrd;
	//图像显示
	ShowThread *mShowTrd;
	//参数显示窗体
	ViewWidget *mViewWidget;

	//放大缩小的临时大小
	int miWidth;
	int miHeight;

	Ui::capture mCaptureUi;	
};

#endif   /* ----- #ifndef CAPTUREWINDOW_INC  ----- */
