#pragma once

#include <qthread.h>

#include "OpHelp.h"

class FusionAlg;
class FusionThread :
	public QThread
{
public:
	FusionThread(FusionAlg *fusion, FilterRun filter = Gaussian);
	~FusionThread(void);

	//ผฬณะ
protected:
	void run();

private:
	FusionAlg *mFusionAlg;
	FilterRun mFilterRun;

	//QMutex mMutex;
};

class GaussianThread
	:public QThread
{
public:
	GaussianThread(FusionAlg *fusion, BGRbmp bgrBmp)
		:mFusionAlg(fusion), mBGRBmp(bgrBmp){}

protected:
	void run();

private:
	FusionAlg *mFusionAlg;
	BGRbmp mBGRBmp;
};

class EdgeeThread 
	:public QThread
{
public:
	EdgeeThread(FusionAlg *fusion, BGRbmp bgrBmp)
		:mFusionAlg(fusion), mBGRBmp(bgrBmp){}

protected:
	void run();

private:
	FusionAlg *mFusionAlg;
	BGRbmp mBGRBmp;

};
