/****************************************************************
 * @file netinput.c
 * @brief 网络输入设备实现层 基于udp
 * @detail 实现向管理层注册网络输入设备
 * @author JunjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 服务器端口 8888
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par v1.0.0 2024.12.4 创建该文件

*****************************************************************/
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


/**
 * @brief 向管理层提供，获取网络设备输入事件 阻塞
 * @param PInputEvent ptInputEvent 要写入的输入事件地址
 * @return 0表示成功 -1表示失败
 * @note 使用recvfrom
 * @warning：无
 * */
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



/**
 * @brief 初始化网络输入事件的套接字接口
 * @param void
 * @return 0表示成功 -1表示失败
 * @note 初始化socket bind服务器的ip和端口
 * @warning：无
 * */
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


/**
 * @brief 关闭网络输入事件的套接字接口
 * @param void
 * @return 0
 * @note close socket
 * @warning：无
 * */
static int NetInputDeviceExit(void)
{
	close(g_iSocketServer);
	return 0;
}


//实例化管理层构造的输入设备结构体对象为网络输入设备
static InputDevice g_tNetInputDev={
	.name="netinput",
	.GetInputEvent=NetInputGetInputEvent,
	.DeviceInit=NetInputDeviceInit,
	.DeviceExit=NetInputDeviceExit,
};



/**
 * @brief 向管理层暴露的注册接口
 * @param void
 * @return 0
 * @note
 * @warning：无
 * */
extern void NetInputDevRegister(void)
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














