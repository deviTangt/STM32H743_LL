#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TEST_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TEST_ENABLE__
#ifndef __USER_TEST_H__	// begin of __USER_TEST_H__
#define __USER_TEST_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//

#define fft_adc_n 1024    // 这里采用的1024点FFT，也可以换成256之类的

//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// test main           //************************************//

extern inline void user_test_func();

//*******************************// extern function         //************************************//

void FFT_deal0(uint16_t ad_value[], float *fft_in_1, int data_length);
void BubbleSort(float k[], int n, uint16_t id[]);

//*******************************// end_h                   //************************************//
#endif	// end of __USER_TEST_H__
#endif	// end of __HARDWARE_CONFIG__USER_TEST_ENABLE__
