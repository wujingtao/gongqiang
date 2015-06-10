#pragma once
#include <qthread.h>

#include "OpHelp.h"

class FusionAlg;
class FusionSThread :
	public QThread
{
public:
	FusionSThread(FusionAlg *fusion, FilterRun filter = Clahe);
	~FusionSThread(void);

		//ผฬณะ
protected:
	void run();

private:
	FusionAlg *mFusionAlg;
	FilterRun mFilterRun;
};


class GaussianSThread
	:public QThread
{
public:
	GaussianSThread(FusionAlg *fusion, BGRbmp bgrBmp)
		:mFusionAlg(fusion), mBGRBmp(bgrBmp){}

protected:
	void run();

private:
	FusionAlg *mFusionAlg;
	BGRbmp mBGRBmp;
};

class ClaheSThread
	:public QThread
{
public:
	ClaheSThread(FusionAlg *fusion, BGRbmp bgrBmp)
		:mFusionAlg(fusion), mBGRBmp(bgrBmp){}

protected:
	void run();

private:
	FusionAlg *mFusionAlg;
	BGRbmp mBGRBmp;

};
