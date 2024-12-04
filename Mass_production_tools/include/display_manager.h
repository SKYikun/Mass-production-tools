/*************************************************************************
 * @file display_manager.h
 * @brief 显示设备管理层的头文件
 * @detail 抽象出了显示设备的框架类型
 * @author JunjieWu
 * @date 2024.12.4
 * @version  v1.0.0
 * @note Region结构体类型来自于<common.h> PFontBitMap结构体类型来自于<font_manager.h>
 * @warning：暂无
 * @par 历史版本
		V1.0.0创建于2024.12.4 更新内容：创建该文件
**************************************************************************/


#ifndef __DISP_MANAGER_H
#define __DISP_MANAGER_H
#include <font_manager.h>
#include <common.h>


//抽象出的显示器资源
typedef struct DispBuf{
	int iXres;  //x方向的分辨率
	int iYres;  //y方向的分辨率
	int iBpp;   //bit per pixel
	char* buf;	//一帧图像的缓存
}DispBuf,*PDispBuf;


//抽象出的显示器操作
typedef struct DispOpr{
	char *name;												//显示设备ID
	int (*DeviceInit)(void);								//初始化函数
	int (*DeviceExit)(void);								//销毁函数
	int (*GetBuffer)(PDispBuf ptDispBuf);					//获得一帧图像的缓存基地址
	int (*FlushRegion)(PRegion ptRegion,PDispBuf ptDispBuf);//把一帧图像的基地址刷到硬件
	struct DispOpr* ptNext;									//下一个显示设备的地址
}DispOpr,*PDispOpr;



/*begin of 给底层调用的接口*/
extern void RegisterDisplayer(PDispOpr ptDispOpr);   //底层注册一个显示设备操作到管理层链表
/*end of 给底层调用的接口*/


/*begin of 给上层调用的接口*/
void DisplaySystemRegister(void);							//注册所有显示设备操作，在管理层构造出一条链表
int SlectDefaultDisplay(char *name);						//选择默认的显示设备操作，保存在管理层
int InitDefaultDisplay(void);								//初始化默认的显示设备硬件
PDispBuf GetDisplayBuf(void);								//获取显示设备的一帧缓存

int PutPixel(int x,int y,unsigned int dwColor);				//在指定位置设写入帧缓存，一个像素
void DrawFontBitmap(PFontBitMap ptFontBitMap,unsigned int dwColor); //根据传入的位图和颜色去绘制像素
void DrawRegion(PRegion ptRegion,unsigned int dwColor);					 //根据传入的区域和颜色绘制像素
void DrawTextInRegionCentural(char* name,PRegion ptRegion,unsigned int dwColor);	//在传入的区域中居中绘制字符串


int DisplayRegionFlush(PRegion ptRegion,PDispBuf ptDisBuf);   //把帧缓存刷到硬件上
/*end of 给上层调用的接口*/



#endif
