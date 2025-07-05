#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__	// begin of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
#ifndef __GPU_TEMP_ADC_H__	// begin of __GPU_TEMP_ADC_H__
#define __GPU_TEMP_ADC_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
#include "stm32h7xx_ll_adc.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//

extern uint16_t TS_CAL1;    // 内部温度系数
extern uint16_t TS_CAL2;

extern uint16_t ADC_Value;
extern double Temp_oC;

//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

void gpu_temp_adc3_config_init();
void gpu_temp_adc3_init();
double MPU_Temperature_Get();
void MPU_Temperature_Show();

//*******************************// end_h                   //************************************//
#endif	// end of __GPU_TEMP_ADC_H__
#endif	// end of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
