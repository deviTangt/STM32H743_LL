#ifndef __HARDWARE_CONFIG_H__ // begin of __HARDWARE_CONFIG_H__
#define __HARDWARE_CONFIG_H__
//*******************************// include extern .h files //************************************//
//*******************************// define statement        //************************************//

//! �弶֧�ְ�������ʱ��tim7��������
#define __HARDWARE_CONFIG__BSP_TIMER_ENABLE__               1       //! ���ð弶֧�ְ�������ʱ��tim7

//! ���ó�ʼ���ļ���������
#define __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__             1       //! ����config_init.h

//? ���ӻ�������Ϣ��������
#define __HARDWARE_CONFIG__VISUALIZE_SHOW_ENABLE__          1       //? ����visualize_show.h

//? LED��KEY��������
#define __HARDWARE_CONFIG__LED_ENABLE__                     1       // ����LED.h
#define __HARDWARE_CONFIG__KEY_ENABLE__                     1       // ����KEY.h 

//? �Ծ���USART HMI��ʾ����������
#define __HARDWARE_CONFIG__TJC_USART_ENABLE__               1       //! ����TJC_USART.h
    #define __HARDWARE_CONFIG__TJC_PAGE_CLEAR_ENABLE__          1       // ����TJC_USART��ʼ��ҳ��ˢ�¹���
        #define __HARDWARE_CONFIG__TJC_PAGE_JUMP_ENABLE__           0       // ����TJC_USART��ʼ��ҳ����ת����
    #define __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__               0       // ����TJC_USART.h�ض���fputc����
    #define __HARDWARE_CONFIG__TJC_printf_sn_ENABLE__           1       //? ����TJC_USART.h�ض���printf_s��printf_n����
    #define __HARDWARE_CONFIG__TJC_printf_ENABLE__              1       //? ����TJC_USART.h�ض���printf����
    #define __HARDWARE_CONFIG__TJC_RX_ENABLE__                  1       // ����TJC_USART�����ж�
    #define __HARDWARE_CONFIG__TJC_DMA_ENABLE__                 1       //? ����TJC_USART DMAģʽ // ռ��DMA1_Stream0��DMA1_Stream1

//? TFTLCD��ʾ����������
#define __HARDWARE_CONFIG__TFTLCD_ENABLE__                  0       // ����TFTLCD.h��TFTLCD_Init.h

//? �ȶ�������ADC����(ADC3)
#define __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__            0       // ����gpu_temp_adc.h

//? ADC��������
#define __HARDWARE_CONFIG__DMA_ADC1_ENABLE__                0       // ����dma_adc1.h
#define __HARDWARE_CONFIG__DMA_ADC2_ENABLE__                0       // ����dma_adc2.h
#define __HARDWARE_CONFIG__DMA_ADC3_ENABLE__                0       // ����dma_adc3.h
    //? ADC dma������������
    #define __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__         1       // ����dma����ADC1���� // ռ��DMA1_Stream2
    #define __HARDWARE_CONFIG__SAMPLE_ADC2_DMA_ENABLE__         1       // ����dma����ADC2���� // ռ��DMA1_Stream3
    #define __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__         1       // ����dma����ADC3���� // ռ��DMA1_Stream4
    //? DMA��������ж�TCʹ������
    #define __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_TC_ENABLE__      1       // ����dma����ADC1�����ж�
    #define __HARDWARE_CONFIG__SAMPLE_ADC2_DMA_TC_ENABLE__      0       // ����dma����ADC2�����ж�
    #define __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_TC_ENABLE__      0       // ����dma����ADC3�����ж�

//? ���ͬ�����
#define __HARDWARE_CONFIG__DETECTION_ENABLE__               0       // ����Detection.h 
    
//? ADS9959��������
#define __HARDWARE_CONFIG__AD9959_ENABLE__                  0       // ����ad9959.h    

//? �û������ö�ʱ����������
#define __HARDWARE_CONFIG__USER_TIMER_ENABLE__              1       // ����user_timer.h    
    #define __HARDWARE_CONFIG__USER_TIMER1_ENABLE__             0       // ����timer1.h    //// �߼����ƶ�ʱ��Timer1(16λ) 
    #define __HARDWARE_CONFIG__USER_TIMER2_ENABLE__             0       // ����timer2.h    //? ͨ�ö�ʱ��Timer2(32λ) 
    #define __HARDWARE_CONFIG__USER_TIMER3_ENABLE__             0       // ����timer3.h    //? ͨ�ö�ʱ��Timer3(16λ)
    #define __HARDWARE_CONFIG__USER_TIMER4_ENABLE__             0       // ����timer4.h    //? ͨ�ö�ʱ��Timer4(16λ)
    #define __HARDWARE_CONFIG__USER_TIMER5_ENABLE__             0       // ����timer5.h    //? ͨ�ö�ʱ��Timer5(32λ)
    #define __HARDWARE_CONFIG__USER_TIMER6_ENABLE__             0       // ����timer6.h    //? ������ʱ��Timer6(16λ)
    #define __HARDWARE_CONFIG__USER_TIMER7_ENABLE__             0       // ����timer7.h    //? ������ʱ��Timer7(16λ)  
    #define __HARDWARE_CONFIG__USER_TIMER8_ENABLE__             0       // ����timer8.h    //// �߼����ƶ�ʱ��Timer8(16λ)
    #define __HARDWARE_CONFIG__USER_TIMER12_ENABLE__            0       // ����timer12.h   //? ������ʱ��Timer12(16λ)
    #define __HARDWARE_CONFIG__USER_TIMER13_ENABLE__            0       // ����timer13.h   //? ������ʱ��Timer13(16λ)
    //define __HARDWARE_CONFIG__USER_TIMER14_ENABLE__           0       // ����timer14.h   //! ������ʱ��Timer14(16λ) //! bsp_timerռ��
    //define __HARDWARE_CONFIG__USER_TIMER15_ENABLE__           0       // ����timer15.h   //! ������ʱ��Timer15(16λ) //! dma_adc1ռ��
    #define __HARDWARE_CONFIG__USER_TIMER16_ENABLE__            0       // ����timer16.h   //? ������ʱ��Timer16(16λ)
    #define __HARDWARE_CONFIG__USER_TIMER17_ENABLE__            0       // ����timer17.h   //? ������ʱ��Timer17(16λ)
        //? ��ʱ��update�ж���������
        #define __HARDWARE_CONFIG__TIMER1_INT_UPDATE_ENABLE__       0       // ����timer1�����ж� 
        #define __HARDWARE_CONFIG__TIMER2_INT_UPDATE_ENABLE__       1       // ����timer2�����ж� // ���벶��
        #define __HARDWARE_CONFIG__TIMER3_INT_UPDATE_ENABLE__       0       // ����timer3�����ж� // adcʵʱʾ������ʾ
        #define __HARDWARE_CONFIG__TIMER4_INT_UPDATE_ENABLE__       1       // ����timer4�����ж� // ��ʱ����adc��������
        #define __HARDWARE_CONFIG__TIMER5_INT_UPDATE_ENABLE__       1       // ����timer5�����ж� // ��ʱˢ�²�����Ϣ
        #define __HARDWARE_CONFIG__TIMER6_INT_UPDATE_ENABLE__       0       // ����timer6�����ж� 
        #define __HARDWARE_CONFIG__TIMER7_INT_UPDATE_ENABLE__       0       // ����timer7�����ж� 
        #define __HARDWARE_CONFIG__TIMER8_INT_UPDATE_ENABLE__       0       // ����timer8�����ж� 
        #define __HARDWARE_CONFIG__TIMER12_INT_UPDATE_ENABLE__      1       // ����timer12�����ж� 
        #define __HARDWARE_CONFIG__TIMER13_INT_UPDATE_ENABLE__      0       // ����timer13�����ж� 
        #define __HARDWARE_CONFIG__TIMER14_INT_UPDATE_ENABLE__      0       // ����timer14�����ж� 
        #define __HARDWARE_CONFIG__TIMER15_INT_UPDATE_ENABLE__      0       // ����timer15�����ж� 
        #define __HARDWARE_CONFIG__TIMER16_INT_UPDATE_ENABLE__      0       // ����timer16�����ж� 
        #define __HARDWARE_CONFIG__TIMER17_INT_UPDATE_ENABLE__      0       // ����timer17�����ж� 
        //? ��ʱ��CC�ж���������
        #define __HARDWARE_CONFIG__TIMER2_INT_CAPTURE_ENABLE__      0       // ����timer2�����ж�

//? �û������ļ�
#define __HARDWARE_CONFIG__USER_TEST_ENABLE__                       0       // ����user_test.h  

//? ����ģ��
#define __HARDWARE_CONFIG__SORT_CODE_ENABLE__                       0       // ����sort_code.h  
#define __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__                     0       // ����search_code.h  

//? SPI-FPGA
#define __HARDWARE_CONFIG__SPI_FPGA_ENABLE__                        0       // ����spi_fpga.h 
//? UART-FPGA
#define __HARDWARE_CONFIG__UART_FPGA_ENABLE__                       1       // ����uart_fpga.h 
    #define __HARDWARE_CONFIG__FPGA_printf_f_ENABLE__           1       //? ����FPGA_USART.h�ض���printf_f����
    #define __HARDWARE_CONFIG__FPGA_RX_ENABLE__                 1       //? ����FPGA_USART�����ж�
    #define __HARDWARE_CONFIG__FPGA_DMA_ENABLE__                1       //? ����FPGA_USART DMAģʽ // ռ��DMA1_Stream5��DMA1_Stream6

//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//
//*******************************// end_h                   //************************************//
#endif // end of __HARDWARE_CONFIG_H__
