#include "MyDockWidget.h"


MyDockWidget::MyDockWidget(QWidget *parent) : QDockWidget(parent)
{
	this->hide();
}


MyDockWidget::~MyDockWidget(void)
{
}


void MyDockWidget::leaveEvent(QEvent *qevent)
{
	this->hide();
}