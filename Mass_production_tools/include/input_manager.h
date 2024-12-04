/****************************************************************
 * @file input_manager.h
 * @brief 输入设备的管理层
 * @detail 实现不同输入设备的数据上报统一
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 使用syst/time.h 里的timvval结构体
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par v1.0.0 2024.12.4 创建该文件

*****************************************************************/
#ifndef __INPUT_MANAGER_H
#define __INPUT_MANAGER_H

#include <sys/time.h>

#ifndef NULL
#define NULL (void*)0
#endif

#define INPUT_TYPE_TOUCH 1              //触摸屏输入
#define INPUT_TYPE_NET   2				//网络输入


//输入事件结构体的抽象
typedef struct InputEvent{
	struct timeval tTime;
	int iType;
	int iX;
	int iY;
	int iPresure;
	char str[1000];
}InputEvent,*PInputEvent;

//输入设备操作的抽像
typedef struct InputDevice{
	char* name;
	int (*GetInputEvent)(PInputEvent ptInputEvent);
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	struct InputDevice* ptNext;
}InputDevice,*PInputDevice;


/*beigin of 给底层提供的函数*/
void RegisterInputDev(PInputDevice pInputDevice);  //注册输入设备到管理层链表
/*end of 给底层提供的函数*/



/*beigin of 给上层提供的函数*/
void InputSystemRegister(void);                    //注册所有输入设备到管理层链表
void InputDeviceInit(void);						   //初始化所有输入设备的硬件，为每个输入设备创建事件监测线程
int  GetInputEvent(PInputEvent ptInputEvent); //从管理层的环形缓冲区读取输入事件
/*end of 给上层提供的函数*/


#endif
