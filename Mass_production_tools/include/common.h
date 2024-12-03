#ifndef __COMMON
#define __COMMON

#ifndef NULL
#define NULL (void *)0
#endif


typedef struct Region{
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iRows;
}Region,*PRegion;

typedef struct RegionCartesian{
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iRows;
}RegionCartesian,*PRegionCartesian;


#endif
