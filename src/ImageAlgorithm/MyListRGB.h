#pragma once
#include "MyList.h"
//qt
#include <qmutex.h>

class MyListRGB
{
public:
	~MyListRGB(void);

	//获取单例
	static MyListRGB* GetInstance();

private:
	explicit MyListRGB(void);

public:
	void AddRGB(unsigned char* r, unsigned char* g, unsigned char* b);
	bool GetRGB(unsigned char** r, unsigned char** g, unsigned char** b);
	void ReleaseRGB();

	void AddGaussianRGB(unsigned char* r, unsigned char* g, unsigned char* b);
	bool GetGaussianRGB(unsigned char** r, unsigned char** g, unsigned char** b);
	void ReleaseGaussianRGB();

	void AddImage(unsigned char* r);
	bool GetImage(unsigned char** r);
	void ReleaseImage();

	//添加到直方算法队列
	void AddClaheRGB(unsigned char* r,unsigned char* g,unsigned char* b);
	//获取直方图处理后的RGB
	bool GetClaheRGB(unsigned char** r, unsigned char** g, unsigned char** b);
	//释放
	void ReleaseClaheRGB();
private:
	MyList<unsigned char*> mLstR;
	MyList<unsigned char*> mLstG;
	MyList<unsigned char*> mLstB;

	MyList<unsigned char*> mLstGaussianR;
	MyList<unsigned char*> mLstGaussianG;
	MyList<unsigned char*> mLstGaussianB;

	MyList<unsigned char*> mLstClaheR;
	MyList<unsigned char*> mLstClaheG;
	MyList<unsigned char*> mLstClaheB;

	MyList<unsigned char*> mLstImage;

	static QMutex mMutex;

	static MyListRGB *s_myListRGB;
};

