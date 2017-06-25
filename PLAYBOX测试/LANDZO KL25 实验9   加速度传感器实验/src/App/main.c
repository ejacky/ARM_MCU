/***********************************************************
 * 文件名       ：main.c
 * 说明         ：用户应用程序
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/


#include "include.h" // Include.h 包含用户自定义的头文件 


/***********************参数定义***************************/
extern uint8 TIME0flag_5ms   ;
extern uint8 TIME0flag_10ms  ;
extern uint8 TIME0flag_20ms  ;
extern uint8 TIME0flag_50ms  ;
extern uint8 TIME0flag_80ms  ;
extern uint8 TIME0flag_100ms ;
extern uint8 TIME0flag_1s;
extern  uint8_t  LPT_INT_count ; //LPT 脉冲计数器
uint16 ReCRC=0;
uint16 AngleAcceleArry[3] ;
uint8  SendData1[10]={0};
/***********************参数定义***************************/

/************************主函数****************************/
void main(void)
{
  DisableInterrupts; //禁止总中断 
  uart_init (UART1, 115200);
  gpio_init(PTC12,GPO,0); // RBG_EN1(PTC12) GPIO引脚初始化配置 推挽输出 输出为低电平 使能 RBG_EN1
  gpio_init(PTB19,GPO,1); //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init(PTB18,GPO,1); //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init(PTC9,GPO,1);  //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  pit_init_ms(PIT0, 5);  //初始化PIT0，定时时间为： 5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //设置中断优先级 8
  setIRQadrr(INT_PIT,PIT_IRQHandler); //设置PIT0定时器中断地址函数 
  while(!LandzoMMA8451_init());  // 加速度角度传感器初始化
  OLED_Init();  //OLED初始化
  EnableInterrupts ;
  while(1)
  {
    if(TIME0flag_5ms == 1) //5ms 执行一次
    {
      TIME0flag_5ms = 0;
      AngleAcceleration_Digital_Data (AngleAcceleArry);   //采集时间 900us
    }
    
    if(TIME0flag_10ms == 1) //20ms 执行一次
    {
      TIME0flag_10ms = 0;
      OLED_Print_Str(0,0,"蓝宙电子/LANDZO");           //汉字字符串显示
      OLED_Print_Str(0,2,"XG:");  //汉字字符串显示 
      OLED_Print_Str(0,4,"YG:");  //汉字字符串显示
      OLED_Print_Str(0,6,"ZG:");  //汉字字符串显示 
      OLED_Print_Num(40,2,AngleAcceleArry[0]);  //汉字字符串显示       
      OLED_Print_Num(40,4,AngleAcceleArry[2]);  //汉字字符串显示 
      OLED_Print_Num(40,6,AngleAcceleArry[1]);  //汉字字符串显示 
      delay_ms(50);
      OLED_Fill(0x00);//黑屏 
      for(int i=0;i<3;i++)
      {        
        SendData1[i*2+0]=(uint8)((uint16)AngleAcceleArry[i]);       //数据低位
        SendData1[i*2+1]= (uint8)((uint16)(AngleAcceleArry[i]>>8)); //数据高位        
      }
      
      ReCRC =  (uint8)AngleAcceleArry[0]+(uint8)(AngleAcceleArry[0]>>8) +(uint8)AngleAcceleArry[1]\
        +(uint8)(AngleAcceleArry[1]>>8) + (uint8)AngleAcceleArry[2]+ (uint8)(AngleAcceleArry[2]>>8) ; 
      SendData1[8]=(uint8)((uint16)ReCRC);
      uart_sendN(UART1,SendData1,9);
    }
    if(TIME0flag_1s == 1) 
    {
      TIME0flag_1s = 0;
      PTB19_OUT = ~PTB19_OUT; //RBG_B输出状态取反，呈现亮灭状态 程序在运行
      PTB18_OUT = ~PTB18_OUT; //RBG_G输出状态取反，呈现亮灭状态 程序在运行
      PTC9_OUT = ~PTC9_OUT;   //RBG_R输出状态取反，呈现亮灭状态 程序在运行
    }
  }
}