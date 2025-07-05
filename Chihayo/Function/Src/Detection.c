#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__DETECTION_ENABLE__	// begin of __HARDWARE_CONFIG__DETECTION_ENABLE__
//*******************************// include _h files    //************************************//
#include "Detection.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint8_t  detect_mode     = 0;  // 1: 长度检测 2: 电阻 / 电容检测 4: 长度检测完成 8: 电阻检测中 16: 电容检测完成 32: 检测开路 64: 电阻检测完成
uint32_t detect_pre_tick = 0;
uint8_t  detect_R_C      = 0;  // 1: 电阻检测 2: 电容检测
uint8_t  load_type       = 0;  // 负载类型 1: 电阻 2: 电容
double   detect_freq     = 0;  // 555震荡器检测频率
double   C_c             = 0;  // 同轴电缆电容(总电容)
double   channel_length  = 0;  // 电缆长度
double   Rx_val          = 0;  // 电阻值
double   Cx_val          = 0;  // 电容值
uint64_t R_V             = 0;

const double C_per_m       = 115.21;   // 同轴电缆电容pF / m
const double C_per_m_short = 322.27;   // 短同轴电缆电容pF / m
const double L_fixed       = 0.9438;   // 短同轴电缆电容固有长度
const double R1            = 41480.0;
const double R2            = 89260.0;
const double Rs            = 18.9740;
const double Vcc           = 4.52794;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void cal_channel_length(uint32_t T)
//-----------------------------------------------------------------
//
// 函数功能: 计算同轴电缆长度
// 入口参数1: uint32_t T: 方波周期值
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void cal_channel_length(uint32_t T){
    C_c = 1.0e+5 * T / (log(2) * (R1 + 2 * R2));
    if (T > 500)
        channel_length = 1.0e+5 * T / (log(2) * (R1 + 2 * R2) * C_per_m) - L_fixed;
    else 
        channel_length = 1.0e+5 * T / (log(2) * (R1 + 2 * R2) * C_per_m) - L_fixed;
    printf_s(4, "%.6lfm", channel_length);
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__DETECTION_ENABLE__