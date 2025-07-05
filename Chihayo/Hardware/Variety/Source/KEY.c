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
// 函数功能: 按键中断3中断服务函数(PE3 KEY1)
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 将此函数加入外部中断服务函数EXTI3_IRQHandler(void)中
//          检测长度
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
// 函数功能: 按键中断5中断服务函数(PC5 KEY2)
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 将此函数加入外部中断服务函数EXTI9_5_IRQHandler(void)中
//          判断电阻 / 电容类型并计算阻值或容值
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

