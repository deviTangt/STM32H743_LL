#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__	// begin of __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__
//*******************************// include _h files    //************************************//

#include "config_init.h"
#include "pic.h"

//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//
//*******************************// config main         //************************************//

//-----------------------------------------------------------------
// inline void config_init()
//-----------------------------------------------------------------
//
// Function Usage  : initialize various periph config
// Input Arugment 1: None
// Return Value    : None
// Something Notice: place it right after the /* USER CODE BEGIN 3 */ in main loop 
//
//-----------------------------------------------------------------
inline void config_init(){
    //? TJC USART HMI Initialized
    #if __HARDWARE_CONFIG__TJC_USART_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_USART_ENABLE__
        TJC_USART_Config_Init();
    #endif // end of __HARDWARE_CONFIG__TJC_USART_ENABLE__  

    //? ADC1 Initialized
    config_adc_sample_init();

    //? TFT LCD Initialized
    #if __HARDWARE_CONFIG__TFTLCD_ENABLE__ // begin of __HARDWARE_CONFIG__TFTLCD_ENABLE__
        LCD_Init();
        LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
        LCD_ShowPicture(0, 0, 128, 134, gImage_HuaXiaoKe);
    #endif // end of __HARDWARE_CONFIG__TFTLCD_ENABLE__

    //? User Timer Initialized
    config_user_timer_init();

    //? FPGA COMMUTED USART Initialized
    #if __HARDWARE_CONFIG__UART_FPGA_ENABLE__ // begin of __HARDWARE_CONFIG__UART_FPGA_ENABLE__
        FPGA_USART_Config_Init();
        //MX_USART1_UART_Init();
    #endif // end of __HARDWARE_CONFIG__UART_FPGA_ENABLE__  
}

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void config_adc_sample_init()
//-----------------------------------------------------------------
//
// Function Usage  : initialize sampler adc(including adc1, 2, 3 if enabled)
// Input Arugment 1: None
// Return Value    : None
// Something Notice: None
//
//-----------------------------------------------------------------
inline void config_adc_sample_init(){
    //? adc1
    #if __HARDWARE_CONFIG__DMA_ADC1_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
        Samp_Adc1_Init((uint32_t)&DMA_ADC1_RX_BUF, ADC1_Sequence_Channel_Num);
        #if 1   // 是否开始采样
            Start_Sample_adc1();
        #endif
    #endif // end of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__

    //? adc2
    #if __HARDWARE_CONFIG__DMA_ADC2_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC2_ENABLE__
        Samp_Adc2_Init((uint32_t)&DMA_ADC2_RX_BUF, ADC2_Sequence_Channel_Num);
        #if 2   // 是否开始采样
            Start_Sample_adc2();
        #endif
    #endif // end of __HARDWARE_CONFIG__DMA_ADC2_ENABLE__

    //? adc3
    #if __HARDWARE_CONFIG__DMA_ADC3_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
        Samp_Adc3_Init((uint32_t)&DMA_ADC3_RX_BUF, ADC3_Sequence_Channel_Num);
        #if 3   // 是否开始采样
            Start_Sample_adc3();
        #endif
    #endif // end of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__

    //? gpu temperature sensor adc config
    #if __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__ // begin of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
        gpu_temp_adc3_config_init();
    #endif // end of __HARDWARE_CONFIG__GPU_TEMP_ADC_ENABLE__
}

//-----------------------------------------------------------------
// inline void delay_us()
//-----------------------------------------------------------------
//
// Function Usage  : initialize user timer
// Input Arugment 1: None
// Return Value    : None
// Something Notice: None
//
//-----------------------------------------------------------------
inline void config_user_timer_init(){
    #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER_ENABLE__
        //? User Timer 1
        #if __HARDWARE_CONFIG__USER_TIMER1_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__
            timer1_config_init(240 - 1, 1000 - 1);
            timer1_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__

        //? User Timer 2
        #if __HARDWARE_CONFIG__USER_TIMER2_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
            timer2_config_init(24 - 1, 4294967295);
            timer2_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__

        //? User Timer 3
        #if __HARDWARE_CONFIG__USER_TIMER3_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__
            timer3_config_init(240 - 1, 1000 - 1);
            timer3_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__

        //? User Timer 4
        #if __HARDWARE_CONFIG__USER_TIMER4_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__
            timer4_config_init(240 - 1, 100 - 1);
            //timer4_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__

        //? User Timer 5
        #if __HARDWARE_CONFIG__USER_TIMER5_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER5_ENABLE__
            timer5_config_init(240 - 1, 300000 - 1);
            timer5_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER5_ENABLE__

        //? User Timer 6
        #if __HARDWARE_CONFIG__USER_TIMER6_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER6_ENABLE__
            timer6_config_init(240 - 1, 1000 - 1);
            timer6_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER6_ENABLE__

        //? User Timer 7
        #if __HARDWARE_CONFIG__USER_TIMER7_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER7_ENABLE__
            timer7_config_init(240 - 1, 1000 - 1);
            timer7_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER7_ENABLE__

        //? User Timer 8
        #if __HARDWARE_CONFIG__USER_TIMER8_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__
            timer8_config_init(240 - 1, 1000 - 1);
            timer8_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__

        //? User Timer 12
        #if __HARDWARE_CONFIG__USER_TIMER12_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER12_ENABLE__
            timer12_config_init(240 - 1, 1000 - 1);
            timer12_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER12_ENABLE__

        //? User Timer 13
        #if __HARDWARE_CONFIG__USER_TIMER13_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
            timer13_config_init(240 - 1, 1000 - 1);
            timer13_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__

        //? User Timer 14
        #if __HARDWARE_CONFIG__USER_TIMER14_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER14_ENABLE__
            timer14_config_init(240 - 1, 1000 - 1);
            timer14_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER14_ENABLE__

        //? User Timer 15
        #if __HARDWARE_CONFIG__USER_TIMER15_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER15_ENABLE__
            timer15_config_init(240 - 1, 1000 - 1);
            timer15_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER15_ENABLE__

        //? User Timer 16
        #if __HARDWARE_CONFIG__USER_TIMER16_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER16_ENABLE__
            timer16_config_init(240 - 1, 1000 - 1);
            timer16_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER16_ENABLE__

        //? User Timer 17
        #if __HARDWARE_CONFIG__USER_TIMER17_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER17_ENABLE__
            timer17_config_init(240 - 1, 1000 - 1);
            timer17_start();
        #endif // end of __HARDWARE_CONFIG__USER_TIMER17_ENABLE__
    #endif // end of __HARDWARE_CONFIG__USER_TIMER_ENABLE__
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__CONFIG_INIT_ENABLE__
