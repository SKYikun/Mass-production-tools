#include <font_manager.h>
#include <string.h>
#include <stdio.h>
#include <common.h>
#define DEBUG 0
static PFontOpr g_ptFonts=NULL;
static PFontOpr g_ptDefaultFont=NULL;
void RegisterFont(PFontOpr ptFontOpr)
{

	ptFontOpr->ptNext=g_ptFonts;
	g_ptFonts=ptFontOpr;

}

void FontSystemRegister(void)
{
	extern void FreetypeRegister(void);
	FreetypeRegister();
}

int SelectAndInitFont(char* name,char* aFontFileName)
{
	PFontOpr ptTmp=g_ptFonts;
	while(ptTmp!=NULL){
		if(strcmp(ptTmp->name,name)==0)
			break;
		else
			ptTmp=ptTmp->ptNext;
	}
	if(!ptTmp)
		return -1;
	g_ptDefaultFont=ptTmp;
	return ptTmp->FontInit(aFontFileName);

}

int SetFontSize(int iFontSize)
{
	if(g_ptDefaultFont)
		return g_ptDefaultFont->SetFontSize(iFontSize);
	else
		return -1;

}

int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	if(g_ptDefaultFont)
		return g_ptDefaultFont->GetFontBitMap(dwCode,ptFontBitMap);
	else
		return -1;
}

int GetStringRegionCar(char* str,PRegionCartesian ptRegionCartesian)
{
	if(g_ptDefaultFont)
		return g_ptDefaultFont->GetSringRegionCar(str,ptRegionCartesian);
	else
		return -1;
}


