#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__DMA_ADC3_ENABLE__	// begin of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
//*******************************// include _h files    //************************************//
#include "dma_adc3.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint16_t DMA_ADC3_RX_BUF[DMA_ADC3_RX_BUF_SIZE] __attribute__((section(".ARM.__at_0x24000400")));	//! STM32H743VIT6 下必须改变默认数组存储位置，不然无法使用DMA1
uint32_t dma_adc3_finish_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// void AD_TIM15_Init()
//-----------------------------------------------------------------
//
// 函数功能: TIM15初始化，此定时器用于采样触发(采样率1M)
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void AD_TIM15_Init_adc3(){
	//? 启用TIM时钟
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM15);
	
	//? 定时器结构体配置
	LL_TIM_InitTypeDef LL_TIM_Struct               = {0};
	                   LL_TIM_Struct.Prescaler     = 23;                          //? 预分频系数
	                   LL_TIM_Struct.Autoreload    = 9;                           //? 周期系数
	                   LL_TIM_Struct.CounterMode   = LL_TIM_COUNTERDIRECTION_UP;  // 计数模式：向上递增
	                   LL_TIM_Struct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;   // 时钟分频：1
	LL_TIM_Init(TIM15, &LL_TIM_Struct);

	//? 其他配置
	LL_TIM_EnableARRPreload(TIM15);								// 启动自动装载模式
	LL_TIM_SetClockSource(TIM15, LL_TIM_CLOCKSOURCE_INTERNAL);	// 内部APB1时钟240MHz
	LL_TIM_SetTriggerOutput(TIM15, LL_TIM_TRGO_UPDATE);			// 事件更新
	LL_TIM_DisableCounter(TIM15);								// 失能计数器
}
 
//-----------------------------------------------------------------
// void Samp_Adc3_Init(uint32_t buff_Addr, uint32_t trans_Num)
//-----------------------------------------------------------------
//
// 函数功能: 采样ADC3初始化
// 入口参数1: uint32_t buff_Addr: 采样数据缓存地址
// 入口参数2: uint32_t trans_Num: DMA一次传输大小
// 返 回 值: 无
// 注意事项: ADC3挂载在AHB4总线上，与ADC12不同
//
//-----------------------------------------------------------------
void Samp_Adc3_Init(uint32_t buff_Addr, uint32_t trans_Num){
	//? 使能相应时钟 
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);	// GPIO时钟
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
 
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);	// 设置ADC时钟源
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_ADC3);	// 使能ADC时钟
	#if __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1); // DMA时钟
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__

	//? 配置ADCGPIO初始化
	LL_SYSCFG_OpenAnalogSwitch(LL_SYSCFG_ANALOG_SWITCH_PC2);
	LL_GPIO_InitTypeDef LL_GPIO_Struct      = {0};
	                    LL_GPIO_Struct.Pin  = LL_GPIO_PIN_4;
	                    LL_GPIO_Struct.Mode = LL_GPIO_MODE_ANALOG;
	                    LL_GPIO_Struct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &LL_GPIO_Struct);

	//? DMA配置
	#if __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__
		//? DMA结构体配置 
		LL_DMA_InitTypeDef LL_DMA_Struct                        = {0};
		                   LL_DMA_Struct.PeriphRequest          = LL_DMAMUX1_REQ_ADC3;                // DMA请求外设：ADC3
		                   LL_DMA_Struct.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;  // DMA传输方向：外设到寄存器
		                   LL_DMA_Struct.Mode                   = LL_DMA_MODE_CIRCULAR;               // DMA循环模式：启用
		                   LL_DMA_Struct.Priority               = LL_DMA_PRIORITY_MEDIUM;             // DMA优先级
		                   LL_DMA_Struct.NbData                 = trans_Num;                          // DMA传输数量
		                   LL_DMA_Struct.PeriphOrM2MSrcAddress  = (uint32_t)&ADC3->DR;                // DMA发送数据源：ADC3数据寄存器
		                   LL_DMA_Struct.MemoryOrM2MDstAddress  = (uint32_t)buff_Addr;                // DMA发送目的地：接收缓存
		                   LL_DMA_Struct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;         // DMA发送数据源数据大小：16bit
		                   LL_DMA_Struct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;         // DMA发送目的地数据大小：16bit
		                   LL_DMA_Struct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;          // DMA发送数据源自增：不启用
		                   LL_DMA_Struct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;            // DMA发送目的地自增：启用
		LL_DMA_Init(DMA1, LL_DMA_STREAM_4, &LL_DMA_Struct);  
		//? 开启DMA传输完成中断 
		#if __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_TC_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_TC_ENABLE__
			LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_4);
		#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_TC_ENABLE__

		//? 配置中断优先级 
		NVIC_SetPriority(DMA1_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
		NVIC_EnableIRQ(DMA1_Stream4_IRQn);
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__

	//? ADC初始化 
	// 退出掉电模式
	LL_ADC_DisableDeepPowerDown(ADC3); 	
	// 启动内部稳压器 
	LL_ADC_EnableInternalRegulator(ADC3); 
	// 等待内部稳压器启动 
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while(wait_loop_index != 0)  wait_loop_index--;	
	
	//? ADC常规组设置
			LL_ADC_CommonInitTypeDef ADC_CommonInitStruct             = {0};
			                         ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV1;   //? 设置ADC分频系数
			                         ADC_CommonInitStruct.Multimode   = LL_ADC_MULTI_INDEPENDENT;  // ADC多通道独立模式
  	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC3), &ADC_CommonInitStruct);	
	
	// BOOST 位控制
	LL_ADC_SetBoostMode(ADC3, LL_ADC_BOOST_MODE_50MHZ);

	//? 采样率设置
	LL_ADC_InitTypeDef ADC_InitStruct              = {0};
	                   ADC_InitStruct.Resolution   = LL_ADC_RESOLUTION_16B;       //? 采样分辨率：16bit
	                   ADC_InitStruct.LeftBitShift = LL_ADC_LEFT_BIT_SHIFT_NONE;  // 数据左移动：不启用
	                   ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;         // 低功耗模式：不启用
	LL_ADC_Init(ADC3, &ADC_InitStruct);

	//! 启动校准
  	LL_ADC_StartCalibration(ADC3, LL_ADC_CALIB_OFFSET, LL_ADC_SINGLE_ENDED);  
	// 等待校准完成
	while(LL_ADC_IsCalibrationOnGoing(ADC3) != 0UL);	

	//? 规则采样参数配置 
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct                  = {0};
	                       ADC_REG_InitStruct.TriggerSource    = LL_ADC_REG_TRIG_EXT_TIM15_TRGO;      //? ADC触发源：TIM15更新事件触发
	                       ADC_REG_InitStruct.DataTransferMode = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;  // 数据传输模式：无限制DMA传输
	                       ADC_REG_InitStruct.ContinuousMode   = LL_ADC_REG_CONV_SINGLE;             // ADC采样连续模式：单次非连续
	                       ADC_REG_InitStruct.SequencerLength  = ADC3_SequencerLength_Num;            //! ADC采样序列长度：扫描
	                       ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;     // ADC采样序列离散
	                       ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_OVERWRITTEN;    // ADC过采样：数据覆盖
	LL_ADC_REG_Init(ADC3, &ADC_REG_InitStruct);
	LL_ADC_REG_SetTriggerEdge(ADC3, LL_ADC_REG_TRIG_EXT_RISING);  // 设置触发源模式：上升沿触发

	//? 配置规则通道 
	LL_ADC_REG_SetSequencerRanks  (ADC3, LL_ADC_REG_RANK_1, ADC3_Sequence_Channel_1);          // 设置采样序列1 ADC通道
	LL_ADC_SetChannelSamplingTime(ADC3, ADC3_Sequence_Channel_1, ADC3_Sequence_Sample_Period);  // 设置采样周期
	LL_ADC_SetChannelSingleDiff   (ADC3, ADC3_Sequence_Channel_1, LL_ADC_SINGLE_ENDED);        // 设置单端口ADC或双端口ADC输入：单端输入

	LL_ADC_REG_SetSequencerRanks  (ADC3, LL_ADC_REG_RANK_2, ADC3_Sequence_Channel_2);          // 设置采样序列2 ADC通道
	LL_ADC_SetChannelSamplingTime(ADC3, ADC3_Sequence_Channel_2, ADC3_Sequence_Sample_Period);  // 设置采样周期
	LL_ADC_SetChannelSingleDiff   (ADC3, ADC3_Sequence_Channel_2, LL_ADC_SINGLE_ENDED);        // 设置单端口ADC或双端口ADC输入：单端输入

	LL_ADC_REG_SetSequencerRanks  (ADC3, LL_ADC_REG_RANK_3, ADC3_Sequence_Channel_3);          // 设置采样序列3 ADC通道
	LL_ADC_SetChannelSamplingTime(ADC3, ADC3_Sequence_Channel_3, ADC3_Sequence_Sample_Period);  // 设置采样周期
	LL_ADC_SetChannelSingleDiff   (ADC3, ADC3_Sequence_Channel_3, LL_ADC_SINGLE_ENDED);        // 设置单端口ADC或双端口ADC输入：单端输入

	LL_ADC_REG_SetSequencerRanks  (ADC3, LL_ADC_REG_RANK_4, ADC3_Sequence_Channel_4);          // 设置采样序列4 ADC通道
	LL_ADC_SetChannelSamplingTime(ADC3, ADC3_Sequence_Channel_4, ADC3_Sequence_Sample_Period);  // 设置采样周期
	LL_ADC_SetChannelSingleDiff   (ADC3, ADC3_Sequence_Channel_4, LL_ADC_DIFFERENTIAL_ENDED);  // 设置单端口ADC或双端口ADC输入：差分输入

	LL_ADC_REG_SetSequencerRanks  (ADC3, LL_ADC_REG_RANK_5, ADC3_Sequence_Channel_5);          // 设置采样序列5 ADC通道
	LL_ADC_SetChannelSamplingTime(ADC3, ADC3_Sequence_Channel_5, ADC3_Sequence_Sample_Period);  // 设置采样周期
	LL_ADC_SetChannelSingleDiff   (ADC3, ADC3_Sequence_Channel_5, LL_ADC_DIFFERENTIAL_ENDED);  // 设置单端口ADC或双端口ADC输入：差分输入

	//? 通道预选设置，这个很关键
	ADC3->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC3_Sequence_Channel_1) & 0x1FUL));
	ADC3->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC3_Sequence_Channel_2) & 0x1FUL));
	ADC3->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC3_Sequence_Channel_3) & 0x1FUL));  
	ADC3->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC3_Sequence_Channel_4) & 0x1FUL));  
	ADC3->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC3_Sequence_Channel_5) & 0x1FUL));  

	//? 设置ADC传输方式
	#if __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__
		LL_ADC_REG_SetDataTransferMode(ADC3, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);						  // DMA无限制传输
	#else
		LL_ADC_REG_SetDataTransferMode(ADC3, LL_ADC_REG_DR_TRANSFER);                                     // 数据寄存器传输
		LL_ADC_EnableIT_EOC           (ADC3);                                                             // 使能E0C中断
		NVIC_SetPriority              (ADC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));  // 设置中断优先级
		NVIC_EnableIRQ                (ADC_IRQn);                                                         // 开始中断服务函数
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__

	//? 初始化采样定时器 
	//AD_TIM15_Init_adc3();
}
 
//-----------------------------------------------------------------
// void Start_Sample_adc3()
//-----------------------------------------------------------------
//
// 函数功能: 开始采样
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Start_Sample_adc3(){
	#if __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__	
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);						 // 使能DMA
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__

	LL_ADC_Enable             (ADC3);                                    // 使能ADC
	while                     (LL_ADC_IsActiveFlag_ADRDY(ADC3) != SET);  // 等待ADC准备就绪
	LL_ADC_REG_StartConversion(ADC3);                                    // 开启ADC转换
	LL_TIM_EnableCounter      (TIM15);                                    // 启用定时器
}
//-----------------------------------------------------------------
// void Stop_Sample_adc3()
//-----------------------------------------------------------------
//
// 函数功能: 停止采样
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Stop_Sample_adc3(){
	LL_TIM_DisableCounter     (TIM15);                                       // 关闭定时器
	LL_ADC_REG_StopConversion (ADC3);                                       // 停止ADC转换
	while(LL_ADC_REG_IsConversionOngoing(ADC3) != 0);  						// 等待转换停止
	LL_ADC_Disable            (ADC3);                                       // 关闭ADC
	LL_ADC_EnableDeepPowerDown(ADC3);                                       // 令ADC处于低功耗状态

	#if __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);  // 关闭DMA通道
		LL_DMA_ClearFlag_TC4(DMA1);                   // 清除TC标志位
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__
}
 
#if __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__
//-----------------------------------------------------------------
// void DMA1_Stream4_IRQHandler_Func()
//-----------------------------------------------------------------
//
// 函数功能: DMA1_Stream4中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void DMA1_Stream4_IRQHandler_Func(){
	if(LL_DMA_IsActiveFlag_TC4(DMA1) == SET){ // 用于检测采样率是否准确的（1M采样率时，DMA传输1000个数据大概时间是1ms）
		LL_DMA_ClearFlag_TC4(DMA1);	   
    
        dma_adc3_finish_cnt ++;
	}
}
 
#else
//-----------------------------------------------------------------
// void ADC_IRQHandler(void)
//-----------------------------------------------------------------
//
// 函数功能: ADC3的中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADC_IRQHandler(void)
{
	if(LL_ADC_IsActiveFlag_EOC(ADC3) != RESET){ // 不使用DMA时，在这里面获取采样值
       LL_ADC_ClearFlag_EOC(ADC3);
      
	}
}
 
#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__


//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
