/*
 * =====================================================================================
 *
 *       Filename:  ShowThread.h
 *        Created:  2014/7/30 10:52:05
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  ��ʾͼ��
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
	 *����: ����ȡ��ͼ����͸��ź�.���������ո��ź�.
	 *����: buffer in bmpͼ��
	        iSize in bmp��С
	 *����: void
	 */
	void ImageSignal(unsigned char *buffer, int iSize);

//�̳з���
protected:
	void run();

private:
	FusionAlg *mFusionAlg;
};

#endif   /* ----- #ifndef SHOWTHREAD_INC  ----- */
