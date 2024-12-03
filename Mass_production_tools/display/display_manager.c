#include <display_manager.h>
#include <stdio.h>
#include <string.h>
#include <font_manager.h>


/*管理底层lcd，web*/
static PDispOpr g_DispDevs=NULL;
static PDispOpr g_DispDefault=NULL;
static DispBuf g_tDispBuf;
static int line_width;
static int pixel_width;

int iLeftUpX;
int iLeftUpY;
int iWidth;
int iRows;

void DrawTextInRegionCentural(char* name,PRegion ptRegion,unsigned int dwColor)
{
	RegionCartesian tRegionCartesian;
	int i=0;
	int err;
	FontBitMap tFontBitMap;

	/*计算字符串的外框*/
	GetStringRegionCar(name,&tRegionCartesian);
	/*算出第一个字符的origin*/
	int iOrigionX=ptRegion->iLeftUpX+(ptRegion->iWidth-tRegionCartesian.iWidth)/2-tRegionCartesian.iLeftUpX;
	int iOrigionY=ptRegion->iLeftUpY+(ptRegion->iRows-tRegionCartesian.iRows)/2+tRegionCartesian.iLeftUpY;



	/*逐个绘制*/
	while(name[i]){
		/*get bitmap*/
		tFontBitMap.iCurOriginX=iOrigionX;
		tFontBitMap.iCurOriginY=iOrigionY;
		err=GetFontBitMap(name[i],&tFontBitMap);
		if(err){
			printf("GetFontBitMap err\n");
			return;
		}
		/*draw on buffer*/
		DrawFontBitmap(&tFontBitMap,dwColor);

		iOrigionX=tFontBitMap.iNextOriginX;
		iOrigionY=tFontBitMap.iNextOriginY;
		i++;
	}

}



void DrawRegion(PRegion ptRegion,unsigned int dwColor)
{
	int x=ptRegion->iLeftUpX;
	int y=ptRegion->iLeftUpY;
	int width=ptRegion->iWidth;
	int rows=ptRegion->iRows;
	int i,j;
	for(j=y;j<y+rows;j++){
		for(i=x;i<x+width;i++){
			if(i>g_tDispBuf.iXres||j>g_tDispBuf.iYres)
				continue;
			PutPixel(i,j,dwColor);
		}
	}
}


void DrawFontBitmap(PFontBitMap ptFontBitMap,unsigned int dwColor)
{
	int  i, j, p, q;
	int x=ptFontBitMap->tRegion.iLeftUpX;
	int y=ptFontBitMap->tRegion.iLeftUpY;
    int  x_max = ptFontBitMap->tRegion.iLeftUpX + ptFontBitMap->tRegion.iWidth;
    int  y_max = ptFontBitMap->tRegion.iLeftUpY+ ptFontBitMap->tRegion.iRows;
	int width=ptFontBitMap->tRegion.iWidth;
	unsigned char* buffer=ptFontBitMap->pucBuffer;
    //printf("x = %d, y = %d\n", x, y);

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if ( i < 0      || j < 0       ||
                i >= g_tDispBuf.iXres || j >= g_tDispBuf.iYres )
            continue;

            //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
            if(buffer[q*width+p])
            	PutPixel(i, j,dwColor);
        }
    }
}

/*在buf里绘制一个pixel*/
int PutPixel(int x,int y,unsigned int dwColor)
{
	unsigned char *pen_8 =(unsigned char *)g_tDispBuf.buf+y*line_width+x*pixel_width;
	unsigned short *pen_16;
	unsigned int *pen_32;

	unsigned int red, green, blue;

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (g_tDispBuf.iBpp)
	{
		case 8:
		{
			*pen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n",g_tDispBuf.iBpp);
			break;
		}
	}
	return 0;
}

/*把绘制好的buf，flush到硬件*/
int  DisplayRegionFlush(PRegion ptRegion,PDispBuf ptDisBuf)
{
	int iRet;
	iRet=g_DispDefault->FlushRegion(ptRegion,ptDisBuf);
	if(iRet){
		printf("FlushRegion err\n");
		return -1;
	}
	return 0;
}




/*
给底层提供注册函数，底层调用后把构造的设备对象注册进此文件内的全局链表
*/
void RegisterDisplayer(PDispOpr ptDispOpr)
{
	ptDispOpr->ptNext=g_DispDevs;
	g_DispDevs=ptDispOpr;
}



/*
选择默认的显示器，遍历显示链表，通过设备名来选择
*/
int SlectDefaultDisplay(char *name)
{
	PDispOpr pTmp=g_DispDevs;
	while(g_DispDevs!=NULL){
		if(strcmp(name,pTmp->name)==0){
			g_DispDefault=pTmp;
			return 0;
		}
	}
	return -1;
}



/*
初始化已选择的默认显示器
通过默认显示对象，调用底层的私有函数
1.硬件初始化
2.获得buf
*/
int  InitDefaultDisplay(void)
{
	int iRet;
	iRet=g_DispDefault->DeviceInit();
	if(iRet){
		printf("DeviceInit err\n");
		return -1;
	}
	iRet=g_DispDefault->GetBuffer(&g_tDispBuf);
		if(iRet){
		printf("GetBuffer err\n");
		return -1;
	}
	line_width=g_tDispBuf.iXres*g_tDispBuf.iBpp/8;
	pixel_width=g_tDispBuf.iBpp/8;
	return 0;
}

/*返回buf*/
PDispBuf GetDisplayBuf(void)
{
	return &g_tDispBuf;
}



/*
初始化所有显示器，把底层所有实例化的对象注册进链表
*/
void DisplaySystemRegister(void)
{
	extern void FrameBufferRegister(void);
	FrameBufferRegister();
}
