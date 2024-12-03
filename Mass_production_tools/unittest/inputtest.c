#include <input_manager.h>
#include <stdio.h>
int main(int argc,char** argv)
{
	int iRet;
	InputEvent tInputEvent;
	InputInit();
	InputDeviceInit();
	while(1){
		iRet=GetInputEvent(&tInputEvent);
		if(iRet==-1){
			printf("GetInputEvent err\n");
		}
		else{
			if(tInputEvent.iType==INPUT_TYPE_TOUCH){
				printf("type=%d\n",tInputEvent.iType);
				printf("iX=%d\n",tInputEvent.iX);
				printf("iY=%d\n",tInputEvent.iY);
				printf("iPresure=%d\n",tInputEvent.iPresure);
			}
			else if(tInputEvent.iType==INPUT_TYPE_NET){
				printf("type=%d\n",tInputEvent.iType);
				printf("str=%s\n",tInputEvent.str);
			}
		}
	}
	return 0;
}

