#ifndef __FONT_MANAGER
#define __FONT_MANAGER
#include <common.h>


typedef struct FontBitMap{
	Region tRegion;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char* pucBuffer;
}FontBitMap,*PFontBitMap;



typedef struct FontOpr{
	char* name;
	int (*FontInit)(char* aFineName);
	int (*SetFontSize)(int iFontSize);
	int (*GetFontBitMap)(unsigned int idwCode, PFontBitMap ptFontBitMap);
	int (*GetSringRegionCar)(char *str,PRegionCartesian ptRegionCartesian);
	struct FontOpr* ptNext;
}FontOpr,*PFontOpr;

void RegisterFont(PFontOpr ptFontOpr);



void FontSystemRegister(void);
int SelectAndInitFont(char* name,char* aFontFileName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int idwCode, PFontBitMap ptFontBitMap);
int GetStringRegionCar(char* str,PRegionCartesian ptRegionCartesian);



#endif

