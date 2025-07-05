#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER4_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__
#ifndef __USER_TIMER4_H__	// begin of __USER_TIMER4_H__
#define __USER_TIMER4_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
#include "user_test.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//

extern uint32_t timer4_call_cnt;

extern uint32_t sample_ad_value[300];
extern uint16_t sample_ad_cnt;

//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

//? 通用定时器Timer4(16位)
extern inline void timer4_config_init(uint16_t _Prescaler, uint32_t _Autoreload);
extern inline void timer4_start();
extern inline void timer4_stop();

//? 中断服务函数
extern inline void TIM4_IRQHandler_Func(void);

//*******************************// end_h                   //************************************//
#endif	// end of __USER_TIMER4_H__
#endif	// end of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__
