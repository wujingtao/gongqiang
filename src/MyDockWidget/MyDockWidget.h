#ifndef MYDOCKWIDGET_H
#define MYDOCKWIDGET_H

#include <QDockWidget>
#include "ui_mydockwidget.h"

class MyDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	MyDockWidget(QWidget *parent = 0);
	~MyDockWidget();

signals:
	void PauseSignal(bool ok);
	void SaveSignal();
	void CancelSignal();
public slots:
	void PauseSlot();
	void SaveSlot();
	void StatusSlot();
	void CancelSlot();

	//ผฬณะ
protected:
	void leaveEvent(QEvent *qevent);

private:
	Ui::MyDockWidget ui;
};

#endif // MYDOCKWIDGET_H
