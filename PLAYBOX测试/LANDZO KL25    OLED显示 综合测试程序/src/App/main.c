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
uint16 AD_VALUE;
uint16 SUN_VALUE;
uint16 TEMP_VALUE;
int16 AngleAcceleArry[3] ;
uint8 AD_buff[3];
/***********************��������***************************/


/************************������****************************/
void main(void)
{
  uint8 Flag=0;
  DisableInterrupts; //��ֹ���ж� 
  pit_init_ms(PIT0, 40);    //��ʼ��PIT0����ʱʱ��Ϊ�� 5ms
  set_irq_priority((INT_PIT + INC_IRQ),1); //�����ж����ȼ� 1
  setIRQadrr(INT_PIT,PIT_IRQHandler);    //����PIT0��ʱ���жϵ�ַ���� 

  TPM_PWM_init(TPM0,CH5,8000,50);  //RBG(PTC9) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  TPM_PWM_init(TPM2,CH0,8000,20);;  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  TPM_PWM_init(TPM2,CH1,8000,10);;  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTC12,GPO,0);  // RBG_EN1(PTC12) GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ ʹ�� RBG_EN1
  gpio_init(PTC13,GPO,0);  // RBG_EN1(PTC12) GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ ʹ�� RBG_EN2
  gpio_init(PTC7,GPO,0);  // RBG_EN1(PTC12) GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ ʹ�� RBG_EN3 
  gpio_init(PTC6,GPO,1);  // RBG_EN1(PTC12) GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ ʹ�� RBG_EN4

  adc_init(ADC0_SE14);     //ADC0_SE14(PTC0)��ʼ��ͨ�� ��λ��
  adc_init(ADC0_SE11);     //ADC0_SE11(PTC2)��ʼ��ͨ�� ���⴫����
  adc_init(ADC0_SE15);     //�¶ȴ����� ADC0_SE15(PTC1)��ʼ��ͨ��
  uart_init (UART1, 115200); //UART1 ������Ϊ115200
  
  TPM_PWM_init(TPM0, CH4, 10,0);//PTC8   ����������
  
  gpio_init(PTB0,GPO,1);  // RBG_EN1(PTC12) GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ ʹ�� RBG_EN4
  
  OLED_Init();  //OLED��ʼ�� 
  OLED_Print_Str(0,0,"�������landzo.cn"); //���� �ַ�����ʾ
  Draw_Landzo();//��ʾ����LOGO
  delay_ms(100);//��ʱ2000ms
  while(!LandzoMMA8451_init());  // ���ٶȽǶȴ�������ʼ��
  OLED_Fill(0x00);//���� ���� 
  EnableInterrupts ;
  while(1)
  {
 //   OLED_Print_Str(0,Flag,"�������landzo.cn"); //���� �ַ�����ʾ
  //  delay_ms(500);
    if(TIME0flag_100ms) {
      TIME0flag_100ms = 0 ;
      OLED_Fill(0x00);//���� ����
    //  Flag+=1;
    //  if(Flag>6) Flag=0;

      AngleAcceleration_Digital_Data (AngleAcceleArry);   //�ɼ�ʱ�� 900us,��λ mg
      
      AD_VALUE = ad_once(ADC0_SE14,ADC_8bit);  //�ɼ�AD
      SUN_VALUE = ad_once(ADC0_SE11,ADC_8bit);  //�ɼ�AD
      TEMP_VALUE = ad_once(ADC0_SE15,ADC_8bit);  //�ɼ�AD
   //   OLED_Print_Str(0,0,"�������landzo.cn"); //���� �ַ�����ʾ
      OLED_Print_Str(0,0,"AD_VALUE:"); //���� �ַ�����ʾ
      OLED_Print_Num(84,0,AD_VALUE) ;
      OLED_Print_Str(0,2,"SUN_VALUE:"); //���� �ַ�����ʾ
      OLED_Print_Num(84,2,SUN_VALUE) ;
      OLED_Print_Str(0,4,"TEMP_VALUE:"); //���� �ַ�����ʾ
      OLED_Print_Num(84,4,TEMP_VALUE) ;
      OLED_Print_Str(0,6,"X:"); //���� �ַ�����ʾ
      if(AngleAcceleArry[0]>=0 )
        OLED_Print_Num(12,6,AngleAcceleArry[0]/10) ;
      else{
        OLED_Print_Str(12,6,"-"); //���� �ַ�����ʾ
        OLED_Print_Num(18,6,-AngleAcceleArry[0]/10) ;
      }
      
      OLED_Print_Str(45,6,"Y:"); //���� �ַ�����ʾ
      if(AngleAcceleArry[1]>=0 )
        OLED_Print_Num(57,6,AngleAcceleArry[1]/10) ;
      else{
        OLED_Print_Str(57,6,"-"); //���� �ַ�����ʾ
        OLED_Print_Num(63,6,-AngleAcceleArry[1]/10) ;
      }
       OLED_Print_Str(90,6,"Z:"); //���� �ַ�����ʾ
      if(AngleAcceleArry[2]>=0 )
        OLED_Print_Num(102,6,AngleAcceleArry[2]/10) ;
      else{
        OLED_Print_Str(102,6,"-"); //���� �ַ�����ʾ
        OLED_Print_Num(108,6,-AngleAcceleArry[2]/10) ;
      }
           
 //     OLED_Print_Str(0,6,"X:"); //���� �ַ�����ʾ
 //     OLED_Print_Str(0,6,"X:"); //���� �ַ�����ʾ
      Flag =AD_VALUE*100/255 ;
      
      TPM_PWM_Duty(TPM0, CH4,Flag); // PWM��� PTC8 ռ�ձ� 50%
            
      TPM_PWM_Duty(TPM0,CH5,Flag);
      TPM_PWM_Duty(TPM2,CH0,(100-Flag));
      TPM_PWM_Duty(TPM2,CH1,Flag/20*15);

      PTC13_OUT = PTC12_OUT ;
      PTC12_OUT = ~PTC12_OUT; //RBG_B���״̬ȡ������������״̬ ����������      
      PTC7_OUT = PTC6_OUT ;
      PTC6_OUT = ~PTC6_OUT; //RBG_B���״̬ȡ������������״̬ ����������
      PTB0_OUT = ~PTB0_OUT ;
      
       AD_buff[0] = AD_VALUE/100; // ȡ��λ
       AD_buff[1] = AD_VALUE%100/10; // ȡʮλ
       AD_buff[2] = AD_VALUE%10; // ȡ��λ
       uart_sendStr(UART1,"AD_VALUE:");// �����ַ���
       for(uint8 i = 0;i < 3;i ++) uart_putchar(UART1, AD_buff[i] + '0'); // ���Ͳɼ�����ADֵ 
       uart_sendStr(UART1, "\r\n"); // �س�����

    }
  }
}