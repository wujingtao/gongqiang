#include "ShowThread.h"

//#include "Fusion.h"
#include "FusionAlg.h"
#include "RWBmp.h"
#include "FlyCapture.h"
#include "ViewWidget.h"

//#include "Log.h"

#include <QTime>
#include <fstream>
#include <sstream>
using std::ofstream;
using std::stringstream;
using std::endl;

#include <assert.h>

int GlobalArgs::g_frame = 0;
ShowThread::ShowThread(FusionAlg *fusion):mFusionAlg(fusion)
{
}


ShowThread::~ShowThread(void)
{
}

void ShowThread::run()
{
	const int iWidth = FlyCapture::ImageWidth;
	const int iHeight = FlyCapture::ImageHeight;

	unsigned char* pImage = 0;
	unsigned char* buffer = 0;

#ifdef QTEST
	ofstream ofs("ShowImage");
#endif

	QTime t;
	t.start();

	while(true)	
	{
		if(!mFusionAlg->GetImage(&pImage))	
		{
			continue;	
		}
		t.restart();

		unsigned int ttime = t.elapsed();
		if(ttime < 30)
		{
			msleep(30 - ttime);	
		}

		#ifdef QTEST
		if(ofs.is_open())
		{
			stringstream ss;	
			ss<<t.elapsed();
			ofs<<ss.str()<<endl;
		}
		#endif

		int iSize = 0;
		if(pImage != 0)
		{
			buffer = RWBmp::Get24BitBmp(pImage, iWidth, iHeight, iSize);
			//buffer = RWBmp::Get8BitBmp(pImage, iWidth, iHeight, iSize);
		}

		mFusionAlg->ReleaseImage();

		if(buffer != 0)
		{
			++GlobalArgs::g_frame;
			emit ImageSignal(buffer, iSize);	
		}
	}
}