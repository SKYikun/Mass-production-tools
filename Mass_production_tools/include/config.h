#ifndef _CONFIG_H
#define _CONFIG_H
#define MAX_ITEM 100
#define Default_CONF_FILE  "/etc/test_gui/gui.conf"

typedef struct ItemCfg{
	int index;
	char name[100];
	int bCanBeTouched;
	char command[100];
}ItemCfg,*PItemCfg;

int GetItemConfigCount(void);
PItemCfg GetItemConfigByIndex(int index);
PItemCfg GetItemConfigByName(char* name);
int ParseConfigFile(void);

#endif
