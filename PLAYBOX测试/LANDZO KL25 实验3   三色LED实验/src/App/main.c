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
uint8 pwmval = 0;
static uint8 RGB_choose = 0;
/***********************参数定义***************************/

#define RGB_EN1  PTC12_OUT // RGB_EN1
#define RGB_EN2  PTC13_OUT // RGB_EN2
#define RGB_EN3  PTC7_OUT  // RGB_EN3
#define RGB_EN4  PTC6_OUT  // RGB_EN4

/************************主函数****************************/
void main(void)
{
  DisableInterrupts; //禁止总中断 
  TPM_PWM_init(TPM0, CH5, 80000,0);//PTC9  RGB_R 
  TPM_PWM_init(TPM2, CH1, 80000,0);//PTB19 RGB_B 
  TPM_PWM_init(TPM2, CH0, 80000,0);//PTB18 RGB_G
  gpio_init (PTC12, GPO, 1);//RGB_EN1 GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init (PTC13, GPO, 1);//RGB_EN2 GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init (PTC7, GPO, 1);//RGB_EN3 GPIO引脚初始化配置 推挽输出 输出为高电平
  gpio_init (PTC6, GPO, 1);//RGB_EN4 GPIO引脚初始化配置 推挽输出 输出为高电平
  EnableInterrupts ;
  while(1)
  {
    delay_ms(100);
    if(RGB_choose == 0) // RGB1 亮 其余RBG灭
    {
      RGB_choose = 1;
      RGB_EN1 = 0; //使能RGB1
      RGB_EN2 = 1; //失能RGB2
      RGB_EN3 = 1; //失能RGB3
      RGB_EN4 = 1; //失能RGB4
    }
    else if(RGB_choose == 1) // RGB2 亮 其余RBG灭
    {
      RGB_choose = 2;
      RGB_EN1 = 1; //失能RGB4
      RGB_EN2 = 0; //使能RGB1
    }
    else if(RGB_choose == 2) // RGB3 亮 其余RBG灭
    {
      RGB_choose = 3;
      RGB_EN2 = 1; //失能RGB4
      RGB_EN3 = 0; //使能RGB1
    }
    else if(RGB_choose == 3) // RGB4 亮 其余RBG灭
    {
      RGB_choose = 4;
      RGB_EN3 = 1; //失能RGB4
      RGB_EN4 = 0; //使能RGB1
    }
    else if(RGB_choose == 4) // 所有RBG亮
    {
      RGB_choose = 0;
      RGB_EN1 = 0; //使能RGB1
      RGB_EN2 = 0; //使能RGB2
      RGB_EN3 = 0; //使能RGB3
      RGB_EN4 = 0; //使能RGB4
    }    
    for(pwmval = 50;pwmval > 0;pwmval --)   // PWM输出控制RBG显示不同颜色
    {
      TPM_PWM_Duty(TPM0,CH5,pwmval);        // PWM输出 PTC9  RGB_R
      TPM_PWM_Duty(TPM2,CH1,100 - pwmval);  // PWM输出 PTB19 RGB_B
      TPM_PWM_Duty(TPM2,CH0,50 - pwmval);   // PWM输出 PTB18 RGB_G
      delay_ms(10);
    }
    for(pwmval = 0;pwmval < 51;pwmval ++)  // PWM输出控制RBG显示不同颜色
    {
      TPM_PWM_Duty(TPM0,CH5,pwmval);        // PWM输出 PTC9  RGB_R
      TPM_PWM_Duty(TPM2,CH1,100 - pwmval);  // PWM输出 PTB19 RGB_B
      TPM_PWM_Duty(TPM2,CH0,50 - pwmval);   // PWM输出 PTB18 RGB_G
      delay_ms(10);
    } 
  }
}