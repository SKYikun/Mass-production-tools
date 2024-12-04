/*************************************************************************
 * @file display_manager.c
 * @brief 显示设备管理层的库
 * @detail 实现显示管理的函数
 * @author JunjieWu
 * @date 2024.12.4
 * @version  v1.0.0
 * @note
 * @warning：<common.h>中有对NULL的宏定义
 * @par 历史版本
		V1.0.0创建于2024.12.4 更新内容：创建该文件
**************************************************************************/

#include <display_manager.h>
#include <stdio.h>
#include <string.h>


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

/**
 * @brief 在指定区域中居中显示一行文字
 * @param char* name 显示的文字,PRegion ptRegion 指定的区域,unsigned int dwColor指定颜色
 * @return void
 * @note 使用freetype库 注意笛卡尔坐标系和lcd坐标系之间的转换
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern void DrawTextInRegionCentural(char* name,PRegion ptRegion,unsigned int dwColor)
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


/**
 * @brief 在帧缓存buf里绘制区域
 * @param PRegion ptRegion 要绘制的区域,unsigned int dwColor 颜色
 * @return void
 * @note   超出屏幕的区域会被无视
 * @warning：无
 * */
extern void DrawRegion(PRegion ptRegion,unsigned int dwColor)
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



/**
 * @brief 在帧缓存buf里绘制位图
 * @param PFontBitMap ptFontBitMap 里面有region区域和bitmap,unsigned int dwColor 颜色
 * @return void
 * @note
 * @warning：无
 * */
extern void DrawFontBitmap(PFontBitMap ptFontBitMap,unsigned int dwColor)
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



/**
 * @brief 在帧缓存buf里绘制一个pixel
 * @param int x,int y,unsigned int dwColor x，y坐标和颜色
 * @return int 成功返回0
 * @note
 * @warning：无
 * */
extern int PutPixel(int x,int y,unsigned int dwColor)
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




/**
 * @brief 调用底层提供的函数把绘制好的buf给flush到硬件
 * @param PRegion ptRegion 区域结构体地址    PDispBuf ptDisBuf 帧缓存结构体地址
 * @return int 成功返回0 失败返回-1
 * @note   得到帧缓存地址后，指定区域即可刷该区域
 * @warning：无
 * */
extern int  DisplayRegionFlush(PRegion ptRegion,PDispBuf ptDisBuf)
{
	int iRet;
	iRet=g_DispDefault->FlushRegion(ptRegion,ptDisBuf);
	if(iRet){
		printf("FlushRegion err\n");
		return -1;
	}
	return 0;
}




/**
 * @brief 给底层提供注册函数，底层调用后把构造的设备对象注册进此文件内的全局链表
 * @param PDispOpr ptDispOpr  底层构造的显示设备操作对象地址
 * @return void
 * @note   需要先创建一个全局链表头
 * @warning：无
 * */
void RegisterDisplayer(PDispOpr ptDispOpr)
{
	ptDispOpr->ptNext=g_DispDevs;
	g_DispDevs=ptDispOpr;
}





/**
 * @brief 通过设备名来选择默认的显示器，遍历显示链表，保存默认显示设备到该文件
 * @param char* name 显示设备名称
 * @return int 成功返回0 失败返回-1
 * @note 需要先注册显示系统
 * @warning：无
 * */
extern int SlectDefaultDisplay(char *name)
{
	PDispOpr pTmp=g_DispDevs;
	while(pTmp!=NULL){
		if(strcmp(name,pTmp->name)==0){
			g_DispDefault=pTmp;
			return 0;
		}
		pTmp=pTmp->ptNext;
	}
	return -1;
}





/**
 * @brief 初始化默认显示设备的硬件，调用底层的私有函数保存默认显示设备的帧缓存及地址在该文件
 * @param void
 * @return int 成功返回0 失败返回-1
 * @note 需要先选择默认显示设备硬件
 * @warning：无
 * */
extern int  InitDefaultDisplay(void)
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






/**
 * @brief 给上层提供获得默认显示设备的帧缓存基地址的接口
 * @param void
 * @return PDispBuf 帧缓存结构体的地址，里面有buf地址
 * @note 需要先初始化默认显示设备硬件
 * @warning：无
 * */
extern PDispBuf GetDisplayBuf(void)
{
	return &g_tDispBuf;
}




/**
 * @brief 初始化所有显示器，把底层所有实例化的对象注册进链表
 * @param void
 * @return void
 * @note 需要调用到底层提供的FrameBufferRegister 要先声明
 * @warning：无
 * */
extern void DisplaySystemRegister(void)
{
	extern void FrameBufferRegister(void);
	FrameBufferRegister();
}
