#include <page_manager.h>
#include <common.h>
#include <string.h>
#include <stdio.h>
static PPageAction g_ptPages;

void RegisterPage(PPageAction ptPageAction)
{
	ptPageAction->ptNext=g_ptPages;
	g_ptPages=ptPageAction;
}

PPageAction Page(char* name)
{
	PPageAction ptTmp=g_ptPages;
	while(ptTmp){
		if(strcmp(name,ptTmp->name)==0){
			return ptTmp;
		}
	}
	return NULL;
}

void PageSystemRegister(void)
{
 	extern void RegisterMainPage(void);
 	RegisterMainPage();
}
