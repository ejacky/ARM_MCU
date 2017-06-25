#ifndef __LCD_H
#define __LCD_H		
#include "include.h"

/////////////////////////////////////用户配置区///////////////////////////////////	 
//以下2个宏定义，定义屏幕的显示方式及IO速度
#define USE_HORIZONTAL  1	//定义是否使用横屏 		0,不使用.1,使用.
#define LCD_FAST_IO     0 	//定义是否使用快速IO	0,不实用.1,使用
//////////////////////////////////////////////////////////////////////////////////	 


 
//TFTLCD部分外要调用的函数		   
extern uint16  POINT_COLOR;//默认红色    
extern uint16  BACK_COLOR; //背景颜色.默认为白色
//定义LCD的尺寸
#if USE_HORIZONTAL==1	//使用横屏
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif
////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//如果使用快速IO，则定义下句，如果不使用，则去掉即可！
//使用快速IO，刷屏速率可以达到28帧每秒！
//普通IO，只能14帧每秒！   
//-----------------LCD端口定义---------------- 
#define	LCD_LED PTA19_OUT //LCD背光    	
#define	LCD_CS	PTB9_OUT  //片选端口  	     
#define	LCD_RS	PTB8_OUT  //数据/命令           
#define	LCD_WR	PTB11_OUT  //写数据	
#define	LCD_RD	PTB10_OUT  //读数据	
#define LCD_RST PTE31_OUT


//PC0~15,作为数据线
#define DATAOUT(x) GPIO_PDOR_REG(PTD_BASE_PTR)=(x&0xff); //数据输出
#define DATAIN     GPIO_PDIR_REG(PTD_BASE_PTR);   //数据输入
//////////////////////////////////////////////////////////////////////
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 	         0XFFE0
#define GBLUE	         0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
extern uint16 BACK_COLOR, POINT_COLOR ;  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16 Color);	
void LCD_SetCursor(uint16 Xpos, uint16 Ypos);
void LCD_DrawPoint(uint16 x,uint16 y);//画点
//uint16  LCD_ReadPoint(uint16 x,uint16 y); //读点
void Draw_Circle(uint16 x0,uint16 y0,uint8 r);
void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2);		   
void LCD_Fill(uint16 xsta,uint16 ysta,uint16 xend,uint16 yend,uint16 color);
void LCD_ShowChar(uint16 x,uint16 y,uint8 num,uint8 size,uint8 mode);//显示一个字符
void LCD_ShowNum(uint16 x,uint16 y,u32 num,uint8 len,uint8 size);  //显示一个数字
void LCD_Show2Num(uint16 x,uint16 y,uint16 num,uint8 len,uint8 size,uint8 mode);//显示2个数字
void LCD_ShowString(uint16 x,uint16 y,const uint8 *p);		 //显示一个字符串,16字体
									    
void LCD_WriteReg(uint8 LCD_Reg, uint8 LCD_RegValue);
uint16 LCD_ReadReg(uint8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16 RGB_Code);
//uint16 LCD_ReadRAM(void);		   
uint16 LCD_BGR2RGB(uint16 c);
void LCD_WR_DATA(uint8 data);
void LCD_WR_TDATA(uint16 data);
																			 
//9320/9325 LCD寄存器  
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5							  		 
#endif  
	 
	 



