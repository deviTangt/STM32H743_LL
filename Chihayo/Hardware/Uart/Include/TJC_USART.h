#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__TJC_USART_ENABLE__
#ifndef __TJC_USART_H__
#define __TJC_USART_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//*******************************// define statement        //************************************//

#define TJC_HDMI_UART                                   UART7   //? TJC_HMI占用的Usart / Uart资源

#define STRING_MAX_LENGTH       255         // 字符串栈长度最大值
#if __HARDWARE_CONFIG__TJC_printf_sn_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_printf_sn_ENABLE__
    #if __HARDWARE_CONFIG__TJC_DMA_ENABLE__  // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
        #define printf_s_c              TJC_DMA_SendCmd_s               //? 向USART HMI发送一条指令，格式化
        #define printf_s_m              TJC_SendTxt_MAIN_s              //? 向USART HMI的main页面t0控件输出文本，格式化
        #define printf_n_m              TJC_DMA_SendTxt_MAIN_n          //? 向USART HMI的main页面t0控件输出文本，固定字节数输出    
        #define printf_s                TJC_DMA_SendTxt_SHOW_s          //! 向USART HMI的show页面某个窗口输出文本，格式化            
        #define printf_n                TJC_DMA_SendTxt_SHOW_n          //? 向USART HMI的show页面某个窗口输出文本，固定字节数输出      
        #define printf_s_d              TJC_DMA_SendString_s            //? 向USART HMI发送一条文本，格式化     
        #define printf_n_d              TJC_DMA_SendString_n            //? 向USART HMI发送一条文本，固定字节数输出   
    #else 
        #define printf_s_c              TJC_SendCmd_s                   //? 向USART HMI发送一条指令，格式化
        #define printf_s_m              TJC_SendTxt_MAIN_s              //? 向USART HMI的main页面t0控件输出文本，格式化
        #define printf_n_m              TJC_SendTxt_MAIN_n              //? 向USART HMI的main页面t0控件输出文本，固定字节数输出
        #define printf_s                TJC_SendTxt_SHOW_s              //! 向USART HMI的show页面某个窗口输出文本，格式化
        #define printf_n                TJC_SendTxt_SHOW_n              //? 向USART HMI的show页面某个窗口输出文本，固定字节数输出
        #define printf_s_d              TJC_SendString_s                //? 向USART HMI发送一条文本，格式化
        #define printf_n_d              TJC_SendString_n                //? 向USART HMI发送一条文本，固定字节数输出
    #endif // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
#endif // end of __HARDWARE_CONFIG__TJC_printf_sn_ENABLE__

#if __HARDWARE_CONFIG__TJC_printf_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_printf_ENABLE__
    #if __HARDWARE_CONFIG__TJC_DMA_ENABLE__  // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
        #define printf                  TJC_SendTxt_MAIN_s              //? 向USART HMI的main页面t0控件输出文本，格式化  
    #else 
        #define printf                  TJC_SendTxt_MAIN_s              //? 向USART HMI的main页面t0控件输出文本，格式化
    #endif // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
#endif // end of __HARDWARE_CONFIG__TJC_printf_ENABLE__

// DMA参数
#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__    
    #define DMA_RX_BUF_MAX          255
    #define DMA_TX_BUF_MAX          255

    #define TJC_RX_HANDLE_TIME      1000
#endif // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__


//*******************************// define structure unity  //************************************//
//*******************************// extern parameters       //************************************//

#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
	extern char DMA_TX_BUF[DMA_TX_BUF_MAX];
	extern char DMA_RX_BUF[DMA_RX_BUF_MAX];
    extern uint16_t DMA_RX_Length;
	extern uint8_t Flag_TJC_RXNE;
    extern uint32_t RX_tick, RX_tick_pre;
#endif // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__

//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

#if __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__
int fputc(int ch, FILE* stream);
#endif // end of __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__

extern inline void TJC_USART_Config_Init();
extern inline void TJC_Usart_init();
extern inline void TJC_PagaInit();

extern inline void UART7_IRQHandler_Func();
extern inline void TJC_USART_RX_HANDLER();

extern inline void TJC_SendByte(uint8_t Byte);

extern inline void TJC_SendString_n(char* string, uint8_t size);
extern void TJC_SendString_s(char *format, ...);
extern inline void TJC_SendString_sp(char *format);

extern void TJC_SendCmd_s(char *format, ...);
extern inline void TJC_SendCmd_sp(char *format);

extern inline void TJC_SendTxt_MAIN_n(char *string, uint8_t size);
extern void TJC_SendTxt_MAIN_s(char *format, ...);
extern inline void TJC_SendTxt_SHOW_n(uint8_t WindowIndex, char *string, uint8_t size);
extern void TJC_SendTxt_SHOW_s(uint8_t WindowIndex, char *format, ...);

#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
extern inline void DMA_TJC_UART_Init();
extern inline void DMA1_Stream1_IRQHandler_Func();

extern void TJC_DMA_SendString_n(const char *string, uint16_t size);
extern void TJC_DMA_SendString_s(char *format, ...);
extern void TJC_DMA_SendString_sp(char *format);
extern void TJC_DMA_SendCmd_s(char *format, ...);
extern void TJC_DMA_SendCmd_sp(char *format);
extern void TJC_DMA_SendTxt_MAIN_n(char *string, uint8_t size);
extern void TJC_DMA_SendTxt_MAIN_s(char *format, ...);
extern void TJC_DMA_SendTxt_SHOW_n(uint8_t WindowIndex, char *string, uint8_t size);
extern void TJC_DMA_SendTxt_SHOW_s(uint8_t WindowIndex, char *format, ...);
#endif // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__

//*******************************// end_h                   //************************************//
#endif
#endif
