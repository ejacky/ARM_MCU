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
uint16 keyflag ;
uint8 LEDCout = 0 ;
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
  
  TPM_PWM_init(TPM0, CH4, 800,0);//PTC8   驱动蜂鸣器
  
   LCD_Init();   //TFT显示
   LCD_Clear(MAGENTA);
  delay_ms(100);//延时2000ms
  while(!LandzoMMA8451_init());  // 加速度角度传感器初始化
  
  gpio_Interrupt_init(PTA1, GPI_UP, ZERO);  //按键 PTA1上拉  低电平触发    左上
  gpio_Interrupt_init(PTA12, GPI_UP, ZERO); //按键 PTA12上拉 低电平触发   左左
  gpio_Interrupt_init(PTA5, GPI_UP, ZERO);  //按键 PTA5上拉  低电平触发   左下
  gpio_Interrupt_init(PTA13, GPI_UP, ZERO); //按键 PTA13上拉 低电平触发  左右
  gpio_Interrupt_init(PTA14, GPI_UP, ZERO); //按键 PTA14上拉 低电平触发  右左
  gpio_Interrupt_init(PTA16, GPI_UP, ZERO); //按键 PTA16上拉 低电平触发 右右
  gpio_Interrupt_init(PTA17, GPI_UP, ZERO); //按键 PTA17上拉 低电平触发 右上
  gpio_Interrupt_init(PTA15, GPI_UP, ZERO); //按键 PTA15上拉 低电平触发,右下
  set_irq_priority((INT_PORTA + INC_IRQ),1); //设置中断优先级 1
  setIRQadrr(INT_PORTA,PORTA_IRQHandler);    //设置PORTA中断地址函数 
  
  /**************************************************
  Arduino 接口初始化
  **************************************************/
  gpio_init(PTE20,GPO,0);
  gpio_init(PTE21,GPO,0);
  gpio_init(PTE22,GPO,0);
  gpio_init(PTE23,GPO,0);
  gpio_init(PTE29,GPO,0);
  gpio_init(PTE30,GPO,0);
  gpio_init(PTE0,GPO,0);
  gpio_init(PTE1,GPO,0);
  gpio_init(PTE2,GPO,0);
  gpio_init(PTE3,GPO,0);
  gpio_init(PTE4,GPO,0);
  gpio_init(PTE5,GPO,0);
  gpio_init(PTC10,GPO,0);
  gpio_init(PTC11,GPO,0);
  gpio_init(PTC5,GPO,0);
  gpio_init(PTB16,GPO,0);
  gpio_init(PTB17,GPO,0);
  
  EnableInterrupts ;
  while(1)
  {
 //   OLED_Print_Str(0,Flag,"蓝宙电子landzo.cn"); //汉字 字符串显示
  //  delay_ms(500);
    if(TIME0flag_100ms) {
      TIME0flag_100ms = 0 ;

      //加速度采集
      AngleAcceleration_Digital_Data (AngleAcceleArry);   //采集时间 900us,单位 mg
      //蜂鸣器采集
      AD_VALUE = ad_once(ADC0_SE14,ADC_8bit);  //采集AD
      //阳光传感器采集
      SUN_VALUE = ad_once(ADC0_SE11,ADC_8bit);  //采集AD
      //温度采集
      TEMP_VALUE = ad_once(ADC0_SE15,ADC_8bit);  //采集AD
      
      //参数显示
      LCD_ShowString(100,10,"Landzo");	
      LCD_ShowString(100,30,"PLAY-BOX TFTLCD TEST");	
      LCD_ShowString(100,50,"www.landzo.cn");
      LCD_ShowString(100,70,"AD_VALUE:");
      LCD_ShowNum(184,70,AD_VALUE,4,16);
      LCD_ShowString(100,90,"SUN_VALUE:");
      LCD_ShowNum(184,90,SUN_VALUE,4,16);
      LCD_ShowString(100,110,"TEMP_VALUE:");
      LCD_ShowNum(184,110,TEMP_VALUE,4,16);
      
      LCD_ShowString(100,130,"acc_X :");
      LCD_Fill(176,130,184,150,MAGENTA);
      if(AngleAcceleArry[0]>=0 ){     
        LCD_ShowString(176,130,"+");
        LCD_ShowNum(184,130,AngleAcceleArry[0],4,16);}
      else{
        LCD_ShowString(176,130,"-");
        LCD_ShowNum(184,130,-AngleAcceleArry[0],4,16);
      }
       LCD_ShowString(216,130,"mg");
      
      LCD_ShowString(100,150,"acc_Y :");
      LCD_Fill(176,150,184,170,MAGENTA);
      if(AngleAcceleArry[1]>=0 ){
        LCD_ShowString(176,150,"+");
        LCD_ShowNum(184,150,AngleAcceleArry[1],4,16);}
      else{
        LCD_ShowString(176,150,"-");
        LCD_ShowNum(184,150,-AngleAcceleArry[1],4,16);
      }
       LCD_ShowString(216,150,"mg");
      
      LCD_ShowString(100,170,"acc_Z :");
      LCD_Fill(176,170,184,190,MAGENTA);
      if(AngleAcceleArry[2]>=0 ){
        LCD_ShowString(176,170,"+");
        LCD_ShowNum(184,170,AngleAcceleArry[2],4,16);}
      else{
        LCD_ShowString(176,170,"-");
        LCD_ShowNum(184,170,-AngleAcceleArry[2],4,16);
      }
       LCD_ShowString(216,170,"mg");
      
     LCD_Fill(0,80,80,96,YELLOW); 
     LCD_ShowString(0,80,"key_print"); 
     LCD_Fill(30,100,50,120,YELLOW); 
     LCD_Fill(00,130,20,150,YELLOW); 
     LCD_Fill(30,160,50,180,YELLOW); 
     LCD_Fill(60,130,80,150,YELLOW); 

     LCD_Fill(240,80,320,96,YELLOW); 
     LCD_ShowString(240,80,"push BLACK"); 
     
     LCD_Fill(270,100,290,120,YELLOW); 
     LCD_Fill(240,130,260,150,YELLOW); 
     LCD_Fill(270,160,290,180,YELLOW); 
     LCD_Fill(300,130,320,150,YELLOW);
     
     if(keyflag==0x01) LCD_Fill(30,100,50,120,BLACK);
     if(keyflag==0x02) LCD_Fill(00,130,20,150,BLACK); 
     if(keyflag==0x04) LCD_Fill(30,160,50,180,BLACK); 
     if(keyflag==0x08) LCD_Fill(60,130,80,150,BLACK);
     if(keyflag==0x10) LCD_Fill(270,100,290,120,BLACK); 
     if(keyflag==0x20) LCD_Fill(240,130,260,150,BLACK); 
     if(keyflag==0x40) LCD_Fill(270,160,290,180,BLACK); 
     if(keyflag==0x80) LCD_Fill(300,130,320,150,BLACK);
     
      Flag =AD_VALUE*100/255 ;
      
      TPM_PWM_Duty(TPM0, CH4,Flag); // PWM输出 PTC8 占空比 50% 蜂鸣器
     //******LED灯显示
      LEDCout++ ;
      if(LEDCout ==1){ 
          LCD_Fill(00,210,250,240,RED); 
          TPM_PWM_Duty(TPM0,CH5,100);
          TPM_PWM_Duty(TPM2,CH0,0);
          TPM_PWM_Duty(TPM2,CH1,0);
      }
      else if(LEDCout ==2){ 
          LCD_Fill(0,210,250,240,BLUE); 
          TPM_PWM_Duty(TPM0,CH5,0);
          TPM_PWM_Duty(TPM2,CH0,100);
          TPM_PWM_Duty(TPM2,CH1,0);
      }
      else if(LEDCout ==3){ 
          LCD_Fill(0,210,250,240,GREEN); 
          TPM_PWM_Duty(TPM0,CH5,0);
          TPM_PWM_Duty(TPM2,CH0,00);
          TPM_PWM_Duty(TPM2,CH1,100);
      }
      if(LEDCout >= 3) LEDCout = 0 ;
     
      PTC13_OUT = PTC12_OUT ;
      PTC12_OUT = ~PTC12_OUT; //RBG_B输出状态取反，呈现亮灭状态 程序在运行      
      PTC7_OUT = PTC6_OUT ;
      PTC6_OUT = ~PTC6_OUT; //RBG_B输出状态取反，呈现亮灭状态 程序在运行
      //******LED灯显示 end
      
      //*******串口发送
       AD_buff[0] = AD_VALUE/100; // 取百位
       AD_buff[1] = AD_VALUE%100/10; // 取十位
       AD_buff[2] = AD_VALUE%10; // 取个位
       uart_sendStr(UART1,"AD_VALUE:");// 发送字符串
       for(uint8 i = 0;i < 3;i ++) uart_putchar(UART1, AD_buff[i] + '0'); // 发送采集到的AD值 
       uart_sendStr(UART1, "\r\n"); // 回车换行
       //*******串口发送 end
       
       //**********Arduino IO
       PTE20_OUT = ~PTE20_OUT;
       PTE21_OUT = ~PTE21_OUT;
       PTE22_OUT = ~PTE22_OUT;
       PTE23_OUT = ~PTE23_OUT;
       PTE29_OUT = ~PTE29_OUT;
       PTE30_OUT = ~PTE30_OUT;
       PTE0_OUT = ~PTE0_OUT;
       PTE1_OUT = ~PTE1_OUT;
       PTE2_OUT = ~PTE2_OUT;
       PTE3_OUT = ~PTE3_OUT;
       PTE4_OUT = ~PTE4_OUT;
       PTE5_OUT = ~PTE5_OUT;
       PTC10_OUT = ~PTC10_OUT;
       PTC11_OUT = ~PTC11_OUT;
       PTC5_OUT = ~PTC5_OUT;
       PTB16_OUT = ~PTB16_OUT;
       PTB17_OUT = ~PTB17_OUT;
       //**********Arduino IO END
    }
  }
}