#include "MyDock.h"

MyDockWidget* MyDock::s_mMyDockWidget = 0;
MyDock::MyDock()
{

}

MyDock::~MyDock()
{

}

MyDockWidget* MyDock::GetInstance(QWidget *parent)
{
	if(s_mMyDockWidget == 0)
	{
		s_mMyDockWidget = new MyDockWidget(parent);	
	}

	return s_mMyDockWidget;
}