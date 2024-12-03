#include <input_manager.h>
#include <tslib.h>
#include <stdio.h>


static struct tsdev *g_Ts;

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


static int TouchScreenDeviceInit(void)
{
	g_Ts = ts_setup(NULL, 0);
	if(!g_Ts){
			printf("ts setup err\n");
			return -1;
	}
	return 0;
}

static int TouchScreenDeviceExit(void)
{
	ts_close(g_Ts);
	return 0;
}


static InputDevice g_tTouchScreenDev={
	.name="touchscreen",
	.GetInputEvent=TouchScreenGetInputEvent,
	.DeviceInit=TouchScreenDeviceInit,
	.DeviceExit=TouchScreenDeviceExit,
};

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














