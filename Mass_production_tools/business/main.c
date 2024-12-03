#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <display_manager.h>
#include <input_manager.h>
#include <font_manager.h>
#include <page_manager.h>
#include <ui.h>

#define DEBUG 1
int main(int argc, char **argv)
{
	PPageAction ptPageAction;
	int err;
	if(argc!=2){
		printf("Usage: %s <fontfile>\n",argv[0]);
		return -1;
	}



	/*初始化显示系统*/
	DisplaySystemRegister();
	SlectDefaultDisplay("fb");
	InitDefaultDisplay();


	/*初始化输入系统*/
	InputSystemRegister();
	InputDeviceInit();




	/*初始化字体系统*/
	FontSystemRegister();
	err=SelectAndInitFont("freetype",argv[1]);
	if(err){
		printf("SelectAndInitFont err\n");
		return -1;
	}

	/*初始化页面系统*/
	PageSystemRegister();


	/*运行业务系统的主页面*/
	ptPageAction=Page("main");
	ptPageAction->Run("NULL");
	return 0;
}


