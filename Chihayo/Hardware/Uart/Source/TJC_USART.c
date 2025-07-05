#include "__HARDWARE_CONFIG__.h" 
#if __HARDWARE_CONFIG__TJC_USART_ENABLE__ // begin of __HARDWARE_CONFIG__TFTLCD_ENABLE__
//*******************************// include _h files    //************************************//
#include "TJC_USART.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

static uint32_t i;
static char String_Temp[STRING_MAX_LENGTH];
#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
	char DMA_TX_BUF[DMA_TX_BUF_MAX] __attribute__ ((section ("DMA_TX_BUF"),  zero_init));
	char DMA_RX_BUF[DMA_RX_BUF_MAX] __attribute__ ((section ("DMA_RX_BUF"),  zero_init));

	uint16_t DMA_RX_Length = 0;
	uint8_t  Flag_TJC_RXNE = 0;
	uint8_t  RX            = 0;
	uint32_t RX_tick       = 0, RX_tick_pre = 0;
#endif // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__


//*******************************// define function     //************************************//

#if __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__  // begin of __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__
//-----------------------------------------------------------------
// int fputc(int ch, FILE* stream)
//-----------------------------------------------------------------
//
// 函数功能: 重定向fputc函数到发送字节到USART1端口
// 入口参数1: int ch: 输入字
// 入口参数2: FILE* stream：文件流
// 返 回 值: 发送的字节
// 注意事项: 隶属TJC_USART.h
//
//-----------------------------------------------------------------
int fputc(int ch, FILE* stream){
    while(LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)!=1);//等待发送完成
    LL_USART_TransmitData8(TJC_HDMI_UART, (uint8_t)ch);
    return ch;
}
#endif // end of __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__

//-----------------------------------------------------------------
// inline void TJC_USART_Config_Init()
//-----------------------------------------------------------------
//
// 函数功能: 初始化TJC USART HMI引脚配置及USART端口
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_USART_Config_Init(){
	//? 时钟配置
	LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART7);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);

	//? GPIO配置 
	//? PE7   ------> UART7_RX
	//? PE8   ------> UART7_TX
	LL_GPIO_InitTypeDef GPIO_InitStruct            = {0};
	                    GPIO_InitStruct.Pin        = TJC_TX_Pin | TJC_RX_Pin;
	                    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
	                    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
	                    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	                    GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
	                    GPIO_InitStruct.Alternate  = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	//? USART DMA配置
	#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
		//? USART DMA TX配置
		LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_0, LL_DMAMUX1_REQ_UART7_TX);  
		LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); 
		LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_HIGH);
		LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_NORMAL);
		LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);
		LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);
		LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_BYTE);
		LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_BYTE);
		LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_0);

		//? USART DMA RX配置
		#if __HARDWARE_CONFIG__TJC_RX_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
			LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_1, LL_DMAMUX1_REQ_UART7_RX);
			LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
			LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_1, LL_DMA_PRIORITY_HIGH);
			LL_DMA_SetMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MODE_CIRCULAR);
			LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_PERIPH_NOINCREMENT);
			LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MEMORY_INCREMENT);
			LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_1, LL_DMA_PDATAALIGN_BYTE);
			LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_1, LL_DMA_MDATAALIGN_BYTE);
			LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_1);
		#endif // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__
	#endif // __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__

	//? USART 中断配置
	NVIC_SetPriority(UART7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 2));
	NVIC_EnableIRQ(UART7_IRQn);

	//? USART结构体配置
	LL_USART_InitTypeDef UART_InitStruct                     = {0};
						UART_InitStruct.PrescalerValue      = LL_USART_PRESCALER_DIV1;   // USART预分频系数：1
						UART_InitStruct.BaudRate            = 921600;                    // USART波特率
						UART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;     // USART传输bit数：8bit
						UART_InitStruct.StopBits            = LL_USART_STOPBITS_1;       // USART停止位数：1bit
						UART_InitStruct.Parity              = LL_USART_PARITY_NONE;      // USART奇偶校验：无
						UART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;  // USART传输方向：TX/RX双向
						UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;   // USART硬件流控制
						UART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;  // USART过采样系数：16
	LL_USART_Init(UART7, &UART_InitStruct);

	//? FIFO配置
	LL_USART_DisableFIFO(UART7);
	LL_USART_SetTXFIFOThreshold(UART7, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(UART7, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_ConfigAsyncMode(UART7);


	//? RXNE中断使能
	#if __HARDWARE_CONFIG__TJC_RX_ENABLE__ & (~__HARDWARE_CONFIG__TJC_DMA_ENABLE__ | 1)// begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
		UART7->RDR = 0x00;
		CLEAR_BIT(UART7->ISR, USART_ISR_RXNE_RXFNE);
		LL_USART_EnableIT_RXNE_RXFNE(UART7);
	#endif // __HARDWARE_CONFIG__TJC_RX_ENABLE__ // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__

	//? DMA设置
	#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
		DMA_TJC_UART_Init();
	#endif // __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__

	//? USART使能
	LL_USART_Enable(UART7);

	// 等待USART初始化完成
	while((!(LL_USART_IsActiveFlag_TEACK(UART7))) || (!(LL_USART_IsActiveFlag_REACK(UART7))))
	{
	}

	//? 初始化TJC USART HMI
    #if __HARDWARE_CONFIG__TJC_USART_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_USART_ENABLE__
      TJC_Usart_init();
    #endif // end of __HARDWARE_CONFIG__TJC_USART_ENABLE__

  /* USER CODE END UART7_Init 2 */
}

//-----------------------------------------------------------------
// inline void TJC_Usart_init()
//-----------------------------------------------------------------
//
// 函数功能: 初始化TJC USART HMI
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 可放在"usart.c"的"void MX_UART7_Init(void)" (即main函数调用"MX_UART7_Init();")  
//          的"/* USER CODE BEGIN UART7_Init 2 */"之后
//
//-----------------------------------------------------------------
inline void TJC_Usart_init(){
	//? 刷新页面
	#if __HARDWARE_CONFIG__TJC_PAGE_CLEAR_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_PAGE_CLEAR_ENABLE__
		TJC_PagaInit();
	#endif // end of __HARDWARE_CONFIG__TJC_PAGE_CLEAR_ENABLE__

    printf_s(21, "It's MyGo!!!!!");
	printf_s(21, "It's MyGo!!!!!");
}
//-----------------------------------------------------------------
// inline void TJC_PagaInit()
//-----------------------------------------------------------------
//
// 函数功能: 初始化TJC USART HMI界面
// 入口参数1: 无
// 注意事项: 信息刷两遍以防疏漏
//
//-----------------------------------------------------------------
inline void TJC_PagaInit(){
	//? Page main
    if (__HARDWARE_CONFIG__TJC_PAGE_JUMP_ENABLE__) {
		printf_s_c("page main"); printf_s_c("page main");
	}
    printf_s_c("click b2,0"); printf_s_c("click b2,0");
    printf_s_c("click b4,0"); printf_s_c("click b4,0");

	//? Paga bonus_wave
	if (__HARDWARE_CONFIG__TJC_PAGE_JUMP_ENABLE__) {
		printf_s_c("page bonus_wave"); printf_s_c("page bonus_wave");
	}
    printf_s_c("cle bonus_wave.s0.id,0"); printf_s_c("cle bonus_wave.s0.id,0");
	
	//? Page show
    if (__HARDWARE_CONFIG__TJC_PAGE_JUMP_ENABLE__) {
		printf_s_c("page show"); printf_s_c("page show");
	}
    printf_s_c("click b6,0"); printf_s_c("click b6,0");
}

//-----------------------------------------------------------------
// inline void UART7_IRQHandler_Func()
//-----------------------------------------------------------------
//
// 函数功能: UART7中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
static uint8_t tjc_rx_state = 0;
inline void UART7_IRQHandler_Func(){
	#if __HARDWARE_CONFIG__TJC_RX_ENABLE__  // begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
		if (LL_USART_IsActiveFlag_RXNE_RXFNE(UART7)){
			RX_tick = bsp_us;
			if (RX_tick > RX_tick_pre + TJC_RX_HANDLE_TIME){
				tjc_rx_state = 0;
				DMA_RX_Length = 0;
			}
	
			RX = LL_USART_ReceiveData8(UART7);

			// 数据包起始段 && (1 / 2)
			if (tjc_rx_state == 0 && Flag_TJC_RXNE == 0){
				if (RX == '&')
					tjc_rx_state = 1;
			}
			// 数据包起始段 && (2 / 2)
			else if (tjc_rx_state == 1){
				if (RX == '&')
					tjc_rx_state = 2;
				else
					tjc_rx_state = 0; 
			}
			// 数据包信息段 不定长
			else if (tjc_rx_state == 2){
				// 数据包终止段 && (1 / 2)
				if (RX == '&')
					tjc_rx_state = 3;
				// 数据包信息段
				else
					DMA_RX_BUF[DMA_RX_Length ++] = RX;
			}
			// 数据包终止段 && (2 / 2)
			else if (tjc_rx_state == 3){
				if (RX == '&'){
					tjc_rx_state = 0;
					Flag_TJC_RXNE = 1;
				}
				else{
					tjc_rx_state = 2;
					DMA_RX_BUF[DMA_RX_Length ++] = '&';
					DMA_RX_BUF[DMA_RX_Length ++] = RX;
				}
			}

			RX_tick_pre = bsp_us;
		}

		//! 必须清除ORE中断 开启RXNE中断会自动允许USART OverRun Error Flag中断触发中断服务函数
		if (LL_USART_IsActiveFlag_ORE(UART7)){ 
			LL_USART_ClearFlag_ORE(UART7);
		}

		if (LL_USART_IsActiveFlag_PE(UART7)){
			LL_USART_DisableIT_PE(UART7);
		}
	#endif // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__
}

//-----------------------------------------------------------------
// inline void TJC_USART_RX_HANDLER()
//-----------------------------------------------------------------
//
// 函数功能: 对淘金驰串口屏接收到的数据进行处理
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_USART_RX_HANDLER(){
	// 串口屏指令结束符
	if (Flag_TJC_RXNE){
        // 打印接收到的数据缓冲器
        //printf("Main Receive:%*s\r\n", DMA_RX_Length, DMA_RX_BUF);
		printf_s(25, "at %3dm %02d.%03ds %03dus\r\n", RX_tick / 60000000, RX_tick /1000000 % 60, RX_tick /1000 % 1000, RX_tick % 1000);
		printf_s(26, "USART RX:   [len %02d]", DMA_RX_Length);
		DMA_RX_BUF[DMA_RX_Length] = '\0';
		printf_s(27, "%s", DMA_RX_BUF);

		// 版本号
		if 		(3 <= DMA_RX_Length && DMA_RX_Length <= 5 && strncmp(DMA_RX_BUF, "--v", 3) == 0){
			printf("Version: v1.16.11\r\n");
			printf("Project: Chihayo Anon\r\n");
			printf("Author : Asriel Devi\r\n");
			printf("Update : 2025-7-1 18:15\r\n");
		}
		// 获取实时时间
		else if (6 <= DMA_RX_Length && DMA_RX_Length <= 8 && strncmp(DMA_RX_BUF, "--time", 6) == 0){
			printf("Run Time: min:%3d sec:%02d.%03d us:%03d\r\n", bsp_min, bsp_sec % 60, bsp_ms % 1000, bsp_us % 1000);
		}
          
		Flag_TJC_RXNE = 0;
	}
} 


//-----------------------------------------------------------------
// inline void TJC_SendByte(uint8_t Byte)
//-----------------------------------------------------------------
//
// 函数功能: 发送1字节到TJC_USART7端口
// 入口参数1: uint8_t Byte: 待发送字节
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_SendByte(uint8_t Byte){
    while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
    LL_USART_TransmitData8(TJC_HDMI_UART, (uint8_t)Byte);
}
//-----------------------------------------------------------------
// inline void TJC_SendString_n(char* string, uint8_t size)
//-----------------------------------------------------------------
//
// 函数功能: 发送指定长度字符串到TJC_USART7端口
// 入口参数1: char* string: 待发送字符串
// 入口参数2: uint8_t size: 发送字符串长度
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_SendString_n(char* string, uint8_t size){
    for (i = 0;i < size;i ++){
        while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
        LL_USART_TransmitData8(TJC_HDMI_UART, string[i]);
    }
}
//-----------------------------------------------------------------
// void TJC_SendString_s(char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送格式化字符串到TJC_USART7端口,含格式化参数
// 入口参数1: char *format: 待发送格式化字符串
// 入口参数2: ... ： 格式化输入参数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_SendString_s(char *format, ...){
	va_list arg;
	va_start(arg, format);
	vsprintf(String_Temp, format, arg);
	va_end(arg);
	for (i = 0;String_Temp[i] != '\0'; i ++){
        while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
        LL_USART_TransmitData8(TJC_HDMI_UART, String_Temp[i]);
    }
}
//-----------------------------------------------------------------
// inline void TJC_SendString_sp(char *format)
//-----------------------------------------------------------------
//
// 函数功能: 发送格式化字符串到TJC_USART7端口,无格式化参数
// 入口参数1: char *format: 待发送指令字符串
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_SendString_sp(char *format){
	for (i = 0;format[i] != '\0'; i ++){
        while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
        LL_USART_TransmitData8(TJC_HDMI_UART, format[i]);
    }
}
//-----------------------------------------------------------------
// void TJC_SendCmd_s(char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口，含格式化参数
// 入口参数1: char *format: 待发送指令字符串
// 入口参数2: ... ： 格式化输入参数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_SendCmd_s(char *format, ...){
	va_list arg;
	va_start(arg, format);
	vsprintf(String_Temp, format, arg);
	va_end(arg);
	TJC_SendString_n("\xff\xff\xff", 3);
	TJC_SendString_sp(String_Temp);
	TJC_SendString_n("\xff\xff\xff", 3);
}
//-----------------------------------------------------------------
// inline void TJC_SendCmd_sp(char *format)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口，无格式化参数
// 入口参数1: char *format: 待发送指令字符串
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_SendCmd_sp(char *format){
	TJC_SendString_n("\xff\xff\xff", 3);
	TJC_SendString_sp(format);
	TJC_SendString_n("\xff\xff\xff", 3);
}
//-----------------------------------------------------------------
// inline void TJC_SendTxt_MAIN_n(char *string, uint8_t size)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏main page中的t0控件显示字符串，指定长度
// 入口参数1: uint8_t WindowIndex: 窗口索引（0~12）
// 入口参数2: char* string: 待显示字符串
// 入口参数3: uint8_t size: 显示字符串长度
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_SendTxt_MAIN_n(char *string, uint8_t size){
	TJC_SendString_sp("\xff\xff\xffmain.t0.txt+=\"");
	TJC_SendString_n(string, size);
	TJC_SendString_n("\"\xff\xff\xff", 4);
}
//-----------------------------------------------------------------
// void TJC_SendTxt_MAIN_s(char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏main page中的t0控件显示字符串，含格式化参数
// 入口参数1: char* format: 待显示格式化字符串
// 入口参数2: ... ： 格式化输入参数
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_SendTxt_MAIN_s(char *format, ...){
	TJC_SendString_sp("\xff\xff\xffmain.t0.txt+=\"");
	
	va_list arg;
	va_start(arg, format);
	vsprintf(String_Temp, format, arg);
	va_end(arg);
	TJC_SendString_sp(String_Temp);
	TJC_SendString_n("\"\xff\xff\xff", 4);
}
//-----------------------------------------------------------------
// inline void TJC_SendTxt_SHOW_n(uint8_t WindowIndex, char *string, uint8_t size)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏show page中的barx控件显示字符串，指定长度
// 入口参数1: uint8_t WindowIndex: 窗口索引（1~28 ）
// 入口参数2: char* string: 待显示字符串
// 入口参数3: uint8_t size: 显示字符串长度
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void TJC_SendTxt_SHOW_n(uint8_t WindowIndex, char *string, uint8_t size){
	TJC_SendString_s("\xff\xff\xffshow.bar%d.txt=\"", WindowIndex);
	
	TJC_SendString_n(string, size);
	TJC_SendString_n("\"\xff\xff\xff", 4);
}
//-----------------------------------------------------------------
// void TJC_SendTxt_SHOW_s(uint8_t WindowIndex, char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏main page中的t0或barx控件显示字符串，含格式化参数
// 入口参数1: uint8_t WindowIndex: 窗口索引（1~28 ）
// 入口参数2: char* format: 待显示格式化字符串
// 入口参数3: ... ： 格式化输入参数
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_SendTxt_SHOW_s(uint8_t WindowIndex, char *format, ...){
	TJC_SendString_s("\xff\xff\xffshow.bar%d.txt=\"", WindowIndex);
	
	va_list arg;
	va_start(arg, format);
	vsprintf(String_Temp, format, arg);
	va_end(arg);
	TJC_SendString_sp(String_Temp);
	TJC_SendString_n("\"\xff\xff\xff", 4);
}

#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
//-----------------------------------------------------------------
// inline void DMA_TJC_UART_Init()
//-----------------------------------------------------------------
//
// 函数功能: 配置USART DMA转移功能
// 入口参数1: 无
// 注意事项: DMA_TX_BUF 和 DMA_RX_BUF 配置地址时记得看数据手册.STMH743的默认地址域DTCM域（0x20000000）不支持DMA1转运，
//			建议地址设在AXI SRAM域（0x24000000）
//
//-----------------------------------------------------------------
inline void DMA_TJC_UART_Init(){
	//? 发送配置
	// 使能DMA USART请求
	LL_USART_EnableDMAReq_TX(TJC_HDMI_UART);

	// 清除DMA传输完成中断与传输错误中断标志位 
	LL_DMA_ClearFlag_TC0(DMA1);  // TCIF Очищаем флаги - это ОБЯЗАТЕЛЬНО
	LL_DMA_ClearFlag_TE0(DMA1);  // TELF

	// 使能DMA传输完成中断与传输错误中断
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);	// TCIE
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_0);	// TEIE

	//? 设置中断优先级
	NVIC_SetPriority(DMA1_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    
	// 设置DMA外设地址
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_0, (uint32_t) &TJC_HDMI_UART->TDR);


	//? 接收配置
	#if __HARDWARE_CONFIG__TJC_RX_ENABLE__ & 0 // begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
		// 使能DMA USART请求
		LL_USART_EnableDMAReq_RX(TJC_HDMI_UART); //Даем добро на запрос DMA TJC_HDMI_UART (передача и прием)

		// 清除DMA传输完成中断与传输错误中断标志位 
		LL_DMA_ClearFlag_TC1(DMA1);  // TCIF
		LL_DMA_ClearFlag_TE1(DMA1);  // TEIF
		// 使能DMA传输完成中断与传输错误中断
		LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);  // TCIE
		LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_1);  // TEIE

		//? 设置中断优先级
		NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
		NVIC_EnableIRQ(DMA1_Stream1_IRQn);

		LL_DMA_SetDataLength    (DMA1,LL_DMA_STREAM_1, DMA_RX_BUF_MAX);               // 设置接收最大数据长度
		LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_1, (uint32_t)&DMA_RX_BUF);       // 设置内存地址，也就是设置接收的数据要放哪
		LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_1, (uint32_t) &TJC_HDMI_UART->RDR);  // 设置DMA外设地址

		LL_USART_ClearFlag_IDLE(UART7);
		// 使能DMA空闲中断
		LL_USART_EnableIT_IDLE(UART7);

		//使能接收DMA STREAM
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);  // 启动接收DMA
	 #endif // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__
}

//-----------------------------------------------------------------
// inline void DMA1_Stream1_IRQHandler_Func()
//-----------------------------------------------------------------
//
// 函数功能: UART7中断服务函数
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void DMA1_Stream1_IRQHandler_Func(){
	//? 接收UART数据
	if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
		LL_DMA_ClearFlag_TC1(DMA1); 
		if (DMA_RX_BUF_MAX - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_1) > 0){  

			// printf("DMA RX:%c\r\n", DMA_RX_BUF[0]);
			// //LL_DMA_SetDataLength    (DMA1,LL_DMA_STREAM_1, 1);                               // 设置接收最大数据长度
			// LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_1, (uint32_t) &DMA_RX_BUF);           // 设置内存地址，也就是设置接收的数据要放哪
			// LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);  // 启动接收DMA

			/* 将接收满的数据打印 */
			//printf("%s", DMA_RX_BUF);
			//printf("TC1 RX Length:%d\r\n", DMA_RX_BUF_MAX - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_1));
			
			/* 打印DMA缓冲满 */
			//printf("DMA data full !\r\n");
		}
	}	

	//? 数据传输错误
	if (LL_DMA_IsActiveFlag_TE1(DMA1)) {
		LL_DMA_ClearFlag_TE1(DMA1);  

		/* 打印传输错误日志 */
    	printf("DMA data transfer failed !\r\n");
	}
}

//-----------------------------------------------------------------
// void TJC_DMA_SendString_n(const char *string, uint16_t size)
//-----------------------------------------------------------------
//
// 函数功能: 发送指定长度字符串到TJC_USART7端口
// 入口参数1: char* string: 待发送字符串
// 入口参数2: uint8_t size: 发送字符串长度
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendString_n(const char *string, uint16_t size){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	strncpy(DMA_TX_BUF, string, size);

	//失能DMA串口发送数据流
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
	// 设置DMA发送长度
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, size);
	// 配置发送DMA
    LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	
	#if 0	// DEBUG
		printf_s_d("S1 %s", DMA_TX_BUF);
		printf_s_d("MemoryAddr: 0x%08x\t", LL_DMA_GetMemoryAddress(DMA1, LL_DMA_STREAM_0));
		printf_s_d("PeriphAddr: 0x%08x\t\r\n", LL_DMA_GetPeriphAddress(DMA1, LL_DMA_STREAM_0));

		printf_s_d("DMA_TX_BUF: 0x%08x\t", &DMA_TX_BUF);
		printf_s_d("U1->TDR:    0x%08x\t\r\n", (uint32_t)&(TJC_HDMI_UART->TDR));
	#endif
	
	//使能DMA串口发送数据流
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
}
//-----------------------------------------------------------------
// void TJC_DMA_SendString_s(char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送格式化字符串到TJC_USART7端口,含格式化参数
// 入口参数1: char *format: 待发送格式化字符串
// 入口参数2: ... ： 格式化输入参数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendString_s(char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	va_list arg;
	va_start(arg, format);
	vsprintf(DMA_TX_BUF, format, arg);
	va_end(arg);

	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, strlen(DMA_TX_BUF));
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
//-----------------------------------------------------------------
// void TJC_DMA_SendString_sp(char *format)
//-----------------------------------------------------------------
//
// 函数功能: 发送格式化字符串到TJC_USART7端口,无格式化参数
// 入口参数1: char *format: 待发送指令字符串
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendString_sp(char *format){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	strncpy(DMA_TX_BUF, format, strlen(format));

	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, strlen(format));
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
//-----------------------------------------------------------------
// void TJC_DMA_SendCmd_s(char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口，含格式化参数
// 入口参数1: char *format: 待发送指令字符串
// 入口参数2: ... ： 格式化输入参数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendCmd_s(char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	strncpy(&DMA_TX_BUF[0], "\xff\xff\xff", 3);
	va_list arg;
	va_start(arg, format);
	vsprintf(&DMA_TX_BUF[3], format, arg);
	va_end(arg);
	i = strlen(DMA_TX_BUF);
	strncpy(&DMA_TX_BUF[i], "\xff\xff\xff", 3);

	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, i + 3);
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
//-----------------------------------------------------------------
// void TJC_DMA_SendCmd_sp(char *format)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口，无格式化参数
// 入口参数1: char *format: 待发送指令字符串
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendCmd_sp(char *format){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	i = strlen(format);

	strncpy(&DMA_TX_BUF[0], "\xff\xff\xff", 3);
	strncpy(&DMA_TX_BUF[3], format, i);
	strncpy(&DMA_TX_BUF[i + 3], "\xff\xff\xff", 3);

	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, i + 6);
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
//-----------------------------------------------------------------
// void TJC_DMA_SendTxt_MAIN_n(char *string, uint8_t size)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏main page中的t0控件显示字符串，指定长度
// 入口参数1: char* string: 待显示字符串
// 入口参数2: uint8_t size: 显示字符串长度
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_MAIN_n(char *string, uint8_t size){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);

	strncpy(DMA_TX_BUF, "\xff\xff\xffmain.t0.txt+=\"", 17);
	strncpy(&DMA_TX_BUF[17], string, size);
	strncpy(&DMA_TX_BUF[17 + size], "\"\xff\xff\xff", 4);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, 17 + size + 4);

	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
//-----------------------------------------------------------------
// void TJC_DMA_SendTxt_MAIN_s(char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏main page中的t0控件显示字符串，含格式化参数
// 入口参数1: char* format: 待显示格式化字符串
// 入口参数2: ... ： 格式化输入参数
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_MAIN_s(char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);

	strncpy(DMA_TX_BUF, "\xff\xff\xffmain.t0.txt+=\"", 17);
	va_list arg;
	va_start(arg, format);
	vsprintf(&DMA_TX_BUF[17], format, arg);
	va_end(arg);
	i = strlen(DMA_TX_BUF);
	strncpy(&DMA_TX_BUF[i], "\"\xff\xff\xff", 4);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, i + 4);

	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
//-----------------------------------------------------------------
// void TJC_DMA_SendTxt_SHOW_n(uint8_t WindowIndex, char *string, uint8_t size)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏show page中的barx控件显示字符串，指定长度
// 入口参数1: uint8_t WindowIndex: 窗口索引（0~12）
// 入口参数2: char* string: 待显示字符串
// 入口参数3: uint8_t size: 显示字符串长度
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_SHOW_n(uint8_t WindowIndex, char *string, uint8_t size){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);

	sprintf(DMA_TX_BUF, "\xff\xff\xffshow.bar%d.txt=\"", WindowIndex);
	i = strlen(DMA_TX_BUF);
	strncpy(&DMA_TX_BUF[i], string, size);
	strncpy(&DMA_TX_BUF[i + size], "\"\xff\xff\xff", 4);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, i + size + 4);

	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
//-----------------------------------------------------------------
// void TJC_DMA_SendTxt_SHOW_s(uint8_t WindowIndex, char *format, ...)
//-----------------------------------------------------------------
//
// 函数功能: 发送指令到TJC_USART7端口,并在串口屏main page中的t0或barx控件显示字符串，含格式化参数
// 入口参数1: uint8_t WindowIndex: 窗口索引（0~12）
// 入口参数2: char* format: 待显示格式化字符串
// 入口参数3: ... ： 格式化输入参数
// 注意事项: 无
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_SHOW_s(uint8_t WindowIndex, char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //等待发送完成
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);

	sprintf(DMA_TX_BUF, "\xff\xff\xffshow.bar%d.txt=\"", WindowIndex);
	i = strlen(DMA_TX_BUF);
	va_list arg;
	va_start(arg, format);
	vsprintf(&DMA_TX_BUF[i], format, arg);
	va_end(arg);
	i = strlen(DMA_TX_BUF);
	strncpy(&DMA_TX_BUF[i], "\"\xff\xff\xff", 4);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, i + 4);
	
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0); 
}
#endif	// end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__


//*******************************// end_c               //************************************//
#endif 	// end of __HARDWARE_CONFIG__TJC_USART_ENABLE__
