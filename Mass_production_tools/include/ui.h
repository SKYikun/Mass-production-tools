#ifndef __UI_H
#define __UI_H


#include <common.h>
#include <display_manager.h>
#include <input_manager.h>

#define BUTTON_DEFAULT_COLOR 0X00FF0000
#define BUTTON_PRESSED_COLOR 0X0000FF00
#define BUTTON_PERCENT_COLOR 0X000000FF
#define BUTTON_TEXT_COLOR 0X00000000

struct Button;
typedef int (*ONDRAW_FUNC)(struct Button *ptButton,PDispBuf ptDispBuf);
typedef int (*ONPRESSED_FUNC)(struct Button *ptButton,PDispBuf ptDispBuf,PInputEvent ptInputEvent);

typedef struct Button{
	char* name;
	int status;
	Region tRegion;
	ONDRAW_FUNC OnDraw;
	ONPRESSED_FUNC OnPressed;
}Button,*PButton;


void InitButton(PButton ptButton,char* name,PRegion ptRegion,ONDRAW_FUNC OnDraw,ONPRESSED_FUNC OnPressed);




#endif
