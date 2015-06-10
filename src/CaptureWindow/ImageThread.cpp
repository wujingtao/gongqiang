#include "ImageThread.h"

#include "RWBmp.h"

//#include "Fusion.h"
#include "FusionAlg.h"
#include "FlyCapture.h"
#include "ViewWidget.h"
#include <QTime>
//flycapture
#include "Camera.h"
#include "Image.h"

#include <fstream>
#include <sstream>
using std::ofstream;
using std::stringstream;
using std::endl;

int GlobalArgs::g_radio = 0;
unsigned int ImageThread::s_mCurrentMode = 0;
ImageThread::ImageThread(FlyCapture2::Camera *pCamera, FusionAlg *fusionAlg) : 
									mpCamera(pCamera),mFusionAlg(fusionAlg)
{

	mpImageR = NULL;
	mpImageG = NULL;
	mpImageB = NULL;

	mbR = false;
	mbG = false;
	mbB = false;

	mbPause = false;
}


ImageThread::~ImageThread(void)
{
	if(mpImageR != NULL)
	{
		delete[] mpImageR;
		mpImageR = NULL;
	}
	if(mpImageG != NULL)
	{
		delete[] mpImageG;
		mpImageG = NULL;
	}
	if(mpImageB != NULL)
	{
		delete[] mpImageB;
		mpImageB = NULL;
	}

	//delete mFusion;
	delete mFusionAlg;
}

void ImageThread::run()
{
	//TODO::8位显示有问题
	showCamImage();
	//showImage24();
}

void ImageThread::showImage24()
{
	FlyCapture2::Image processedImage, convertedImage;

	//图像大小参数
	const int iWidth = FlyCapture::ImageWidth;
	const int iHeight = FlyCapture::ImageHeight;
	const int iImageSize = iWidth * iHeight;

	QWaitCondition pauseWaitCondition;

	//算法线程
	//mFusionAlg->fusioning();
	mFusionAlg->fusioningS();

	//gpio电平参数
	unsigned int gpio3Add = 0;
	unsigned int gpio4Add = 0;
	unsigned int cColor = 0;

	//初始化rgb保存
	if(mpImageR == NULL)
	{
		mpImageR = new unsigned char[iImageSize];
	}
	if(mpImageG == NULL)
	{
		mpImageG = new unsigned char[iImageSize];
	}
	if(mpImageB == NULL)
	{
		mpImageB = new unsigned char[iImageSize];
	}

	//TODO:用于记录测试
	QTime t;
	t.start();
	
#ifdef DEF_ImageThread
	ofstream ofs("imageThread");
#endif

	while(true)	
	{
		t.restart();
		//获取数据
		mErr = mpCamera->RetrieveBuffer( &processedImage);
		if(mErr != FlyCapture2::PGRERROR_OK)
		{
			break;
		}

#ifdef DEF_ImageThread
		int ttime = t.elapsed();
		stringstream sstr;
		sstr<<ttime<<endl;
		ofs<<sstr.str();
#endif
		/*
		//获取电平
		mpCamera->ReadRegister(OPGPIO::GPIO3_ADD, &gpio3Add);
		mpCamera->ReadRegister(OPGPIO::GPIO4_ADD, &gpio4Add);
		int iL = gpio3Add & 0x0001;
		int iH = gpio4Add & 0x0001;
		cColor = (iH<<1) + iL;	
		*/
		if(cColor == 3)
		{
			cColor = 1;	
		}
		else
		{
			++cColor;	
		}	




		//TODO: 当前模式需修改
		int mode = s_mCurrentMode;
		if(mode == OPGPIO::RGB)
		{
			switch(cColor)	
			{
			case OPGPIO::RColor:
				memcpy(mpImageR, processedImage.GetData(), iImageSize);
				mbR = true;
				++GlobalArgs::g_radio;
				break;
			case OPGPIO::GColor:
				memcpy(mpImageG, processedImage.GetData(), iImageSize);
				mbG= true;
				++GlobalArgs::g_radio;
				break;
			case OPGPIO::BColor:
				memcpy(mpImageB, processedImage.GetData(), iImageSize);
				mbB= true;
				++GlobalArgs::g_radio;
				break;
			}
		}

		//if(mbR && mbB && mbG)
		{
			switch(mode)
			{
			case OPGPIO::RGB:
				/*
				unsigned int ttime = t.elapsed();
				if(ttime < 30)
				{
					msleep(30 - ttime);	
				}
				*/

				mFusionAlg->AddRGB(mpImageR, mpImageG, mpImageB);
				break;
			}

			/*
			//自写暂停线程
			while(mbPause)
			{
				msleep(1000);
				continue;
			}
			*/

			if(mbPause)
			{
				mMutex.lock();
				mWaitCondition.wait(&mMutex);	
				mMutex.unlock();
			}
		}


	}
}

void ImageThread::showCamImage()
{
	const int iWidth = FlyCapture::ImageWidth;
	const int iHeight = FlyCapture::ImageHeight;
	const int iImageSize = iWidth * iHeight;

	FlyCapture2::Image rawImage,processedImage;
	QTime t;
	t.start();
	while(true)
	{
		unsigned int ttime = t.elapsed();
		if(ttime < 30)
		{
			msleep(30 - ttime);	
		}

		//获取图像
		mErr = mpCamera->RetrieveBuffer(&rawImage);
		if(mErr != FlyCapture2::PGRERROR_OK)
		{
			continue;
		}

		//mErr = rawImage.Convert( FlyCapture2::PIXEL_FORMAT_RAW8, &processedImage );
		mErr = rawImage.Convert( FlyCapture2::PIXEL_FORMAT_BGR, &processedImage );
		if(mErr != FlyCapture2::PGRERROR_OK)
		{
			string str = mErr.GetDescription();
			continue;
		}
		int size = processedImage.GetDataSize();


		mFusionAlg->AddImage(processedImage.GetData());

	}

	/*
		if(buffer != NULL)
		{
			//emit ImageSignal(buffer, iSize);	
		}

		//最多显示50张，超过容易卡死
		msleep(20);
	}	
*/
}

void ImageThread::SetPause(bool pause)
{
	mbPause = pause;
	if(mbPause == false)
	{
		mWaitCondition.wakeAll();
	}
}

bool ImageThread::GetRGB(unsigned char** r, unsigned char** g, unsigned char** b)
{
	if(mpImageR == NULL || mpImageG == NULL || mpImageB == NULL)
	{
		return false;	
	}
	*r = mpImageR;
	*g = mpImageR;
	*b = mpImageR;

	return true;
}
