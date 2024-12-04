/**
 * @file common.h
 * @brief 实现了一些公共类型的声明
 * @detail Region lcd坐标系区域     RegionCartesian 笛卡尔坐标系区域
 * @author junjieWu
 * @date 2024.12.4
 * @version v1.0.0
 * @note 使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		v1.0.0创建于2024.12.4，更新内容：创建该文件
 * */
#ifndef __COMMON_H
#define __COMMON_H

#ifndef NULL
#define NULL (void *)0
#endif


typedef struct Region{
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iRows;
}Region,*PRegion;

typedef struct RegionCartesian{
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iRows;
}RegionCartesian,*PRegionCartesian;


#endif
