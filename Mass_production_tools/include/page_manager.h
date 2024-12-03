#ifndef __PAGE_MANAGER_H
#define __PAGE_MANAGER_H

typedef struct PageAction{
	char* name;
	int (*Run)(void* pParam);
	struct PageAction* ptNext;
}PageAction,*PPageAction;

void RegisterPage(PPageAction ptPageAction);



void PageSystemRegister(void);
PPageAction Page(char* name);
#endif
