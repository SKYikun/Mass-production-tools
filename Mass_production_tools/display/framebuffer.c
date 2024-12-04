/*************************************************************************
 * @file framebuffer.c
 * @brief 显示设备底层
 * @detail 实现frambuffer显示设备向管理层的注册
 * @author JunjieWu
 * @date 2024.12.4
 * @version  v1.0.0
 * @note  内部的函数和变量均为static文件内链接（除注册函数）只能通过向管理层注册的结构体访问
 * @warning：
 * @par 历史版本
		V1.0.0创建于2024.12.4 更新内容：创建该文件
**************************************************************************/
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <display_manager.h>


static int fd_fb;
static struct fb_var_screeninfo var;	/* Current var */
static int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

/**
 * @brief 向管理层提供，frambuffer的硬件初始化
 * @param  void
 * @return int  成功返回0 失败返回-1
 * @note
 * @warning：无
 * */
static int FbDeviceInit(void)
{
	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var\n");
		return -1;
	}

	line_width  = var.xres * var.bits_per_pixel / 8;
	pixel_width = var.bits_per_pixel / 8;
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
	fb_base = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}
	memset(fb_base,0x00ffffff,screen_size);
	return 0;
}





/**
 * @brief 向管理层提供，frambuffer的硬件初退出
 * @param  void
 * @return int  返回0
 * @note
 * @warning：无
 * */
static int FbDeviceExit(void)
{
	munmap(fb_base,screen_size);
	close(fd_fb);
	return 0;
}



/**
 * @brief  向管理层提供，帧缓存地址和分辨率等参数
 * @param  PDispBuf ptDispBuf 需要被写入的显示资源对象
 * @return int 返回0
 * @note   需要先调用FbDeviceInit把fb_base存到该文件变量
 * @warning：无
 * */
static int FbGetBuffer(PDispBuf ptDispBuf)
{
	ptDispBuf->iXres=var.xres;
	ptDispBuf->iYres=var.yres;
	ptDispBuf->iBpp=var.bits_per_pixel;
	ptDispBuf->buf=(char *)fb_base;
	return 0;
}




/**
 * @brief GetBuffer直接提供了显存地址，暂时未实现网页显示，因此该函数没用
 * @param  void
 * @return void
 * @note
 * @warning：无
 * */
static int FbFlushRegion(PRegion pRegion,PDispBuf ptDispBuf)
{
	return 0;
}




/*实例化管理层提供的显示设备操作对象结构*/
static DispOpr g_tFrameBufferOpr={
	.name="fb",
	.DeviceInit=FbDeviceInit,
	.DeviceExit=FbDeviceExit,
	.GetBuffer=FbGetBuffer,
	.FlushRegion=FbFlushRegion,
};





/**
 * @brief 向管理层提供，注册实例化的结构体的接口
 * @param  void
 * @return void
 * @note   外部链接暴露注册接口
 * @warning：无
 * */
extern void FrameBufferRegister(void)
{
	RegisterDisplayer(&g_tFrameBufferOpr);
}
