#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__DMA_ADC1_ENABLE__	// begin of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
//*******************************// include _h files    //************************************//
#include "dma_adc1.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint16_t DMA_ADC1_RX_BUF[DMA_ADC1_RX_BUF_SIZE] __attribute__ ((section ("DMA_ADC1_RX_BUF"),  zero_init));	//! STM32H743VIT6 �±���ı�Ĭ������洢λ�ã���Ȼ�޷�ʹ��DMA1
uint32_t dma_adc1_finish_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// void AD_TIM15_Init()
//-----------------------------------------------------------------
//
// ��������: TIM15��ʼ�����˶�ʱ�����ڲ�������(������1M)
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void AD_TIM15_Init(){
	//? ����TIMʱ��
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM15);
	
	//? ��ʱ���ṹ������
	LL_TIM_InitTypeDef LL_TIM_Struct               = {0};
	                   LL_TIM_Struct.Prescaler     = 24 - 1;                      //? Ԥ��Ƶϵ��
	                   LL_TIM_Struct.Autoreload    = 100 - 1;                     //? ����ϵ��
	                   LL_TIM_Struct.CounterMode   = LL_TIM_COUNTERDIRECTION_UP;  // ����ģʽ�����ϵ���
	                   LL_TIM_Struct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;   // ʱ�ӷ�Ƶ��1
	LL_TIM_Init(TIM15, &LL_TIM_Struct);

	//? ��������
	LL_TIM_EnableARRPreload(TIM15);								// �����Զ�װ��ģʽ
	LL_TIM_SetClockSource(TIM15, LL_TIM_CLOCKSOURCE_INTERNAL);	// �ڲ�APB1ʱ��240MHz
	LL_TIM_SetTriggerOutput(TIM15, LL_TIM_TRGO_UPDATE);			// �¼�����
	LL_TIM_DisableCounter(TIM15);								// ʧ�ܼ�����
}
 
//-----------------------------------------------------------------
// void Samp_Adc1_Init(uint32_t buff_Addr, uint32_t trans_Num)
//-----------------------------------------------------------------
//
// ��������: ����ADC1��ʼ��
// ��ڲ���1: uint32_t buff_Addr: �������ݻ����ַ
// ��ڲ���2: uint32_t trans_Num: DMAһ�δ����С
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Samp_Adc1_Init(uint32_t buff_Addr, uint32_t trans_Num){
	//? ʹ����Ӧʱ�� 
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);	// GPIOʱ��
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
  	
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);	// ����ADCʱ��Դ
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ADC12);	// ʹ��ADCʱ��
	#if __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1); // DMAʱ��
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__

	//? ����ADCGPIO��ʼ��
	LL_GPIO_InitTypeDef LL_GPIO_Struct      = {0};
	                    LL_GPIO_Struct.Pin  = LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_6;
	                    LL_GPIO_Struct.Mode = LL_GPIO_MODE_ANALOG;
	                    LL_GPIO_Struct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &LL_GPIO_Struct);

	                    LL_GPIO_Struct.Pin  = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
	                    LL_GPIO_Struct.Mode = LL_GPIO_MODE_ANALOG;
	                    LL_GPIO_Struct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &LL_GPIO_Struct);

	                    LL_GPIO_Struct.Pin  = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
	                    LL_GPIO_Struct.Mode = LL_GPIO_MODE_ANALOG;
	                    LL_GPIO_Struct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &LL_GPIO_Struct);

	//? DMA����
	#if __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__
		//? DMA�ṹ������ 
		LL_DMA_InitTypeDef LL_DMA_Struct                        = {0};
		                   LL_DMA_Struct.PeriphRequest          = LL_DMAMUX1_REQ_ADC1;                // DMA�������裺ADC1
		                   LL_DMA_Struct.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;  // DMA���䷽�����赽�Ĵ���
		                   LL_DMA_Struct.Mode                   = LL_DMA_MODE_CIRCULAR;               // DMAѭ��ģʽ������
		                   LL_DMA_Struct.Priority               = LL_DMA_PRIORITY_MEDIUM;             // DMA���ȼ�
		                   LL_DMA_Struct.NbData                 = trans_Num;                          // DMA��������
		                   LL_DMA_Struct.PeriphOrM2MSrcAddress  = (uint32_t)&ADC1->DR;                // DMA��������Դ��ADC1���ݼĴ���
		                   LL_DMA_Struct.MemoryOrM2MDstAddress  = (uint32_t)buff_Addr;                // DMA����Ŀ�ĵأ����ջ���
		                   LL_DMA_Struct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;         // DMA��������Դ���ݴ�С��16bit
		                   LL_DMA_Struct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;         // DMA����Ŀ�ĵ����ݴ�С��16bit
		                   LL_DMA_Struct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;          // DMA��������Դ������������
		                   LL_DMA_Struct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;            // DMA����Ŀ�ĵ�����������
		LL_DMA_Init(DMA1, LL_DMA_STREAM_2, &LL_DMA_Struct);  
		//? ����DMA��������ж� 
		#if __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_TC_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_TC_ENABLE__
			LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_2);
		#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_TC_ENABLE__

		//? �����ж����ȼ� 
		NVIC_SetPriority(DMA1_Stream2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
		NVIC_EnableIRQ(DMA1_Stream2_IRQn);
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__

	//? ADC��ʼ�� 
	// �˳�����ģʽ
	LL_ADC_DisableDeepPowerDown(ADC1); 	
	// �����ڲ���ѹ�� 
	LL_ADC_EnableInternalRegulator(ADC1); 
	// �ȴ��ڲ���ѹ������ 
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while(wait_loop_index != 0)  wait_loop_index--;	
	
	//? ADC����������
			LL_ADC_CommonInitTypeDef ADC_CommonInitStruct             = {0};
			                         ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV1;   //? ����ADC��Ƶϵ��
			                         ADC_CommonInitStruct.Multimode   = LL_ADC_MULTI_INDEPENDENT;  // ADC��ͨ������ģʽ
  	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);	
	
	// BOOST λ����
	LL_ADC_SetBoostMode(ADC1, LL_ADC_BOOST_MODE_50MHZ);

	//? ����������
	LL_ADC_InitTypeDef ADC_InitStruct              = {0};
	                   ADC_InitStruct.Resolution   = LL_ADC_RESOLUTION_16B;       //? �����ֱ��ʣ�16bit
	                   ADC_InitStruct.LeftBitShift = LL_ADC_LEFT_BIT_SHIFT_NONE;  // �������ƶ���������
	                   ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;         // �͹���ģʽ��������
	LL_ADC_Init(ADC1, &ADC_InitStruct);

	//! ����У׼
  	LL_ADC_StartCalibration(ADC1, LL_ADC_CALIB_OFFSET, LL_ADC_SINGLE_ENDED);  
	// �ȴ�У׼���
	while(LL_ADC_IsCalibrationOnGoing(ADC1) != 0UL);	

	//? ��������������� 
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct                  = {0};
	                       ADC_REG_InitStruct.TriggerSource    = LL_ADC_REG_TRIG_EXT_TIM15_TRGO;      //? ADC����Դ��TIM15�����¼�����
	                       ADC_REG_InitStruct.DataTransferMode = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;  // ���ݴ���ģʽ��������DMA����
	                       ADC_REG_InitStruct.ContinuousMode   = LL_ADC_REG_CONV_SINGLE;             // ADC��������ģʽ�����η�����
	                       ADC_REG_InitStruct.SequencerLength  = ADC1_SequencerLength_Num;            //! ADC�������г��ȣ�ɨ��
	                       ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;     // ADC����������ɢ
	                       ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_OVERWRITTEN;    // ADC�����������ݸ���
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
	LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);  // ���ô���Դģʽ�������ش���

	//? ���ù���ͨ�� 
	LL_ADC_REG_SetSequencerRanks  (ADC1, LL_ADC_REG_RANK_1, ADC1_Sequence_Channel_1);          // ���ò�������1 ADCͨ��
	LL_ADC_SetChannelSamplingTime(ADC1, ADC1_Sequence_Channel_1, ADC1_Sequence_Sample_Period);  // ���ò�������
	LL_ADC_SetChannelSingleDiff   (ADC1, ADC1_Sequence_Channel_1, LL_ADC_SINGLE_ENDED);        // ���õ��˿�ADC��˫�˿�ADC���룺��������

	LL_ADC_REG_SetSequencerRanks  (ADC1, LL_ADC_REG_RANK_2, ADC1_Sequence_Channel_2);           // ���ò�������2 ADCͨ��
	LL_ADC_SetChannelSamplingTime(ADC1, ADC1_Sequence_Channel_2, ADC1_Sequence_Sample_Period);  // ���ò�������
	LL_ADC_SetChannelSingleDiff   (ADC1, ADC1_Sequence_Channel_2, LL_ADC_SINGLE_ENDED);         // ���õ��˿�ADC��˫�˿�ADC���룺��������

	LL_ADC_REG_SetSequencerRanks  (ADC1, LL_ADC_REG_RANK_3, ADC1_Sequence_Channel_3);           // ���ò�������3 ADCͨ��
	LL_ADC_SetChannelSamplingTime(ADC1, ADC1_Sequence_Channel_3, ADC1_Sequence_Sample_Period);  // ���ò�������
	LL_ADC_SetChannelSingleDiff   (ADC1, ADC1_Sequence_Channel_3, LL_ADC_SINGLE_ENDED);         // ���õ��˿�ADC��˫�˿�ADC���룺��������

	LL_ADC_REG_SetSequencerRanks  (ADC1, LL_ADC_REG_RANK_4, ADC1_Sequence_Channel_4);           // ���ò�������4 ADCͨ��
	LL_ADC_SetChannelSamplingTime(ADC1, ADC1_Sequence_Channel_4, ADC1_Sequence_Sample_Period);  // ���ò�������
	LL_ADC_SetChannelSingleDiff   (ADC1, ADC1_Sequence_Channel_4, LL_ADC_DIFFERENTIAL_ENDED);   // ���õ��˿�ADC��˫�˿�ADC���룺�������

	LL_ADC_REG_SetSequencerRanks  (ADC1, LL_ADC_REG_RANK_5, ADC1_Sequence_Channel_5);           // ���ò�������5 ADCͨ��
	LL_ADC_SetChannelSamplingTime(ADC1, ADC1_Sequence_Channel_5, ADC1_Sequence_Sample_Period);  // ���ò�������
	LL_ADC_SetChannelSingleDiff   (ADC1, ADC1_Sequence_Channel_5, LL_ADC_DIFFERENTIAL_ENDED);   // ���õ��˿�ADC��˫�˿�ADC���룺�������

	//? ͨ��Ԥѡ���ã�����ܹؼ�
	ADC1->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC1_Sequence_Channel_1) & 0x1FUL));
	ADC1->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC1_Sequence_Channel_2) & 0x1FUL));
	ADC1->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC1_Sequence_Channel_3) & 0x1FUL));  
	ADC1->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC1_Sequence_Channel_4) & 0x1FUL));  
	ADC1->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(ADC1_Sequence_Channel_5) & 0x1FUL));  

	//? ����ADC���䷽ʽ
	#if __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__
		LL_ADC_REG_SetDataTransferMode(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);						  // DMA�����ƴ���
	#else
		LL_ADC_REG_SetDataTransferMode(ADC1, LL_ADC_REG_DR_TRANSFER);                                     // ���ݼĴ�������
		LL_ADC_EnableIT_EOC           (ADC1);                                                             // ʹ��E0C�ж�
		NVIC_SetPriority              (ADC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));  // �����ж����ȼ�
		NVIC_EnableIRQ                (ADC_IRQn);                                                         // ��ʼ�жϷ�����
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__

	//? ��ʼ��������ʱ�� 
	AD_TIM15_Init();
}
 
//-----------------------------------------------------------------
// void Start_Sample_adc1()
//-----------------------------------------------------------------
//
// ��������: ��ʼ����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Start_Sample_adc1(){
	#if __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__	
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);						 // ʹ��DMA
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__

	LL_ADC_Enable             (ADC1);                                    // ʹ��ADC
	while                     (LL_ADC_IsActiveFlag_ADRDY(ADC1) != SET);  // �ȴ�ADC׼������
	LL_ADC_REG_StartConversion(ADC1);                                    // ����ADCת��
	LL_TIM_EnableCounter      (TIM15);                                    // ���ö�ʱ��
}
//-----------------------------------------------------------------
// void Stop_Sample_adc1()
//-----------------------------------------------------------------
//
// ��������: ֹͣ����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Stop_Sample_adc1(){
	LL_TIM_DisableCounter     (TIM15);                                       // �رն�ʱ��
	LL_ADC_REG_StopConversion (ADC1);                                       // ֹͣADCת��
	while(LL_ADC_REG_IsConversionOngoing(ADC1) != 0);  						// �ȴ�ת��ֹͣ
	LL_ADC_Disable            (ADC1);                                       // �ر�ADC
	LL_ADC_EnableDeepPowerDown(ADC1);                                       // ��ADC���ڵ͹���״̬

	#if __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);  // �ر�DMAͨ��
		LL_DMA_ClearFlag_TC2(DMA1);                   // ���TC��־λ
	#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__
}
 
#if __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__
//-----------------------------------------------------------------
// void DMA1_Stream2_IRQHandler_Func()
//-----------------------------------------------------------------
//
// ��������: DMA1_Stream2�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void DMA1_Stream2_IRQHandler_Func(){
	if(LL_DMA_IsActiveFlag_TC2(DMA1) == SET){ // ���ڼ��������Ƿ�׼ȷ�ģ�1M������ʱ��DMA����1000�����ݴ��ʱ����1ms��
		LL_DMA_ClearFlag_TC2(DMA1);	   
    
        dma_adc1_finish_cnt ++;
	}
}
 
#else
//-----------------------------------------------------------------
// void ADC_IRQHandler(void)
//-----------------------------------------------------------------
//
// ��������: ADC1���жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADC_IRQHandler(void)
{
	if(LL_ADC_IsActiveFlag_EOC(ADC1) != RESET){ // ��ʹ��DMAʱ�����������ȡ����ֵ
       LL_ADC_ClearFlag_EOC(ADC1);
      
	}
}
 
#endif // end of __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__


//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
