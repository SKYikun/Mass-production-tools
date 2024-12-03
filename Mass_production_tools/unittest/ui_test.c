#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <display_manager.h>
#include <stdlib.h>
#include <font_manager.h>
#include <ui.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	int err;
	PDispBuf ptDisBuf;
	Button tButton;
	Region tRegion;

	if(argc!=2){
		printf("Usage: %s <fontfile>\n",argv[0]);
		return -1;
	}

	DisplayInit();
	SlectDefaultDisplay("fb");
	InitDefaultDisplay();
	ptDisBuf=GetDisplayBuf();


	FontsRegister();
	err=SelectAndInitFont("freetype",argv[1]);
	if(err){
		printf("SelectAndInitFont err\n");
		return -1;
	}
	tRegion.iLeftUpX =200;
	tRegion.iLeftUpY =200;
	tRegion.iWidth   =200;
	tRegion.iRows    =100;

	InitButton(&tButton,"Test",&tRegion,NULL,NULL);
	tButton.OnDraw(&tButton,ptDisBuf);
	while(1){
		tButton.OnPressed(&tButton,ptDisBuf,NULL);
		sleep(2);
	}
	return 0;
}


