#include "CaptureWindow.h"

//include
#include "win_qextserialport.h"
#include "FlyCapture.h"
#include "ImageThread.h"
#include "ShowThread.h"
#include "FusionAlg.h"
#include "ViewWidget.h"
#include "RWBmp.h"
//qt
#include <qmessagebox.h>
#include <QDate>
#include <QTime>
#include <qdir.h>
#include <QDesktopWidget>
#include <QWheelEvent>
#include <qmessagebox.h>

#include <sstream>
#include <fstream>
using std::stringstream;
using std::ofstream;

#include "MyDock.h"
#include "MyToolBar.h"

CaptureWindow::CaptureWindow(void)
{
	mFusionAlg = NULL;
	mImgTrd = NULL;
	mShowTrd = NULL;
	mViewWidget = NULL;
	miVal = 0;

	mCaptureUi.setupUi(this);

	//1.toolbar
	MyToolBar* myToolBar = new MyToolBar(this);
	//2.action
	QAction *toolAct = new QAction("������", this);
	QIcon icon;
	icon.addFile(QString::fromUtf8("img/tool.png"), QSize(), QIcon::Normal, QIcon::Off);
	toolAct->setIcon(icon);
	myToolBar->addAction(toolAct);
	//3.dockwidget
	QDockWidget* dockWidget = (QDockWidget*)MyDock::GetInstance(this);

	this->addToolBar(Qt::RightToolBarArea, myToolBar);
	this->addDockWidget(static_cast<Qt::DockWidgetArea>(0x2), dockWidget); 


	//ͼ�������ʾ
	miWidth = FlyCapture::ImageWidth; 
	miHeight = FlyCapture::ImageHeight;
	int ax = (QApplication::desktop()->width() - miWidth)/2;
	int ay = (QApplication::desktop()->height() - miWidth)/2;
	if(ax >0 && ay >0)
	{
		mCaptureUi.lbImage->setGeometry(ax, ay, FlyCapture::ImageWidth, FlyCapture::ImageHeight);
	}

	//������
	connect(myToolBar, SIGNAL(StatusChanged()), (QObject*)dockWidget, SLOT(StatusSlot()));
	//��ͣ
	connect((QObject*)dockWidget, SIGNAL(PauseSignal(bool)), this, SLOT(PauseSlot(bool)));
	//ͼ�񱣴�
	connect((QObject*)dockWidget, SIGNAL(SaveSignal()), this, SLOT(SaveSlot()));
	//�رճ���
	connect((QObject*)dockWidget, SIGNAL(CancelSignal()), this, SLOT(CancelSlot()));

	Begin();
}


CaptureWindow::~CaptureWindow(void)
{
}



void CaptureWindow::PauseSlot(bool ok)
{
	mImgTrd->SetPause(ok);
}

void CaptureWindow::SaveSlot()
{
	unsigned char* r = NULL;
	unsigned char* g = NULL;
	unsigned char* b = NULL;

	QString strDate = QDate::currentDate().toString("yyyyMMdd");
	QString strTime = QTime::currentTime().toString("hhmmss");
	QString strPath = "d:/result/"+strDate;

	//�����ļ���
	QDir dir;
	if(!dir.exists(strPath))
	{
		dir.mkpath(strPath);	
	}
	strPath = strPath+"/"+strTime;

	if(mImgTrd == NULL)
	{return;}
	//����r,g,b8λͼ��
	if(mImgTrd->GetRGB(&r, &g, &b))
	{
		RWBmp::Write8BitBmp(r, FlyCapture::ImageWidth, FlyCapture::ImageHeight, (strPath.append("R.bmp")).toStdString().c_str());
		RWBmp::Write8BitBmp(g, FlyCapture::ImageWidth, FlyCapture::ImageHeight, (strPath.append("G.bmp")).toStdString().c_str());
		RWBmp::Write8BitBmp(b, FlyCapture::ImageWidth, FlyCapture::ImageHeight, (strPath.append("B.bmp")).toStdString().c_str());
	}

	//����rgb24ͼ��
	//�д�
	/*
	unsigned char* img = mShowTrd->GetSaveImg();
	if(img != NULL)
	{
		RWBmp::Write24BitBmp(img, FlyCapture::ImageWidth, FlyCapture::ImageHeight, (strPath.append(".bmp").toStdString()).c_str());
	}
	*/

	mViewWidget->SetSave(true);	
}

void CaptureWindow::Begin()
{
	//1.��ʼ������
	struct PortSettings myComSetting = {BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
	QString sCom = "com4";
	mMyCom = new Win_QextSerialPort(sCom, myComSetting, QextSerialBase::EventDriven);
	bool bOpenCom = mMyCom ->open(QIODevice::ReadWrite);
	
	if(!bOpenCom)
	{
		QMessageBox::information(this, "��Ϣ", sCom + "���ڴ���");
	}
	//���ڽ�����Ϣ����Ӧ
	//����
	connect(mMyCom,SIGNAL(readyRead()),this,SLOT(MyComSlot()));

	//2.��ʼ���㷨
	mFusionAlg = new FusionAlg(FlyCapture::ImageWidth, FlyCapture::ImageHeight, 
		FlyCapture::ImageWidth/2, FlyCapture::ImageHeight/2, 1, 2);
	  
	//3.��ʼ������ͷ
	FlyCapture flyCapture;	
	string sMsg="";
	bool fInit = flyCapture.Init(sMsg);
	if(fInit)
	{
		if(flyCapture.StartCapture())
		{
			//�߳̿�ʼ��ȡͼ��
			mImgTrd = new ImageThread(flyCapture.GetCamera(), mFusionAlg);
			//connect(imgTrd, SIGNAL(ImageSignal(unsigned char*, int)), this, SLOT(ImageSlot(unsigned char*, int)));
			mImgTrd->start();

			mShowTrd = new ShowThread(mFusionAlg);
			connect(mShowTrd, SIGNAL(ImageSignal(unsigned char*, int)), this, SLOT(ImageSlot(unsigned char*, int)));
			mShowTrd->start();
		}
	}
	else
	{

		QMessageBox::warning(this, "�������", QString::fromStdString(sMsg),"ȷ��");
	}

	//4.������ʾ����
	mViewWidget = new ViewWidget(this);
	mViewWidget->show();
}

void CaptureWindow::MyComSlot()
{
	//������1�ֽڷ���4��Ϊһ����
	QByteArray temp = mMyCom->readAll();
	/*
	//unsigned char cTmp = temp.at(0);
	if(temp.size() == 0)
	{
		return;
	}
	if(miVal == 0)
	{
		if(temp.at(miVal) == 0xaa)		
		{
			mcRs232[miVal] = 0xaa;	
			++miVal;
		}
		else
		{
			miVal = 0;	
		}
		return;
	}
	

	if(miVal == 1)
	{
		if(temp.at(miVal) == 0x55)	
		{
			mcRs232[miVal] = 0x55;	
			++miVal;
		}
		else
		{
			miVal = 0;	
		}
	}

	if(miVal == 2)
	{
		mcRs232[miVal] = temp.at(miVal);		
		++miVal;
	}
	if(miVal == 3)
	{
		mcRs232[miVal] = temp.at(miVal);	
		++miVal;
	}

	if(miVal == 4)
	{
		miVal = 0;

		char FilterPosition=mcRs232[2];
		FilterPosition>>=6;
		switch (FilterPosition&0x03)
		{
			case OPGPIO::Filter:
				SetFilter();
				break;
			case OPGPIO::Key:
				SetKey();
				break;
		}

	}
	*/
}

void CaptureWindow::SetFilter()
{
	unsigned int mode = 0;

	char FilterPosition=mcRs232[2];
	FilterPosition>>=4;
	switch (FilterPosition&0x03)
	{
	case OPGPIO::RGB:
		mode=OPGPIO::RGB;
		break;
	case OPGPIO::NBI:
		mode=OPGPIO::NBI;
		break;
	case OPGPIO::AFI:
		mode=OPGPIO::AFI;
		break;
	default:
		mode=OPGPIO::OtherMode;
		break;
	}

	//���õ�ǰģʽ
	//ImageThread::SetMode(mode);
}

void CaptureWindow::SetKey()
{}


void CaptureWindow::ImageSlot(unsigned char* cBuffer, int iSize)
{
		//TODO::cBuffer���޸�
	if(iSize != 0)
	{
		if(cBuffer != NULL)
		{	
			QImage image;
			image.loadFromData(cBuffer, iSize);
			//Ĭ�ϴ�С
			QSize qsize = mCaptureUi.lbImage->size();
			image = image.scaled(qsize, Qt::KeepAspectRatio);

			QPixmap pixmap = QPixmap::fromImage(image);
			mCaptureUi.lbImage->setPixmap(pixmap);

		}

	}
}

void CaptureWindow::wheelEvent(QWheelEvent *qevent)
{
	 int numDegrees = qevent->delta() / 8;//�����ĽǶȣ�*8�����������ľ���
     int numSteps = numDegrees / 15;//�����Ĳ�����*15�����������ĽǶ�
     if (qevent->orientation() == Qt::Horizontal) {       
         //ˮƽ����
     } else {
         //��ֱ����

		if(numSteps > 0)
		{
				//�ŵ�
			if(miWidth < 1920 || miHeight < 1080)
			{
				//����16�ı���
				int tmp = miWidth;
				miWidth = miWidth * 1.25;	
				miWidth = miWidth - miWidth%16;
				miHeight = (miWidth* miHeight)/tmp;

			}


		}
		else
		{
				//��С
			if(miWidth >100)
			{
				int tmp = miWidth;
				miWidth = miWidth * 0.75;
				miWidth = miWidth - miWidth%16;
				miHeight = (miWidth * miHeight)/tmp;
			}
	
		}
		int ax = (QApplication::desktop()->width() - miWidth)/2;
		int ay = (QApplication::desktop()->height() - miWidth)/2;
		if(ax >0 && ay >0)
		{
			mCaptureUi.lbImage->setGeometry(ax, ay, miWidth, miHeight);
		}
     }
     qevent->accept();
}

void CaptureWindow::CancelSlot()
{
	this->close();		
}
