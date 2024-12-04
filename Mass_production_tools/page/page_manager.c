/*************************************************************************
 * @file main_page.c
 * @brief  主程序主要运行菜单
 * @detail 实现了Button里draw/onpressesd函数的重写 实现了Button界面的生成
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		v1.0.0创建于2024.12.4，更新内容：创建该文件
 ************************************************************************/
#include <page_manager.h>
#include <common.h>
#include <string.h>
#include <stdio.h>
static PPageAction g_ptPages;


/**
 * @brief 注册一个Page到该文件的全局链表
 * @param PPageAction ptPageAction 底层构造的Page对象的地址
 * @return void
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
void RegisterPage(PPageAction ptPageAction)
{
	ptPageAction->ptNext=g_ptPages;
	g_ptPages=ptPageAction;
}




/**
 * @brief 根据传入的名字来找到对应的Page
 * @param char* name page名
 * @return PPageAction Page指针
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
PPageAction Page(char* name)
{
	PPageAction ptTmp=g_ptPages;
	while(ptTmp){
		if(strcmp(name,ptTmp->name)==0){
			return ptTmp;
		}
		ptTmp=ptTmp->ptNext;
	}
	return NULL;
}




/**
 * @brief 注册页面系统
 * @param void
 * @return void
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern void PageSystemRegister(void)
{
 	extern void RegisterMainPage(void);
 	RegisterMainPage();
}
