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
// sample_ad_value是你前面ADC采样得到的值
// ADC采样频率需要大于奈奎斯特采样定理的f_min，理论上讲f_s越贴近f_min，FFT精确度更高


float fft_in_adc_data_1[fft_adc_n * 2] = {0}; // FFT输入，实部是ADC值，虚部都是0
// 这个值是FFT_deal0的返回值，不用你干什么


// &arm_cfft_sR_f32_len1024 是DSP库里关于fft的依据，你有这个就能实现1024点的fft


float fft_out_adc_data_1[fft_adc_n * 2] = {0};  // FFT输出 数组大小必须*2 否则arm_cmplx_mag_f32()函数卡死
// 这个是处理完后返回的fft值，可以直接打印看看结果

float f_s = 100000.0;           //  采样频率100kHz
float f_a, f_b;                 //  A和B的频率
float A_a, A_b;                 //  A和B的幅度

int f_pre = 0;

uint16_t id[fft_adc_n];

//*******************************// test main           //************************************//

//-----------------------------------------------------------------
// inline void user_test_func()
//-----------------------------------------------------------------
//
// 函数功能: 测试函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void user_test_func(){
    printf("Begin fft test\r\n");
    // 处理AD采样数据
    FFT_deal0(sample_ad_value, fft_in_adc_data_1, fft_adc_n);

    // fft变换
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_in_adc_data_1, 0, 1);

    // 取模
    arm_cmplx_mag_f32(fft_in_adc_data_1, fft_out_adc_data_1, fft_adc_n);

    // 打印结果
    for ( i = 0; i < fft_adc_n; i++ ) {
        //printf("[%d] a_%.2f b_%.2f\n", i, fft_in_adc_data_1[2 * i], fft_in_adc_data_1[2 * i + 1]);
    }
    printf("xxxxxxxxxxxxxxxx\n");
    printf("xxxxxxxxxxxxxxxxx\r\n");

    for ( i = 0; i < fft_adc_n; i++ ) {
        id[i] = i;
    }
    // 排序
    //BubbleSort(fft_out_adc_data_1, fft_adc_n / 2, id);

    // 打印结果
    for ( i = 0; i < fft_adc_n; i++ ) {
        printf("[%d]  mod_%.2f\r\n", i, fft_out_adc_data_1[i]);
    }

    for ( i = 0; i < fft_adc_n / 2; i++ ) {
        if ( id[i] < fft_adc_n / 2 ) {
            printf("[%d]  mod_%.2f\r\n", id[i], fft_out_adc_data_1[i]);
        }
    }

    if ( id[i - 1] < id[i - 2] ) {     //  找到分量最高的频率
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
