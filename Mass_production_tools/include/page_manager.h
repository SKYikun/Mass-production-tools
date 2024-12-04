/***********************************************************************
 * @file page_manager.h
 * @brief  页面管理头文件
 * @detail 抽象出了页面结构体对象
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		v1.0.0创建于2024.12.4，更新内容：创建该文件
 ***********************************************************************/

#ifndef __PAGE_MANAGER_H
#define __PAGE_MANAGER_H



//抽象出页面对象
typedef struct PageAction{
	char* name;
	int (*Run)(void* pParam);
	struct PageAction* ptNext;
}PageAction,*PPageAction;

/*begin of 底层函数接口*/
void RegisterPage(PPageAction ptPageAction); //底层能够使用该接口注册一个Page对象
/*end of 底层函数接口*/


/*begin of 上层函数接口*/
void PageSystemRegister(void);					//上层调用，注册所有的Page对象到该文件的全局链表
PPageAction Page(char* name);					//上层调用，遍历链表名字来找到对应的page
/*end of 上层函数接口*/

#endif
