#include "FusionAlg.h"

#include "OpHelp.h"
//#include "ListRGB.h"
#include "MyListRGB.h"
#include "FusionThread.h"
#include "FusionSThread.h"

//#include "Log.h"

#include <QTime>

#include <fstream>
#include <sstream>
#include <string>
using std::ofstream;
using std::stringstream;
using std::endl;
using std::string;

FusionAlg::FusionAlg(int width, int height, int circleX, int circleY, float gainR, float gainB)
	:mImage(NULL),mGaussianR(NULL),mGaussianG(NULL),mGaussianB(NULL)
{
	miWidth = width;
	miHeight = height;
	miCircleX = circleX;
	miCircleY = circleY;
	mfGainR = gainR;
	mfGainB = gainB;

	mEdgeeR = NULL;
	mEdgeeG = NULL;
	mEdgeeB = NULL;

	mGaussianRS = NULL;
	mGaussianGS = NULL;
	mGaussianBS = NULL;
}


FusionAlg::~FusionAlg(void)
{
	
}

void FusionAlg::fusioningS()
{
	//算法后最终输出的图像
	if(mImage == NULL)
	{
		mImage = new unsigned char[ImageSize() * 3];
	}

	FusionSThread *claheTrd = new FusionSThread(this, Clahe);
	claheTrd->start();
	FusionSThread *gaussianTrd = new FusionSThread(this, Gaussian);
	gaussianTrd->start();

}

void FusionAlg::fusioning()
{
	if(mImage == NULL)
	{
		mImage = new unsigned char[ImageSize() * 3];
	}

	FusionThread *gaussianTrd = new FusionThread(this, Gaussian);
	gaussianTrd->start();
	FusionThread *edgeeTrd = new FusionThread(this, Edgee);
	edgeeTrd->start();
}



void FusionAlg::gaussianFilter(BGRbmp bgrBmp)  
{   


	int i, j, index;
	double sum;   
	int Offset;
	int templates[9] = { 1, 2, 1,2, 4, 2,1, 2, 1 };  

	//临时增益
	int iGain = 1;
	//临时输入
	unsigned char* putimg = NULL;
	//临时输出空间
	unsigned char* outimg = NULL;

		switch(bgrBmp)
		{
		case ROffset:
			putimg = mR;
			mGaussianR = new unsigned char[ImageSize()];
			outimg = mGaussianR;
			//增益值
			iGain = mfGainR * 0.8;	
			break;
		case GOffset:
			putimg = mG;
			mGaussianG = new unsigned char[ImageSize()];
			outimg = mGaussianG;
			break;
		case BOffset:
			putimg = mB;
			mGaussianB = new unsigned char[ImageSize()];
			outimg = mGaussianB;
			//增益值
			iGain = mfGainB;	
			break;
		}

	for (i = 1; i < miHeight-1; i ++)
	{
		Offset=i * miWidth;
		for (j =1 ; j < miWidth-1; j ++)
		{

			{
				sum = 0.0;  
				index = 0;
				for(int m = i - 1;m < i + 2;m++)  
				{ 
					for(int n = j - 1; n < j + 2;n++) 
					{ 
						sum += iGain * putimg[m * miWidth + n] *  templates[index++]; 
					}  
				}  

				outimg[Offset+ j ] =((int)(sum / 16));
			}
		}  
	}  
}

void FusionAlg::MallocGaussian()
{
	//ListRGB *listRGB = ListRGB::GetInstance();
	MyListRGB *listRGB = MyListRGB::GetInstance();
	listRGB->AddGaussianRGB(mGaussianR, mGaussianG, mGaussianB);

	listRGB->ReleaseRGB();
}

void FusionAlg::EdgeE(BGRbmp bgrBmp)  
{
	int depth = 0.5;
	int position = bgrBmp;

	unsigned char* pImg = NULL;

	//ListRGB *listRGB = ListRGB::GetInstance();

		switch(bgrBmp)
		{
		case ROffset:
			//if(!listRGB->GetGaussianR(&pImg))
			//{return;}
			pImg = mEdgeeR;
			break;
		case GOffset:
			//if(!listRGB->GetGaussianG(&pImg))
			//{return;}
			pImg = mEdgeeG;
			break;
		case BOffset:
			//if(!listRGB->GetGaussianB(&pImg))
			//{return;}
			pImg = mEdgeeB;
			break;
		}

	int TempValue;int value; int evgValue;
	for(int i = 1; i < miHeight-1; i ++)
	{
		for(int j = 1; j < miWidth-1; j ++)
		{
			{
				value = pImg[ i * miWidth + j ];
                evgValue = 0;
				TempValue= abs( pImg[ (i ) * miWidth + j ]-pImg[ (i - 1) * miWidth + j -1]);
			    for(int m = -1; m <= 1; m++)
				{
					for(int n = -1; n <= 1; n++)
					{
						evgValue +=  pImg[ (i+m) * miWidth + (j + n) ];
					}
				}
                evgValue = (evgValue-value)/8;
				if(TempValue>20 &&evgValue<value )
				{
					value= pImg[ i * miWidth + j ]+depth * TempValue;
				     if(value> 255)
						 value = 255;
					 
				}
			mImage[3 * i * miWidth + j * 3 + position ]=value;
			
			}
		}
	}
}

void FusionAlg::MallocImage()
{
	//ListRGB *listRGB = ListRGB::GetInstance();
	MyListRGB *listRGB = MyListRGB::GetInstance();

	listRGB->ReleaseGaussianRGB();
	//listRGB->GaussianOpera();
	listRGB->AddImage(mImage);
	//listRGB->ImageOpera(mImage);

	mImage = new unsigned char[ImageSize() * 3];	
}

void FusionAlg::CLAHES(BGRbmp bgrBmp)     
{   

#ifdef DEF_FusionAlg
	string str = "";
	if(bgrBmp == ROffset)
	{
		str = "ClaheRS";	
	}else if(bgrBmp == GOffset)
	{
		str = "ClaheGS";	
	}else if(bgrBmp == BOffset)
	{
		str = "ClaheBS";	
	}
	ofstream ofs(str, ofstream::app);
	stringstream ss;

	QTime t;
	t.start();
#endif

	//unsigned char* pImage,unsigned char* qImage
	int uiXRes = miWidth;
	int uiYRes = miHeight;
	int origin_X = miCircleX;
	int origin_Y = miCircleY;
	//double Circle_R = 1;
	int fCliplimit = 1.5;


	unsigned char Min = 0;
	unsigned char Max = 255;
	unsigned int uiNrX = 8;
	unsigned int uiNrY = 8;  
    unsigned int uiNrBins= 256;

	//临时增益
	int iGain = 1;
	//临时输入
	unsigned char* pImage = NULL;
	//临时输出
	unsigned char* qImage = NULL;

	switch(bgrBmp)
	{
		case ROffset:
			pImage = mR;
			mClaheRS = new unsigned char[ImageSize()];
			qImage = mClaheRS;
			//增益值
			iGain = mfGainR;	
			break;
		case GOffset:
			pImage = mG;
			mClaheGS = new unsigned char[ImageSize()];
			qImage = mClaheGS;
			break;
		case BOffset:
			pImage = mB;
			mClaheBS = new unsigned char[ImageSize()];
			qImage = mClaheBS;
			//增益值
			iGain = mfGainB;	
			break;
	}
	
    for (int i = 0;i < uiYRes; i++)    
    {   
		for(int j = 0; j < uiXRes; j++)
		{
			int value = iGain*pImage[i * uiXRes + j];

			if(value < 0)
			{
				value = 0;
			}
			if(value >255)
			{
				value = 255;
			}
		    qImage[i * uiXRes + j] = value;
		}
	}
   
#ifdef DEF_FusionAlg
	int ttime = t.elapsed();	
	ss<<ttime<<endl;
	ofs<<ss.str();
#endif

    unsigned int uiX, uiY;        /* counters */   
    unsigned int uiXSize, uiYSize, uiSubX, uiSubY; /* size of context. reg. and subimages */   
    unsigned int uiXL, uiXR, uiYU, uiYB;  /* auxiliary variables interpolation routine */   
    unsigned long ulClipLimit, ulNrPixels;/* clip limit and region pixel count */   
    unsigned char aLUT[uiNR_OF_GREY];      /* lookup table used for scaling of input image */   
	unsigned char *pPointImage;
    unsigned long* pulHist, *pulMapArray; /* pointer to histogram and mappings*/   
    unsigned long* pulLU, *pulLB, *pulRU, *pulRB; /* auxiliary pointers interpolation */   
       
    if (uiNrX > uiMAX_REG_X) return;     /* # of regions x-direction too large */   
    if (uiNrY > uiMAX_REG_Y) return;     /* # of regions y-direction too large */   
       
       
    //    if (uiXRes % uiNrX) return -3;      /* x-resolution no multiple of uiNrX */    
    //    if (uiYRes % uiNrY) return -4;      /* y-resolution no multiple of uiNrY */    
       
       
       
    if (Max >= uiNR_OF_GREY) return;     /* maximum too large */   
    if (Min >= Max) return;         /* minimum equal or larger than maximum */   
    if (uiNrX < 2 || uiNrY < 2) return;/* at least 4 contextual regions required */   
    if (fCliplimit == 1.0) return;      /* is OK, immediately returns original image. */   
    if (uiNrBins == 0) uiNrBins = 128;    /* default value when not specified */   
       
    pulMapArray=(unsigned long *)malloc(sizeof(unsigned long)*uiNrX*uiNrY*uiNrBins);   
    if (pulMapArray == 0) return ;      /* Not enough memory! (try reducing uiNrBins) */   
       
    uiXSize = uiXRes/uiNrX; uiYSize = uiYRes/uiNrY;  /* Actual size of contextual regions */   
    ulNrPixels = (unsigned long)uiXSize * (unsigned long)uiYSize;   
       
    if(fCliplimit > 0.0) {         /* Calculate actual cliplimit  */   
        ulClipLimit = (unsigned long) (fCliplimit * (uiXSize * uiYSize) / uiNrBins);   
        ulClipLimit = (ulClipLimit < 1UL) ? 1UL : ulClipLimit;   
    }   
    else ulClipLimit = 1UL<<14;         /* Large value, do not clip (AHE) */   
    MakeLut(aLUT, Min, Max, uiNrBins);    /* Make lookup table for mapping of greyvalues */   
    /* Calculate greylevel mappings for each contextual region */   
    for (uiY = 0, pPointImage = qImage; uiY < uiNrY; uiY++)    
    {   
        for (uiX = 0; uiX < uiNrX; uiX++, pPointImage += uiXSize)    
        {   
            pulHist = &pulMapArray[uiNrBins * (uiY * uiNrX + uiX)];   
            MakeHistogram(pPointImage,uiXRes,uiXSize,uiYSize,pulHist,uiNrBins,aLUT);   
            ClipHistogram(pulHist, uiNrBins, ulClipLimit);   
            MapHistogram(pulHist, Min, Max, uiNrBins, ulNrPixels);   
        }   
        pPointImage += (uiYSize - 1) * uiXRes;         /* skip lines, set pointer */   
    }   
       
    /* Interpolate greylevel mappings to get CLAHE image */   
    for (pPointImage = qImage, uiY = 0; uiY <= uiNrY; uiY++)    
    {   
        if (uiY == 0)    
        {                     /* special case: top row */   
            uiSubY = uiYSize >> 1;  uiYU = 0; uiYB = 0;   
        }   
        else    
        {   
            if (uiY == uiNrY) {               /* special case: bottom row */   
                uiSubY = uiYSize >> 1;    uiYU = uiNrY-1;  uiYB = uiYU;   
            }   
            else    
            {                     /* default values */   
                uiSubY = uiYSize; uiYU = uiY - 1; uiYB = uiYU + 1;   
            }   
        }   
        for (uiX = 0; uiX <= uiNrX; uiX++)    
        {   
            if (uiX == 0)    
            {                 /* special case: left column */   
                uiSubX = uiXSize >> 1; uiXL = 0; uiXR = 0;   
            }   
            else    
            {   
                if (uiX == uiNrX)    
                {             /* special case: right column */   
                    uiSubX = uiXSize >> 1;  uiXL = uiNrX - 1; uiXR = uiXL;   
                }   
                else    
                {                     /* default values */   
                    uiSubX = uiXSize; uiXL = uiX - 1; uiXR = uiXL + 1;   
                }   
            }   
               
            pulLU = &pulMapArray[uiNrBins * (uiYU * uiNrX + uiXL)];   
            pulRU = &pulMapArray[uiNrBins * (uiYU * uiNrX + uiXR)];   
            pulLB = &pulMapArray[uiNrBins * (uiYB * uiNrX + uiXL)];   
            pulRB = &pulMapArray[uiNrBins * (uiYB * uiNrX + uiXR)];   
            Interpolate(pPointImage,uiXRes,pulLU,pulRU,pulLB,pulRB,uiSubX,uiSubY,aLUT);   
            pPointImage += uiSubX;             /* set pointer on next matrix */   
        }   
        pPointImage += (uiSubY - 1) * uiXRes;   
    }   
    
    free(pulMapArray);                    /* free space for histograms */      


#ifdef DEF_FusionAlg
	ttime = t.elapsed();	
	ss<<ttime<<endl;
	ofs<<"clahe:"<<ss.str();
#endif
}   

void FusionAlg::MallocClaheS()
{
	MyListRGB *listRGB = MyListRGB::GetInstance();
	listRGB->AddClaheRGB(mClaheRS, mClaheGS, mClaheBS);

	listRGB->ReleaseRGB();	
}

void FusionAlg::gaussianFilterS(BGRbmp bgrBmp)  
{   
#ifdef DEF_FusionAlg
	string str = "";
	if(bgrBmp == ROffset)
	{
		str = "gaussianRS";	
	}else if(bgrBmp == GOffset)
	{
		str = "gaussianGS";	
	}else if(bgrBmp == BOffset)
	{
		str = "gaussianBS";	
	}
	ofstream ofs(str, ofstream::app);
	stringstream ss;

	QTime t;
	t.start();
#endif

	int m_width = miWidth;
	int m_height = miHeight;
	int point_x = miCircleX;
	int point_y = miCircleY;
	//int r = 1;
	int position = bgrBmp;

	//unsigned char* putimg,unsigned char* outimg

	//临时输入
	unsigned char* putimg = NULL;
	switch(bgrBmp)
	{
		case ROffset:
			putimg = mGaussianRS;
			break;
		case GOffset:
			putimg = mGaussianGS;
			break;
		case BOffset:
			putimg = mGaussianBS;
			break;
	}

	int i, j, index;
	double sum;   
	int Offset;
	int templates[9] = { 1, 2, 1,2, 4, 2,1, 2, 1 };  

	for (i = 1; i < m_height-1; i ++)
	{
		Offset=i * m_width;
		for (j =1 ; j < m_width-1; j ++)
		{
#if DRAWCYCLE
			for (i = point_y-r; i < point_y+r; i ++)
			{
				Offset=i * m_width;
				for (j = point_x-r ; j < point_x+r; j ++)
				{
			if(((i-point_y)*(i-point_y)+(j-point_x)*(j-point_x))<r*r)
#endif
			{
				sum = 0.0;  
				index = 0;
				for(int m = i - 1;m < i + 2;m++)  
				{ 
					for(int n = j - 1; n < j + 2;n++) 
					{ 
						sum += putimg[m * m_width + n] *  templates[index++]; 
					}  
				}  

				mImage[Offset* 3 + j * 3+position] =((int)(sum / 16));//g_GammaLUT[((UINT)(sum / 16))];
			}
		}  
	}  

#ifdef DEF_FusionAlg
	int ttime = t.elapsed();	
	ss<<ttime<<endl;
	ofs<<ss.str();
	ofs.clear();
	ofs.close();
#endif
}

void FusionAlg::MallocImageS()
{
	MyListRGB *listRGB = MyListRGB::GetInstance();

	listRGB->ReleaseClaheRGB();
	listRGB->AddImage(mImage);

	mImage = new unsigned char[ImageSize() * 3];
}

void FusionAlg::AddRGB(unsigned char* r, unsigned char* g, unsigned char* b)
{
	unsigned char *pr = new unsigned char[ImageSize()];
	unsigned char *pg = new unsigned char[ImageSize()];
	unsigned char *pb = new unsigned char[ImageSize()];

	memcpy(pr, r, ImageSize());
	memcpy(pg, g, ImageSize());
	memcpy(pb, b, ImageSize());

	//ListRGB *listRGB = ListRGB::GetInstance();
	MyListRGB *listRGB = MyListRGB::GetInstance();
	listRGB->AddRGB(pr, pg, pb);
	//listRGB->RGBOpera(pr, pg, pb);
}


bool FusionAlg::IsRGB()
{
	//ListRGB *listRGB = ListRGB::GetInstance();
	MyListRGB *listRGB = MyListRGB::GetInstance();
	return listRGB->GetRGB(&mR, &mG, &mB);
}

bool FusionAlg::IsGaussianRGB()
{
	//ListRGB *listRGB = ListRGB::GetInstance();
	MyListRGB *listRGB = MyListRGB::GetInstance();
	return listRGB->GetGaussianRGB(&mEdgeeR, &mEdgeeG, &mEdgeeB);
}

bool FusionAlg::IsClaheRGBS()
{
	MyListRGB *listRGB = MyListRGB::GetInstance();
	return listRGB->GetClaheRGB(&mGaussianRS, &mGaussianGS, &mGaussianBS);
}

void FusionAlg::AddImage(unsigned char* img)
{
	unsigned char* tmpImg = new unsigned char[ImageSize()*3];
	memcpy(tmpImg, img, ImageSize()*3);

	MyListRGB *listRGB = MyListRGB::GetInstance();
	listRGB->AddImage(tmpImg);
}

bool FusionAlg::GetImage(unsigned char** img)
{
	//ListRGB *listRGB = ListRGB::GetInstance();
	MyListRGB *listRGB = MyListRGB::GetInstance();

	return listRGB->GetImage(img);
}

void FusionAlg::ReleaseImage()
{
	//ListRGB *listRGB = ListRGB::GetInstance();
	MyListRGB *listRGB = MyListRGB::GetInstance();
	listRGB->ReleaseImage();
	//listRGB->ImageOpera();
}



//算法
void FusionAlg::ClipHistogram (unsigned long* pulHistogram, unsigned int   
                    uiNrGreylevels, unsigned long ulClipLimit)   
                    /* This function performs clipping of the histogram and redistribution of bins.  
                    * The histogram is clipped and the number of excess pixels is counted. Afterwards  
                    * the excess pixels are equally redistributed across the whole histogram (providing  
                    * the bin count is smaller than the cliplimit).  
                    */   
{   
    unsigned long* pulBinPointer, *pulEndPointer, *pulHisto;   
    unsigned long ulNrExcess, ulOldNrExcess, ulUpper, ulBinIncr, ulStepSize, i;   
    long lBinExcess;   
       
    ulNrExcess = 0;  pulBinPointer = pulHistogram;   
    for (i = 0; i < uiNrGreylevels; i++)    
    { /* calculate total number of excess pixels */   
        lBinExcess = (long) pulBinPointer[i] - (long) ulClipLimit;   
        if (lBinExcess > 0) ulNrExcess += lBinExcess;      /* excess in current bin */   
    };   
       
    /* Second part: clip histogram and redistribute excess pixels in each bin */   
    ulBinIncr = ulNrExcess / uiNrGreylevels;          /* average binincrement */   
    ulUpper =  ulClipLimit - ulBinIncr;  /* Bins larger than ulUpper set to cliplimit */   
       
    for (i = 0; i < uiNrGreylevels; i++)    
    {   
        if (pulHistogram[i] > ulClipLimit)    
            pulHistogram[i] = ulClipLimit; /* clip bin */   
        else    
        {   
            if (pulHistogram[i] > ulUpper)    
            {       /* high bin count */   
                ulNrExcess -= ( ulUpper - pulHistogram[i] ); pulHistogram[i]=ulClipLimit;   
            }   
            else    
            {                   /* low bin count */   
                ulNrExcess -= ulBinIncr; pulHistogram[i] += ulBinIncr;   
            }   
        }   
    }   
       
       
       
    /* ####  
      
      IAC Modification:  
      In the original version of the loop below it was possible for an infinite loop to get  
      created.  If there was more pixels to be redistributed than available space then the  
      while loop would never end.  This problem has been fixed by stopping the loop when all  
      pixels have been redistributed OR when no pixels where redistributed in the previous iteration.  
      This change allows very low clipping levels to be used.  
        
#### */   
       
    do {   /* Redistribute remaining excess  */   
        pulEndPointer = &pulHistogram[uiNrGreylevels]; pulHisto = pulHistogram;   
           
        ulOldNrExcess = ulNrExcess;     /* Store number of excess pixels for test later. */   
           
        while (ulNrExcess && pulHisto < pulEndPointer)    
        {   
            ulStepSize = uiNrGreylevels / ulNrExcess;   
            if (ulStepSize < 1)    
                ulStepSize = 1;       /* stepsize at least 1 */   
            for (pulBinPointer=pulHisto; pulBinPointer < pulEndPointer && ulNrExcess;   
            pulBinPointer += ulStepSize)    
            {   
                if (*pulBinPointer < ulClipLimit)    
                {   
                    (*pulBinPointer)++;  ulNrExcess--;    /* reduce excess */   
                }   
            }   
            pulHisto++;       /* restart redistributing on other bin location */   
        }   
    } while ((ulNrExcess) && (ulNrExcess < ulOldNrExcess));   
    /* Finish loop when we have no more pixels or we can't redistribute any more pixels */   
}   
   
   
void FusionAlg::MakeHistogram (unsigned char* pImage, unsigned int uiXRes,   
                    unsigned int uiSizeX, unsigned int uiSizeY,   
                    unsigned long* pulHistogram,   
                    unsigned int uiNrGreylevels, unsigned char* pLookupTable)   
                    /* This function classifies the greylevels present in the array image into  
                    * a greylevel histogram. The pLookupTable specifies the relationship  
                    * between the greyvalue of the pixel (typically between 0 and 4095) and  
                    * the corresponding bin in the histogram (usually containing only 128 bins).  
                    */   
{   
    unsigned char* pImagePointer;   
    unsigned int i;   
       
    for (i = 0; i < uiNrGreylevels; i++)    
        pulHistogram[i] = 0L; /* clear histogram */   
       
    for (i = 0; i < uiSizeY; i++)    
    {   
        pImagePointer = &pImage[uiSizeX];   
        while (pImage < pImagePointer)    
            pulHistogram[pLookupTable[*pImage++]]++;   
        pImagePointer += uiXRes;   
        pImage = pImagePointer-uiSizeX;   
    }   
}   
   
void FusionAlg::MapHistogram (unsigned long* pulHistogram, unsigned char Min, unsigned char Max,   
                   unsigned int uiNrGreylevels, unsigned long ulNrOfPixels)   
                   /* This function calculates the equalized lookup table (mapping) by  
                   * cumulating the input histogram. Note: lookup table is rescaled in range [Min..Max].  
                   */   
{   
    unsigned int i;  unsigned long ulSum = 0;   
    const float fScale = ((float)(Max - Min)) / ulNrOfPixels;   
    const unsigned long ulMin = (unsigned long) Min;   
       
    for (i = 0; i < uiNrGreylevels; i++)    
    {   
        ulSum += pulHistogram[i];    
        pulHistogram[i]=(unsigned long)(ulMin+ulSum*fScale);   
        if (pulHistogram[i] > Max)    
            pulHistogram[i] = Max;   
    }   
}   
   
void FusionAlg::MakeLut (unsigned char * pLUT, unsigned char Min, unsigned char Max, unsigned int uiNrBins)   
/* To speed up histogram clipping, the input image [Min,Max] is scaled down to  
* [0,uiNrBins-1]. This function calculates the LUT(look up table).  
*/   
{   
    int i;   
    const unsigned char BinSize = (unsigned char) (1 + (Max - Min) / uiNrBins);   
       
    for (i = Min; i <= Max; i++)     
        pLUT[i] = (i - Min) / BinSize;   
}   

void FusionAlg::Interpolate (unsigned char * pImage, int uiXRes, unsigned long * pulMapLU,   
                  unsigned long * pulMapRU, unsigned long * pulMapLB,  unsigned long * pulMapRB,   
                  unsigned int uiXSize, unsigned int uiYSize, unsigned char * pLUT)   
                  /* pImage      - pointer to input/output image  
                  * uiXRes      - resolution of image in x-direction  
                  * pulMap*     - mappings of greylevels from histograms  
                  * uiXSize     - uiXSize of image submatrix  
                  * uiYSize     - uiYSize of image submatrix  
                  * pLUT           - lookup table containing mapping greyvalues to bins  
                  * This function calculates the new greylevel assignments of pixels within a submatrix  
                  * of the image with size uiXSize and uiYSize. This is done by a bilinear interpolation  
                  * between four different mappings in order to eliminate boundary artifacts.  
                  * It uses a division; since division is often an expensive operation, I added code to  
                  * perform a logical shift instead when feasible.  
                  */   
{   
    const unsigned int uiIncr = uiXRes-uiXSize; /* Pointer increment after processing row */   
    unsigned char GreyValue; unsigned int uiNum = uiXSize*uiYSize; /* Normalization factor */   
       
    unsigned int uiXCoef, uiYCoef, uiXInvCoef, uiYInvCoef, uiShift = 0;   
       
    if (uiNum & (uiNum - 1))   /* If uiNum is not a power of two, use division */   
        for (uiYCoef = 0, uiYInvCoef = uiYSize; uiYCoef < uiYSize;   
        uiYCoef++, uiYInvCoef--,pImage+=uiIncr)    
        {   
            for (uiXCoef = 0, uiXInvCoef = uiXSize; uiXCoef < uiXSize;   
            uiXCoef++, uiXInvCoef--)    
            {   
                GreyValue = pLUT[*pImage];         /* get histogram bin value */   
                *pImage++ = (unsigned char ) ((uiYInvCoef * (uiXInvCoef*pulMapLU[GreyValue]   
                    + uiXCoef * pulMapRU[GreyValue])   
                    + uiYCoef * (uiXInvCoef * pulMapLB[GreyValue]   
                    + uiXCoef * pulMapRB[GreyValue])) / uiNum);   
            }   
        }   
        else {             /* avoid the division and use a right shift instead */   
            while (uiNum >>= 1) uiShift++;           /* Calculate 2log of uiNum */   
            for (uiYCoef = 0, uiYInvCoef = uiYSize; uiYCoef < uiYSize;   
            uiYCoef++, uiYInvCoef--,pImage+=uiIncr)    
            {   
                for (uiXCoef = 0, uiXInvCoef = uiXSize; uiXCoef < uiXSize;   
                uiXCoef++, uiXInvCoef--)    
                {   
                    GreyValue = pLUT[*pImage];    /* get histogram bin value */   
                    *pImage++ = (unsigned char)((uiYInvCoef* (uiXInvCoef * pulMapLU[GreyValue]   
                        + uiXCoef * pulMapRU[GreyValue])   
                        + uiYCoef * (uiXInvCoef * pulMapLB[GreyValue]   
                        + uiXCoef * pulMapRB[GreyValue])) >> uiShift);   
                }   
            }   
        }   
}   
