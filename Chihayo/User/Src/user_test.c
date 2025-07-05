#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TEST_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TEST_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_test.h"
#include "dma_adc1.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

static uint32_t i = 0;
uint16_t sample_ad_cnt = 0;
uint16_t sample_ad_value[fft_adc_n] = {0};
// sample_ad_value����ǰ��ADC�����õ���ֵ
// ADC����Ƶ����Ҫ�����ο�˹�ز��������f_min�������Ͻ�f_sԽ����f_min��FFT��ȷ�ȸ���


float fft_in_adc_data_1[fft_adc_n * 2] = {0}; // FFT���룬ʵ����ADCֵ���鲿����0
// ���ֵ��FFT_deal0�ķ���ֵ���������ʲô


// &arm_cfft_sR_f32_len1024 ��DSP�������fft�����ݣ������������ʵ��1024���fft


float fft_out_adc_data_1[fft_adc_n * 2] = {0};  // FFT��� �����С����*2 ����arm_cmplx_mag_f32()��������
// ����Ǵ�����󷵻ص�fftֵ������ֱ�Ӵ�ӡ�������

float f_s = 100000.0;           //  ����Ƶ��100kHz
float f_a, f_b;                 //  A��B��Ƶ��
float A_a, A_b;                 //  A��B�ķ���

int f_pre = 0;

uint16_t id[fft_adc_n];

//*******************************// test main           //************************************//

//-----------------------------------------------------------------
// inline void user_test_func()
//-----------------------------------------------------------------
//
// ��������: ���Ժ���
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void user_test_func(){
    printf("Begin fft test\r\n");
    // ����AD��������
    FFT_deal0(sample_ad_value, fft_in_adc_data_1, fft_adc_n);

    // fft�任
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_in_adc_data_1, 0, 1);

    // ȡģ
    arm_cmplx_mag_f32(fft_in_adc_data_1, fft_out_adc_data_1, fft_adc_n);

    // ��ӡ���
    for ( i = 0; i < fft_adc_n; i++ ) {
        //printf("[%d] a_%.2f b_%.2f\n", i, fft_in_adc_data_1[2 * i], fft_in_adc_data_1[2 * i + 1]);
    }
    printf("xxxxxxxxxxxxxxxx\n");
    printf("xxxxxxxxxxxxxxxxx\r\n");

    for ( i = 0; i < fft_adc_n; i++ ) {
        id[i] = i;
    }
    // ����
    //BubbleSort(fft_out_adc_data_1, fft_adc_n / 2, id);

    // ��ӡ���
    for ( i = 0; i < fft_adc_n; i++ ) {
        printf("[%d]  mod_%.2f\r\n", i, fft_out_adc_data_1[i]);
    }

    for ( i = 0; i < fft_adc_n / 2; i++ ) {
        if ( id[i] < fft_adc_n / 2 ) {
            printf("[%d]  mod_%.2f\r\n", id[i], fft_out_adc_data_1[i]);
        }
    }

    if ( id[i - 1] < id[i - 2] ) {     //  �ҵ�������ߵ�Ƶ��
        f_pre = id[i - 2];
        f_a = (float) id[i - 1] / fft_adc_n * f_s;
        A_a = fft_out_adc_data_1[i - 1];
        f_b = (float) f_pre / fft_adc_n * f_s;
        A_b = fft_out_adc_data_1[i - 2];
    } else {
        f_pre = id[i - 1];
        f_a = (float) id[i - 2] / fft_adc_n * f_s;
        A_a = fft_out_adc_data_1[i - 2];
        f_b = (float) f_pre / fft_adc_n * f_s;
        A_b = fft_out_adc_data_1[i - 1];
    }
}

//*******************************// define function     //************************************//

void FFT_deal0(uint16_t sample_ad_value[], float *fft_in_1, int data_length) {
    int i;
    for ( i = 0; i < data_length; i++ ) {
        fft_in_1[2 * i] = (float) sample_ad_value[i];
        fft_in_1[2 * i + 1] = 0;
    }
}

void BubbleSort(float k[], int n, uint16_t id[]) {
    int i, j, flag = 1;
    float temp;
    uint16_t tmp;
    for ( i = n - 1; i > 0 && flag == 1; i-- ) {
        flag = 0;
        for ( j = 1; j < i; j++ )
            if ( k[j] >= k[j + 1] ) {
                temp = k[j];
                k[j] = k[j + 1];
                k[j + 1] = temp;
                tmp = id[j];
                id[j] = id[j + 1];
                id[j + 1] = tmp;
                flag = 1;
            }
    }
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TEST_ENABLE__
