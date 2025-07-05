#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__	// begin of __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__
#ifndef _SEARCH_CODE_H	// begin of _SEARCH_CODE_H
#define _SEARCH_CODE_H
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

extern uint32_t binary_search(uint32_t *a,uint32_t array_size,uint32_t key);
extern uint32_t linear_search(uint32_t *a,uint32_t array_size,uint32_t key);

//*******************************// end_h                   //************************************//
#endif	// end of _SEARCH_CODE_H
#endif	// end of __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__
