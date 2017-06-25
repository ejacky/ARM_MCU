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
  uint8 ii = 0;
  DisableInterrupts; //��ֹ���ж� 
  
  TPM_PWM_init(TPM0, CH4, 0,50);//PTC8   ����������   
  gpio_init (PTC12, GPO, 0);//RGB_EN1 GPIO���ų�ʼ������ ������� ���Ϊ�͵�ƽ RGB_EN1 ʹ��
  gpio_init(PTB19,GPO,1);  //RBG(PTB19) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTB18,GPO,1);  //RBG(PTB18) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  gpio_init(PTC9,GPO,1);  //RBG(PTC9) GPIO���ų�ʼ������ ������� ���Ϊ�ߵ�ƽ
  
//  ILI9341GPIO_Config();  
//  ILI9341_Initial();
//
//  LCD_SetCursor(30,30);
//
//  ILI9341_DisplayEnlargeWord(WHITE ,DARKBLUE ,zimo);
  LCD_Init();
//  LCD_Clear(GREEN);  
  EnableInterrupts ;
  while(1)
  {
/**********************20ms����ִ�д����********************/ 
   switch(ii)
    {
      case 0: LCD_Clear(RED);
      break;
      case 1: LCD_Clear(BLUE);
      break;
      case 2: LCD_Clear(YELLOW);
      break;
      case 3: LCD_Clear(GREEN);
      break;
      case 4: LCD_Clear(MAGENTA);
      break;
      case 5: LCD_Clear(BROWN);
      break;
      case 6: LCD_Clear(BRRED);
      break;
      case 7: LCD_Clear(GRAY);
      break;
      case 8: LCD_Clear(LIGHTBLUE);
      break;
    }
    LCD_ShowString(100,10,"Landzo");	
    LCD_ShowString(100,30,"PLAY-BOX TFTLCD TEST");	
    LCD_ShowString(100,50,"www.landzo.cn");  
    PTB19_OUT = ~PTB19_OUT; //RBG_B���״̬ȡ������������״̬ ����������
    PTB18_OUT = ~PTB18_OUT; //RBG_G���״̬ȡ������������״̬ ����������
    PTC9_OUT = ~PTC9_OUT;   //RBG_R���״̬ȡ������������״̬ ����������
    delay_ms(500);
    ii++;
    if(ii > 8) ii = 0;
  }
}

