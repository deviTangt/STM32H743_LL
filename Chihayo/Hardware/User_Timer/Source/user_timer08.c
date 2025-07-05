#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER8_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer08.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer8_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer8_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// 函数功能: timer8定时器配置初始化
// 入口参数1: uint16_t _Prescaler:  预分频值
// 入口参数2: uint32_t _Autoreload: 重载系数
// 返 回 值: 无
// 注意事项: 挂载在APB2总线上，与一般定时器不同
//
//-----------------------------------------------------------------
inline void timer8_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? 初始化时钟
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);

	//? 配置中断
	NVIC_SetPriority(TIM8_UP_TIM13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

	//? 配置定时器结构体 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // 预分频
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // 计数模式：向上计数
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // 自动重装载值      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // 时钟分频
	LL_TIM_Init(TIM8, &TIM_InitStruct);

	//? 其他配置
	LL_TIM_DisableARRPreload(TIM8);                                // 启动自动装载模式
	LL_TIM_SetClockSource    (TIM8, LL_TIM_CLOCKSOURCE_INTERNAL);  // 设置时钟源：内部晶振
	LL_TIM_SetTriggerOutput  (TIM8, LL_TIM_TRGO_RESET);            // 复位更新
	LL_TIM_DisableMasterSlaveMode(TIM8);

  //? UPDATE标志位
	#if __HARDWARE_CONFIG__TIMER8_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER8_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM8);  // 清除向上计数溢出标志位
		LL_TIM_EnableIT_UPDATE (TIM8);  // 使能定时器向上计数中断
  #endif // end of __HARDWARE_CONFIG__TIMER8_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer8_start()
//-----------------------------------------------------------------
//
// 函数功能: 开启定时器8
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer8_start(){
  if(__HARDWARE_CONFIG__TIMER8_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM8);  // 清除向上计数溢出标志位
  LL_TIM_EnableCounter   (TIM8);  // 使能定时器开始计数
}
//-----------------------------------------------------------------
// inline void timer8_stop()
//-----------------------------------------------------------------
//SSSSS
// 函数功能: 关闭定时器8
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer8_stop(){
  if(__HARDWARE_CONFIG__TIMER8_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM8);   // 清除向上计数溢出标志位
  LL_TIM_DisableCounter   (TIM8);  // 关闭定时器计数
}

//-----------------------------------------------------------------
// inline void TIM8_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// 函数功能: 定时器8中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 将此函数加入定时器中断服务函数TIM8_IRQHandler(void)中
//
//-----------------------------------------------------------------
inline void TIM8_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM8) == SET){	//判断定时器是否溢出
		LL_TIM_ClearFlag_UPDATE(TIM8);				//清除向上计数溢出标志位
		
		timer8_call_cnt ++;
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__



#if 0 //// stm32h7xx_it.c替换
/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER8_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM8_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM8->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM3 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM8_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__

  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER13_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM13_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM13->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM13 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM13_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}
#endif
