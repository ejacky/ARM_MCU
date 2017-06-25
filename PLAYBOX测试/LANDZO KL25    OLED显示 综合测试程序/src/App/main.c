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
uint16 AD_VALUE;
uint16 SUN_VALUE;
uint16 TEMP_VALUE;
int16 AngleAcceleArry[3] ;
uint8 AD_buff[3];
/***********************参数定义***************************/


/************************主函数****************************/
void main(void)
{
  uint8 Flag=0;
  DisableInterrupts; //禁止总中断 
  pit_init_ms(PIT0, 40);    //初始化PIT0，定时时间为： 5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //设置中断优先级 1
  setIRQadrr(INT_PIT,PIT_IRQHandler);    //设置PIT0定时器中断地址函数 

  TPM_PWM_init(TPM0,CH5,8000,50);  //RBG(PTC9) GPIO引脚初始化配置 推挽输出 输出为高电平
  TPM_PWM_init(TPM2,CH0,8000,20);;  //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  TPM_PWM_init(TPM2,CH1,8000,10);;  //RBG(PTB19) GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init(PTC12,GPO,0);  // RBG_EN1(PTC12) GPIO引脚初始化配置 推挽输出 输出为低电平 使能 RBG_EN1
  gpio_init(PTC13,GPO,0);  // RBG_EN1(PTC12) GPIO引脚初始化配置 推挽输出 输出为低电平 使能 RBG_EN2
  gpio_init(PTC7,GPO,0);  // RBG_EN1(PTC12) GPIO引脚初始化配置 推挽输出 输出为低电平 使能 RBG_EN3 
  gpio_init(PTC6,GPO,1);  // RBG_EN1(PTC12) GPIO引脚初始化配置 推挽输出 输出为低电平 使能 RBG_EN4

  adc_init(ADC0_SE14);     //ADC0_SE14(PTC0)初始化通道 电位器
  adc_init(ADC0_SE11);     //ADC0_SE11(PTC2)初始化通道 阳光传感器
  adc_init(ADC0_SE15);     //温度传感器 ADC0_SE15(PTC1)初始化通道
  uart_init (UART1, 115200); //UART1 波特率为115200
  
  TPM_PWM_init(TPM0, CH4, 10,0);//PTC8   驱动蜂鸣器
  
  gpio_init(PTB0,GPO,1);  // RBG_EN1(PTC12) GPIO引脚初始化配置 推挽输出 输出为低电平 使能 RBG_EN4
  
  OLED_Init();  //OLED初始化 
  OLED_Print_Str(0,0,"蓝宙电子landzo.cn"); //汉字 字符串显示
  Draw_Landzo();//显示蓝宙LOGO
  delay_ms(100);//延时2000ms
  while(!LandzoMMA8451_init());  // 加速度角度传感器初始化
  OLED_Fill(0x00);//清屏 黑屏 
  EnableInterrupts ;
  while(1)
  {
 //   OLED_Print_Str(0,Flag,"蓝宙电子landzo.cn"); //汉字 字符串显示
  //  delay_ms(500);
    if(TIME0flag_100ms) {
      TIME0flag_100ms = 0 ;
      OLED_Fill(0x00);//清屏 黑屏
    //  Flag+=1;
    //  if(Flag>6) Flag=0;

      AngleAcceleration_Digital_Data (AngleAcceleArry);   //采集时间 900us,单位 mg
      
      AD_VALUE = ad_once(ADC0_SE14,ADC_8bit);  //采集AD
      SUN_VALUE = ad_once(ADC0_SE11,ADC_8bit);  //采集AD
      TEMP_VALUE = ad_once(ADC0_SE15,ADC_8bit);  //采集AD
   //   OLED_Print_Str(0,0,"蓝宙电子landzo.cn"); //汉字 字符串显示
      OLED_Print_Str(0,0,"AD_VALUE:"); //汉字 字符串显示
      OLED_Print_Num(84,0,AD_VALUE) ;
      OLED_Print_Str(0,2,"SUN_VALUE:"); //汉字 字符串显示
      OLED_Print_Num(84,2,SUN_VALUE) ;
      OLED_Print_Str(0,4,"TEMP_VALUE:"); //汉字 字符串显示
      OLED_Print_Num(84,4,TEMP_VALUE) ;
      OLED_Print_Str(0,6,"X:"); //汉字 字符串显示
      if(AngleAcceleArry[0]>=0 )
        OLED_Print_Num(12,6,AngleAcceleArry[0]/10) ;
      else{
        OLED_Print_Str(12,6,"-"); //汉字 字符串显示
        OLED_Print_Num(18,6,-AngleAcceleArry[0]/10) ;
      }
      
      OLED_Print_Str(45,6,"Y:"); //汉字 字符串显示
      if(AngleAcceleArry[1]>=0 )
        OLED_Print_Num(57,6,AngleAcceleArry[1]/10) ;
      else{
        OLED_Print_Str(57,6,"-"); //汉字 字符串显示
        OLED_Print_Num(63,6,-AngleAcceleArry[1]/10) ;
      }
       OLED_Print_Str(90,6,"Z:"); //汉字 字符串显示
      if(AngleAcceleArry[2]>=0 )
        OLED_Print_Num(102,6,AngleAcceleArry[2]/10) ;
      else{
        OLED_Print_Str(102,6,"-"); //汉字 字符串显示
        OLED_Print_Num(108,6,-AngleAcceleArry[2]/10) ;
      }
           
 //     OLED_Print_Str(0,6,"X:"); //汉字 字符串显示
 //     OLED_Print_Str(0,6,"X:"); //汉字 字符串显示
      Flag =AD_VALUE*100/255 ;
      
      TPM_PWM_Duty(TPM0, CH4,Flag); // PWM输出 PTC8 占空比 50%
            
      TPM_PWM_Duty(TPM0,CH5,Flag);
      TPM_PWM_Duty(TPM2,CH0,(100-Flag));
      TPM_PWM_Duty(TPM2,CH1,Flag/20*15);

      PTC13_OUT = PTC12_OUT ;
      PTC12_OUT = ~PTC12_OUT; //RBG_B输出状态取反，呈现亮灭状态 程序在运行      
      PTC7_OUT = PTC6_OUT ;
      PTC6_OUT = ~PTC6_OUT; //RBG_B输出状态取反，呈现亮灭状态 程序在运行
      PTB0_OUT = ~PTB0_OUT ;
      
       AD_buff[0] = AD_VALUE/100; // 取百位
       AD_buff[1] = AD_VALUE%100/10; // 取十位
       AD_buff[2] = AD_VALUE%10; // 取个位
       uart_sendStr(UART1,"AD_VALUE:");// 发送字符串
       for(uint8 i = 0;i < 3;i ++) uart_putchar(UART1, AD_buff[i] + '0'); // 发送采集到的AD值 
       uart_sendStr(UART1, "\r\n"); // 回车换行

    }
  }
}