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
uint8 cout ;
/***********************��������***************************/

#define choose_tube1  PTB8_OUT//ѡ�������1
#define choose_tube2  PTB9_OUT //ѡ�������2  
#define choose_tube3  PTB10_OUT //ѡ�������3
#define choose_tube4  PTB2_OUT //ѡ�������4
#define choose_tube5  PTB1_OUT //ѡ�������5
#define choose_tube6  PTB0_OUT //ѡ�������6
#define Show_NUM   GPIO_PDOR_REG(PTD_BASE_PTR) //д������
static unsigned char table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x40,0x80,0,};//���干���������ʾ����
                                  // 0    1    2     3    4    5    6    7   8    9    a    b    c    d    e    f    -    .  Ϩ��
    //��0��ѡ����������� 0-9 a-f - ��
/************************������****************************/
void main(void)
{
  DisableInterrupts; //��ֹ���ж� 
  gpio_init(PTB10, GPO, 0); // �����1 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ
  gpio_init(PTB9, GPO, 0); // �����2 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ
  gpio_init(PTB8, GPO, 0); // �����3 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ
  gpio_init(PTB0, GPO, 0); // �����4 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ
  gpio_init(PTB1, GPO, 0); // �����5 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ
  gpio_init(PTB2, GPO, 0); // �����6 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ 
  //����������������IOΪ���������
  gpio_init(PTD0,GPO,0);	
  gpio_init(PTD1,GPO,0);
  gpio_init(PTD2,GPO,0);
  gpio_init(PTD3,GPO,0);
  gpio_init(PTD4,GPO,0);
  gpio_init(PTD5,GPO,0);
  gpio_init(PTD6,GPO,0);
  gpio_init(PTD7,GPO,0);
  EnableInterrupts ; //�������ж�
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
      delay_ms(100); // ��ʱһ��ʱ��  

 
  }
}