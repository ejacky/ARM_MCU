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
/***********************参数定义***************************/

//#define Key_Interrupt     // 定义按键中断 检测按键时 两者屏蔽一个保留一个
#define Key_INPUT       // 定义按键输入 检测按键时 两者屏蔽一个保留一个

#if defined(Key_INPUT)  // 使用外部输入检测按键
#define UP_Key      PTA1_INT  
#define LEFT_Key    PTA12_INT 
#define DOWN_Key    PTA5_INT   
#define RIGHT_Key   PTA13_INT 

#define A_Key   PTA14_INT
#define B_Key   PTA16_INT
#define C_Key   PTA17_INT
#define D_Key   PTA15_INT    
#endif
/************************主函数****************************/
void main(void)
{
  DisableInterrupts; //禁止总中断 
  uart_init (UART1, 115200); //UART1 波特率为115200
  gpio_init (PTB0, GPO, 1); //LED GPIO引脚初始化配置 推挽输出 输出为高电平
#if defined(Key_Interrupt)  //使用外部中断检测按键
  gpio_Interrupt_init(PTA1, GPI_UP, FALLING);  //按键 PTA1上拉 下降沿触发
  gpio_Interrupt_init(PTA12, GPI_UP, FALLING); //按键 PTA12上拉 下降沿触发
  gpio_Interrupt_init(PTA5, GPI_UP, FALLING);  //按键 PTA5上拉 下降沿触发
  gpio_Interrupt_init(PTA13, GPI_UP, FALLING); //按键 PTA13上拉 下降沿触发
  gpio_Interrupt_init(PTA14, GPI_UP, FALLING); //按键 PTA14上拉 下降沿触发
  gpio_Interrupt_init(PTA16, GPI_UP, FALLING); //按键 PTA16上拉 下降沿触发
  gpio_Interrupt_init(PTA17, GPI_UP, FALLING); //按键 PTA17上拉 下降沿触发
  gpio_Interrupt_init(PTA15, GPI_UP, FALLING); //按键 PTA15上拉 下降沿触发
  set_irq_priority((INT_PORTA + INC_IRQ),1); //设置中断优先级 1
  setIRQadrr(INT_PORTA,PORTA_IRQHandler);    //设置PORTA中断地址函数 
#else
  gpio_init (PTA1, GPI_UP, 1);  //按键 PTA1 GPIO引脚初始化配置 上拉输入
  gpio_init (PTA12, GPI_UP, 1); //按键 PTA12 GPIO引脚初始化配置 上拉输入
  gpio_init (PTA5, GPI_UP, 1);  //按键 PTA5 GPIO引脚初始化配置 上拉输入
  gpio_init (PTA13, GPI_UP, 1); //按键 PTA13 GPIO引脚初始化配置 上拉输入
  gpio_init (PTA14, GPI_UP, 1); //按键 PTA14 GPIO引脚初始化配置 上拉输入
  gpio_init (PTA16, GPI_UP, 1); //按键 PTA16 GPIO引脚初始化配置 上拉输入
  gpio_init (PTA17, GPI_UP, 1); //按键 PTA17 GPIO引脚初始化配置 上拉输入
  gpio_init (PTA15, GPI_UP, 1); //按键 PTA15 GPIO引脚初始化配置 上拉输入
#endif  

  EnableInterrupts ; //开启总中断
  while(1)
  {
#if defined(Key_INPUT)  // 使用外部输入检测按键
   if(LEFT_Key == 0) 
   {
     delay_ms(5); //消抖
     if(LEFT_Key == 0) uart_sendStr(UART1,"LEFT_Key Down!\r\n"); // 发送 LEFT_Key Down 提示此按键被按下
     while(!LEFT_Key); //等待按键松开
   }
   else if(RIGHT_Key == 0) 
   {
     delay_ms(5); //消抖
     if(RIGHT_Key == 0) uart_sendStr(UART1,"RIGHT_Key Down!\r\n"); // 发送 RIGHT_Key Down 提示此按键被按下
     while(!RIGHT_Key); //等待按键松开
   }
   else if(UP_Key == 0) 
   {
     delay_ms(5); //消抖
     if(UP_Key == 0) uart_sendStr(UART1,"UP_Key Down!\r\n"); // 发送 UP_Key Down 提示此按键被按下
     while(!UP_Key); //等待按键松开
   }
   else if(DOWN_Key == 0) 
   {
     delay_ms(5); //消抖
     if(DOWN_Key == 0) uart_sendStr(UART1,"DOWN_Key Down!\r\n"); // 发送 DOWN_Key Down 提示此按键被按下
     while(!DOWN_Key); //等待按键松开
   }
   else if(A_Key == 0) 
   {
     delay_ms(5); //消抖
     if(A_Key == 0) uart_sendStr(UART1,"A_Key Down!\r\n"); // 发送 A_Key Down 提示此按键被按下
     while(!A_Key); //等待按键松开
   }
   else if(B_Key == 0) 
   {
     delay_ms(5); //消抖
     if(B_Key == 0) uart_sendStr(UART1,"B_Key Down!\r\n"); // 发送 B_Key Down 提示此按键被按下
     while(!B_Key); //等待按键松开
   }
   else if(C_Key == 0) 
   {
     delay_ms(5); //消抖
     if(C_Key == 0) uart_sendStr(UART1,"C_Key Down!\r\n"); // 发送 C_Key Down 提示此按键被按下
     while(!C_Key); //等待按键松开
   }
   else if(D_Key == 0) 
   {
     delay_ms(5); //消抖
     if(D_Key == 0) uart_sendStr(UART1,"D_Key Down!\r\n"); // 发送 D_Key Down 提示此按键被按下
     while(!D_Key); //等待按键松开
   }
#endif  
  }
}