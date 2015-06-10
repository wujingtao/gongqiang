#include "FusionThread.h"


#include "FusionAlg.h"

//#include "Log.h"
#include <sstream>
using std::stringstream;

#include <QTime>

FusionThread::FusionThread(FusionAlg *fusion, FilterRun filter)
	:mFusionAlg(fusion),  mFilterRun(filter)
{
}


FusionThread::~FusionThread(void)
{
}

void FusionThread::run()
{
	if(mFilterRun == Gaussian)
	{//start if
	
	GaussianThread *gaussianRTrd = new GaussianThread(mFusionAlg, ROffset);
	GaussianThread *gaussianGTrd = new GaussianThread(mFusionAlg, GOffset);
	GaussianThread *gaussianBTrd = new GaussianThread(mFusionAlg, BOffset);

	QTime t;
	while(true)
	{
		while(!mFusionAlg->IsRGB())
		{
			int ttime = t.elapsed();
			t.restart();
			if(ttime < 30)
			{
				msleep(30-ttime);	
			}
			continue;
		}

		gaussianRTrd->start();	
		gaussianGTrd->start();	
		gaussianBTrd->start();	
		gaussianRTrd->wait();
		gaussianGTrd->wait();
		gaussianBTrd->wait();

		mFusionAlg->MallocGaussian();
	}

	}//end if
	else if(mFilterRun == Edgee)
	{//start 1 else if
	
	EdgeeThread *edgeeRTrd = new EdgeeThread(mFusionAlg, ROffset);
	EdgeeThread *edgeeGTrd = new EdgeeThread(mFusionAlg, GOffset);
	EdgeeThread *edgeeBTrd = new EdgeeThread(mFusionAlg, BOffset);
	QTime t;
	while(true)
	{
		//while(!mListRGB->IsGaussianRGB())
		while(!mFusionAlg->IsGaussianRGB())
		{
			int ttime = t.elapsed();
			t.restart();
			if(ttime < 30)
			{
				msleep(30-ttime);	
			}
			continue;
		}

		edgeeRTrd->start(); 
		edgeeGTrd->start(); 
		edgeeBTrd->start();
		edgeeRTrd->wait(); 
		edgeeGTrd->wait(); 
		edgeeBTrd->wait(); 

		mFusionAlg->MallocImage();	
	}

	}//end 1 else if
}


void GaussianThread::run()
{
	mFusionAlg->gaussianFilter(mBGRBmp);	
}


void EdgeeThread::run()
{
	mFusionAlg->EdgeE(mBGRBmp);
}