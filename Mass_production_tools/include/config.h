/**
 * @file config.h
 * @brief 实现了配置函数一行数据的抽象
 * @detail 线性表的数据结构管理配置文件读出的一行数据对象
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 需要把工程中的配置文件拷贝到单板的"/etc/test_gui/gui.conf" 路径
 * @warning：
 * @par 历史版本
		v1.0.0创建于2024.12.4，更新内容：创建该文件
 * */
#ifndef _CONFIG_H
#define _CONFIG_H
#define MAX_ITEM 100
#define Default_CONF_FILE  "/etc/test_gui/gui.conf"    //默认配置文件路径



//配置函数一行数据的抽象
typedef struct ItemCfg{
	int index;
	char name[100];
	int bCanBeTouched;
	char command[100];
}ItemCfg,*PItemCfg;

extern int GetItemConfigCount(void);
extern PItemCfg GetItemConfigByIndex(int index);
extern PItemCfg GetItemConfigByName(char* name);
extern int ParseConfigFile(void);

#endif
