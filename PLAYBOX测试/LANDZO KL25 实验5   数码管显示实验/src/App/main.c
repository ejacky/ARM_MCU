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
uint8 cout ;
/***********************参数定义***************************/

#define choose_tube1  PTB8_OUT//选择数码管1
#define choose_tube2  PTB9_OUT //选择数码管2  
#define choose_tube3  PTB10_OUT //选择数码管3
#define choose_tube4  PTB2_OUT //选择数码管4
#define choose_tube5  PTB1_OUT //选择数码管5
#define choose_tube6  PTB0_OUT //选择数码管6
#define Show_NUM   GPIO_PDOR_REG(PTD_BASE_PTR) //写入数据
static unsigned char table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x40,0x80,0,};//定义共阴数码管显示段码
                                  // 0    1    2     3    4    5    6    7   8    9    a    b    c    d    e    f    -    .  熄灭
    //Ｐ0段选，共阴数码管 0-9 a-f - 表
/************************主函数****************************/
void main(void)
{
  DisableInterrupts; //禁止总中断 
  gpio_init(PTB10, GPO, 0); // 数码管1 GPIO引脚初始化配置 推挽输出 输出为低电平
  gpio_init(PTB9, GPO, 0); // 数码管2 GPIO引脚初始化配置 推挽输出 输出为低电平
  gpio_init(PTB8, GPO, 0); // 数码管3 GPIO引脚初始化配置 推挽输出 输出为低电平
  gpio_init(PTB0, GPO, 0); // 数码管4 GPIO引脚初始化配置 推挽输出 输出为低电平
  gpio_init(PTB1, GPO, 0); // 数码管5 GPIO引脚初始化配置 推挽输出 输出为低电平
  gpio_init(PTB2, GPO, 0); // 数码管6 GPIO引脚初始化配置 推挽输出 输出为低电平 
  //设置数码管数据输出IO为推挽输出；
  gpio_init(PTD0,GPO,0);	
  gpio_init(PTD1,GPO,0);
  gpio_init(PTD2,GPO,0);
  gpio_init(PTD3,GPO,0);
  gpio_init(PTD4,GPO,0);
  gpio_init(PTD5,GPO,0);
  gpio_init(PTD6,GPO,0);
  gpio_init(PTD7,GPO,0);
  EnableInterrupts ; //开启总中断
  while(1)
  {
      choose_tube1 = 1;
      choose_tube2 = 1;
      choose_tube3 = 1;
      choose_tube4 = 1;
      choose_tube5 = 1;
      choose_tube6 = 1;
      if(cout) {
        Show_NUM = 0xff;
        cout = 0 ;}
      else{
        Show_NUM = table[0x12];
        cout = 1 ;
      }
      delay_ms(100); // 延时一段时间  

 
  }
}