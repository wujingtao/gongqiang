/*
 * =====================================================================================
 *
 *       Filename:  CaptureWindow.h
 *        Created:  2014/7/8 9:13:12
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  ������
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

  //��������˳��
	void Begin();

public slots:
	//��ͣ����
	void PauseSlot(bool ok);
	//���水ť
	void SaveSlot();
	  /* 
	 *����: ����ͼƬ�źź󴥷��ú���
	 *����: buffer in bitmapͼ���ֽ�
	        iSize in �ֽڴ�С
	 *����: void
	  */

	//�ر����
	void CancelSlot();

	void ImageSlot(unsigned char* buffer, int iSize);
	/* 
	*����: �������յ���Ϣ�󴥷��ú���
	*����: null
	*����: void
	*/
	void MyComSlot();

	//�̳�
protected:
	//�������¼�
	void wheelEvent(QWheelEvent *event);

private:
	/* 
	 *����: �����յ���ϢΪ�˲��̺�����
	 *����: null
	 *����: void
	 */
	void SetFilter();

	/* 
	 *����: �����յ���ϢΪ������������
	 *����: null
	 *����: void
	 */
	void SetKey();

private:
	//����
	Win_QextSerialPort *mMyCom;
	//���ڽ�����Ϣ4��Ϊһ����
	int miVal;
	//���洮��4����Ϣ
	char mcRs232[4];

	//�㷨��
	FusionAlg* mFusionAlg;
	//ͼ������
	ImageThread *mImgTrd;
	//ͼ����ʾ
	ShowThread *mShowTrd;
	//������ʾ����
	ViewWidget *mViewWidget;

	//�Ŵ���С����ʱ��С
	int miWidth;
	int miHeight;

	Ui::capture mCaptureUi;	
};

#endif   /* ----- #ifndef CAPTUREWINDOW_INC  ----- */
