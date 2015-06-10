/*
 * =====================================================================================
 *
 *       Filename:  FlyCapture.h
 *        Created:  2014/7/8 9:49:42
 *       Compiler:  vs2010 
 *         Author:  
 *
 *    Description:  ��ʼ�������
 *
 * =====================================================================================
 */

#ifndef FLYCAPTURE_H
#define FLYCAPTURE_H

//FlyCapture
#include "Error.h"

#include <string>
using std::string;

namespace FlyCapture2
{
	class Camera;
}

class CaptureWindow;
class FlyCapture
{
public:
	FlyCapture();
	virtual ~FlyCapture();

public:

	/* 
	 *����: ��ʼ���������
	 *����: err out ���ش�����Ϣ 
	 *����: ��ʼ���ɹ�Ϊtrue������false 
	 */
	bool Init(string &err);

	/* 
	 *����: �����ʼ¼��
	 *����: null
	 *����: ��ʼ¼��ɹ�Ϊtrue������false 
	 */
	bool StartCapture(char* err = "");

	/* 
	 *����: ��ȡ�������
	 *����: null
	 *����: ����Camera����
	 */
	FlyCapture2::Camera* GetCamera() const;

public:
	//���Ϊ1280
	const static int ImageWidth = 864;
	//const static int ImageWidth = 688;
	//���Ϊ1024
	const static int ImageHeight= 648;

private:
	FlyCapture2::Camera* mpCamera;
	FlyCapture2::Error mErr;

};

#endif // FLYCAPTURE_H
