#pragma once
//qt
#include <qtoolbar.h>
#include <QWidget>
class MyToolBar :
	public QToolBar
{
	Q_OBJECT
public:
	MyToolBar(QWidget *parent);
	~MyToolBar(void);

//�̳��鷽��
protected:
	void enterEvent(QEvent *qevent);

signals:
	void StatusChanged();
};

