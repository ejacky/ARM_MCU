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
uint16 ReCRC=0;
uint16 AngleAcceleArry[3] ;
uint8  SendData1[10]={0};
/***********************��������***************************/

/************************������****************************/
void main(void)
{
  DisableInterrupts; //��ֹ���ж� 
  uart_init (UART1, 115200);
  gpio_init(PTC12,GPO,0); // RBG_EN1(PTC12) GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ ʹ�� RBG_EN1
  gpio_init(PTB19,GPO,1); //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTB18,GPO,1); //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTC9,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  pit_init_ms(PIT0, 5);  //��ʼ��PIT0����ʱʱ��Ϊ�� 5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //�����ж����ȼ� 8
  setIRQadrr(INT_PIT,PIT_IRQHandler); //����PIT0��ʱ���жϵ�ַ���� 
  while(!LandzoMMA8451_init());  // ���ٶȽǶȴ�������ʼ��
  OLED_Init();  //OLED��ʼ��
  EnableInterrupts ;
  while(1)
  {
    if(TIME0flag_5ms == 1) //5ms ִ��һ��
    {
      TIME0flag_5ms = 0;
      AngleAcceleration_Digital_Data (AngleAcceleArry);   //�ɼ�ʱ�� 900us
    }
    
    if(TIME0flag_10ms == 1) //20ms ִ��һ��
    {
      TIME0flag_10ms = 0;
      OLED_Print_Str(0,0,"�������/LANDZO");           //�����ַ�����ʾ
      OLED_Print_Str(0,2,"XG:");  //�����ַ�����ʾ 
      OLED_Print_Str(0,4,"YG:");  //�����ַ�����ʾ
      OLED_Print_Str(0,6,"ZG:");  //�����ַ�����ʾ 
      OLED_Print_Num(40,2,AngleAcceleArry[0]);  //�����ַ�����ʾ       
      OLED_Print_Num(40,4,AngleAcceleArry[2]);  //�����ַ�����ʾ 
      OLED_Print_Num(40,6,AngleAcceleArry[1]);  //�����ַ�����ʾ 
      delay_ms(50);
      OLED_Fill(0x00);//���� 
      for(int i=0;i<3;i++)
      {        
        SendData1[i*2+0]=(uint8)((uint16)AngleAcceleArry[i]);       //���ݵ�λ
        SendData1[i*2+1]= (uint8)((uint16)(AngleAcceleArry[i]>>8)); //���ݸ�λ        
      }
      
      ReCRC =  (uint8)AngleAcceleArry[0]+(uint8)(AngleAcceleArry[0]>>8) +(uint8)AngleAcceleArry[1]\
        +(uint8)(AngleAcceleArry[1]>>8) + (uint8)AngleAcceleArry[2]+ (uint8)(AngleAcceleArry[2]>>8) ; 
      SendData1[8]=(uint8)((uint16)ReCRC);
      uart_sendN(UART1,SendData1,9);
    }
    if(TIME0flag_1s == 1) 
    {
      TIME0flag_1s = 0;
      PTB19_OUT = ~PTB19_OUT; //RBG_B���״̬ȡ������������״̬ ����������
      PTB18_OUT = ~PTB18_OUT; //RBG_G���״̬ȡ������������״̬ ����������
      PTC9_OUT = ~PTC9_OUT;   //RBG_R���״̬ȡ������������״̬ ����������
    }
  }
}