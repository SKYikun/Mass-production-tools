/**
 * @file main.c
 * @brief 实现了main入口函数
 * @detail 初始化所有设备，进入main_page
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		v1.0.0创建于2024.12.4，更新内容：创建该文件
 * */
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

#define DEBUG 0
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


