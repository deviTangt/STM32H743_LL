#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER2_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
#ifndef __USER_TIMER2_H__	// begin of __USER_TIMER2_H__
#define __USER_TIMER2_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
#include "dma_adc1.h"
//*******************************// define statement        //************************************//

#define TIM2CH1_CAPTURE_RECOVERY_TIME       1

//*******************************// extern parameters       //************************************//

extern uint32_t timer2_call_cnt;

extern uint8_t  flag_IC_array_handle_wait ;
extern uint32_t IC_valid_low              ;
extern uint32_t IC_valid_high             ;
extern uint16_t IC_cnt                    ;
extern uint32_t IC_valid_Period           ;

//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//

#define             IC_ARRAY_SIZE   32
extern uint32_t IC_array[IC_ARRAY_SIZE];

//*******************************// extern function         //************************************//

//? 通用定时器Timer2(32位) 
extern inline void timer2_config_init(uint16_t _Prescaler, uint32_t _Autoreload);
extern inline void timer2_start();
extern inline void timer2_stop();

//? 中断服务函数
extern inline void TIM2_IRQHandler_Func(void);

//*******************************// end_h                   //************************************//
#endif	// end of __USER_TIMER2_H__
#endif	// end of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
