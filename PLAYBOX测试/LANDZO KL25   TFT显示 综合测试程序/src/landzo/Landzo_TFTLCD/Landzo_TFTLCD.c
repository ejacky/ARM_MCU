#include "Landzo_TFTLCD.h"
#include "font.h" 
#include "KL2x_delay.h"	 
					 
//画笔颜色,背景颜色
uint16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
uint16 DeviceCode;	 


#if LCD_FAST_IO==1 //快速IO
//写寄存器函数
void LCD_RESET(void)
{ 
	LCD_RST_SET;//高电平  
 	delay_ms(1);
	LCD_RST_CLR;//低电平
	delay_ms(20);
	LCD_RST_SET;//高电平  
	delay_ms(20);
}
#else//正常IO
//写寄存器函数
void LCD_RESET(void)
{ 
	LCD_RST=1;  
 	delay_ms(10);
	LCD_RST=0;//低电平
	delay_ms(50);
	LCD_RST=1;//高电平  
	delay_ms(120);
} 	
#endif

#if LCD_FAST_IO==1 //快速IO
//写寄存器函数
void LCD_WR_REG(uint8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
#else//正常IO
//写寄存器函数
void LCD_WR_REG(uint8 data)
{ 
	LCD_RS=0;//写地址  
 	LCD_CS=0; 
	DATAOUT(data); 
	LCD_WR=0; 
	LCD_WR=1; 
 	LCD_CS=1;   
} 	
#endif	
//写8位数据函数
//用宏定义,提高速度.
#if LCD_FAST_IO==1 //快速IO
void LCD_WR_DATA(uint8 data)
{
  LCD_RS_SET;
  LCD_CS_CLR;
  DATAOUT(data);
  LCD_WR_CLR;
  LCD_WR_SET;
  LCD_CS_SET;
} 
#else//正常IO
void LCD_WR_DATA(uint8 data)
{
  LCD_RS=1;
  LCD_CS=0;
  DATAOUT(data);
  LCD_WR=0;
  LCD_WR=1;
  LCD_CS=1;
}	
#endif	
void LCD_WR_TDATA(uint16 data)
{
  LCD_RS=1;
  LCD_CS=0;
  DATAOUT(data & 0xFF00);
  LCD_WR=0;
  LCD_WR=1;
  DATAOUT(data << 8);
  LCD_WR=0;
  LCD_WR=1;
  LCD_CS=1;
}
//写寄存器
void LCD_WriteReg(uint8 LCD_Reg, uint8 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
//读寄存器
uint16 LCD_ReadReg(uint8 LCD_Reg)
{										   
    uint16 t;
    LCD_WR_REG(LCD_Reg);  //写入要读的寄存器号  
//    GPIOC->CRL=0X88888888; //PC0-7  上拉输入
//    GPIOC->CRH=0X88888888; //PC8-15 上拉输入
//    GPIOC->ODR=0XFFFF;    //全部输出高
         //设置数据输出IO为推挽输出；
    gpio_init(PTD0,GPI_UP,1);	
    gpio_init(PTD1,GPI_UP,1);
    gpio_init(PTD2,GPI_UP,1);
    gpio_init(PTD3,GPI_UP,1);
    gpio_init(PTD4,GPI_UP,1);
    gpio_init(PTD5,GPI_UP,1);
    gpio_init(PTD6,GPI_UP,1);
    gpio_init(PTD7,GPI_UP,1);
    
    LCD_RS=1;
    LCD_CS=0;
    //读取数据(读寄存器时,并不需要读2次)
    LCD_RD=0;					   
    LCD_RD=1;
    t = DATAIN;  
    LCD_CS=1;   
//    GPIOC->CRL=0X33333333; //PC0-7  上拉输出
//    GPIOC->CRH=0X33333333; //PC8-15 上拉输出
//    GPIOC->ODR=0XFFFF;    //全部输出高
    gpio_init(PTD0,GPO,1);	
    gpio_init(PTD1,GPO,1);
    gpio_init(PTD2,GPO,1);
    gpio_init(PTD3,GPO,1);
    gpio_init(PTD4,GPO,1);
    gpio_init(PTD5,GPO,1);
    gpio_init(PTD6,GPO,1);
    gpio_init(PTD7,GPO,1);
    return t; 
}  

//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x2c);
}	 
//LCD写GRAM
void LCD_WriteRAM(uint16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16 LCD_BGR2RGB(uint16 c)
{
	uint16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 
//读取个某点的颜色值	 
//x:0~239
//y:0~319
//返回值:此点的颜色
/*uint16 LCD_ReadPoint(uint16 x,uint16 y)
{
	uint16 t;	
	if(x>=LCD_W||y>=LCD_H)return 0;//超过了范围,直接返回		   
	LCD_SetCursor(x,y);
	LCD_WR_REG(R34);       //选择GRAM地址 
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出高
#if LCD_FAST_IO==1 //快速IO
	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读GRAM时,需要读2次)
	LCD_RD_CLR;					   
	LCD_RD_SET;
	delay_us(2);//FOR 9320,延时2us					   
	//dummy READ
	LCD_RD_CLR;					   
	delay_us(2);//FOR 8989,延时2us					   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET;
#else
	LCD_RS=1;
	LCD_CS=0;
	//读取数据(读GRAM时,需要读2次)
	LCD_RD=0;					   
 	LCD_RD=1;
 	//dummy READ
	LCD_RD=0;					   
 	LCD_RD=1;
	t=DATAIN;  
	LCD_CS=1;  
#endif 
	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高  
	if(DeviceCode==0X4535||DeviceCode==0X4531||DeviceCode==0X8989||DeviceCode==0XB505)return t;//这几种IC直接返回颜色值
	else return LCD_BGR2RGB(t);
}
*/
//LCD开启显示
void LCD_DisplayOn(void)
{					   
	LCD_WriteReg(R7, 0x0173); //26万色显示开启
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(R7, 0x0);//关闭显示 
}   
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
 void LCD_SetCursor(uint16 Xpos, uint16 Ypos)
{	
    LCD_WR_REG(0x2b);
    LCD_WR_DATA(Xpos >> 8);
    LCD_WR_DATA(Xpos & 0xff);
    LCD_WR_DATA(Xpos >> 8);
    LCD_WR_DATA(Xpos & 0xff);

    LCD_WR_REG(0x2a);
    LCD_WR_DATA(Ypos >> 8);
    LCD_WR_DATA(Ypos & 0xff);			 
    LCD_WR_DATA(Ypos >> 8);
    LCD_WR_DATA(Ypos & 0xff);
}  
//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16 x,uint16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WR_REG(0x2c);//开始写入GRAM
        LCD_WR_DATA(POINT_COLOR>>8);  
        LCD_WR_DATA(POINT_COLOR&0xff); 
} 

#define Delay delay_ms	 
//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void LCD_Init(void)
{ 
        gpio_init(PTB11,GPO,1); //设置RW 推挽输出，初始输出高电平 
        gpio_init(PTB8,GPO,1);  //设置RS
        gpio_init(PTB10,GPO,1); //设置RD
        gpio_init(PTB9,GPO,1);  //设置CS
        gpio_init(PTA19,GPO,1); //设置BK背光控制
               
        //设置数据输出IO为推挽输出；
        gpio_init(PTD0,GPO,1);	
        gpio_init(PTD1,GPO,1);
        gpio_init(PTD2,GPO,1);
        gpio_init(PTD3,GPO,1);
        gpio_init(PTD4,GPO,1);
        gpio_init(PTD5,GPO,1);
        gpio_init(PTD6,GPO,1);
        gpio_init(PTD7,GPO,1);
        //TFT复位
        gpio_init(PTE31,GPO,1);   //设置RST并拉低进行复位

	LCD_RESET();//复位
	  					 
	delay_ms(15); // delay 50 ms 
	LCD_WriteReg(0x00,0x01);
	delay_ms(15); // delay 50 ms  
        DeviceCode = LCD_ReadReg(0x04);   
//	printf(" LCD ID:%x\n",DeviceCode);   
 	
	//************* Start Initial Sequence **********//   ILI9341+IVO2.8
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xD9); 
	LCD_WR_DATA(0X30); 
	 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x78); 
	 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x21);   //VRH[5:0] 
	 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x12);   //SAP[2:0];BT[3:0] 
	 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x32); 
	LCD_WR_DATA(0x3C); 
	 
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XC1); 
	 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x88);   // 0x08(从左到右,从上到下) -> 0x88(从左到右,从下到上)
	 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x18); 
	 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	
	 
	 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x20); 
	LCD_WR_DATA(0x1E); 
	LCD_WR_DATA(0x09); 
	LCD_WR_DATA(0x12); 
	LCD_WR_DATA(0x0B); 
	LCD_WR_DATA(0x50); 
	LCD_WR_DATA(0XBA); 
	LCD_WR_DATA(0x44); 
	LCD_WR_DATA(0x09); 
	LCD_WR_DATA(0x14); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x23); 
	LCD_WR_DATA(0x21); 
	LCD_WR_DATA(0x00); 
	 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x19); 
	LCD_WR_DATA(0x19); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x12); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x02); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x25); 
	LCD_WR_DATA(0x2D); 
	LCD_WR_DATA(0x0F); 
	 
	LCD_WR_REG(0x11);    //Exit Sleep 
	Delay(120); 
	LCD_WR_REG(0x29);    //Display on 

	LCD_LED=0;//点亮背光	 
//	LCD_Clear(WHITE);
}  		  

//清屏函数
//Color:要清屏的填充色
void LCD_Clear(uint16 Color)
{
/*	uint32 index=0; 
        LCD_SetCursor(0,0);
        LCD_WriteRAM_Prepare();     //开始写入GRAM		 	  
	for(index=0;index<76800;index++)
	{
          LCD_WR_DATA(Color>>8);  
          LCD_WR_DATA(Color&0xff); 
	}
*/ 
      uint16 index,index1;
      for(index=0;index<320;index++)
       { 
         for(index1=0;index1<240;index1++)
         { 
            LCD_SetCursor(index,index1);//设置光标位置 
            LCD_WR_REG(0x2c);//开始写入GRAM
            LCD_WR_DATA(Color>>8);  
            LCD_WR_DATA(Color&0xff); 
         }
       }
} 
//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(uint16 xsta,uint16 ysta,uint16 xend,uint16 yend,uint16 color)
{          
	uint16 i,j;
	uint16 xlen=0;
#if USE_HORIZONTAL==1
	xlen=yend-ysta+1;	   
	for(i=xsta;i<=xend;i++)
	{
			  
		for(j=ysta;j<yend;j++)
                {
	 	  LCD_SetCursor(i,j);      //设置光标位置 
	          LCD_WR_REG(0x2c);//开始写入GRAM
                  LCD_WR_DATA(color>>8);  
                  LCD_WR_DATA(color&0xff);  
                }
	}
#else
	xlen=xend-xsta+1;	   
	for(i=ysta;i<=yend;i++)
	{
	 	for(j=ysta;j<yend;j++)
                {
	 	  LCD_SetCursor(i,j);      //设置光标位置 
	          LCD_WR_REG(0x2c);//开始写入GRAM
                  LCD_WR_DATA(color>>8);  
                  LCD_WR_DATA(color&0xff);  
                }
	}
#endif						  	    
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	uint16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(uint16 x0,uint16 y0,uint8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//在指定位置显示一个字符
//x:0~234
//y:0~308
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
//在指定位置显示一个字符
//x:0~234
//y:0~308
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16 x,uint16 y,uint8 num,uint8 size,uint8 mode)
{  
#if USE_HORIZONTAL==1
#define MAX_CHAR_POSX 320
#define MAX_CHAR_POSY 240 
#else     
#define MAX_CHAR_POSX 240
#define MAX_CHAR_POSY 320
#endif 
  uint8 temp;
  uint8 pos,t;
  uint16 x0=x;
  uint16 colortemp=POINT_COLOR;      
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
    //设置窗口		   
    num=num-' ';//得到偏移后的值
    if(!mode) //非叠加方式
    {
        for(pos=0;pos<size;pos++)
        {
          if(size==12)temp=asc2_1206[num][pos];//调用1206字体
          else temp=asc2_1608[num][pos];		 //调用1608字体
          for(t=0;t<size/2;t++)
          {                 
            if(temp&0x01)POINT_COLOR=colortemp;
            else POINT_COLOR=BACK_COLOR;
            LCD_DrawPoint(x,y);	
            temp>>=1; 
            x++;
          }
          x=x0;
          y++;
        }	
    }else//叠加方式
    {
        for(pos=0;pos<size;pos++)
        {
          if(size==12)temp=asc2_1206[num][pos];//调用1206字体
          else temp=asc2_1608[num][pos];		 //调用1608字体
          for(t=0;t<size/2;t++)
          {                 
            if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
            temp>>=1; 
          }
        }
    }
    POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n函数
uint32 mypow(uint8 m,uint8 n)
{
	uint32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);	 
void LCD_ShowNum(uint16 x,uint16 y,uint32 num,uint8 len,uint8 size)
{         	
	uint8 t,temp;
	uint8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示2个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~99);	 
void LCD_Show2Num(uint16 x,uint16 y,uint16 num,uint8 len,uint8 size,uint8 mode)
{         	
	uint8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(uint16 x,uint16 y,const uint8 *p)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear(WHITE);}
        LCD_ShowChar(x,y,*p,16,1);
        x+=8;
        p++;
    }  
}

