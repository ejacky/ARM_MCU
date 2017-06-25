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
extern uint8 TIME0flag_1s;
extern  uint8_t  LPT_INT_count ; //LPT ���������
uint16 AD_VALUE;
uint8 AD_buff[3];
/***********************��������***************************/

/************************������****************************/
void main(void)
{
  DisableInterrupts;   //��ֹ���ж�
  uart_init (UART1, 115200); //UART1 ������Ϊ115200
  pit_init_ms(PIT0, 5); //�����ж϶�ʱ��PIT0 ��ʱ5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //�����ж����ȼ� 1
  setIRQadrr(INT_PIT,PIT_IRQHandler); //����PIT0��ʱ���жϵ�ַ���� 
  gpio_init(PTC12,GPO,0);  // RBG_EN1(PTC12) GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ ʹ�� RBG_EN1
  gpio_init(PTB19,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTB18,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTC9,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  adc_init(ADC0_SE14);     //ADC0_SE14(PTC0)��ʼ��ͨ��
  EnableInterrupts;   //�������ж� 
  while(1)
  {
     if(TIME0flag_20ms == 1) // 20ms����һ��
     {
       TIME0flag_20ms = 0;
       AD_VALUE = ad_once(ADC0_SE14,ADC_8bit);  //�ɼ�AD
       AD_buff[0] = AD_VALUE/100; // ȡ��λ
       AD_buff[1] = AD_VALUE%100/10; // ȡʮλ
       AD_buff[2] = AD_VALUE%10; // ȡ��λ
       uart_sendStr(UART1,"AD_VALUE:");// �����ַ���
       for(uint8 i = 0;i < 3;i ++) uart_putchar(UART1, AD_buff[i] + 0x30); // ���Ͳɼ�����ADֵ 
       uart_sendStr(UART1, "\r\n"); // �س�����
       
     }
     if(TIME0flag_1s == 1) // 1s����һ��
     { 
       TIME0flag_1s = 0;
       PTB19_OUT = ~PTB19_OUT; //RBG_B���״̬ȡ������������״̬ ����������
       PTB18_OUT = ~PTB18_OUT; //RBG_G���״̬ȡ������������״̬ ����������
       PTC9_OUT = ~PTC9_OUT;   //RBG_R���״̬ȡ������������״̬ ����������
     }
  }
}