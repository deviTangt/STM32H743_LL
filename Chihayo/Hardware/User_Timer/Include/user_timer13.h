#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER13_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
#ifndef __USER_TIMER13_H__	// begin of __USER_TIMER13_H__
#define __USER_TIMER13_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//

extern uint32_t timer13_call_cnt;

//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

//? ͨ�ö�ʱ��Timer13(16λ)
extern inline void timer13_config_init(uint16_t _Prescaler, uint32_t _Autoreload);
extern inline void timer13_start();
extern inline void timer13_stop();
s
//? �жϷ�����
extern inline void TIM13_IRQHandler_Func(void);

//*******************************// end_h                   //************************************//
#endif	// end of __USER_TIMER13_H__
#endif	// end of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
