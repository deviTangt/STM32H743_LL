#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER3_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer03.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer3_call_cnt = 0;

extern uint16_t DMA_ADC1_RX_BUF[DMA_ADC1_RX_BUF_SIZE];
extern uint32_t dma_adc1_finish_cnt;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer3_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// 函数功能: timer3定时器配置初始化
// 入口参数1: uint16_t _Prescaler:  预分频值
// 入口参数2: uint32_t _Autoreload: 重载系数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer3_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? 初始化时钟
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

	//? 配置中断
	NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM3_IRQn);

	//? 配置定时器结构体 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // 预分频
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // 计数模式：向上计数
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // 自动重装载值      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // 时钟分频
	LL_TIM_Init(TIM3, &TIM_InitStruct);

	//? 其他配置
	LL_TIM_DisableARRPreload     (TIM3);                                // 禁用自动装载模式
	LL_TIM_SetClockSource         (TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);  // 设置时钟源：内部晶振
	LL_TIM_SetTriggerOutput       (TIM3, LL_TIM_TRGO_RESET);            // 复位更新
	LL_TIM_DisableMasterSlaveMode(TIM3);                                // 禁用主从模式

  //? UPDATE标志位
	#if __HARDWARE_CONFIG__TIMER3_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER3_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM3);  // 清除向上计数溢出标志位
		LL_TIM_EnableIT_UPDATE (TIM3);  // 使能定时器向上计数中断
  #endif // end of __HARDWARE_CONFIG__TIMER3_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer3_start()
//-----------------------------------------------------------------
//
// 函数功能: 开启定时器3
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer3_start(){
    if(__HARDWARE_CONFIG__TIMER3_INT_UPDATE_ENABLE__)
      LL_TIM_ClearFlag_UPDATE(TIM3);  // 清除向上计数溢出标志位
    LL_TIM_EnableCounter   (TIM3);  // 使能定时器开始计数
}
//-----------------------------------------------------------------
// inline void timer3_stop()
//-----------------------------------------------------------------
//
// 函数功能: 关闭定时器3
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer3_stop(){
    if(__HARDWARE_CONFIG__TIMER3_INT_UPDATE_ENABLE__)
      LL_TIM_ClearFlag_UPDATE(TIM3);   // 清除向上计数溢出标志位
    LL_TIM_DisableCounter   (TIM3);  // 关闭定时器计数
}

//-----------------------------------------------------------------
// inline void TIM3_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// 函数功能: 定时器3中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 将此函数加入定时器中断服务函数TIM3_IRQHandler(void)中
//
//-----------------------------------------------------------------
inline void TIM3_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3) == SET){	//判断定时器是否溢出
		LL_TIM_ClearFlag_UPDATE(TIM3);				//清除向上计数溢出标志位
		
		#if __HARDWARE_CONFIG__DMA_ADC1_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
			printf_s_c("add bonus_wave.s0.id,0,%d", DMA_ADC1_RX_BUF[0] >> 8);
		#endif // end of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
		
		timer3_call_cnt ++;
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__


#if 0 //// stm32h7xx_it.c替换
/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER3_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM3_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM3->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM3 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM3_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__
  /* USER CODE END TIM3_IRQn 0 */
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}
#endif
