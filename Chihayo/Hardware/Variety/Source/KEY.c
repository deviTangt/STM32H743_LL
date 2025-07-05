#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__KEY_ENABLE__ // begin of __HARDWARE_CONFIG__TFTLCD_ENABLE__
//*******************************// include _h files    //************************************//
#include "key.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//
//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void EXTI3_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: �����ж�3�жϷ�����(PE3 KEY1)
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺��������ⲿ�жϷ�����EXTI3_IRQHandler(void)��
//          ��ⳤ��
//
//-----------------------------------------------------------------
inline void EXTI3_IRQHandler_Func(void){
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET){
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);

        if (~KEY1_ON){
            LED_TURN();
        }
    }
}

//-----------------------------------------------------------------
// inline void EXTI9_5_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: �����ж�5�жϷ�����(PC5 KEY2)
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺��������ⲿ�жϷ�����EXTI9_5_IRQHandler(void)��
//          �жϵ��� / �������Ͳ�������ֵ����ֵ
//
//-----------------------------------------------------------------
inline void EXTI9_5_IRQHandler_Func(void){
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_5) != RESET){
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);

        if (~KEY2_ON){
            LED_TURN();
        }
    }
}


//*******************************// end_c               //************************************//
#endif // end of __HARDWARE_CONFIG__TFTLCD_ENABLE__

