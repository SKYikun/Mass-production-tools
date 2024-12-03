#include <font_manager.h>
#include <page_manager.h>
#include <config.h>
#include <stdio.h>
#include <ui.h>
#include <math.h>
#include <input_manager.h>
#include <string.h>
#include <stdlib.h>


static Button g_tButtons[MAX_ITEM];
static int g_tButtonCnt;
int  iFontSize;

#define X_GAP 5
#define Y_GAP 5

#define DEBUG 1

static float GetFontSizeForAllButton(void);

static int MainOnpressed(struct Button *ptButton,PDispBuf ptDispBuff,PInputEvent ptInputEvent)
{
	unsigned int dwcolor=BUTTON_DEFAULT_COLOR;
	char name[100];
	char status[100];
	char *strButton;
	char *command_status[3]={"err","ok","percent"};
	int command_status_index=0;
	char command[1000];
	char strburn[100]="burn";
	PItemCfg ptItemCfg;

	strButton=ptButton->name;

	/*1对于触摸屏事件*/
	if(ptInputEvent->iType==INPUT_TYPE_TOUCH&&ptInputEvent->iPresure>0){
		/*1.1分辨能否被点击*/
		if(GetItemConfigByName(ptButton->name)->bCanBeTouched==0)
			return -1;

		/*1.2修改颜色*/
		ptButton->status=!ptButton->status;
		if(ptButton->status){
			dwcolor=BUTTON_PRESSED_COLOR;
			command_status_index=1;
		}

	}
	else if(ptInputEvent->iType==INPUT_TYPE_NET){
		/*2.对于网络事件*/
		/*2.1根据传入字符串修改颜色*/

		sscanf(ptInputEvent->str,"%s %s",name,status);
		if(strcmp(status,"ok")==0){
			dwcolor=BUTTON_PRESSED_COLOR;
			command_status_index=1;
		}
		else if(strcmp(status,"err")==0){
			dwcolor=BUTTON_DEFAULT_COLOR;
				command_status_index=0;
		}
		else if(status[0]>='0'&&status[0]<='9'){
			dwcolor=BUTTON_PERCENT_COLOR;
			command_status_index=2;
			strcpy(strburn+4,status);
			strburn[strlen(strburn)]='%';
			strburn[strlen(strburn)]='\0';
			strButton=status;
			strcpy(status,strburn);
		}
		else
			return -1;
	}
	else
		return -1;


	/*绘制按键*/
	DrawRegion(&ptButton->tRegion,dwcolor);

	/*居中写文字*/
	DrawTextInRegionCentural(strButton,&ptButton->tRegion,BUTTON_TEXT_COLOR);

	/*把buff刷到硬件上*/
	DisplayRegionFlush(&ptButton->tRegion,ptDispBuff);


	/*执行command*/
	ptItemCfg=GetItemConfigByName(ptButton->name);
	if(ptItemCfg->command[0]!='\0'){
		sprintf(command,"%s %s",ptItemCfg->command,command_status[command_status_index]);
		system(command);
	}
	return 0;

}


static void GenerateButtons(void)
{
	int width,height;
	int n_per_line;
	int rows,row;
	int col;
	int Xres,Yres;
	int start_x,start_y;
	PButton ptButton;
	PDispBuf ptDispBuf;
	int i=0;


	/*算出单个按钮的widt和height*/
	g_tButtonCnt=GetItemConfigCount();
	ptDispBuf=GetDisplayBuf();
	Xres=ptDispBuf->iXres;
	Yres=ptDispBuf->iYres;
	width=sqrt(1.0/0.618*Xres * Yres / g_tButtonCnt);
	n_per_line=Xres / width+1;
	width=Xres / n_per_line;
	height=width*0.618;



	/*居中显示：计算每个按钮的region*/
	start_x=(Xres-width*n_per_line)/2;
	rows=g_tButtonCnt/n_per_line;
	if(rows * n_per_line<g_tButtonCnt)
		rows++;
	start_y=(Yres-rows*height)/2;



	/*计算每个按钮的region*/
	for(row=0;row<rows&&i<g_tButtonCnt;row++){
		for(col=0;col<n_per_line&&i<g_tButtonCnt;col++){
			ptButton=&g_tButtons[i];
			ptButton->tRegion.iLeftUpX=start_x+col*width;
			ptButton->tRegion.iLeftUpY=start_y+row*height;
			ptButton->tRegion.iWidth=width-X_GAP;
			ptButton->tRegion.iRows=height-Y_GAP;
			/*InitButton*/

			InitButton(ptButton,GetItemConfigByIndex(i)->name,NULL,NULL,MainOnpressed);
			i++;
		}
	}
	printf("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	iFontSize=GetFontSizeForAllButton();
	printf("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	SetFontSize(iFontSize);
	printf("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	/*OnDraw*/
	for(i=0;i<g_tButtonCnt;i++){
		printf("i=%d\n",i);
		g_tButtons[i].OnDraw(&g_tButtons[i],ptDispBuf);

	}

}
static int isTouchPointInRegion(int iX,int iY,PRegion ptRegion)
{
	if(iX<ptRegion->iLeftUpX || iX>=ptRegion->iLeftUpX+ptRegion->iWidth)
		return 0;
	if(iY<ptRegion->iLeftUpY || iY>=ptRegion->iLeftUpY+ptRegion->iRows)
		return 0;
	return 1;
}

static PButton GetButtonByName(char* name)
{
	int i;
	for(i=0;i<g_tButtonCnt;i++){
		if(strcmp(g_tButtons[i].name,name)==0){
			return &g_tButtons[i];
		}
	}
	return NULL;
}

static PButton GetButtonByInputEvent(PInputEvent ptInputEvent)
{
	int i;
	char name[100];
	if(ptInputEvent->iType==INPUT_TYPE_TOUCH){
		for(i=0;i<g_tButtonCnt;i++)
			if(isTouchPointInRegion(ptInputEvent->iX,ptInputEvent->iY,&g_tButtons[i].tRegion))
				return &g_tButtons[i];
	}
	else if(ptInputEvent->iType==INPUT_TYPE_NET){
		sscanf(ptInputEvent->str,"%s",name);
		return GetButtonByName(name);
	}
	else
		return NULL;
	return NULL;
}

static float GetFontSizeForAllButton(void)
{
	int i;
	int iMaxLen=-1;
	int len;
	int MaxIndex=0;
	RegionCartesian tRegionCartesian;
	float k,kx,ky;
	/*1.找出name最长的Button*/
	for(i=0;i<g_tButtonCnt;i++){
		len=strlen(g_tButtons[i].name);
		if(len>iMaxLen){
			iMaxLen=len;
			MaxIndex=i;
		}
	}
	/*2.以font_size=100为例，算出他的外框*/
	SetFontSize(100);
	GetStringRegionCar(g_tButtons[MaxIndex].name,&tRegionCartesian);
	/*3.把文字的外框缩放为Button的外框*/
	kx=(float)g_tButtons[MaxIndex].tRegion.iWidth/tRegionCartesian.iWidth;
	ky=(float)g_tButtons[MaxIndex].tRegion.iRows/tRegionCartesian.iRows;
	k=kx<ky? kx:ky;

	/*反算出font_size*/
	return k*100*0.8;
}
static int MainPageRun(void* pParam)
{
	int err;
	InputEvent tInputEvent;
	PButton ptButton;
	PDispBuf ptDispBuf;
	/*读取配置文件*/
	err=ParseConfigFile();
	if(err){
		return err;
	}

	/*根据配置文件生成按钮,界面*/
	GenerateButtons();
	while(1){
		/*读取输入事件*/
		err=GetInputEvent(&tInputEvent);
		if(err)
			continue;
		/*根据输入事件找到按钮*/
		ptButton=GetButtonByInputEvent(&tInputEvent);
		if(!ptButton)
			continue;
		/*调用按钮的OnPressed函数*/
		ptDispBuf=GetDisplayBuf();
		ptButton->OnPressed(ptButton,ptDispBuf,&tInputEvent);
	}
	return 0;
}

static PageAction g_tMainPage={
	.name="main",
	.Run =MainPageRun,
};

void RegisterMainPage(void)
{
	RegisterPage(&g_tMainPage);
}



