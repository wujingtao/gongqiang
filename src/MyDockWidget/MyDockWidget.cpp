#include "MyDockWidget.h"

MyDockWidget::MyDockWidget(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);

	//暂停
	connect(ui.pbPause, SIGNAL(clicked()), this, SLOT(PauseSlot()));
	//保存
	connect(ui.pbSave, SIGNAL(clicked()), this, SLOT(SaveSlot()));
	//退出系统
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(CancelSlot()));
}

MyDockWidget::~MyDockWidget()
{

}

void MyDockWidget::PauseSlot()
{
	QString str = ui.lbPause->text();
	if(str == "暂停")
	{
		emit PauseSignal(true);
		//mImgTrd->SetPause(true);
		ui.lbPause->setText("开始");

		QIcon icon;
        icon.addFile(QString::fromUtf8("img/playWhite.png"), QSize(), QIcon::Normal, QIcon::Off);
		ui.pbPause->setIcon(icon);
	}
	else if(str == "开始")
	{
		emit PauseSignal(false);
		//mImgTrd->SetPause(false);	
		ui.lbPause->setText("暂停");

		QIcon icon;
        icon.addFile(QString::fromUtf8("img/pauseWhite.png"), QSize(), QIcon::Normal, QIcon::Off);
		ui.pbPause->setIcon(icon);
	}
}

void MyDockWidget::SaveSlot()
{
	emit SaveSignal();
}

void MyDockWidget::leaveEvent(QEvent *qevent)
{
	this->hide();
}

void MyDockWidget::StatusSlot()
{
	this->show();
}

void MyDockWidget::CancelSlot()
{
	emit CancelSignal();
}