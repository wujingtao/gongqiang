#pragma once
//qt
#include <qdockwidget.h>

class MyDockWidget :
	public QDockWidget
{
public:
	MyDockWidget(QWidget *parent = 0);
	virtual ~MyDockWidget(void);

//ผฬณะ
protected:
	void leaveEvent(QEvent *qevent);
};

