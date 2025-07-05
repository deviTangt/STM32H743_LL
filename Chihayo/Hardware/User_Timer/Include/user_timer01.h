#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER1_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__
#ifndef __USER_TIMER1_H__	// begin of __USER_TIMER1_H__
#define __USER_TIMER1_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//

extern uint32_t timer1_call_cnt;

//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

//? 高级控制定时器Timer1(16位) 
extern inline void timer1_config_init(uint16_t _Prescaler, uint32_t _Autoreload);
extern inline void timer1_start();s
extern inline void timer1_stop();

//? 中断服务函数
extern inline void TIM1_IRQHandler_Func(void);

//*******************************// end_h                   //************************************//
#endif	// end of __USER_TIMER1_H__
#endif	// end of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__
