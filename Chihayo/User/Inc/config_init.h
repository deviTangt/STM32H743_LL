#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__	// begin of __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__
#ifndef __CONFIG_INIT_H__	// begin of __CONFIG_INIT_H__
#define __CONFIG_INIT_H__
//*******************************// include extern .h files //************************************//
#include "main.h"

#include "adc.h"
#include "dma.h"
#include "memorymap.h"
#include "usart.h"
#include "gpio.h"
//#include "spi.h"

#include "LED.h"
#include "KEY.h"
#include "TJC_USART.h"
#include "TFTLCD_Init.h"
#include "TFTLCD.h"
#include "dma_adc1.h"
#include "dma_adc2.h"
#include "dma_adc3.h"
#include "user_timer01.h"
#include "user_timer02.h"
#include "user_timer03.h"
#include "user_timer04.h"
#include "user_timer05.h"
#include "user_timer06.h"
#include "user_timer07.h"
#include "user_timer08.h"
#include "user_timer12.h"
#include "user_timer13.h"
#include "user_timer14.h"
#include "user_timer15.h"
#include "user_timer16.h"
#include "user_timer17.h"
#include "spi_fpga.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

extern inline void config_init();
extern inline void config_user_timer_init();
extern inline void config_adc_sample_init();

//*******************************// end_h                   //************************************//
#endif	// end of __CONFIG_INIT_H__
#endif	// end of __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__
