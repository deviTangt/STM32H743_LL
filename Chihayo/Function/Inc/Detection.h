#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__DETECTION_ENABLE__	// begin of __HARDWARE_CONFIG__DETECTION_ENABLE__
#ifndef __DETECTION_H__	// begin of ____DETECTION_H___H__
#define ____DETECTION_H___H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//

extern uint8_t detect_mode;
extern uint32_t detect_pre_tick;

extern uint8_t detect_R_C;
extern double  detect_freq   ;  // 555�������Ƶ��
extern double  C_c           ;  // ͬ����µ���
extern double  channel_length;  // ���³���
extern double  Rx_val        ;  // ����ֵ
extern double  Cx_val        ;  // ����ֵ

extern const double C_per_m ;  // ͬ����µ���pF / m
extern const double R1      ;
extern const double R2      ;
extern const double Rs      ;
extern const double Vcc     ;

extern uint64_t R_V;


//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

extern inline void cal_channel_length();

//*******************************// end_h                   //************************************//
#endif	// end of ____DETECTION_H___H__
#endif	// end of __HARDWARE_CONFIG__DETECTION_ENABLE__
