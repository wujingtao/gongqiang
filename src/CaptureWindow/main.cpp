#include "CaptureWindow.h"
#include <QtGui/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

	CaptureWindow w;
	//�����ö�
	//w.setWindowFlags(Qt::WindowStaysOnTopHint);
	//ȫ��
	//w.showFullScreen();
	//��ʾ
	w.show();

	return a.exec();
}
