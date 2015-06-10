#include "FlyCapture.h"

#include "OpDefs.h"
//FlyCapture2
#include "BusManager.h"
#include "Camera.h"

//log
#include "Log.h"

FlyCapture::FlyCapture()
{
	init();
}

FlyCapture::~FlyCapture()
{

}


bool FlyCapture::init()
{
	#if OPLOG
	Log *log = Log::GetInstance();
	#endif

	FlyCapture2::BusManager busMangager;

	//1.获取已连接相机的数量
	unsigned int iNumCamera = 0;
	mErr = busMangager.GetNumOfCameras(&iNumCamera);

	if (mErr != FlyCapture2::PGRERROR_OK)
	{
		#if OPLOG
		log->WriteLog("GetNumOfCameras error");
		#endif

		return false;
	}
	if (iNumCamera < 1)
	{
		#if OPLOG
		log->WriteLog("iNumCamera<1");
		#endif

		return false;
	}
	
	//2.获取连接相机的标识
	FlyCapture2::PGRGuid pgrGuid;
	mErr = busMangager.GetCameraFromIndex(0, &pgrGuid);
	if (mErr != FlyCapture2::PGRERROR_OK)
	{
		#if OPLOG
		log->WriteLog("GetCameraFromIndex error");
		#endif

		return false;
	}

	//3.根据相机标识连接相机
	mpCamera = new FlyCapture2::Camera();
	mErr = mpCamera->Connect(&pgrGuid);
	if (mErr != FlyCapture2::PGRERROR_OK)
	{
		#if OPLOG
		log->WriteLog("Connect camera error");
		#endif

		return false;
	}

	//3.设置GPIO
	mpCamera->SetGPIOPinDirection(OPGPIO::GPIO3, 0);
	mpCamera->SetGPIOPinDirection(OPGPIO::GPIO4, 0);

	#if OPLOG
	log->WriteLog("init success");
	#endif

	return true;
}

bool FlyCapture::StartCapture()
{
	//开始录像
	mErr = mpCamera->StartCapture();
	if (mErr != FlyCapture2::PGRERROR_OK)
	{
		#if OPLOG
		Log *log = Log::GetInstance();
		log->WriteLog("StartCapture error");
		#endif

		return false;
	}

	return true;
}

bool FlyCapture::GetCamImage(unsigned char* R, unsigned char* G, unsigned char* B)
{
	return true;
}