#include "ViewWidget.h"
#include <qdatetime.h>

#include <windows.h>

ViewWidget::ViewWidget(QWidget *parent):QWidget(parent)
{
	mbSave = false;

	mViewUi.setupUi(this);
	mViewUi.lbMsg->setVisible(false);
	startTimer(1000);
}


ViewWidget::~ViewWidget(void)
{
}


 void ViewWidget::timerEvent( QTimerEvent *qevent )
 {
	QString str;
 	mViewUi.lbTime->setText((QDateTime::currentDateTime()).toString("yyyy-MM-dd hh:mm:ss"));
	mViewUi.lbRadio->setText(str.setNum(GlobalArgs::g_radio));
	mViewUi.lbFrame->setText(str.setNum(GlobalArgs::g_frame));
	mViewUi.lbMsg->setVisible(mbSave); 
	mbSave = false;

	GlobalArgs::g_radio = 0;
	GlobalArgs::g_frame= 0;
 }

 void ViewWidget::SetSave(bool save)
 {
	 mbSave = save;
 }