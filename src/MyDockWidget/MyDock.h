#ifndef MYDOCK_H
#define MYDOCK_H

#include "mydock_global.h"
#include "MyDockWidget.h"

class MYDOCK_EXPORT MyDock 
{
public:
	MyDock();
	~MyDock();

public:
	static MyDockWidget* GetInstance(QWidget *parent);

private:
	static MyDockWidget* s_mMyDockWidget;
	
};

#endif // MYDOCK_H
