#include "MyDockWidget.h"

MyDockWidget::MyDockWidget(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);

	//��ͣ
	connect(ui.pbPause, SIGNAL(clicked()), this, SLOT(PauseSlot()));
	//����
	connect(ui.pbSave, SIGNAL(clicked()), this, SLOT(SaveSlot()));
	//�˳�ϵͳ
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(CancelSlot()));
}

MyDockWidget::~MyDockWidget()
{

}

void MyDockWidget::PauseSlot()
{
	QString str = ui.lbPause->text();
	if(str == "��ͣ")
	{
		emit PauseSignal(true);
		//mImgTrd->SetPause(true);
		ui.lbPause->setText("��ʼ");

		QIcon icon;
        icon.addFile(QString::fromUtf8("img/playWhite.png"), QSize(), QIcon::Normal, QIcon::Off);
		ui.pbPause->setIcon(icon);
	}
	else if(str == "��ʼ")
	{
		emit PauseSignal(false);
		//mImgTrd->SetPause(false);	
		ui.lbPause->setText("��ͣ");

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