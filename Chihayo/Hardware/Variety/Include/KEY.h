#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__KEY_ENABLE__
#ifndef __KEY_H__
#define __KEY_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//

//-----------------------------------------------------------------
// KEY1_ON
//-----------------------------------------------------------------
//
// 宏定义功能: 检测按键1按下
// 入口参数1: 无
// 返 回 值: 按键1是否按下
// 注意事项: 无
//
//-----------------------------------------------------------------
#define KEY1_ON (!LL_GPIO_IsInputPinSet(KEY_1_GPIO_Port, KEY_1_Pin))
//-----------------------------------------------------------------
// KEY2_ON
//-----------------------------------------------------------------
//
// 宏定义功能: 检测按键2按下
// 入口参数1: 无
// 返 回 值: 按键2是否按下
// 注意事项: 无
//
//-----------------------------------------------------------------
#define KEY2_ON (!LL_GPIO_IsInputPinSet(KEY_2_GPIO_Port, KEY_2_Pin))

//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

extern inline void EXTI3_IRQHandler_Func(void);
extern inline void EXTI9_5_IRQHandler_Func(void);

//*******************************// end_h                   //************************************//
#endif
#endif

#if 0
/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */

  /* USER CODE BEGIN LL_EXTI_LINE_3 */
  #if __HARDWARE_CONFIG__KEY_ENABLE__ // begin of __HARDWARE_CONFIG__KEY_ENABLE__
    //-----------------------------------------------------------------
    //? EXTI3_IRQHandler_KEY_1_DETECT
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Upon KEY_1 pressed, then turn the lighting state of LED.
    // Detected Case: Low state of KEY_1
    // Returned Value: The lighting state of LED 
    // Notice: None
    //
    //-----------------------------------------------------------------
    EXTI3_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__KEY_ENABLE__
  /* USER CODE END LL_EXTI_LINE_3 */
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */

  /* USER CODE BEGIN LL_EXTI_LINE_5 */
  #if __HARDWARE_CONFIG__KEY_ENABLE__ // begin of __HARDWARE_CONFIG__KEY_ENABLE__
    //-----------------------------------------------------------------
    //? EXTI5_IRQHandler_KEY_2_DETECT
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Upon KEY_2 pressed, then turn the lighting state of LED.
    // Detected Case: Low state of KEY_2
    // Returned Value: The lighting state of LED 
    // Notice: None
    //
    //-----------------------------------------------------------------
    EXTI9_5_IRQHandler_Func
  #endif
  /* USER CODE END LL_EXTI_LINE_5 */
  
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}
#endif
