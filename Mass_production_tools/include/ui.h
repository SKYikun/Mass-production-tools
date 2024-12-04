/****************************************************************************
 * @file ui.h
 * @brief 实现了lcd按钮的抽象
 * @detail 抽象出了按钮对象，对于不同按钮可以指定属性和重写draw/onpressed函数
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 无
 * @warning：无
 * @par 历史版本
		 v1.0.0创建于2024.12.4，更新内容：创建该文件
 ****************************************************************************/
#ifndef __UI_H
#define __UI_H


#include <common.h>
#include <display_manager.h>
#include <input_manager.h>

#define BUTTON_DEFAULT_COLOR 0X00FF0000
#define BUTTON_PRESSED_COLOR 0X0000FF00
#define BUTTON_PERCENT_COLOR 0X000000FF
#define BUTTON_TEXT_COLOR 0X00000000

struct Button;
typedef int (*ONDRAW_FUNC)(struct Button *ptButton,PDispBuf ptDispBuf);
typedef int (*ONPRESSED_FUNC)(struct Button *ptButton,PDispBuf ptDispBuf,PInputEvent ptInputEvent);

//抽象出了Button结构体对象 没有使用链表结构管理 直接使用线性表管理更简单
typedef struct Button{
	char* name;
	int status;
	Region tRegion;
	ONDRAW_FUNC OnDraw;
	ONPRESSED_FUNC OnPressed;
}Button,*PButton;


extern void InitButton(PButton ptButton,char* name,PRegion ptRegion,ONDRAW_FUNC OnDraw,ONPRESSED_FUNC OnPressed);//初始化按钮，根据是否传入函数指针来重写函数




#endif
