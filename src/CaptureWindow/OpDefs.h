
#ifndef  OPDEFS_INC
#define  OPDEFS_INC

//log
//#include "Log.h"

namespace OPGPIO
{
	enum Source{Filter=1, OtherSouce, Key};
	enum FilterMode
	{
	RGB=0,NBI,AFI,OtherMode
	};
	enum RGB{RColor = 0x01, GColor, BColor};
	enum GPIO{GPIO1=1, GPIO2, GPIO3, GPIO4};
	enum GPIOADD
	{
		GPIO1_ADD=0x1110, GPIO2_ADD = 0x1120,
		GPIO3_ADD=0x1130, GPIO4_ADD = 0x1140,
	};
	enum VIEWIMAGE{Eight, Twenty};
};

#endif   /* ----- #ifndef OPDEFS_INC  ----- */
