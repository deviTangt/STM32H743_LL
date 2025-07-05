#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__DETECTION_ENABLE__	// begin of __HARDWARE_CONFIG__DETECTION_ENABLE__
//*******************************// include _h files    //************************************//
#include "Detection.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint8_t  detect_mode     = 0;  // 1: ���ȼ�� 2: ���� / ���ݼ�� 4: ���ȼ����� 8: �������� 16: ���ݼ����� 32: ��⿪· 64: ���������
uint32_t detect_pre_tick = 0;
uint8_t  detect_R_C      = 0;  // 1: ������ 2: ���ݼ��
uint8_t  load_type       = 0;  // �������� 1: ���� 2: ����
double   detect_freq     = 0;  // 555�������Ƶ��
double   C_c             = 0;  // ͬ����µ���(�ܵ���)
double   channel_length  = 0;  // ���³���
double   Rx_val          = 0;  // ����ֵ
double   Cx_val          = 0;  // ����ֵ
uint64_t R_V             = 0;

const double C_per_m       = 115.21;   // ͬ����µ���pF / m
const double C_per_m_short = 322.27;   // ��ͬ����µ���pF / m
const double L_fixed       = 0.9438;   // ��ͬ����µ��ݹ��г���
const double R1            = 41480.0;
const double R2            = 89260.0;
const double Rs            = 18.9740;
const double Vcc           = 4.52794;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void cal_channel_length(uint32_t T)
//-----------------------------------------------------------------
//
// ��������: ����ͬ����³���
// ��ڲ���1: uint32_t T: ��������ֵ
// �� �� ֵ: ��
// ע������: ��
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