/***********************************************************
 * �ļ���       ��main.c
 * ˵��         ���û�Ӧ�ó���
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/


#include "include.h" // Include.h �����û��Զ����ͷ�ļ� 


/***********************��������***************************/
extern uint8 TIME0flag_5ms   ;
extern uint8 TIME0flag_10ms  ;
extern uint8 TIME0flag_20ms  ;
extern uint8 TIME0flag_50ms  ;
extern uint8 TIME0flag_80ms  ;
extern uint8 TIME0flag_100ms ;
extern uint8 TIME0flag_1s ;
extern  uint8_t  LPT_INT_count ; //LPT ���������
uint8 pwmval = 0;
static uint8 RGB_choose = 0;
/***********************��������***************************/

#define RGB_EN1  PTC12_OUT // RGB_EN1
#define RGB_EN2  PTC13_OUT // RGB_EN2
#define RGB_EN3  PTC7_OUT  // RGB_EN3
#define RGB_EN4  PTC6_OUT  // RGB_EN4

/************************������****************************/
void main(void)
{
  DisableInterrupts; //��ֹ���ж� 
  TPM_PWM_init(TPM0, CH5, 80000,0);//PTC9  RGB_R 
  TPM_PWM_init(TPM2, CH1, 80000,0);//PTB19 RGB_B 
  TPM_PWM_init(TPM2, CH0, 80000,0);//PTB18 RGB_G
  gpio_init (PTC12, GPO, 1);//RGB_EN1 GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init (PTC13, GPO, 1);//RGB_EN2 GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init (PTC7, GPO, 1);//RGB_EN3 GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init (PTC6, GPO, 1);//RGB_EN4 GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  EnableInterrupts ;
  while(1)
  {
    delay_ms(100);
    if(RGB_choose == 0) // RGB1 �� ����RBG��
    {
      RGB_choose = 1;
      RGB_EN1 = 0; //ʹ��RGB1
      RGB_EN2 = 1; //ʧ��RGB2
      RGB_EN3 = 1; //ʧ��RGB3
      RGB_EN4 = 1; //ʧ��RGB4
    }
    else if(RGB_choose == 1) // RGB2 �� ����RBG��
    {
      RGB_choose = 2;
      RGB_EN1 = 1; //ʧ��RGB4
      RGB_EN2 = 0; //ʹ��RGB1
    }
    else if(RGB_choose == 2) // RGB3 �� ����RBG��
    {
      RGB_choose = 3;
      RGB_EN2 = 1; //ʧ��RGB4
      RGB_EN3 = 0; //ʹ��RGB1
    }
    else if(RGB_choose == 3) // RGB4 �� ����RBG��
    {
      RGB_choose = 4;
      RGB_EN3 = 1; //ʧ��RGB4
      RGB_EN4 = 0; //ʹ��RGB1
    }
    else if(RGB_choose == 4) // ����RBG��
    {
      RGB_choose = 0;
      RGB_EN1 = 0; //ʹ��RGB1
      RGB_EN2 = 0; //ʹ��RGB2
      RGB_EN3 = 0; //ʹ��RGB3
      RGB_EN4 = 0; //ʹ��RGB4
    }    
    for(pwmval = 50;pwmval > 0;pwmval --)   // PWM�������RBG��ʾ��ͬ��ɫ
    {
      TPM_PWM_Duty(TPM0,CH5,pwmval);        // PWM��� PTC9  RGB_R
      TPM_PWM_Duty(TPM2,CH1,100 - pwmval);  // PWM��� PTB19 RGB_B
      TPM_PWM_Duty(TPM2,CH0,50 - pwmval);   // PWM��� PTB18 RGB_G
      delay_ms(10);
    }
    for(pwmval = 0;pwmval < 51;pwmval ++)  // PWM�������RBG��ʾ��ͬ��ɫ
    {
      TPM_PWM_Duty(TPM0,CH5,pwmval);        // PWM��� PTC9  RGB_R
      TPM_PWM_Duty(TPM2,CH1,100 - pwmval);  // PWM��� PTB19 RGB_B
      TPM_PWM_Duty(TPM2,CH0,50 - pwmval);   // PWM��� PTB18 RGB_G
      delay_ms(10);
    } 
  }
}