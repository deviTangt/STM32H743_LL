#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__LED_ENABLE__ // begin of __HARDWARE_CONFIG__TFTLCD_ENABLE__
#ifndef __LED_H__ // begin of __LED_H__
#define __LED_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

extern inline void LED_ON();
extern inline void LED_OFF();
extern inline void LED_TURN();

//*******************************// end_h                   //************************************//
#endif // begin of __LED_H__
#endif // end of __HARDWARE_CONFIG__TFTLCD_ENABLE__
 