#include <stdio.h>
#include <config.h>
#include <string.h>
static ItemCfg g_ItemCfgs[MAX_ITEM];
static int g_ItemCfgCount=0;

int ParseConfigFile(void)
{
	FILE *fp;
	char buf[100];
	char* p=buf;
	/*1.打开文件*/
	fp=fopen(Default_CONF_FILE, "r");
	if(!fp){
		printf("can not open %s\n",Default_CONF_FILE);
	}
	/*2.处理每一行*/
	while(fgets(buf,100,fp)){
		/*2.1read each line*/
		buf[99]='\0';
		/*吃掉开头空格和tab*/
		p=buf;
		while(*p==' '||*p=='\t')
			p++;

		/*2.3忽略注释*/
		if(*p=='#')
			continue;

		/*2.4解析数据*/
		g_ItemCfgs[g_ItemCfgCount].command[0]='\0';
		g_ItemCfgs[g_ItemCfgCount].index=g_ItemCfgCount;
		sscanf(p, "%s %d %s",g_ItemCfgs[g_ItemCfgCount].name\
						   ,&g_ItemCfgs[g_ItemCfgCount].bCanBeTouched\
						   ,g_ItemCfgs[g_ItemCfgCount].command);
		g_ItemCfgCount++;
	}
	return 0;
}

int GetItemConfigCount(void)
{
	return g_ItemCfgCount;

}

PItemCfg GetItemConfigByIndex(int index)
{
	if(index<g_ItemCfgCount)
		return &g_ItemCfgs[index];
	else
		return NULL;
}

PItemCfg GetItemConfigByName(char* name)
{
	int i;
	for(i=0;i<g_ItemCfgCount;i++){
		if(strcmp(name,g_ItemCfgs[i].name)==0){
			return &g_ItemCfgs[i];
		}
	}
	return NULL;

}
