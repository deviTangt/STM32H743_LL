#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__	// begin of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
//*******************************// include _h files    //************************************//
#include "gpu_temp_adc.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

//-----------------------------------------------------------------
// double MPU_Temperature_Get()
//-----------------------------------------------------------------
uint16_t TS_CAL1;    // 内部温度系数
uint16_t TS_CAL2;

uint16_t ADC_Value;
double Temp_oC;
//-----------------------------------------------------------------
// void MPU_Temperature_Show()
//-----------------------------------------------------------------
double Temp_MPU;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// void delay_us()
//-----------------------------------------------------------------
//
// 函数功能: 初始化GPU内部温度传感器ADC3配置
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void gpu_temp_adc3_config_init(){
    //? 使能相应时钟 
    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_ADC3);

    LL_ADC_SetOverSamplingScope(ADC3, LL_ADC_OVS_DISABLE); // 过采样范围：不启用

    //? ADC初始化结构体配置
    LL_ADC_DeInit(ADC3);
    LL_ADC_InitTypeDef ADC_InitStruct              = {0};
                       ADC_InitStruct.Resolution   = LL_ADC_RESOLUTION_16B;
                       ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
    LL_ADC_Init(ADC3, &ADC_InitStruct);

    //? ADC寄存器结构体配置
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct                  = {0};
                           ADC_REG_InitStruct.TriggerSource    = LL_ADC_REG_TRIG_SOFTWARE;
                           ADC_REG_InitStruct.SequencerLength  = LL_ADC_REG_SEQ_SCAN_DISABLE;
                           ADC_REG_InitStruct.SequencerDiscont = DISABLE;
                           ADC_REG_InitStruct.ContinuousMode   = LL_ADC_REG_CONV_SINGLE;
                           ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_PRESERVED;
    LL_ADC_REG_Init(ADC3, &ADC_REG_InitStruct);

    LL_ADC_REG_SetDataTransferMode(ADC3, LL_ADC_REG_DR_TRANSFER); // 设置传输模式：数据寄存器传输

    //? ADC常规组配置
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct             = {0};
                             ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV6;
                             ADC_CommonInitStruct.Multimode   = LL_ADC_MULTI_INDEPENDENT;
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC3), &ADC_CommonInitStruct);

    //? ADC初始化 
	  // 退出掉电模式
    LL_ADC_DisableDeepPowerDown(ADC3);
    // 启动内部稳压器 
    LL_ADC_EnableInternalRegulator(ADC3);
    // 等待内部稳压器启动 
    __IO uint32_t wait_loop_index;
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    //? 规则采样参数配置 
    LL_ADC_REG_SetSequencerRanks  (ADC3, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_TEMPSENSOR);                // 设置采样序列1 ADC通道
    LL_ADC_SetChannelSamplingTime (ADC3, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_810CYCLES_5);  // 设置采样周期
    LL_ADC_SetChannelSingleDiff   (ADC3, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SINGLE_ENDED);              // 设置单端口ADC或双端口ADC输入：单端输入
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC3), LL_ADC_AWD_CH_TEMPSENSOR_REG);      // 设置采样路径：内部温度传感器

    //? 初始化GPU内部温度传感器ADC3
    #if __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__ // begin of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
      gpu_temp_adc3_init();
    #endif // end of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
}
//-----------------------------------------------------------------
// void gpu_temp_adc3_init()
//-----------------------------------------------------------------
//
// 函数功能: 初始化GPU内部温度传感器ADC3
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 放在"adc.c"的"void MX_ADC3_Init(void)" (即main函数调用"MX_ADC3_Init();")  
//          的"/* USER CODE BEGIN ADC3_Init 2 */"之后
//
//-----------------------------------------------------------------
void gpu_temp_adc3_init(){
    LL_ADC_StartCalibration(ADC3, LL_ADC_CALIB_OFFSET, LL_ADC_SINGLE_ENDED);  // calibration
    while(LL_ADC_IsCalibrationOnGoing(ADC3));                                 // wait for the end of calibration
    LL_ADC_Enable(ADC3);                                                      // enable adc3
    
    //-----------------------------------------------------------------
    // double MPU_Temperature_Get()
    //-----------------------------------------------------------------
    TS_CAL1 = *(__IO uint16_t *)(0x1FF1E820);
    TS_CAL2 = *(__IO uint16_t *)(0x1FF1E840);
}
//-----------------------------------------------------------------
// double MPU_Temperature_Get()
//-----------------------------------------------------------------
//
// 函数功能: 获取MPU温度
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: DMA转移实现，需要先初始化ADC3和使能DMA
//
//-----------------------------------------------------------------
double MPU_Temperature_Get(){
    LL_ADC_REG_StartConversion(ADC3);         // 启动内部温度传感器ADC工作
    while(!LL_ADC_IsActiveFlag_EOS(ADC3));    // 等待启动传输
  
    ADC_Value = LL_ADC_REG_ReadConversionData16(ADC3);
    Temp_oC = ((110.0f - 30.0f) / (TS_CAL2 - TS_CAL1)) * (ADC_Value - TS_CAL1) + 30.0f;

    return Temp_oC;
}
//-----------------------------------------------------------------
// void MPU_Temperature_Show()
//-----------------------------------------------------------------
//
// 函数功能: 显示MPU当前温度
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void MPU_Temperature_Show(){
  Temp_MPU = MPU_Temperature_Get();
  if (10.0 < Temp_MPU && Temp_MPU < 100.0){
    printf_s(19, "MPU Temp:%3.5f Celsius", Temp_MPU);
  }
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
