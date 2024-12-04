/****************************************************************************
 * @file button.c
 * @brief button的库
 * @detail 提供了按钮初始化函数和默认的draw/pressed函数
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 无
 * @warning：无
 * @par 历史版本
		 v1.0.0创建于2024.12.4，更新内容：创建该文件
 ****************************************************************************/
#include <ui.h>
#include <display_manager.h>


/**
 * @brief Button默认的绘制函数
 * @param struct Button *ptButton 按钮对象指针,PDispBuf ptDispBuff 显存地址
 * @return int 返回0
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
static int DefaultOnDraw(struct Button *ptButton,PDispBuf ptDispBuff)
{
	/*绘制按键*/
	DrawRegion(&ptButton->tRegion,BUTTON_DEFAULT_COLOR);

	/*居中写文字*/
	DrawTextInRegionCentural(ptButton->name,&ptButton->tRegion,BUTTON_TEXT_COLOR);

	/*把buff刷到硬件上*/
	DisplayRegionFlush(&ptButton->tRegion,ptDispBuff);
	return  0;

}






/**
 * @brief Button默认的按下响应函数
 * @param struct Button *ptButton 按钮对象指针,PDispBuf ptDispBuff 显存地址，PInputEvent ptInputEvent输入事件指针
 * @return int 返回0
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
static int DefaultOnpressed(struct Button *ptButton,PDispBuf ptDispBuff,PInputEvent ptInputEvent)
{
	unsigned int dwcolor=BUTTON_DEFAULT_COLOR;
	ptButton->status=!ptButton->status;
	if(ptButton->status)
		dwcolor=BUTTON_PRESSED_COLOR;

	/*绘制按键*/
	DrawRegion(&ptButton->tRegion,dwcolor);

	/*居中写文字*/
	DrawTextInRegionCentural(ptButton->name,&ptButton->tRegion,BUTTON_TEXT_COLOR);

	/*把buff刷到硬件上*/
	DisplayRegionFlush(&ptButton->tRegion,ptDispBuff);
	return 0;

}





/**
 * @brief 根据传入的参数初始化Button
 * @param struct Button *ptButton 按钮对象指针,char* name 按钮名字,PRegion ptRegion 按钮占据区域,ONDRAW_FUNC OnDraw绘制函数,ONPRESSED_FUNC OnPressed 按下响应函数
 * @return int 返回0
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern void InitButton(PButton ptButton,char* name,PRegion ptRegion,ONDRAW_FUNC OnDraw,ONPRESSED_FUNC OnPressed)
{
	ptButton->name=name;
	ptButton->status=0;
	if(ptRegion)
		ptButton->tRegion=*ptRegion;
	ptButton->OnDraw=OnDraw ? OnDraw:DefaultOnDraw;
	ptButton->OnPressed=OnPressed ?OnPressed:DefaultOnpressed;
}

