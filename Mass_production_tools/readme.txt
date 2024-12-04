
功能：
实现了一个电字产品量产化工具。能够通过检测功能是否正常改变对应按钮的颜色。对于网络传输的设备信息需要使用一个udp的客户端来向本机发送信息。

注意事项：需要交叉编译tslib库和freetype库
　etc目录里有配置文件
　misc目录里有udp客户端源码和led shell脚本 运行时要指定的字体文件














Linux开发下c语言编程规范
1. 命名规范
1.1 常量命名规范
1.2 变量命名规范
1.3 结构体命名规范
1.4 枚举类型命名规范
1.5 函数命名规范
2. 预处理规范
2.1 #include包含规范
2.2 #ifdef/#ifndef规范
2.3 #define配置参数规范
2.4 #if规范
3.注释规范
3.1单行代码注释
3.2 函数注释
3.3文件注释（总注释）



















1. 命名规范
1.1 常量命名规范
   	对于常量，通常使用全大写+下划线分隔单词的命名规范，DEFAULT_FONT_SIZE
1.2 变量命名规范
 一律使用匈牙利命名法，如iSocketServer表示int类型变量

	类型            前缀	说明
	bool            b        （boll类型）
　　char  			c		（char 类型）
	int   			i       (int 类型)  
	float 			f		（float 类型）				
	unsigned int    dw  	（双字表示32位）
	array 			a    	（数组类型）
	struct          t     	（结构体类型）
	
1.3 结构体命名规范
模板：
　　typedef struct Node{
　　	char* Name;
　　	void* (*Run)(void*);
　　	struct Node* ptNext;
　　} Node,*PNode;
	结构体统一使用typedef重命名，struct后跟匈牙利命名的类型名，后面的	类型名称和struct后的名称一致，结构体指针类型名为结构体类型名前+P;	结构体内的变量，函数指针都使用匈牙利命名；
1.4 枚举类型命名规范
模板：
　　typedef enum Status{
　　	OFF=0,
　　	ON,
　　} Status，*PStatus；
　　枚举内的成员都为常量，应该使用全大写命名
1.5 函数命名规范
模板：
	Void RegisterNode(PNode ptNode);
	使用匈牙利命名法。
	对于函数形参，遵循匈牙利。如ptNode表示Node类型的结构体指针
	
	在.c文件中定义函数时内部链接函数加static，外部链接函数强制加extern
	.h文件中声明函数时加extern，不同的功能分模块放，如先提供底层接	口，	再提供应用层接口


2. 预处理规范
2.1 #include包含规范
　
2.2 #ifdef/#ifndef规范
　　对于.h库文件开头用于防止重定义的符号常量，使用全大写模块名+‘_H‘命	名，如：__DISP_MANAGER__H
2.3. #define配置参数规范
	采用全大写命名，单词与单词之间使用下划线隔开，如：
	POWER_SAVE_MODE_ENABLE
2.4#if规范
	示例：
	{
		#if DEBUG
		#else
		#endif
	}
	使用{}表示一个代码块
　　
注意常用const！























3.注释规范

3.1 单行代码注释
	使用//单行注释

3.2函数注释
注释模板：
/**
 * @brief 函数概述
 * @param 形参名 形参描述(功能，范围，单位等) 
 * @return 返回值描述
 * @note 备注信息
 * @warning：需要函数使用者注意的信息，比如：功能未经完全验证
 * */
　　以/**开头，以**/结尾，中间每一行都以*开头。@brief后填写函数功能的概述；@param 后跟形参名，形参名后描述参数功能或用途，注意描述参数输入范围和单位（如有）；@return后填写返回值描述；@note后填写该函数的注意事项，例如OLEDDriver中绘图的原点等。@warning后填写函数注意事项。
　　对于定义在.c文件中断static静态函数，注释应写在.c文件中。
3.3 文件注释（总注释）
　　在库文件(.h)开头写上注释，注释使用Doxygen格式，必须包含以下几项：
/**
 * @file 哪个文件
 * @brief 库/文件概述
 * @detail 库/文件详细描述
 * @author 作者
 * @date 文件最后更改日期
 * @version 文件版本，如：v1.0
 * @note 使用注意事项
 * @warning：需要库使用者注意的信息，比如：功能未经完全验证，已知Bug
 * @par 历史版本
		Vxxx创建于xxx，更新内容：xxxx
 * */


