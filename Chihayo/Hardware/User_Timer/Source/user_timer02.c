#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER2_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer02.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer2_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer2_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// 函数功能: timer2定时器配置初始化
// 入口参数1: uint16_t _Prescaler:  预分频值
// 入口参数2: uint32_t _Autoreload: 重载系数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer2_config_init(uint16_t _Prescaler, uint32_t _Autoreload){ // 输入捕获
	//? 初始化时钟
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

	//? 配置中断
	NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(TIM2_IRQn);

	/**TIM2 GPIO Configuration
	 PA0   ------> TIM2_CH1
	*/
	//? 初始化GPIO
	LL_GPIO_InitTypeDef GPIO_InitStruct            = {0};
						GPIO_InitStruct.Pin        = LL_GPIO_PIN_0;
						GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
						GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
						GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
						GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
						GPIO_InitStruct.Alternate  = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//? 配置定时器结构体 5ms
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // 预分频
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // 计数模式：向上计数
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // 自动重装载值      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // 时钟分频
	LL_TIM_Init(TIM2, &TIM_InitStruct);


	//? 其他配置
	LL_TIM_EnableARRPreload      (TIM2);                                                   // 启动自动装载模式
	LL_TIM_SetClockSource         (TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);                     // 设置时钟源：内部晶振
	LL_TIM_SetTriggerOutput       (TIM2, LL_TIM_TRGO_RESET);                               // 复位更新
	LL_TIM_DisableMasterSlaveMode(TIM2);                                                   // 禁用主从模式
	LL_TIM_IC_SetActiveInput     (TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);  // 设置外部直接触发
	LL_TIM_IC_SetPrescaler        (TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);           // 预分频
	LL_TIM_IC_SetFilter           (TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);      // 极性
	LL_TIM_IC_SetPolarity         (TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);   // 上升沿触发

    //? UPDATE标志位
	#if __HARDWARE_CONFIG__TIMER2_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER2_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM2);  // 清除向上计数溢出标志位
		LL_TIM_EnableIT_UPDATE (TIM2);  // 使能定时器向上计数中断
  	#endif // end of __HARDWARE_CONFIG__TIMER2_INT_UPDATE_ENABLE__
	//? 输入捕获中断
	#if __HARDWARE_CONFIG__TIMER2_INT_CAPTURE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER2_INT_CAPTURE_ENABLE__
		LL_TIM_ClearFlag_CC1    (TIM2);                    // 清除捕获标志位
		LL_TIM_EnableIT_CC1     (TIM2);                    // 捕获通道使能
		LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH1);  // 通道使能
	#endif // end of __HARDWARE_CONFIG__TIMER2_INT_CAPTURE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer2_start()
//-----------------------------------------------------------------
//
// 函数功能: 开启定时器2
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer2_start(){
    if(__HARDWARE_CONFIG__TIMER2_INT_UPDATE_ENABLE__)
		LL_TIM_ClearFlag_UPDATE(TIM2);  // 清除向上计数溢出标志位
    LL_TIM_EnableCounter   (TIM2);  // 使能定时器开始计数
}
//-----------------------------------------------------------------
// inline void timer2_stop()
//-----------------------------------------------------------------
//
// 函数功能: 关闭定时器2
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void timer2_stop(){
    if(__HARDWARE_CONFIG__TIMER2_INT_UPDATE_ENABLE__)
		LL_TIM_ClearFlag_UPDATE(TIM2);   // 清除向上计数溢出标志位
    LL_TIM_DisableCounter   (TIM2);  // 关闭定时器计数
}

//-----------------------------------------------------------------
// inline void TIM2_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// 函数功能: 定时器2中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 将此函数加入定时器中断服务函数TIM2_IRQHandler(void)中
//
//-----------------------------------------------------------------
inline void TIM2_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == SET){	//判断定时器是否溢出
		LL_TIM_ClearFlag_UPDATE(TIM2);				//清除向上计数溢出标志位
		
        timer2_call_cnt ++;
	}
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__



#if 0 //// stm32h7xx_it.c替换
/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER2_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM2_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM2->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM3 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM2_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}
#endif
