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
extern  uint8_t  LPT_INT_count ; //LPT 脉冲计数器
uint16 AD_VALUE;
uint8 AD_buff[3];
/***********************参数定义***************************/

/************************主函数****************************/
void main(void)
{
  float Temp;
  DisableInterrupts;   //禁止总中断
  uart_init (UART1, 115200); //串口初始化
  pit_init_ms(PIT0, 5); //周期中断定时器PIT0 定时5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //设置中断优先级 1
  setIRQadrr(INT_PIT,PIT_IRQHandler); //设置PIT0定时器中断地址函数 
  gpio_init (PTC12, GPO, 0);//RGB_EN1 GPIO引脚初始化配置 推挽输出 输出为低电平 RGB_EN1 使能
  gpio_init(PTB19,GPO,1);  //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init(PTB18,GPO,1);  //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init(PTC9,GPO,1);  //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  adc_init(ADC0_SE15);     //温度传感器 ADC0_SE15(PTC1)初始化通道
  
  EnableInterrupts;   //开启总中断 
  while(1)
  {
   if(TIME0flag_20ms == 1) // 20ms进入一次
   {
      TIME0flag_20ms = 0;
      AD_VALUE = ad_once(ADC0_SE15,ADC_8bit);  //采集AD
      AD_buff[0] = AD_VALUE/100; // 取百位
      AD_buff[1] = AD_VALUE%100/10; // 取十位
      AD_buff[2] = AD_VALUE%10; // 取个位
      uart_sendStr(UART1,"Temperature_AD_VALUE:"); // 发送字符串
      for(uint8 i = 0;i < 3;i ++) uart_putchar(UART1, AD_buff[i] + 0x30); // 发送采集到的AD值 
      uart_sendStr(UART1, "\r\n"); // 回车换行
     
   }
   if(TIME0flag_100ms == 1) // 100ms进入一次
   { 
     TIME0flag_100ms = 0;
      PTB19_OUT = ~PTB19_OUT; //RBG_B输出状态取反，呈现亮灭状态 程序在运行
      PTB18_OUT = ~PTB18_OUT; //RBG_G输出状态取反，呈现亮灭状态 程序在运行
      PTC9_OUT = ~PTC9_OUT;   //RBG_R输出状态取反，呈现亮灭状态 程序在运行

   }
  }
}