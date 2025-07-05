#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__DMA_ADC3_ENABLE__	// begin of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
#ifndef __DMA_ADC3_H__	// begin of __DMA_ADC3_H__
#define __DMA_ADC3_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//

//! 设置采样通道序列
#define ADC3_Sequence_Channel_Num 			1                             // ADC采样通道数 
#define ADC3_SequencerLength_Num            LL_ADC_REG_SEQ_SCAN_DISABLE   // 采样扫描长度
	/**ADC3 GPIO Configuration
	PC2   ------> ADC3_INP4     LL_ADC_CHANNEL_0   // PC2
	*/
#define ADC3_Sequence_Channel_1 		    LL_ADC_CHANNEL_0   // PC2
#define ADC3_Sequence_Channel_2 		    LL_ADC_CHANNEL_15  // PA3
#define ADC3_Sequence_Channel_3 		    LL_ADC_CHANNEL_16  // PA0
#define ADC3_Sequence_Channel_4 		    LL_ADC_CHANNEL_18  // PA4
#define ADC3_Sequence_Channel_5 		    LL_ADC_CHANNEL_10  // PC0 - PC1

#define ADC3_Sequence_Sample_Period         LL_ADC_SAMPLINGTIME_16CYCLES_5 // 采样周期

#define DMA_ADC3_RX_BUF_SIZE                 64

//*******************************// extern parameters       //************************************//

extern uint16_t DMA_ADC3_RX_BUF[DMA_ADC3_RX_BUF_SIZE];
extern uint32_t dma_adc3_finish_cnt;

//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

void AD_TIM15_Init_adc3();
void Samp_Adc3_Init(uint32_t buff_Addr, uint32_t trans_Num);
void Start_Sample_adc3(void);
void Stop_Sample_adc3(void);
void DMA1_Stream4_IRQHandler_Func();

//*******************************// end_h                   //************************************//
#endif	// end of __DMA_ADC3_H__
#endif	// end of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
