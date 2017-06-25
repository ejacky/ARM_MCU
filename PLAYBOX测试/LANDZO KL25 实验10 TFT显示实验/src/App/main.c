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
extern uint8 TIME0flag_1s ;
extern  uint8_t  LPT_INT_count ; //LPT 脉冲计数器
/***********************参数定义***************************/


/************************主函数****************************/
void main(void)
{
  uint8 ii = 0;
  DisableInterrupts; //禁止总中断 
  
  TPM_PWM_init(TPM0, CH4, 0,50);//PTC8   驱动蜂鸣器   
  gpio_init (PTC12, GPO, 0);//RGB_EN1 GPIO引脚初始化配置 推挽输出 输出为低电平 RGB_EN1 使能
  gpio_init(PTB19,GPO,1);  //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init(PTB18,GPO,1);  //RBG(PTB18) GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init(PTC9,GPO,1);  //RBG(PTC9) GPIO引脚初始化配置 推挽输出 输出为高电平
  
//  ILI9341GPIO_Config();  
//  ILI9341_Initial();
//
//  LCD_SetCursor(30,30);
//
//  ILI9341_DisplayEnlargeWord(WHITE ,DARKBLUE ,zimo);
  LCD_Init();
//  LCD_Clear(GREEN);  
  EnableInterrupts ;
  while(1)
  {
/**********************20ms程序执行代码段********************/ 
   switch(ii)
    {
      case 0: LCD_Clear(RED);
      break;
      case 1: LCD_Clear(BLUE);
      break;
      case 2: LCD_Clear(YELLOW);
      break;
      case 3: LCD_Clear(GREEN);
      break;
      case 4: LCD_Clear(MAGENTA);
      break;
      case 5: LCD_Clear(BROWN);
      break;
      case 6: LCD_Clear(BRRED);
      break;
      case 7: LCD_Clear(GRAY);
      break;
      case 8: LCD_Clear(LIGHTBLUE);
      break;
    }
    LCD_ShowString(100,10,"Landzo");	
    LCD_ShowString(100,30,"PLAY-BOX TFTLCD TEST");	
    LCD_ShowString(100,50,"www.landzo.cn");  
    PTB19_OUT = ~PTB19_OUT; //RBG_B输出状态取反，呈现亮灭状态 程序在运行
    PTB18_OUT = ~PTB18_OUT; //RBG_G输出状态取反，呈现亮灭状态 程序在运行
    PTC9_OUT = ~PTC9_OUT;   //RBG_R输出状态取反，呈现亮灭状态 程序在运行
    delay_ms(500);
    ii++;
    if(ii > 8) ii = 0;
  }
}

