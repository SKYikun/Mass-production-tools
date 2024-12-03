#include <input_manager.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>



#define SERVER_PORT 8888
static int g_iSocketServer;



static int NetInputGetInputEvent(PInputEvent ptInputEvent)
{
	struct timeval tv;
	struct sockaddr_in tSocketClientAddr;
	int iRecvLen;
	char aRecvBuf[1000];

	unsigned int iAddrLen = sizeof(struct sockaddr);
	iRecvLen = recvfrom(g_iSocketServer, aRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
	if (iRecvLen > 0){
		aRecvBuf[iRecvLen] = '\0';
		//printf("Get Msg From %s : %s\n", inet_ntoa(tSocketClientAddr.sin_addr), aRecvBuf);
		strncpy(ptInputEvent->str,aRecvBuf,1000);
		gettimeofday(&tv, NULL);
		ptInputEvent->iType=INPUT_TYPE_NET;
		ptInputEvent->str[999]='\0';
		ptInputEvent->tTime=tv;
		return 0;
	}
	else
		return -1;

}


static int NetInputDeviceInit(void)
{
	struct sockaddr_in tSocketServerAddr;
	int iRet;

	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == g_iSocketServer){
		printf("socket error!\n");
		return -1;
	}

	tSocketServerAddr.sin_family	  = AF_INET;
	tSocketServerAddr.sin_port		  = htons(SERVER_PORT);  /* host to net, short */
	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);

	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet){
		printf("bind error!\n");
		return -1;
	}
	return 0;

}

static int NetInputDeviceExit(void)
{
	close(g_iSocketServer);
	return 0;
}


static InputDevice g_tNetInputDev={
	.name="netinput",
	.GetInputEvent=NetInputGetInputEvent,
	.DeviceInit=NetInputDeviceInit,
	.DeviceExit=NetInputDeviceExit,
};

void NetInputDevRegister(void)
{
	RegisterInputDev(&g_tNetInputDev);
}

#if 0
int main(int argc,char** argv)
{
	int iRet;
	InputEvent tInputEvent;
	g_tNetInputDev.DeviceInit();

	while(1){
		iRet=g_tNetInputDev.GetInputEvent(&tInputEvent);
		if(iRet){
			//printf("GetInputEvent err\n");
		}
		else{
			printf("type=%d\n",tInputEvent.iType);
			printf("str=%s\n",tInputEvent.str);
		}
	}
	return 0;
}

#endif














