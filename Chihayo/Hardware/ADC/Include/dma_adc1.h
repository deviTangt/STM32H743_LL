#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__DMA_ADC1_ENABLE__	// begin of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
#ifndef __DMA_ADC1_H__	// begin of __DMA_ADC1_H__
#define __DMA_ADC1_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//

//! 设置采样通道序列
#define ADC1_Sequence_Channel_Num 			1                             // ADC采样通道数 
#define ADC1_SequencerLength_Num            LL_ADC_REG_SEQ_SCAN_DISABLE   // 采样扫描长度
	/**ADC1 GPIO Configuration
	PA6   ------> ADC1_INP3     LL_ADC_CHANNEL_3   // PA6
	PB1   ------> ADC1_INP5     LL_ADC_CHANNEL_5   // PB1 - PB0
    PB0   ------> ADC1_INN5
    PC0   ------> ADC1_INP10    LL_ADC_CHANNEL_10  // PC0 - PC1
	PC1   ------> ADC1_INN10
    PA3   ------> ADC1_INP15    LL_ADC_CHANNEL_18  // PA4
    PA0   ------> ADC1_INP16    LL_ADC_CHANNEL_16  // PA0
    PA4   ------> ADC1_INP18    LL_ADC_CHANNEL_18  // PA4
	*/
#define ADC1_Sequence_Channel_1 		    LL_ADC_CHANNEL_3  // PA6
#define ADC1_Sequence_Channel_2 		    LL_ADC_CHANNEL_15  // PA3
#define ADC1_Sequence_Channel_3 		    LL_ADC_CHANNEL_16  // PA0
#define ADC1_Sequence_Channel_4 		    LL_ADC_CHANNEL_18  // PA4
#define ADC1_Sequence_Channel_5 		    LL_ADC_CHANNEL_10  // PC0 - PC1

#define ADC1_Sequence_Sample_Period         LL_ADC_SAMPLINGTIME_16CYCLES_5 // 采样周期

#define DMA_ADC1_RX_BUF_SIZE                64

//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

void AD_TIM15_Init();
void Samp_Adc1_Init(uint32_t buff_Addr, uint32_t trans_Num);
void Start_Sample_adc1(void);
void Stop_Sample_adc1(void);
void DMA1_Stream2_IRQHandler_Func();

//*******************************// end_h                   //************************************//
#endif	// end of __DMA_ADC1_H__
#endif	// end of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
