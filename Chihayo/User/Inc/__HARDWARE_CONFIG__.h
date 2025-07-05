#ifndef __HARDWARE_CONFIG_H__ // begin of __HARDWARE_CONFIG_H__
#define __HARDWARE_CONFIG_H__
//*******************************// include extern .h files //************************************//
//*******************************// define statement        //************************************//

//! 板级支持包基本定时器tim7启用设置
#define __HARDWARE_CONFIG__BSP_TIMER_ENABLE__               1       //! 启用板级支持包基本定时器tim7

//! 配置初始化文件启用设置
#define __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__             1       //! 启用config_init.h

//? 可视化调试信息启用设置
#define __HARDWARE_CONFIG__VISUALIZE_SHOW_ENABLE__          1       //? 启用visualize_show.h

//? LED、KEY启用设置
#define __HARDWARE_CONFIG__LED_ENABLE__                     1       // 启用LED.h
#define __HARDWARE_CONFIG__KEY_ENABLE__                     1       // 启用KEY.h 

//? 淘晶驰USART HMI显示屏启用设置
#define __HARDWARE_CONFIG__TJC_USART_ENABLE__               1       //! 启用TJC_USART.h
    #define __HARDWARE_CONFIG__TJC_PAGE_CLEAR_ENABLE__          1       // 启用TJC_USART初始化页面刷新功能
        #define __HARDWARE_CONFIG__TJC_PAGE_JUMP_ENABLE__           0       // 启用TJC_USART初始化页面跳转功能
    #define __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__               0       // 启用TJC_USART.h重定向fputc功能
    #define __HARDWARE_CONFIG__TJC_printf_sn_ENABLE__           1       //? 启用TJC_USART.h重定义printf_s和printf_n功能
    #define __HARDWARE_CONFIG__TJC_printf_ENABLE__              1       //? 启用TJC_USART.h重定义printf功能
    #define __HARDWARE_CONFIG__TJC_RX_ENABLE__                  1       // 启用TJC_USART接收中断
    #define __HARDWARE_CONFIG__TJC_DMA_ENABLE__                 1       //? 启用TJC_USART DMA模式 // 占用DMA1_Stream0与DMA1_Stream1

//? TFTLCD显示屏启用设置
#define __HARDWARE_CONFIG__TFTLCD_ENABLE__                  0       // 启用TFTLCD.h、TFTLCD_Init.h

//? 稳定传感器ADC设置(ADC3)
#define __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__            0       // 启用gpu_temp_adc.h

//? ADC采样设置
#define __HARDWARE_CONFIG__DMA_ADC1_ENABLE__                0       // 启用dma_adc1.h
#define __HARDWARE_CONFIG__DMA_ADC2_ENABLE__                0       // 启用dma_adc2.h
#define __HARDWARE_CONFIG__DMA_ADC3_ENABLE__                0       // 启用dma_adc3.h
    //? ADC dma传输启用设置
    #define __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_ENABLE__         1       // 启用dma传输ADC1采样 // 占用DMA1_Stream2
    #define __HARDWARE_CONFIG__SAMPLE_ADC2_DMA_ENABLE__         1       // 启用dma传输ADC2采样 // 占用DMA1_Stream3
    #define __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_ENABLE__         1       // 启用dma传输ADC3采样 // 占用DMA1_Stream4
    //? DMA传输接收中断TC使能设置
    #define __HARDWARE_CONFIG__SAMPLE_ADC1_DMA_TC_ENABLE__      1       // 启用dma传输ADC1发送中断
    #define __HARDWARE_CONFIG__SAMPLE_ADC2_DMA_TC_ENABLE__      0       // 启用dma传输ADC2发送中断
    #define __HARDWARE_CONFIG__SAMPLE_ADC3_DMA_TC_ENABLE__      0       // 启用dma传输ADC3发送中断

//? 检测同轴电缆
#define __HARDWARE_CONFIG__DETECTION_ENABLE__               0       // 启用Detection.h 
    
//? ADS9959启用设置
#define __HARDWARE_CONFIG__AD9959_ENABLE__                  0       // 启用ad9959.h    

//? 用户可配置定时器启用设置
#define __HARDWARE_CONFIG__USER_TIMER_ENABLE__              1       // 启用user_timer.h    
    #define __HARDWARE_CONFIG__USER_TIMER1_ENABLE__             0       // 启用timer1.h    //// 高级控制定时器Timer1(16位) 
    #define __HARDWARE_CONFIG__USER_TIMER2_ENABLE__             0       // 启用timer2.h    //? 通用定时器Timer2(32位) 
    #define __HARDWARE_CONFIG__USER_TIMER3_ENABLE__             0       // 启用timer3.h    //? 通用定时器Timer3(16位)
    #define __HARDWARE_CONFIG__USER_TIMER4_ENABLE__             0       // 启用timer4.h    //? 通用定时器Timer4(16位)
    #define __HARDWARE_CONFIG__USER_TIMER5_ENABLE__             0       // 启用timer5.h    //? 通用定时器Timer5(32位)
    #define __HARDWARE_CONFIG__USER_TIMER6_ENABLE__             0       // 启用timer6.h    //? 基本定时器Timer6(16位)
    #define __HARDWARE_CONFIG__USER_TIMER7_ENABLE__             0       // 启用timer7.h    //? 基本定时器Timer7(16位)  
    #define __HARDWARE_CONFIG__USER_TIMER8_ENABLE__             0       // 启用timer8.h    //// 高级控制定时器Timer8(16位)
    #define __HARDWARE_CONFIG__USER_TIMER12_ENABLE__            0       // 启用timer12.h   //? 基本定时器Timer12(16位)
    #define __HARDWARE_CONFIG__USER_TIMER13_ENABLE__            0       // 启用timer13.h   //? 基本定时器Timer13(16位)
    //define __HARDWARE_CONFIG__USER_TIMER14_ENABLE__           0       // 启用timer14.h   //! 基本定时器Timer14(16位) //! bsp_timer占用
    //define __HARDWARE_CONFIG__USER_TIMER15_ENABLE__           0       // 启用timer15.h   //! 基本定时器Timer15(16位) //! dma_adc1占用
    #define __HARDWARE_CONFIG__USER_TIMER16_ENABLE__            0       // 启用timer16.h   //? 基本定时器Timer16(16位)
    #define __HARDWARE_CONFIG__USER_TIMER17_ENABLE__            0       // 启用timer17.h   //? 基本定时器Timer17(16位)
        //? 定时器update中断启用设置
        #define __HARDWARE_CONFIG__TIMER1_INT_UPDATE_ENABLE__       0       // 启用timer1更新中断 
        #define __HARDWARE_CONFIG__TIMER2_INT_UPDATE_ENABLE__       1       // 启用timer2更新中断 // 输入捕获
        #define __HARDWARE_CONFIG__TIMER3_INT_UPDATE_ENABLE__       0       // 启用timer3更新中断 // adc实时示波器显示
        #define __HARDWARE_CONFIG__TIMER4_INT_UPDATE_ENABLE__       1       // 启用timer4更新中断 // 定时填入adc采样数据
        #define __HARDWARE_CONFIG__TIMER5_INT_UPDATE_ENABLE__       1       // 启用timer5更新中断 // 定时刷新部分信息
        #define __HARDWARE_CONFIG__TIMER6_INT_UPDATE_ENABLE__       0       // 启用timer6更新中断 
        #define __HARDWARE_CONFIG__TIMER7_INT_UPDATE_ENABLE__       0       // 启用timer7更新中断 
        #define __HARDWARE_CONFIG__TIMER8_INT_UPDATE_ENABLE__       0       // 启用timer8更新中断 
        #define __HARDWARE_CONFIG__TIMER12_INT_UPDATE_ENABLE__      1       // 启用timer12更新中断 
        #define __HARDWARE_CONFIG__TIMER13_INT_UPDATE_ENABLE__      0       // 启用timer13更新中断 
        #define __HARDWARE_CONFIG__TIMER14_INT_UPDATE_ENABLE__      0       // 启用timer14更新中断 
        #define __HARDWARE_CONFIG__TIMER15_INT_UPDATE_ENABLE__      0       // 启用timer15更新中断 
        #define __HARDWARE_CONFIG__TIMER16_INT_UPDATE_ENABLE__      0       // 启用timer16更新中断 
        #define __HARDWARE_CONFIG__TIMER17_INT_UPDATE_ENABLE__      0       // 启用timer17更新中断 
        //? 定时器CC中断启用设置
        #define __HARDWARE_CONFIG__TIMER2_INT_CAPTURE_ENABLE__      0       // 启用timer2捕获中断

//? 用户测试文件
#define __HARDWARE_CONFIG__USER_TEST_ENABLE__                       0       // 启用user_test.h  

//? 计算模块
#define __HARDWARE_CONFIG__SORT_CODE_ENABLE__                       0       // 启用sort_code.h  
#define __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__                     0       // 启用search_code.h  

//? SPI-FPGA
#define __HARDWARE_CONFIG__SPI_FPGA_ENABLE__                        0       // 启用spi_fpga.h 
//? UART-FPGA
#define __HARDWARE_CONFIG__UART_FPGA_ENABLE__                       1       // 启用uart_fpga.h 
    #define __HARDWARE_CONFIG__FPGA_printf_f_ENABLE__           1       //? 启用FPGA_USART.h重定义printf_f功能
    #define __HARDWARE_CONFIG__FPGA_RX_ENABLE__                 1       //? 启用FPGA_USART接收中断
    #define __HARDWARE_CONFIG__FPGA_DMA_ENABLE__                1       //? 启用FPGA_USART DMA模式 // 占用DMA1_Stream5与DMA1_Stream6

//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//
//*******************************// end_h                   //************************************//
#endif // end of __HARDWARE_CONFIG_H__
