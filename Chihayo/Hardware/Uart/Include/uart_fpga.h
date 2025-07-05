#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__UART_FPGA_ENABLE__	// begin of __HARDWARE_CONFIG__UART_FPGA_ENABLE__
#ifndef __UART_FPGA_H__	// begin of __UART_FPGA_H__
#define __UART_FPGA_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//

#define FPGA_UART                       USART1   //? FPGA通信串口占用的Usart / Uart资源

#define STRING_FPGA_MAX_LENGTH          255            // 字符串栈长度最大值
#if __HARDWARE_CONFIG__FPGA_printf_f_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_printf_f_ENABLE__
    #if __HARDWARE_CONFIG__FPGA_DMA_ENABLE__  // begin of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__
        #define printf_f                  FPGA_DMA_SendString_s          //! 向FPGA 串口输出文本，格式化  
        #define printf_f_s                FPGA_DMA_SendString_s          //! 向FPGA 串口输出文本，格式化            
        #define printf_f_n                FPGA_DMA_SendString_n          //? 向FPGA 串口输出文本，固定字节数输出    
    #else 
        #define printf_f                  FPGA_SendString_s             //! 向FPGA 串口输出文本，格式化  
        #define printf_f_s                FPGA_SendString_s             //! 向FPGA 串口输出文本，格式化            
        #define printf_f_n                FPGA_SendString_n             //? 向FPGA 串口输出文本，固定字节数输出  
    #endif // end of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__
#endif // end of __HARDWARE_CONFIG__FPGA_printf_f_ENABLE__

// DMA参数
#if __HARDWARE_CONFIG__FPGA_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__    
    #define DMA_FPGA_RX_BUF_MAX             255
    #define DMA_FPGA_TX_BUF_MAX             255

    #define FPGA_RX_HANDLE_TIME             1000
#endif // end of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__

//*******************************// extern function         //************************************//

extern inline void FPGA_USART_Config_Init();

extern inline void USART1_IRQHandler_Func();
extern inline void FPGA_RX_HANDLER();

extern inline void FPGA_SendByte(uint8_t Byte);

extern inline void FPGA_SendString_n(char* string, uint8_t size);
extern void FPGA_SendString_s(char *format, ...);
extern inline void FPGA_SendString_sp(char *format);

#if __HARDWARE_CONFIG__FPGA_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__
extern inline void DMA_FPGA_UART_Init();
extern inline void DMA1_Stream5_IRQHandler_Func();

extern void FPGA_DMA_SendString_n(const char *string, uint16_t size);
extern void FPGA_DMA_SendString_s(char *format, ...);
extern void FPGA_DMA_SendString_sp(char *format);
#endif // end of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__

//*******************************// end_h                   //************************************//
#endif	// end of __UART_FPGA_H__
#endif	// end of __HARDWARE_CONFIG__UART_FPGA_ENABLE__
