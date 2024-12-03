#ifndef __DISP_MANAGER
#define __DISP_MANAGER
#include <font_manager.h>
#include <common.h>

typedef struct DispBuf{
	int iXres;
	int iYres;
	int iBpp;
	char* buf;
}DispBuf,*PDispBuf;



typedef struct DispOpr{
	char *name;
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	int (*GetBuffer)(PDispBuf ptDispBuf);
	int (*FlushRegion)(PRegion ptRegion,PDispBuf ptDispBuf);
	struct DispOpr* ptNext;
}DispOpr,*PDispOpr;

void RegisterDisplayer(PDispOpr ptDispOpr);



void DisplaySystemRegister(void);
int SlectDefaultDisplay(char *name);
int InitDefaultDisplay(void);
PDispBuf GetDisplayBuf(void);

int PutPixel(int x,int y,unsigned int dwColor);
void DrawFontBitmap(PFontBitMap ptFontBitMap,unsigned int dwColor);
void DrawRegion(PRegion ptRegion,unsigned int dwColor);
void DrawTextInRegionCentural(char* name,PRegion ptRegion,unsigned int dwColor);


int DisplayRegionFlush(PRegion ptRegion,PDispBuf ptDisBuf);





#endif
