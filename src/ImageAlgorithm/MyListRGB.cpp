#include "MyListRGB.h"

MyListRGB *MyListRGB::s_myListRGB = 0;
QMutex MyListRGB::mMutex;
MyListRGB::MyListRGB(void)
{
}


MyListRGB::~MyListRGB(void)
{
}


MyListRGB* MyListRGB::GetInstance()
{
	//锁写在这浪费锁时间
	if(s_myListRGB == 0)
	{
		//双重判断
		mMutex.lock();
		if(s_myListRGB == 0)
		{
			s_myListRGB = new MyListRGB();
		}
		mMutex.unlock();
	}

	return s_myListRGB;
}

void MyListRGB::AddRGB(unsigned char* r, unsigned char* g, unsigned char* b)
{
	if(r == 0 || g==0 || b==0)
	{
		return;
	}

	if(mLstR.size() >= 10 || mLstG.size() >= 10 || mLstB.size() >= 10)		
	{
		delete[] r;
		delete[] g;
		delete[] b;

		return;
	}
	
	mLstR.push_back(r);
	mLstG.push_back(g);
	mLstB.push_back(b);
	
}

bool MyListRGB::GetRGB(unsigned char** r, unsigned char** g, unsigned char** b)
{
	if(mLstR.size() <= 0 || mLstG.size() <= 0 || mLstB.size() <=0)
	{
		return false;	
	}

	*r = mLstR.front();
	*g = mLstG.front();
	*b = mLstB.front();

	return true;
}

void MyListRGB::ReleaseRGB()
{

	mLstR.pop_front();		
	mLstG.pop_front();		
	mLstB.pop_front();		

}


void MyListRGB::AddGaussianRGB(unsigned char* r, unsigned char* g, unsigned char* b)
{
	if(r == 0 || g==0 || b==0)
	{
		return;
	}

	if(mLstGaussianR.size() >= 10 || mLstGaussianG.size() >= 10 || mLstGaussianB.size() >= 10)		
	{
		delete[] r;
		delete[] g;
		delete[] b;

		return;
	}
	
	mLstGaussianR.push_back(r);
	mLstGaussianG.push_back(g);
	mLstGaussianB.push_back(b);
	
}

bool MyListRGB::GetGaussianRGB(unsigned char** r, unsigned char** g, unsigned char** b)
{
	if(mLstGaussianR.size() <= 0 || mLstGaussianG.size() <= 0 || mLstGaussianB.size() <=0)
	{
		return false;	
	}

	*r = mLstGaussianR.front();
	*g = mLstGaussianG.front();
	*b = mLstGaussianB.front();

	return true;
}

void MyListRGB::ReleaseGaussianRGB()
{
	mLstGaussianR.pop_front();
	mLstGaussianG.pop_front();
	mLstGaussianB.pop_front();
}

void MyListRGB::AddClaheRGB(unsigned char* r, unsigned char* g, unsigned char* b)
{
	if(r == 0 || g==0 || b==0)
	{
		return;
	}

	if(mLstClaheR.size() >= 10 || mLstClaheG.size() >= 10 || mLstClaheB.size() >= 10)		
	{
		delete[] r;
		delete[] g;
		delete[] b;

		return;
	}
	
	mLstClaheR.push_back(r);
	mLstClaheG.push_back(g);
	mLstClaheB.push_back(b);
}

bool MyListRGB::GetClaheRGB(unsigned char** r, unsigned char** g, unsigned char** b)
{
	if(mLstClaheR.size() <= 0 || mLstClaheG.size() <= 0 || mLstClaheB.size() <=0)
	{
		return false;	
	}

	*r = mLstClaheR.front();
	*g = mLstClaheG.front();
	*b = mLstClaheB.front();

	return true;
}

void MyListRGB::ReleaseClaheRGB()
{
	mLstClaheR.pop_front();
	mLstClaheG.pop_front();
	mLstClaheB.pop_front();
}


void MyListRGB::AddImage(unsigned char* img)
{
	if(img == 0)
	{
		return;
	}

	if(mLstImage.size() >= 10 )		
	{
		delete[] img;
		return;
	}
	
	mLstImage.push_back(img);
	
}

bool MyListRGB::GetImage(unsigned char** img)
{
	if(mLstImage.size() <= 0)
	{
		return false;	
	}

	*img = mLstImage.front();

	return true;
}

void MyListRGB::ReleaseImage()
{
	if(mLstImage.size() > 0)
	{
		//unsigned char* img = mLstImage.front();
		mLstImage.pop_front();
		//delete[] img;	
	}
}

