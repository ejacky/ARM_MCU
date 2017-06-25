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

/***********************��������***************************/


/************************������****************************/
void main(void)
{
  DisableInterrupts; //��ֹ���ж� 
  pit_init_ms(PIT0, 5);   //�����ж϶�ʱ��PIT0 ��ʱ5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //�����ж����ȼ� 1
  setIRQadrr(INT_PIT,PIT_IRQHandler); //����PIT0��ʱ���жϵ�ַ���� 
  TPM_PWM_init(TPM0, CH4, 0,50);//PTC8   ����������   
  gpio_init (PTC12, GPO, 0);//RGB_EN1 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ RGB_EN1 ʹ��
  gpio_init(PTB19,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTB18,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTC9,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  EnableInterrupts ;
  while(1)
  {
    delay_ms(5);
    TPM_PWM_freq(TPM0,5000); // PWM��� PTC8 Ƶ�� 5000
    TPM_PWM_Duty(TPM0, CH4,50); // PWM��� PTC8 ռ�ձ� 50%
    if(TIME0flag_100ms == 1) //100ms����һ�� 
    {
      TIME0flag_100ms = 0;
	  PTB19_OUT = ~PTB19_OUT; //RBG_B���״̬ȡ������������״̬ ����������
	  PTB18_OUT = ~PTB18_OUT; //RBG_G���״̬ȡ������������״̬ ����������
	  PTC9_OUT = ~PTC9_OUT;   //RBG_R���״̬ȡ������������״̬ ����������
    }
  }
}