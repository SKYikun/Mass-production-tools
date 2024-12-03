#include <ui.h>
#include <display_manager.h>
static int DefaultOnDraw(struct Button *ptButton,PDispBuf ptDispBuff)
{
	/*绘制按键*/
	DrawRegion(&ptButton->tRegion,BUTTON_DEFAULT_COLOR);

	/*居中写文字*/
	DrawTextInRegionCentural(ptButton->name,&ptButton->tRegion,BUTTON_TEXT_COLOR);

	/*把buff刷到硬件上*/
	DisplayRegionFlush(&ptButton->tRegion,ptDispBuff);
	return  0;

}
static int DefaultOnpressed(struct Button *ptButton,PDispBuf ptDispBuff,PInputEvent ptInputEvent)
{
	unsigned int dwcolor=BUTTON_DEFAULT_COLOR;
	ptButton->status=!ptButton->status;
	if(ptButton->status)
		dwcolor=BUTTON_PRESSED_COLOR;

	/*绘制按键*/
	DrawRegion(&ptButton->tRegion,dwcolor);

	/*居中写文字*/
	DrawTextInRegionCentural(ptButton->name,&ptButton->tRegion,BUTTON_TEXT_COLOR);

	/*把buff刷到硬件上*/
	DisplayRegionFlush(&ptButton->tRegion,ptDispBuff);
	return 0;

}

void InitButton(PButton ptButton,char* name,PRegion ptRegion,ONDRAW_FUNC OnDraw,ONPRESSED_FUNC OnPressed)
{
	ptButton->name=name;
	ptButton->status=0;
	if(ptRegion)
		ptButton->tRegion=*ptRegion;
	ptButton->OnDraw=OnDraw ? OnDraw:DefaultOnDraw;
	ptButton->OnPressed=OnPressed ?OnPressed:DefaultOnpressed;
}

