#include "CaptureWindow.h"
#include <QtGui/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

	CaptureWindow w;
	//…Ë÷√÷√∂•
	//w.setWindowFlags(Qt::WindowStaysOnTopHint);
	//»´∆¡
	//w.showFullScreen();
	//œ‘ æ
	w.show();

	return a.exec();
}
