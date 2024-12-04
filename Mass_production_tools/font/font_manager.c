/****************************************************************
 * @file font_manager.c
 * @brief 字体管理层库
 * @detail 实现了不同字体的选择
 * @author JunjieWu
 * @date 2024.12.4
 * @version  v1.0.0
 * @note使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		 v1.0.0创建于2024.12.4，更新内容：创建该文件
 * **************************************************************/
#include <font_manager.h>
#include <string.h>
#include <stdio.h>
#include <common.h>
#define DEBUG 0
static PFontOpr g_ptFonts=NULL;
static PFontOpr g_ptDefaultFont=NULL;


/**
 * @brief 底层提供的字体注册
 * @param PFontOpr ptFontOpr 字体对象地址
 * @return void
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern void RegisterFont(PFontOpr ptFontOpr)
{

	ptFontOpr->ptNext=g_ptFonts;
	g_ptFonts=ptFontOpr;

}




/**
 * @brief 注册字体系统到该文件的全局链表
 * @param void
 * @return void
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern void FontSystemRegister(void)
{
	extern void FreetypeRegister(void);
	FreetypeRegister();
}




/**
 * @brief 选择和初始化字体
 * @param char* name 字体名,char* aFontFileName 字库文件名
 * @return void
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern int SelectAndInitFont(char* name,char* aFontFileName)
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



/**
 * @brief 调用底层字体对象提供的接口设置字体大小
 * @param int iFontSize 字体大小 （像素单位）
 * @return int 成功返回0 失败返回-1
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern int SetFontSize(int iFontSize)
{
	if(g_ptDefaultFont)
		return g_ptDefaultFont->SetFontSize(iFontSize);
	else
		return -1;

}


/**
 * @brief 调用底层字体对象提供的接口通过unicode值得到字符位图
 * @param unsigned int dwCode unicode值, PFontBitMap ptFontBitMap 需要写入的位图地址
 * @return int 成功返回0 失败返回-1
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	if(g_ptDefaultFont)
		return g_ptDefaultFont->GetFontBitMap(dwCode,ptFontBitMap);
	else
		return -1;
}



/**
 * @brief 调用底层字体对象提供的接口获得笛卡尔坐标系下字符串的外框
 * @param char* str 字符串,PRegionCartesian ptRegionCartesian 需要写入的外框地址
 * @return int 成功返回0 失败返回-1
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern int GetStringRegionCar(char* str,PRegionCartesian ptRegionCartesian)
{
	if(g_ptDefaultFont)
		return g_ptDefaultFont->GetSringRegionCar(str,ptRegionCartesian);
	else
		return -1;
}


