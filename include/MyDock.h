#ifndef MYDOCK_H
#define MYDOCK_H

class MyDockWidget;
class MyDock
{
public:
	MyDock();
	~MyDock();

public:
	static MyDockWidget* GetInstance(QWidget* parent);

private:
	static MyDockWidget* s_mMyDockWidget;
	
};

#endif // MYDOCK_H
