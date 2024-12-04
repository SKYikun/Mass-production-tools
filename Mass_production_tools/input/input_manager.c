/****************************************************************
 * @file input_manager.c
 * @brief 输入设备的管理层
 * @detail 实现不同输入设备的数据上报统一
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 使用了pthread库注意链接pthread库
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par v1.0.0 2024.12.4 创建该文件

*****************************************************************/
#include <input_manager.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

static PInputDevice g_InputDevs=NULL;

static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;




/*start of circle buffer*/
#define BUFFER_LEN 20
static InputEvent g_aInputEvents[BUFFER_LEN];
static int g_iRead=0;
static int g_iWrite=0;


/**
 * @brief 判断环形缓冲区是否满
 * @param void
 * @return 1表示满 0表示未满
 * @note 无
 * @warning：无
 * */
static int isInputBufferFull(void)
{
	return ((g_iWrite+1)%BUFFER_LEN==g_iRead);

}


/**
 * @brief 判断环形缓冲区是否空
 * @param void
 * @return 1表示空 0表示非空
 * @note 无
 * @warning：无
 * */
static int isInputBufferEmpty(void)
{
	return (g_iRead==g_iWrite);

}


/**
 * @brief 把输入事件放入缓冲区
 * @param PInputEvent ptInputEvent  要写入的输入事件地址
 * @return void
 * @note 如果环形缓冲区满了就不做处理
 * @warning：无
 * */
static void PutInputEventToBuffer(PInputEvent ptInputEvent)
{
	if(!isInputBufferFull()){
		g_aInputEvents[g_iWrite]=*ptInputEvent;
		g_iWrite=(g_iWrite+1)%BUFFER_LEN;
	}

}



/**
 * @brief 从缓冲区得到输入事件
 * @param PInputEvent ptInputEvent  读出的事件的存放地址
 * @return int 1读成功 0读失败
 * @note 如果环形缓冲区满了就不做处理
 * @warning：无
 * */
static int  GetInputEventFromBuffer(PInputEvent ptInputEvent)
{
	if(!isInputBufferEmpty()){
		*ptInputEvent=g_aInputEvents[g_iRead];
		g_iRead=(g_iRead+1)%BUFFER_LEN;
		return 1;
	}
	else{
		return 0;
	}

}

/*end of circle buffer*/

















/**
 * @brief 向底层提供的设备注册函数
 * @param PInputDevice pInputDevice 输入设备对象的地址
 * @return void
 * @note 无
 * @warning：无
 * */
extern void RegisterInputDev(PInputDevice pInputDevice)
{
	pInputDevice->ptNext=g_InputDevs;
	g_InputDevs=pInputDevice;
}




/**
 * @brief 把所有输入设备注册进该文件的全局链表
 * @param void
 * @return void
 * @note 注意声明底层提供的注册函数
 * @warning：无
 * */
extern void InputSystemRegister(void)
{
	extern void TouchScreenDevRegister(void);
	extern void NetInputDevRegister(void);
	/*register touchscreen*/
	TouchScreenDevRegister();
	/*register netinput*/
	NetInputDevRegister();
}




/**
 * @brief 输入设备的线程函数
 * @param void* data PInputDevice 设备对象地址 用这个区分不同设备
 * @return void*
 * @note 无
 * @warning：无
 * */
static void* input_recv_thread_func(void* data)
{
	int iRet;
	InputEvent tInputEvent;
	PInputDevice ptInputDev=(PInputDevice)data;
	while(1){

		iRet=ptInputDev->GetInputEvent(&tInputEvent);
		if(!iRet){
			/*保存数据*/
			pthread_mutex_lock(&g_tMutex);
			PutInputEventToBuffer(&tInputEvent);
			/*唤醒读取线程*/
			pthread_cond_signal(&g_tConVar);
			pthread_mutex_unlock(&g_tMutex);


		}
	}
	return NULL;

}




/**
 * @brief 初始化链表里所有输入设备的硬件，并且为每个设备创建接收输入事件的线程
 * @param void
 * @return void
 * @note 需要先注册所有的输入设备
 * @warning：无
 * */
extern void InputDeviceInit(void)
{
	/*for each devices  init hardware && create thread*/
	int iRet;
	pthread_t tid;
	PInputDevice pTemp=g_InputDevs;
	while(pTemp!=NULL){
		/*init hardware*/
		iRet=pTemp->DeviceInit();
		if(!iRet){
			/*create thread*/
			iRet = pthread_create(&tid, NULL, input_recv_thread_func, pTemp);
		}
		pTemp=pTemp->ptNext;
	}
}



/**
 * @brief 从环形缓冲区读取数据，阻塞
 * @param PInputEvent ptInputEvent 需要写入的输入事件地址
 * @return int  成功返回0 失败返回-1
 * @note 需要先初始化所有的输入设备的硬件
 * @warning：无
 * */
extern int  GetInputEvent(PInputEvent ptInputEvent)
{
	int ret;
	InputEvent tInputEvent;
	/*无数据则休眠*/
	pthread_mutex_lock(&g_tMutex);
	if(GetInputEventFromBuffer(&tInputEvent)){
		pthread_mutex_unlock(&g_tMutex);
		*ptInputEvent=tInputEvent;
		return 0;
	}
	else{
		/*休眠等待唤醒*/
		pthread_cond_wait(&g_tConVar, &g_tMutex);
		if(GetInputEventFromBuffer(&tInputEvent)){
			*ptInputEvent=tInputEvent;
			ret=0;
		}
		else{
			ret=-1;
		}
		pthread_mutex_unlock(&g_tMutex);
	}
	/*否则返回数据*/
	return ret;
}






