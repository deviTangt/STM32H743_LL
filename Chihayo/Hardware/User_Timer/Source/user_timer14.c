#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER14_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER14_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer14.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer14_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer14_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// 函数功能: timer14定时器配置初始化
// 入口参数1: uint16_t _Prescaler:  预分频值
// 入口参数2: uint32_t _Autoreload: 重载系数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer14_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? 初始化时钟
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);

	//? 配置中断
	NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);

	//? 配置定时器结构体 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // 预分频
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // 计数模式：向上计数
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // 自动重装载值      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // 时钟分频
	LL_TIM_Init(TIM14, &TIM_InitStruct);

	//? 其他配置
	LL_TIM_DisableARRPreload(TIM14);                                // 启动自动装载模式
	LL_TIM_SetClockSource    (TIM14, LL_TIM_CLOCKSOURCE_INTERNAL);  // 设置时钟源：内部晶振
	LL_TIM_SetTriggerOutput  (TIM14, LL_TIM_TRGO_RESET);            // 复位更新
	LL_TIM_DisableMasterSlaveMode(TIM14);

  //? UPDATE标志位
	#if __HARDWARE_CONFIG__TIMER14_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER14_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM14);  // 清除向上计数溢出标志位
		LL_TIM_EnableIT_UPDATE (TIM14);  // 使能定时器向上计数中断
  #endif // end of __HARDWARE_CONFIG__TIMER14_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer14_start()
//-----------------------------------------------------------------
//
// 函数功能: 开启定时器14
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer14_start(){
  if(__HARDWARE_CONFIG__TIMER14_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM14);  // 清除向上计数溢出标志位
  LL_TIM_EnableCounter   (TIM14);  // 使能定时器开始计数
}
//-----------------------------------------------------------------
// inline void timer14_stop()
//-----------------------------------------------------------------
//
// 函数功能: 关闭定时器14
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer14_stop(){
  if(__HARDWARE_CONFIG__TIMER14_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM14);   // 清除向上计数溢出标志位
  LL_TIM_DisableCounter   (TIM14);  // 关闭定时器计数
}

//-----------------------------------------------------------------
// inline void TIM14_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// 函数功能: 定时器14中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 将此函数加入定时器中断服务函数TIM14_IRQHandler(void)中
//
//-----------------------------------------------------------------
inline void TIM14_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM14) == SET){	//判断定时器是否溢出
		LL_TIM_ClearFlag_UPDATE(TIM14);				//清除向上计数溢出标志位
		
		timer14_call_cnt ++;
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER14_ENABLE__


#if 0 //// stm32h7xx_it.c替换
/**
  * @brief This function handles TIM8 trigger and commutation interrupts and TIM14 global interrupt.
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER14_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER14_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM14_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM14->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM14 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM14_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER14_ENABLE__
  /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 1 */

  /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 1 */
}
#endif
