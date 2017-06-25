
/***********************************************************
 * �ļ���       ��system.c
 * ˵��         ��ϵͳƵ�ʳ�ʼ��
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://bbs.landzo.com
 * ��汾       ��V1.0
 *ʱ��          ��15.5.26
************************************************************/

#include "common.h"
#include "system.h"

MCGPLL  Gmcgpll ;
SIMDIV  Gsimdiv ;


/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

//uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

     
/*-------------------------------------------------------------------------*
//������: syscloption                                                   *
//��  ��:  ����ѡ��ϵͳʱ��ѡ��˳�����                                                   *
//��  ��: SYSPLL  ѡ��ʱ��							  	   *
//��  ��: ��                                                               *
//˵  ��: K60DZ  K64F12 �ļ��㹫ʽ SYSPLL =   OSCCLK *(pllVDIV+24)  /(pllPRDIV+1) 
*         K60FX        �ļ��㹫ʽ SYSPLL =   OSCCLK *(pllVDIV+16)  /(pllPRDIV+1)
//-------------------------------------------------------------------------*/
 void syscloption(system_option SYSPLL){
   uint32 systemclock ;
   /**************
   ����pll��Ƶ����,
   ��ΪK60FX������оƬ��һ����
   **************/
   #if (defined(MKL25Z4)||defined(MKL26Z4))
         #if( OSCCLK == 50)
           if(SYSPLL == 48 ){
             Gmcgpll.pllPRDIV = 24 ;
             Gmcgpll.pllVDIV  = 0 ;
           }           
           else if(SYSPLL == 50 ){
             Gmcgpll.pllPRDIV = 24 ;
             Gmcgpll.pllVDIV  = 1 ;
           }
           else if(SYSPLL <= 130){
             Gmcgpll.pllPRDIV = 19 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK- 24) ;
           }
           else if(SYSPLL <= 210){
             Gmcgpll.pllPRDIV = 13 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
           }
           else {
             Gmcgpll.pllPRDIV = 13 ;
             Gmcgpll.pllVDIV = 16 ;
           }

         #elif( OSCCLK == 12)
            if(SYSPLL == 50 ){
             Gmcgpll.pllPRDIV = 5 ;
             Gmcgpll.pllVDIV  = 1 ;
           }
           else if(SYSPLL <= 130){
             Gmcgpll.pllPRDIV = 3 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
           }else if(SYSPLL <= 220){
             Gmcgpll.pllPRDIV = 2 ;
             Gmcgpll.pllVDIV = (uint8)(SYSPLL*(Gmcgpll.pllPRDIV+1) /OSCCLK - 24) ;
           }else {
             Gmcgpll.pllPRDIV = 2 ;
             Gmcgpll.pllVDIV =  24 ;
           }
         #endif
       if(Gmcgpll.pllVDIV > 0x1F) Gmcgpll.pllVDIV = 0x1F ;       
        systemclock = OSCCLK *(Gmcgpll.pllVDIV+24)/(Gmcgpll.pllPRDIV+1) - 2; 
  #endif
    /**************
   ����ϵͳʱ��
   **************/
        
   Gsimdiv.coreDIV = 0 ;
   Gsimdiv.busDIV = systemclock / BUSCLOCKMAX ;
//    Gsimdiv.FlexBusDIV = systemclock / FLEXCLOCKMAX ;
//     Gsimdiv.FlashDIV = systemclock/FLASHCLOCKMAX ;
   if(Gsimdiv.busDIV > 0x8) Gsimdiv.busDIV = 0X08 ;
//       if(Gsimdiv.FlexBusDIV > 0xF) Gsimdiv.FlexBusDIV = 0X0F ;
//       if(Gsimdiv.FlashDIV > 0xF) Gsimdiv.FlashDIV = 0X0F ;
  
 }
/*-------------------------------------------------------------------------*
//������: system_init                                                  *
//��  ��: ϵͳ��ʼ��                                                   *
//��  ��: SYSPLL  ϵͳѡ��ʱ��							  	   *
//��  ��: sysclolck   ���õ�ϵͳʱ��                                                              *
//˵  ��: оƬʹ��PLL������оƬ����ĸ���ʱ�ӣ�оƬ��λ��MCGĬ�Ͻ���FEIģʽ��
//         �谴��FEI->FEB->PBE->PEE��˳��������á�                                                     *
//-------------------------------------------------------------------------*/
 uint32 system_init(system_option SYSPLL){
   uint32 systemclock ;
   
   syscloption(SYSPLL) ;
   
   //Ĭ�Ͽ�ʼ״̬ΪFEIģʽ���л���FBEģʽ
    // Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0
     MCG_C2 = MCG_C2_RANGE0(2) | MCG_C2_HGO0_MASK;          
    //ѡ���ⲿ����50MHz����Ϊʱ��Դ������FLL��Ƶ������IREFS�����ⲿ����
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(6);
    //while (!(MCG_S & MCG_S_OSCINIT0_MASK)) {};  //�ȴ������ȶ�
    //�ȴ��ο�ʱ��״̬λ����
    while (MCG_S & MCG_S_IREFST_MASK){}; 
    //�ȴ�ʱ��״̬λ��ʾʱ��Դ�����ⲿ�ο�ʱ��
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; 

    //�ѽ���FBEģʽ���л���PBEģʽ
    //����PLL��Ƶ��Ϊ�ⲿʱ��Դ�ķ�Ƶ���õ���Ƶʱ��
    MCG_C5 = MCG_C5_PRDIV0(Gmcgpll.pllPRDIV);
    //ȷ��MCG_C6���ڸ�λ״̬����ֹLOLIE��PLL����ʱ�ӿ���������PLL VCO��Ƶ��
    MCG_C6 = 0x0;
    //ѡ��PLL VCO��Ƶ����ϵͳʱ�ӷ�Ƶ��ȡ����ʱ��ѡ��
//???    set_sys_dividers(Gsimdiv.coreDIV,Gsimdiv.busDIV,Gsimdiv.FlexBusDIV,Gsimdiv.FlashDIV) ;  
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(Gsimdiv.coreDIV) | SIM_CLKDIV1_OUTDIV4(Gsimdiv.busDIV);

    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(Gmcgpll.pllVDIV) ;
    while (!(MCG_S & MCG_S_PLLST_MASK)){}; // �ȴ�PLL�ȶ�
    while (!(MCG_S & MCG_S_LOCK0_MASK)){};  // �ȴ�PLL����
    //�ѽ���PBEģʽ���л���PEEģʽ
    //ͨ������CLKSλ������PEEģʽ
    // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 &= ~MCG_C1_CLKS_MASK; 
        // Wait for clock status bits to update
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};
   
#if (defined(MKL25Z4)||defined(MKL26Z4)) 
     systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 24)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;
#elif defined(MK60F15)
     systemclock = OSCCLK*((MCG_C6 & MCG_C6_VDIV0_MASK) + 16)/((MCG_C5&MCG_C5_PRDIV0_MASK) + 1) ;      
#endif
    systemclock *=1000 ;
   return systemclock ;
 }







///*-------------------------------------------------------------------------*
////�������ƣ�set_sys_dividers
////�������أ���
////����˵����outdiv1-4��Ԥ��Ƶֵ
////���ܸ�Ҫ��ϵͳ��Ƶ���趨����
////����˵�����˺����������RAM��ִ�У�������������e2448����FLASHʱ�ӷ�Ƶ�ı�
////         ʱ�������ֹFLASH��Ԥȡ���ܡ���ʱ�ӷ�Ƶ�ı�֮�󣬱�����ʱһ��ʱ��
////         ��ſ�������ʹ��Ԥȡ���ܡ�					
////-------------------------------------------------------------------------*/
//__RAMFUN void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
//{
//    uint32 temp_reg;
//    uint8 i;
//    
//    //����FMC_PFAPR��ǰ��ֵ
//    temp_reg = FMC_PFAPR;
//
//    //ͨ��M&PFD��λM0PFD����ֹԤȡ����
//    FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK 
//               | FMC_PFAPR_M6PFD_MASK 
//               | FMC_PFAPR_M5PFD_MASK
//               | FMC_PFAPR_M4PFD_MASK 
//               | FMC_PFAPR_M3PFD_MASK 
//               | FMC_PFAPR_M2PFD_MASK
//               | FMC_PFAPR_M1PFD_MASK 
//               | FMC_PFAPR_M0PFD_MASK;
//
//    //��ʱ�ӷ�Ƶ����������ֵ  
//    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) 
//                | SIM_CLKDIV1_OUTDIV2(outdiv2) 
//                | SIM_CLKDIV1_OUTDIV3(outdiv3) 
//                | SIM_CLKDIV1_OUTDIV4(outdiv4);
//
//    //�ȴ���Ƶ���ı�
//    for (i = 0 ; i < outdiv4 ; i++){}
//
//    //���´�FMC_PFAPR��ԭʼֵ
//    FMC_PFAPR = temp_reg;
//}

