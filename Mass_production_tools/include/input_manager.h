#ifndef __INPUT_MANAGER
#define __INPUT_MANAGER

#include <sys/time.h>

#ifndef NULL
#define NULL (void*)0
#endif

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET   2

typedef struct InputEvent{
	struct timeval tTime;
	int iType;
	int iX;
	int iY;
	int iPresure;
	char str[1000];
}InputEvent,*PInputEvent;

typedef struct InputDevice{
	char* name;
	int (*GetInputEvent)(PInputEvent ptInputEvent);
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	struct InputDevice* ptNext;
}InputDevice,*PInputDevice;

void RegisterInputDev(PInputDevice pInputDevice);



void InputSystemRegister(void);
void InputDeviceInit(void);
int  GetInputEvent(PInputEvent ptInputEvent);


#endif
