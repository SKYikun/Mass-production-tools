/****************************************************************
 * @file freetype.c
 * @brief freetype字体的实现
 * @detail 向上层注册freetype_opr
 * @author JunjieWu
 * @date 2024.12.4
 * @version  v1.0.0
 * @note使用注意事项  需要链接freetype库
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		 v1.0.0创建于2024.12.4，更新内容：创建该文件
 * **************************************************************/
#include <font_manager.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <ft2build.h>
#include <common.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Face 	g_tFace;
static int g_iDefaultFontSize=12;

/**
 * @brief 向管理层提供，获取笛卡尔坐标系下字符串的的外框
 * @param char *str字符串,PRegionCartesian ptRegionCartesian 要写入的外框地址
 * @return int 0表示成功 -1表示失败
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
static int FreetypeGetSringRegionCar(char *str,PRegionCartesian ptRegionCartesian)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = g_tFace->glyph;

    /* 初始化 */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* 指定原点为(0, 0) */
    pen.x = 0;
    pen.y = 0;

    /* 计算每个字符的bounding box */
    /* 先translate, 再load char, 就可以得到它的外框了 */
    for (i = 0; i < strlen(str); i++)
    {
        /* 转换：transformation */
        FT_Set_Transform(g_tFace, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(g_tFace, str[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(g_tFace->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }

        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* 更新外框 */
        if ( glyph_bbox.xMin < bbox.xMin )
            bbox.xMin = glyph_bbox.xMin;

        if ( glyph_bbox.yMin < bbox.yMin )
            bbox.yMin = glyph_bbox.yMin;

        if ( glyph_bbox.xMax > bbox.xMax )
            bbox.xMax = glyph_bbox.xMax;

        if ( glyph_bbox.yMax > bbox.yMax )
            bbox.yMax = glyph_bbox.yMax;

        /* 计算下一个字符的原点: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    /* return string bbox */
    //*abbox = bbox;
    ptRegionCartesian->iLeftUpX=bbox.xMin;
    ptRegionCartesian->iLeftUpY=bbox.yMax;
    ptRegionCartesian->iWidth=bbox.xMax-bbox.xMin+1;
    ptRegionCartesian->iRows=bbox.yMax-bbox.yMin+1;
	return 0;
}




/**
 * @brief 向管理层提供，freetype字体初始化
 * @param char* aFineName 字库文件名
 * @return int 0表示成功 -1表示失败
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
static int FreeTypeFontInit(char* aFineName)
{
	FT_Library    library;
	int err;
	err = FT_Init_FreeType( &library );              /* initialize library */
	if(err){
		printf("FT_Init_FreeType err\n");
		return -1;
	}

    err = FT_New_Face( library,aFineName, 0, &g_tFace ); /* create face object */
	if(err){
		printf("FT_New_Face err\n");
		return -1;
	}

	FT_Set_Pixel_Sizes(g_tFace, g_iDefaultFontSize, 0);
	return 0;

}






/**
 * @brief 向管理层提供，设置字体的大小
 * @param int iFontSize 字体大小
 * @return int 0表示成功
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
static int FreeTypeSetFontSize(int iFontSize)
{
	FT_Set_Pixel_Sizes(g_tFace, iFontSize, 0);
	return 0;
}





/**
 * @brief 向管理层提供，根据传入的unicode值，获得字符的位图
 * @param unsigned int dwCode unicode值, PFontBitMap ptFontBitMap 要写入到位图地址
 * @return int 0表示成功 -1表示失败
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
static int FreeTypeGetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	int err;
	FT_Vector pen;                             //原点
    FT_GlyphSlot slot =g_tFace->glyph;

	pen.x =ptFontBitMap->iCurOriginX * 64; /* 单位: 1/64像素 */
    pen.y =ptFontBitMap->iCurOriginY * 64; /* 单位: 1/64像素 */

	FT_Set_Transform(g_tFace, 0, &pen);


	/* 加载位图: load glyph image into the slot (erase previous one) */
	err = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER);
	if (err){
		printf("FT_Load_Char error\n");
		return -1;
	}
	ptFontBitMap->pucBuffer=slot->bitmap.buffer;

	ptFontBitMap->tRegion.iLeftUpX=slot->bitmap_left;
	ptFontBitMap->tRegion.iLeftUpY=ptFontBitMap->iCurOriginY*2-slot->bitmap_top;
	ptFontBitMap->tRegion.iRows=slot->bitmap.rows;
	ptFontBitMap->tRegion.iWidth=slot->bitmap.width;
	ptFontBitMap->iNextOriginX=ptFontBitMap->iCurOriginX+slot->advance.x/64;
	ptFontBitMap->iNextOriginY=ptFontBitMap->iCurOriginY;

	return 0;
}




//实例化管理层抽象出的字体操作结构体对象
static FontOpr g_tFontOperation={
	.name="freetype",
	.FontInit=FreeTypeFontInit,
	.SetFontSize=FreeTypeSetFontSize,
	.GetFontBitMap=FreeTypeGetFontBitMap,
	.GetSringRegionCar=FreetypeGetSringRegionCar,
};




/**
 * @brief 向管理层暴露的字体注册接口
 * @param void
 * @return void
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
void FreetypeRegister(void)
{
	RegisterFont(&g_tFontOperation);
}


