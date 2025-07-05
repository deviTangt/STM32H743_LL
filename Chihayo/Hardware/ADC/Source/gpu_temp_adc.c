#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__	// begin of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
//*******************************// include _h files    //************************************//
#include "gpu_temp_adc.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

//-----------------------------------------------------------------
// double MPU_Temperature_Get()
//-----------------------------------------------------------------
uint16_t TS_CAL1;    // �ڲ��¶�ϵ��
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
// ��������: ��ʼ��GPU�ڲ��¶ȴ�����ADC3����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void gpu_temp_adc3_config_init(){
    //? ʹ����Ӧʱ�� 
    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_ADC3);

    LL_ADC_SetOverSamplingScope(ADC3, LL_ADC_OVS_DISABLE); // ��������Χ��������

    //? ADC��ʼ���ṹ������
    LL_ADC_DeInit(ADC3);
    LL_ADC_InitTypeDef ADC_InitStruct              = {0};
                       ADC_InitStruct.Resolution   = LL_ADC_RESOLUTION_16B;
                       ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
    LL_ADC_Init(ADC3, &ADC_InitStruct);

    //? ADC�Ĵ����ṹ������
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct                  = {0};
                           ADC_REG_InitStruct.TriggerSource    = LL_ADC_REG_TRIG_SOFTWARE;
                           ADC_REG_InitStruct.SequencerLength  = LL_ADC_REG_SEQ_SCAN_DISABLE;
                           ADC_REG_InitStruct.SequencerDiscont = DISABLE;
                           ADC_REG_InitStruct.ContinuousMode   = LL_ADC_REG_CONV_SINGLE;
                           ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_PRESERVED;
    LL_ADC_REG_Init(ADC3, &ADC_REG_InitStruct);

    LL_ADC_REG_SetDataTransferMode(ADC3, LL_ADC_REG_DR_TRANSFER); // ���ô���ģʽ�����ݼĴ�������

    //? ADC����������
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct             = {0};
                             ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV6;
                             ADC_CommonInitStruct.Multimode   = LL_ADC_MULTI_INDEPENDENT;
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC3), &ADC_CommonInitStruct);

    //? ADC��ʼ�� 
	  // �˳�����ģʽ
    LL_ADC_DisableDeepPowerDown(ADC3);
    // �����ڲ���ѹ�� 
    LL_ADC_EnableInternalRegulator(ADC3);
    // �ȴ��ڲ���ѹ������ 
    __IO uint32_t wait_loop_index;
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    //? ��������������� 
    LL_ADC_REG_SetSequencerRanks  (ADC3, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_TEMPSENSOR);                // ���ò�������1 ADCͨ��
    LL_ADC_SetChannelSamplingTime (ADC3, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_810CYCLES_5);  // ���ò�������
    LL_ADC_SetChannelSingleDiff   (ADC3, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SINGLE_ENDED);              // ���õ��˿�ADC��˫�˿�ADC���룺��������
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC3), LL_ADC_AWD_CH_TEMPSENSOR_REG);      // ���ò���·�����ڲ��¶ȴ�����

    //? ��ʼ��GPU�ڲ��¶ȴ�����ADC3
    #if __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__ // begin of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
      gpu_temp_adc3_init();
    #endif // end of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
}
//-----------------------------------------------------------------
// void gpu_temp_adc3_init()
//-----------------------------------------------------------------
//
// ��������: ��ʼ��GPU�ڲ��¶ȴ�����ADC3
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ����"adc.c"��"void MX_ADC3_Init(void)" (��main��������"MX_ADC3_Init();")  
//          ��"/* USER CODE BEGIN ADC3_Init 2 */"֮��
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
// ��������: ��ȡMPU�¶�
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: DMAת��ʵ�֣���Ҫ�ȳ�ʼ��ADC3��ʹ��DMA
//
//-----------------------------------------------------------------
double MPU_Temperature_Get(){
    LL_ADC_REG_StartConversion(ADC3);         // �����ڲ��¶ȴ�����ADC����
    while(!LL_ADC_IsActiveFlag_EOS(ADC3));    // �ȴ���������
  
    ADC_Value = LL_ADC_REG_ReadConversionData16(ADC3);
    Temp_oC = ((110.0f - 30.0f) / (TS_CAL2 - TS_CAL1)) * (ADC_Value - TS_CAL1) + 30.0f;

    return Temp_oC;
}
//-----------------------------------------------------------------
// void MPU_Temperature_Show()
//-----------------------------------------------------------------
//
// ��������: ��ʾMPU��ǰ�¶�
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
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
