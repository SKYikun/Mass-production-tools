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

static int isInputBufferFull(void)
{
	return ((g_iWrite+1)%BUFFER_LEN==g_iRead);

}


static int isInputBufferEmpty(void)
{
	return (g_iRead==g_iWrite);

}


static void PutInputEventToBuffer(PInputEvent ptInputEvent)
{
	if(!isInputBufferFull()){
		g_aInputEvents[g_iWrite]=*ptInputEvent;
		g_iWrite=(g_iWrite+1)%BUFFER_LEN;
	}

}

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


void RegisterInputDev(PInputDevice pInputDevice)
{
	pInputDevice->ptNext=g_InputDevs;
	g_InputDevs=pInputDevice;
}

void InputSystemRegister(void)
{
	extern void TouchScreenDevRegister(void);
	extern void NetInputDevRegister(void);
	/*register touchscreen*/
	TouchScreenDevRegister();
	/*register netinput*/
	NetInputDevRegister();
}


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




void InputDeviceInit(void)
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

int  GetInputEvent(PInputEvent ptInputEvent)
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






