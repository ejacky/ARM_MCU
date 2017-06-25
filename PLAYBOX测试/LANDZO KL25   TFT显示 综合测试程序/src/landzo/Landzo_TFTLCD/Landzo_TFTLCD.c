#include "Landzo_TFTLCD.h"
#include "font.h" 
#include "KL2x_delay.h"	 
					 
//������ɫ,������ɫ
uint16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
uint16 DeviceCode;	 


#if LCD_FAST_IO==1 //����IO
//д�Ĵ�������
void LCD_RESET(void)
{ 
	LCD_RST_SET;//�ߵ�ƽ  
 	delay_ms(1);
	LCD_RST_CLR;//�͵�ƽ
	delay_ms(20);
	LCD_RST_SET;//�ߵ�ƽ  
	delay_ms(20);
}
#else//����IO
//д�Ĵ�������
void LCD_RESET(void)
{ 
	LCD_RST=1;  
 	delay_ms(10);
	LCD_RST=0;//�͵�ƽ
	delay_ms(50);
	LCD_RST=1;//�ߵ�ƽ  
	delay_ms(120);
} 	
#endif

#if LCD_FAST_IO==1 //����IO
//д�Ĵ�������
void LCD_WR_REG(uint8 data)
{ 
	LCD_RS_CLR;//д��ַ  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
#else//����IO
//д�Ĵ�������
void LCD_WR_REG(uint8 data)
{ 
	LCD_RS=0;//д��ַ  
 	LCD_CS=0; 
	DATAOUT(data); 
	LCD_WR=0; 
	LCD_WR=1; 
 	LCD_CS=1;   
} 	
#endif	
//д8λ���ݺ���
//�ú궨��,����ٶ�.
#if LCD_FAST_IO==1 //����IO
void LCD_WR_DATA(uint8 data)
{
  LCD_RS_SET;
  LCD_CS_CLR;
  DATAOUT(data);
  LCD_WR_CLR;
  LCD_WR_SET;
  LCD_CS_SET;
} 
#else//����IO
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
//д�Ĵ���
void LCD_WriteReg(uint8 LCD_Reg, uint8 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
//���Ĵ���
uint16 LCD_ReadReg(uint8 LCD_Reg)
{										   
    uint16 t;
    LCD_WR_REG(LCD_Reg);  //д��Ҫ���ļĴ�����  
//    GPIOC->CRL=0X88888888; //PC0-7  ��������
//    GPIOC->CRH=0X88888888; //PC8-15 ��������
//    GPIOC->ODR=0XFFFF;    //ȫ�������
         //�����������IOΪ���������
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
    //��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
    LCD_RD=0;					   
    LCD_RD=1;
    t = DATAIN;  
    LCD_CS=1;   
//    GPIOC->CRL=0X33333333; //PC0-7  �������
//    GPIOC->CRH=0X33333333; //PC8-15 �������
//    GPIOC->ODR=0XFFFF;    //ȫ�������
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

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x2c);
}	 
//LCDдGRAM
void LCD_WriteRAM(uint16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
uint16 LCD_BGR2RGB(uint16 c)
{
	uint16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 
//��ȡ��ĳ�����ɫֵ	 
//x:0~239
//y:0~319
//����ֵ:�˵����ɫ
/*uint16 LCD_ReadPoint(uint16 x,uint16 y)
{
	uint16 t;	
	if(x>=LCD_W||y>=LCD_H)return 0;//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);
	LCD_WR_REG(R34);       //ѡ��GRAM��ַ 
	GPIOB->CRL=0X88888888; //PB0-7  ��������
	GPIOB->CRH=0X88888888; //PB8-15 ��������
	GPIOB->ODR=0XFFFF;     //ȫ�������
#if LCD_FAST_IO==1 //����IO
	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(��GRAMʱ,��Ҫ��2��)
	LCD_RD_CLR;					   
	LCD_RD_SET;
	delay_us(2);//FOR 9320,��ʱ2us					   
	//dummy READ
	LCD_RD_CLR;					   
	delay_us(2);//FOR 8989,��ʱ2us					   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET;
#else
	LCD_RS=1;
	LCD_CS=0;
	//��ȡ����(��GRAMʱ,��Ҫ��2��)
	LCD_RD=0;					   
 	LCD_RD=1;
 	//dummy READ
	LCD_RD=0;					   
 	LCD_RD=1;
	t=DATAIN;  
	LCD_CS=1;  
#endif 
	GPIOB->CRL=0X33333333; //PB0-7  �������
	GPIOB->CRH=0X33333333; //PB8-15 �������
	GPIOB->ODR=0XFFFF;    //ȫ�������  
	if(DeviceCode==0X4535||DeviceCode==0X4531||DeviceCode==0X8989||DeviceCode==0XB505)return t;//�⼸��ICֱ�ӷ�����ɫֵ
	else return LCD_BGR2RGB(t);
}
*/
//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	LCD_WriteReg(R7, 0x0173); //26��ɫ��ʾ����
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(R7, 0x0);//�ر���ʾ 
}   
//���ù��λ��
//Xpos:������
//Ypos:������
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
//����
//x:0~239
//y:0~319
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(uint16 x,uint16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_WR_REG(0x2c);//��ʼд��GRAM
        LCD_WR_DATA(POINT_COLOR>>8);  
        LCD_WR_DATA(POINT_COLOR&0xff); 
} 

#define Delay delay_ms	 
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void LCD_Init(void)
{ 
        gpio_init(PTB11,GPO,1); //����RW �����������ʼ����ߵ�ƽ 
        gpio_init(PTB8,GPO,1);  //����RS
        gpio_init(PTB10,GPO,1); //����RD
        gpio_init(PTB9,GPO,1);  //����CS
        gpio_init(PTA19,GPO,1); //����BK�������
               
        //�����������IOΪ���������
        gpio_init(PTD0,GPO,1);	
        gpio_init(PTD1,GPO,1);
        gpio_init(PTD2,GPO,1);
        gpio_init(PTD3,GPO,1);
        gpio_init(PTD4,GPO,1);
        gpio_init(PTD5,GPO,1);
        gpio_init(PTD6,GPO,1);
        gpio_init(PTD7,GPO,1);
        //TFT��λ
        gpio_init(PTE31,GPO,1);   //����RST�����ͽ��и�λ

	LCD_RESET();//��λ
	  					 
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
	LCD_WR_DATA(0x88);   // 0x08(������,���ϵ���) -> 0x88(������,���µ���)
	 
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

	LCD_LED=0;//��������	 
//	LCD_Clear(WHITE);
}  		  

//��������
//Color:Ҫ���������ɫ
void LCD_Clear(uint16 Color)
{
/*	uint32 index=0; 
        LCD_SetCursor(0,0);
        LCD_WriteRAM_Prepare();     //��ʼд��GRAM		 	  
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
            LCD_SetCursor(index,index1);//���ù��λ�� 
            LCD_WR_REG(0x2c);//��ʼд��GRAM
            LCD_WR_DATA(Color>>8);  
            LCD_WR_DATA(Color&0xff); 
         }
       }
} 
//��ָ�����������ָ����ɫ
//�����С:
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
	 	  LCD_SetCursor(i,j);      //���ù��λ�� 
	          LCD_WR_REG(0x2c);//��ʼд��GRAM
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
	 	  LCD_SetCursor(i,j);      //���ù��λ�� 
	          LCD_WR_REG(0x2c);//��ʼд��GRAM
                  LCD_WR_DATA(color>>8);  
                  LCD_WR_DATA(color&0xff);  
                }
	}
#endif						  	    
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	uint16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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
//������
void LCD_DrawRectangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(uint16 x0,uint16 y0,uint8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
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
    //���ô���		   
    num=num-' ';//�õ�ƫ�ƺ��ֵ
    if(!mode) //�ǵ��ӷ�ʽ
    {
        for(pos=0;pos<size;pos++)
        {
          if(size==12)temp=asc2_1206[num][pos];//����1206����
          else temp=asc2_1608[num][pos];		 //����1608����
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
    }else//���ӷ�ʽ
    {
        for(pos=0;pos<size;pos++)
        {
          if(size==12)temp=asc2_1206[num][pos];//����1206����
          else temp=asc2_1608[num][pos];		 //����1608����
          for(t=0;t<size/2;t++)
          {                 
            if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
            temp>>=1; 
          }
        }
    }
    POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
uint32 mypow(uint8 m,uint8 n)
{
	uint32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);	 
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
//��ʾ2������
//x,y:�������
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~99);	 
void LCD_Show2Num(uint16 x,uint16 y,uint16 num,uint8 len,uint8 size,uint8 mode)
{         	
	uint8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
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

