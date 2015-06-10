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
	 *����: ��ȡ���RGBͼ��
	 *����: null
	 *����: void
	 */
	bool GetCamImage(unsigned char* R, unsigned char* G, unsigned char* B);

private:
	/* 
	 *����: ��ʼ�����
	 *����: null
	 *����: void
	 */
	bool init();

private:
	FlyCapture2::Camera* mpCamera;
	FlyCapture2::Error mErr;

};

#endif // FLYCAPTURE_H
