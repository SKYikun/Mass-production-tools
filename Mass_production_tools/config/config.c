/**
 * @file config.c
 * @brief 实现了配置文件的读取，解析，线性表的方式管理数据
 * @detail
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		v1.0.0创建于2024.12.4，更新内容：创建该文件
 * */
#include <stdio.h>
#include <config.h>
#include <string.h>
static ItemCfg g_ItemCfgs[MAX_ITEM];
static int g_ItemCfgCount=0;

/**
 * @brief 解析配置文件
 * @param void
 * @return 0
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern int ParseConfigFile(void)
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



/**
 * @brief 获取线性表长度
 * @param void
 * @return int g_ItemCfgCount线性表长度
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern int GetItemConfigCount(void)
{
	return g_ItemCfgCount;

}



/**
 * @brief 通过索引取得保存在该文件全局线性表里的对象
 * @param int index 配置项索引
 * @return ItemCfg 配置项指针
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern PItemCfg GetItemConfigByIndex(int index)
{
	if(index<g_ItemCfgCount)
		return &g_ItemCfgs[index];
	else
		return NULL;
}



/**
 * @brief 通过名字取得保存在该文件全局线性表里的对象
 * @param char* name 配置项名字属性
 * @return PItemCfg 配置项指针
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
extern PItemCfg GetItemConfigByName(char* name)
{
	int i;
	for(i=0;i<g_ItemCfgCount;i++){
		if(strcmp(name,g_ItemCfgs[i].name)==0){
			return &g_ItemCfgs[i];
		}
	}
	return NULL;
}
