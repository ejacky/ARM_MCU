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
extern  uint8_t  LPT_INT_count ; //LPT ���������
/***********************��������***************************/

//#define Key_Interrupt     // ���尴���ж� ��ⰴ��ʱ ��������һ������һ��
#define Key_INPUT       // ���尴������ ��ⰴ��ʱ ��������һ������һ��

#if defined(Key_INPUT)  // ʹ���ⲿ�����ⰴ��
#define UP_Key      PTA1_INT  
#define LEFT_Key    PTA12_INT 
#define DOWN_Key    PTA5_INT   
#define RIGHT_Key   PTA13_INT 

#define A_Key   PTA14_INT
#define B_Key   PTA16_INT
#define C_Key   PTA17_INT
#define D_Key   PTA15_INT    
#endif
/************************������****************************/
void main(void)
{
  DisableInterrupts; //��ֹ���ж� 
  uart_init (UART1, 115200); //UART1 ������Ϊ115200
  gpio_init (PTB0, GPO, 1); //LED GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
#if defined(Key_Interrupt)  //ʹ���ⲿ�жϼ�ⰴ��
  gpio_Interrupt_init(PTA1, GPI_UP, FALLING);  //���� PTA1���� �½��ش���
  gpio_Interrupt_init(PTA12, GPI_UP, FALLING); //���� PTA12���� �½��ش���
  gpio_Interrupt_init(PTA5, GPI_UP, FALLING);  //���� PTA5���� �½��ش���
  gpio_Interrupt_init(PTA13, GPI_UP, FALLING); //���� PTA13���� �½��ش���
  gpio_Interrupt_init(PTA14, GPI_UP, FALLING); //���� PTA14���� �½��ش���
  gpio_Interrupt_init(PTA16, GPI_UP, FALLING); //���� PTA16���� �½��ش���
  gpio_Interrupt_init(PTA17, GPI_UP, FALLING); //���� PTA17���� �½��ش���
  gpio_Interrupt_init(PTA15, GPI_UP, FALLING); //���� PTA15���� �½��ش���
  set_irq_priority((INT_PORTA + INC_IRQ),1); //�����ж����ȼ� 1
  setIRQadrr(INT_PORTA,PORTA_IRQHandler);    //����PORTA�жϵ�ַ���� 
#else
  gpio_init (PTA1, GPI_UP, 1);  //���� PTA1 GPIO���ų�ʼ������ ��������
  gpio_init (PTA12, GPI_UP, 1); //���� PTA12 GPIO���ų�ʼ������ ��������
  gpio_init (PTA5, GPI_UP, 1);  //���� PTA5 GPIO���ų�ʼ������ ��������
  gpio_init (PTA13, GPI_UP, 1); //���� PTA13 GPIO���ų�ʼ������ ��������
  gpio_init (PTA14, GPI_UP, 1); //���� PTA14 GPIO���ų�ʼ������ ��������
  gpio_init (PTA16, GPI_UP, 1); //���� PTA16 GPIO���ų�ʼ������ ��������
  gpio_init (PTA17, GPI_UP, 1); //���� PTA17 GPIO���ų�ʼ������ ��������
  gpio_init (PTA15, GPI_UP, 1); //���� PTA15 GPIO���ų�ʼ������ ��������
#endif  

  EnableInterrupts ; //�������ж�
  while(1)
  {
#if defined(Key_INPUT)  // ʹ���ⲿ�����ⰴ��
   if(LEFT_Key == 0) 
   {
     delay_ms(5); //����
     if(LEFT_Key == 0) uart_sendStr(UART1,"LEFT_Key Down!\r\n"); // ���� LEFT_Key Down ��ʾ�˰���������
     while(!LEFT_Key); //�ȴ������ɿ�
   }
   else if(RIGHT_Key == 0) 
   {
     delay_ms(5); //����
     if(RIGHT_Key == 0) uart_sendStr(UART1,"RIGHT_Key Down!\r\n"); // ���� RIGHT_Key Down ��ʾ�˰���������
     while(!RIGHT_Key); //�ȴ������ɿ�
   }
   else if(UP_Key == 0) 
   {
     delay_ms(5); //����
     if(UP_Key == 0) uart_sendStr(UART1,"UP_Key Down!\r\n"); // ���� UP_Key Down ��ʾ�˰���������
     while(!UP_Key); //�ȴ������ɿ�
   }
   else if(DOWN_Key == 0) 
   {
     delay_ms(5); //����
     if(DOWN_Key == 0) uart_sendStr(UART1,"DOWN_Key Down!\r\n"); // ���� DOWN_Key Down ��ʾ�˰���������
     while(!DOWN_Key); //�ȴ������ɿ�
   }
   else if(A_Key == 0) 
   {
     delay_ms(5); //����
     if(A_Key == 0) uart_sendStr(UART1,"A_Key Down!\r\n"); // ���� A_Key Down ��ʾ�˰���������
     while(!A_Key); //�ȴ������ɿ�
   }
   else if(B_Key == 0) 
   {
     delay_ms(5); //����
     if(B_Key == 0) uart_sendStr(UART1,"B_Key Down!\r\n"); // ���� B_Key Down ��ʾ�˰���������
     while(!B_Key); //�ȴ������ɿ�
   }
   else if(C_Key == 0) 
   {
     delay_ms(5); //����
     if(C_Key == 0) uart_sendStr(UART1,"C_Key Down!\r\n"); // ���� C_Key Down ��ʾ�˰���������
     while(!C_Key); //�ȴ������ɿ�
   }
   else if(D_Key == 0) 
   {
     delay_ms(5); //����
     if(D_Key == 0) uart_sendStr(UART1,"D_Key Down!\r\n"); // ���� D_Key Down ��ʾ�˰���������
     while(!D_Key); //�ȴ������ɿ�
   }
#endif  
  }
}