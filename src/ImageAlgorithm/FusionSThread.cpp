#include "FusionSThread.h"

#include <QTime>

#include "FusionAlg.h"

#include <fstream>
#include <sstream>
using std::ofstream;
using std::stringstream;
using std::endl;

#define DEF_FusionSThread

FusionSThread::FusionSThread(FusionAlg *fusion, FilterRun filter)
	:mFusionAlg(fusion),  mFilterRun(filter)
{
}


FusionSThread::~FusionSThread(void)
{
}

void FusionSThread::run()
{
	QTime t;
	if(mFilterRun == Clahe)
	{//start if

#ifdef DEF_FusionSThread
		ofstream ofs("claheTrd");
#endif
	
	ClaheSThread *claheRTrd = new ClaheSThread(mFusionAlg, ROffset);
	ClaheSThread *claheGTrd = new ClaheSThread(mFusionAlg, GOffset);
	ClaheSThread *claheBTrd = new ClaheSThread(mFusionAlg, BOffset);

	while(true)
	{
		while(!mFusionAlg->IsRGB())			
		{
			//30ms取一次RGB,避免while浪费
			int ttime = t.elapsed();
			t.restart();
			if(ttime < 30)
			{
				msleep(30-ttime);	
			}
			continue;
		}

		//开始线程
		claheRTrd->start();
		claheGTrd->start();
		claheBTrd->start();
		//等待线程完成
		claheRTrd->wait();
		claheGTrd->wait();
		claheBTrd->wait();

		mFusionAlg->MallocClaheS();

#ifdef DEF_FusionSThread
		stringstream ss;
		int ttime = t.elapsed();
		ss<<ttime<<endl;
		ofs<<ss.str();
		ofs.clear();
		t.restart();
#endif 

	}

	}//end if
	else if(mFilterRun == Gaussian)
	{//start else if

		#ifdef DEF_FusionSThread
		ofstream ofs("GaussianTrd");
		#endif


	GaussianSThread *gaussianRTrd = new GaussianSThread(mFusionAlg, ROffset);
	GaussianSThread *gaussianGTrd = new GaussianSThread(mFusionAlg, GOffset);
	GaussianSThread *gaussianBTrd = new GaussianSThread(mFusionAlg, BOffset);

	QTime t;
	while(true)
	{//start while

		while(!mFusionAlg->IsClaheRGBS())
		{
			//30ms取一次RGB,避免while浪费
			int ttime = t.elapsed();
			t.restart();
			if(ttime < 30)
			{
				msleep(30-ttime);	
			}
			continue;
		}

		//开始线程
		gaussianRTrd->start(); 
		gaussianGTrd->start(); 
		gaussianBTrd->start();
		//等待线程结束
		gaussianRTrd->wait(); 
		gaussianGTrd->wait(); 
		gaussianBTrd->wait(); 

		//算法后处理
		mFusionAlg->MallocImageS();

		#ifdef DEF_FusionSThread
		stringstream ss;
		int ttime = t.elapsed();
		ss<<ttime<<endl;
		ofs<<ss.str();
		ofs.clear();
		t.restart();
		#endif 

	}//end while
	
	}//end else if
}

void ClaheSThread::run()
{
	mFusionAlg->CLAHES(mBGRBmp);	
}

void GaussianSThread::run()
{
	mFusionAlg->gaussianFilterS(mBGRBmp);	
}