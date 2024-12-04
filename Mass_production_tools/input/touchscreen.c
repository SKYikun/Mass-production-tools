/****************************************************************
 * @file  touchscreen.c
 * @brief 触摸屏设备实现层 基于udp
 * @detail 实现向管理层注册触摸屏输入设备
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 	使用了tslib库 需要连接tslib
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par v1.0.0 2024.12.4 创建该文件

*****************************************************************/
#include <input_manager.h>
#include <tslib.h>
#include <stdio.h>


static struct tsdev *g_Ts;



/**
 * @brief 向管理层提供，获取触摸屏设备输入事件 阻塞
 * @param PInputEvent ptInputEvent 要写入的输入事件地址
 * @return 0表示成功 -1表示失败
 * @note
 * @warning：无
 * */
static int TouchScreenGetInputEvent(PInputEvent ptInputEvent)
{
	int iRet;
	struct ts_sample samp;
	iRet = ts_read(g_Ts, &samp, 1);
	if(iRet!=1)
		return -1;
 	ptInputEvent->iType    =INPUT_TYPE_TOUCH;
 	ptInputEvent->iX        =samp.x;
 	ptInputEvent->iY       =samp.y;
 	ptInputEvent->iPresure =samp.pressure;
 	ptInputEvent->tTime    =samp.tv;
	return 0;
}



/**
 * @brief 向管理层提供，触摸屏设备硬件初始化
 * @param void
 * @return 0表示成功 -1表示失败
 * @note 自动检测ts设备，tslib根据配置文件注册调用链表，以便后续递归调用
 * @warning：无
 * */
static int TouchScreenDeviceInit(void)
{
	g_Ts = ts_setup(NULL, 0);
	if(!g_Ts){
			printf("ts setup err\n");
			return -1;
	}
	return 0;
}



/**
 * @brief 向管理层提供，触摸屏设备硬件关闭
 * @param void
 * @return 0
 * @note
 * @warning：无
 * */
static int TouchScreenDeviceExit(void)
{
	ts_close(g_Ts);
	return 0;
}



//实例化管理层构造的输入设备结构体对象为触摸屏输入设备
static InputDevice g_tTouchScreenDev={
	.name="touchscreen",
	.GetInputEvent=TouchScreenGetInputEvent,
	.DeviceInit=TouchScreenDeviceInit,
	.DeviceExit=TouchScreenDeviceExit,
};



/**
 * @brief 向管理层暴露的注册接口
 * @param void
 * @return 0
 * @note
 * @warning：无
 * */
void TouchScreenDevRegister(void)
{
	RegisterInputDev(&g_tTouchScreenDev);
}


#if 0
int main(int argc,char** argv)
{
	int iRet;
	InputEvent tInputEvent;
	g_tTouchScreenDev.DeviceInit();

	while(1){
		iRet=g_tTouchScreenDev.GetInputEvent(&tInputEvent);
		if(iRet){
			printf("GetInputEvent err\n");
			return -1;
		}
		else{
			printf("type=%d\n",tInputEvent.iType);
			printf("x=%d\n",tInputEvent.iX);
			printf("y=%d\n",tInputEvent.iY);
			printf("presure=%d\n",tInputEvent.iPresure);
		}
	}

	return 0;
}

#endif














