#include "MyToolBar.h"


MyToolBar::MyToolBar(QWidget *parent) : QToolBar(parent)
{
	this->setWindowTitle("������ʾ");
	this->setAllowedAreas(Qt::RightToolBarArea);

}


MyToolBar::~MyToolBar(void)
{
}


void MyToolBar::enterEvent(QEvent *qevent)
{
	emit StatusChanged();	
}