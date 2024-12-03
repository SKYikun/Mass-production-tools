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


static int DeviceInit(void)
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

static int DeviceExit(void)
{
	munmap(fb_base,screen_size);
	close(fd_fb);
	return 0;
}

static int FbGetBuffer(PDispBuf ptDispBuf)
{
	ptDispBuf->iXres=var.xres;
	ptDispBuf->iYres=var.yres;
	ptDispBuf->iBpp=var.bits_per_pixel;
	ptDispBuf->buf=(char *)fb_base;
	return 0;
}

static int FbFlushRegion(PRegion pRegion,PDispBuf ptDispBuf)
{
	return 0;
}


static DispOpr g_tFrameBufferOpr={
	.name="fb",
	.DeviceInit=DeviceInit,
	.DeviceExit=DeviceExit,
	.GetBuffer=FbGetBuffer,
	.FlushRegion=FbFlushRegion,
};

void FrameBufferRegister(void)
{
	RegisterDisplayer(&g_tFrameBufferOpr);
}
