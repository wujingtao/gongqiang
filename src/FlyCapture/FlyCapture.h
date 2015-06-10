#ifndef FLYCAPTURE_H
#define FLYCAPTURE_H

#include "flycapture_global.h"


//FlyCapture
#include "Error.h"
namespace FlyCapture2
{
	class Camera;
}

class FLYCAPTURE_EXPORT FlyCapture
{
public:
	FlyCapture();
	~FlyCapture();

public:

	bool StartCapture();

	/* 
	 *功能: 获取相机RGB图像
	 *参数: null
	 *返回: void
	 */
	bool GetCamImage(unsigned char* R, unsigned char* G, unsigned char* B);

private:
	/* 
	 *功能: 初始化相机
	 *参数: null
	 *返回: void
	 */
	bool init();

private:
	FlyCapture2::Camera* mpCamera;
	FlyCapture2::Error mErr;

};

#endif // FLYCAPTURE_H
