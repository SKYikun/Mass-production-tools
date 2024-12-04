/****************************************************************
 * @file font_manager.h
 * @brief 字体管理层头文件
 * @detail 实现了位图对象和字符操作对象的抽象
 * @author JunjieWu
 * @date 2024.12.4
 * @version  v1.0.0
 * @note使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		 v1.0.0创建于2024.12.4，更新内容：创建该文件
 * **************************************************************/





#ifndef __FONT_MANAGER_H
#define __FONT_MANAGER_H
#include <common.h>

//抽象出的字体位图结构体对象
typedef struct FontBitMap{
	Region tRegion;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char* pucBuffer;
}FontBitMap,*PFontBitMap;


//抽象出的字体操作结构体对象
typedef struct FontOpr{
	char* name;
	int (*FontInit)(char* aFineName);
	int (*SetFontSize)(int iFontSize);
	int (*GetFontBitMap)(unsigned int idwCode, PFontBitMap ptFontBitMap);
	int (*GetSringRegionCar)(char *str,PRegionCartesian ptRegionCartesian);
	struct FontOpr* ptNext;
}FontOpr,*PFontOpr;


/*begin of 向底层提供的调用接口*/
extern void RegisterFont(PFontOpr ptFontOpr);
/*end of 向底层提供的调用接口*/



/*begin of 向上层提供的调用接口*/
extern void FontSystemRegister(void);                        //把所有字体注册进管理层的链表
extern int SelectAndInitFont(char* name,char* aFontFileName); //根据名字遍历链表选择和初始化字体
extern int SetFontSize(int iFontSize);								//设置字体大小
extern int GetFontBitMap(unsigned int idwCode, PFontBitMap ptFontBitMap);  //根据unicode值，得到字符的位图
extern int GetStringRegionCar(char* str,PRegionCartesian ptRegionCartesian);	//得到字符串在笛卡尔坐标系下的边框
/*begin of 向上层提供的调用接口*/



#endif

