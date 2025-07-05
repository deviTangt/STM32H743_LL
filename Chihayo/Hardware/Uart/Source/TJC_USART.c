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
// ��������: �ض���fputc�����������ֽڵ�USART1�˿�
// ��ڲ���1: int ch: ������
// ��ڲ���2: FILE* stream���ļ���
// �� �� ֵ: ���͵��ֽ�
// ע������: ����TJC_USART.h
//
//-----------------------------------------------------------------
int fputc(int ch, FILE* stream){
    while(LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)!=1);//�ȴ��������
    LL_USART_TransmitData8(TJC_HDMI_UART, (uint8_t)ch);
    return ch;
}
#endif // end of __HARDWARE_CONFIG__TJC_FPUTC_ENABLE__

//-----------------------------------------------------------------
// inline void TJC_USART_Config_Init()
//-----------------------------------------------------------------
//
// ��������: ��ʼ��TJC USART HMI�������ü�USART�˿�
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void TJC_USART_Config_Init(){
	//? ʱ������
	LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART7);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);

	//? GPIO���� 
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

	//? USART DMA����
	#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
		//? USART DMA TX����
		LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_0, LL_DMAMUX1_REQ_UART7_TX);  
		LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); 
		LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_HIGH);
		LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_NORMAL);
		LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);
		LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);
		LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_BYTE);
		LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_BYTE);
		LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_0);

		//? USART DMA RX����
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

	//? USART �ж�����
	NVIC_SetPriority(UART7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 2));
	NVIC_EnableIRQ(UART7_IRQn);

	//? USART�ṹ������
	LL_USART_InitTypeDef UART_InitStruct                     = {0};
						UART_InitStruct.PrescalerValue      = LL_USART_PRESCALER_DIV1;   // USARTԤ��Ƶϵ����1
						UART_InitStruct.BaudRate            = 921600;                    // USART������
						UART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;     // USART����bit����8bit
						UART_InitStruct.StopBits            = LL_USART_STOPBITS_1;       // USARTֹͣλ����1bit
						UART_InitStruct.Parity              = LL_USART_PARITY_NONE;      // USART��żУ�飺��
						UART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;  // USART���䷽��TX/RX˫��
						UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;   // USARTӲ��������
						UART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;  // USART������ϵ����16
	LL_USART_Init(UART7, &UART_InitStruct);

	//? FIFO����
	LL_USART_DisableFIFO(UART7);
	LL_USART_SetTXFIFOThreshold(UART7, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(UART7, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_ConfigAsyncMode(UART7);


	//? RXNE�ж�ʹ��
	#if __HARDWARE_CONFIG__TJC_RX_ENABLE__ & (~__HARDWARE_CONFIG__TJC_DMA_ENABLE__ | 1)// begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
		UART7->RDR = 0x00;
		CLEAR_BIT(UART7->ISR, USART_ISR_RXNE_RXFNE);
		LL_USART_EnableIT_RXNE_RXFNE(UART7);
	#endif // __HARDWARE_CONFIG__TJC_RX_ENABLE__ // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__

	//? DMA����
	#if __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_DMA_ENABLE__
		DMA_TJC_UART_Init();
	#endif // __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // end of __HARDWARE_CONFIG__TJC_DMA_ENABLE__

	//? USARTʹ��
	LL_USART_Enable(UART7);

	// �ȴ�USART��ʼ�����
	while((!(LL_USART_IsActiveFlag_TEACK(UART7))) || (!(LL_USART_IsActiveFlag_REACK(UART7))))
	{
	}

	//? ��ʼ��TJC USART HMI
    #if __HARDWARE_CONFIG__TJC_USART_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_USART_ENABLE__
      TJC_Usart_init();
    #endif // end of __HARDWARE_CONFIG__TJC_USART_ENABLE__

  /* USER CODE END UART7_Init 2 */
}

//-----------------------------------------------------------------
// inline void TJC_Usart_init()
//-----------------------------------------------------------------
//
// ��������: ��ʼ��TJC USART HMI
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: �ɷ���"usart.c"��"void MX_UART7_Init(void)" (��main��������"MX_UART7_Init();")  
//          ��"/* USER CODE BEGIN UART7_Init 2 */"֮��
//
//-----------------------------------------------------------------
inline void TJC_Usart_init(){
	//? ˢ��ҳ��
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
// ��������: ��ʼ��TJC USART HMI����
// ��ڲ���1: ��
// ע������: ��Ϣˢ�����Է���©
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
// ��������: UART7�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
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

			// ���ݰ���ʼ�� && (1 / 2)
			if (tjc_rx_state == 0 && Flag_TJC_RXNE == 0){
				if (RX == '&')
					tjc_rx_state = 1;
			}
			// ���ݰ���ʼ�� && (2 / 2)
			else if (tjc_rx_state == 1){
				if (RX == '&')
					tjc_rx_state = 2;
				else
					tjc_rx_state = 0; 
			}
			// ���ݰ���Ϣ�� ������
			else if (tjc_rx_state == 2){
				// ���ݰ���ֹ�� && (1 / 2)
				if (RX == '&')
					tjc_rx_state = 3;
				// ���ݰ���Ϣ��
				else
					DMA_RX_BUF[DMA_RX_Length ++] = RX;
			}
			// ���ݰ���ֹ�� && (2 / 2)
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

		//! �������ORE�ж� ����RXNE�жϻ��Զ�����USART OverRun Error Flag�жϴ����жϷ�����
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
// ��������: ���Խ�۴��������յ������ݽ��д���
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void TJC_USART_RX_HANDLER(){
	// ������ָ�������
	if (Flag_TJC_RXNE){
        // ��ӡ���յ������ݻ�����
        //printf("Main Receive:%*s\r\n", DMA_RX_Length, DMA_RX_BUF);
		printf_s(25, "at %3dm %02d.%03ds %03dus\r\n", RX_tick / 60000000, RX_tick /1000000 % 60, RX_tick /1000 % 1000, RX_tick % 1000);
		printf_s(26, "USART RX:   [len %02d]", DMA_RX_Length);
		DMA_RX_BUF[DMA_RX_Length] = '\0';
		printf_s(27, "%s", DMA_RX_BUF);

		// �汾��
		if 		(3 <= DMA_RX_Length && DMA_RX_Length <= 5 && strncmp(DMA_RX_BUF, "--v", 3) == 0){
			printf("Version: v1.16.11\r\n");
			printf("Project: Chihayo Anon\r\n");
			printf("Author : Asriel Devi\r\n");
			printf("Update : 2025-7-1 18:15\r\n");
		}
		// ��ȡʵʱʱ��
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
// ��������: ����1�ֽڵ�TJC_USART7�˿�
// ��ڲ���1: uint8_t Byte: �������ֽ�
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void TJC_SendByte(uint8_t Byte){
    while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
    LL_USART_TransmitData8(TJC_HDMI_UART, (uint8_t)Byte);
}
//-----------------------------------------------------------------
// inline void TJC_SendString_n(char* string, uint8_t size)
//-----------------------------------------------------------------
//
// ��������: ����ָ�������ַ�����TJC_USART7�˿�
// ��ڲ���1: char* string: �������ַ���
// ��ڲ���2: uint8_t size: �����ַ�������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void TJC_SendString_n(char* string, uint8_t size){
    for (i = 0;i < size;i ++){
        while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
        LL_USART_TransmitData8(TJC_HDMI_UART, string[i]);
    }
}
//-----------------------------------------------------------------
// void TJC_SendString_s(char *format, ...)
//-----------------------------------------------------------------
//
// ��������: ���͸�ʽ���ַ�����TJC_USART7�˿�,����ʽ������
// ��ڲ���1: char *format: �����͸�ʽ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_SendString_s(char *format, ...){
	va_list arg;
	va_start(arg, format);
	vsprintf(String_Temp, format, arg);
	va_end(arg);
	for (i = 0;String_Temp[i] != '\0'; i ++){
        while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
        LL_USART_TransmitData8(TJC_HDMI_UART, String_Temp[i]);
    }
}
//-----------------------------------------------------------------
// inline void TJC_SendString_sp(char *format)
//-----------------------------------------------------------------
//
// ��������: ���͸�ʽ���ַ�����TJC_USART7�˿�,�޸�ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void TJC_SendString_sp(char *format){
	for (i = 0;format[i] != '\0'; i ++){
        while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
        LL_USART_TransmitData8(TJC_HDMI_UART, format[i]);
    }
}
//-----------------------------------------------------------------
// void TJC_SendCmd_s(char *format, ...)
//-----------------------------------------------------------------
//
// ��������: ����ָ�TJC_USART7�˿ڣ�����ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// �� �� ֵ: ��
// ע������: ��
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
// ��������: ����ָ�TJC_USART7�˿ڣ��޸�ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// �� �� ֵ: ��
// ע������: ��
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����main page�е�t0�ؼ���ʾ�ַ�����ָ������
// ��ڲ���1: uint8_t WindowIndex: ����������0~12��
// ��ڲ���2: char* string: ����ʾ�ַ���
// ��ڲ���3: uint8_t size: ��ʾ�ַ�������
// ע������: ��
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����main page�е�t0�ؼ���ʾ�ַ���������ʽ������
// ��ڲ���1: char* format: ����ʾ��ʽ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// ע������: ��
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����show page�е�barx�ؼ���ʾ�ַ�����ָ������
// ��ڲ���1: uint8_t WindowIndex: ����������1~28 ��
// ��ڲ���2: char* string: ����ʾ�ַ���
// ��ڲ���3: uint8_t size: ��ʾ�ַ�������
// ע������: ��
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����main page�е�t0��barx�ؼ���ʾ�ַ���������ʽ������
// ��ڲ���1: uint8_t WindowIndex: ����������1~28 ��
// ��ڲ���2: char* format: ����ʾ��ʽ���ַ���
// ��ڲ���3: ... �� ��ʽ���������
// ע������: ��
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
// ��������: ����USART DMAת�ƹ���
// ��ڲ���1: ��
// ע������: DMA_TX_BUF �� DMA_RX_BUF ���õ�ַʱ�ǵÿ������ֲ�.STMH743��Ĭ�ϵ�ַ��DTCM��0x20000000����֧��DMA1ת�ˣ�
//			�����ַ����AXI SRAM��0x24000000��
//
//-----------------------------------------------------------------
inline void DMA_TJC_UART_Init(){
	//? ��������
	// ʹ��DMA USART����
	LL_USART_EnableDMAReq_TX(TJC_HDMI_UART);

	// ���DMA��������ж��봫������жϱ�־λ 
	LL_DMA_ClearFlag_TC0(DMA1);  // TCIF ����ڧ�ѧ֧� ��ݧѧԧ� - ���� ����������������������
	LL_DMA_ClearFlag_TE0(DMA1);  // TELF

	// ʹ��DMA��������ж��봫������ж�
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);	// TCIE
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_0);	// TEIE

	//? �����ж����ȼ�
	NVIC_SetPriority(DMA1_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    
	// ����DMA�����ַ
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_0, (uint32_t) &TJC_HDMI_UART->TDR);


	//? ��������
	#if __HARDWARE_CONFIG__TJC_RX_ENABLE__ & 0 // begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
		// ʹ��DMA USART����
		LL_USART_EnableDMAReq_RX(TJC_HDMI_UART); //���ѧ֧� �է�ҧ�� �ߧ� �٧ѧ���� DMA TJC_HDMI_UART (��֧�֧էѧ�� �� ���ڧ֧�)

		// ���DMA��������ж��봫������жϱ�־λ 
		LL_DMA_ClearFlag_TC1(DMA1);  // TCIF
		LL_DMA_ClearFlag_TE1(DMA1);  // TEIF
		// ʹ��DMA��������ж��봫������ж�
		LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);  // TCIE
		LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_1);  // TEIE

		//? �����ж����ȼ�
		NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
		NVIC_EnableIRQ(DMA1_Stream1_IRQn);

		LL_DMA_SetDataLength    (DMA1,LL_DMA_STREAM_1, DMA_RX_BUF_MAX);               // ���ý���������ݳ���
		LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_1, (uint32_t)&DMA_RX_BUF);       // �����ڴ��ַ��Ҳ�������ý��յ�����Ҫ����
		LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_1, (uint32_t) &TJC_HDMI_UART->RDR);  // ����DMA�����ַ

		LL_USART_ClearFlag_IDLE(UART7);
		// ʹ��DMA�����ж�
		LL_USART_EnableIT_IDLE(UART7);

		//ʹ�ܽ���DMA STREAM
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);  // ��������DMA
	 #endif // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__
}

//-----------------------------------------------------------------
// inline void DMA1_Stream1_IRQHandler_Func()
//-----------------------------------------------------------------
//
// ��������: UART7�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void DMA1_Stream1_IRQHandler_Func(){
	//? ����UART����
	if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
		LL_DMA_ClearFlag_TC1(DMA1); 
		if (DMA_RX_BUF_MAX - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_1) > 0){  

			// printf("DMA RX:%c\r\n", DMA_RX_BUF[0]);
			// //LL_DMA_SetDataLength    (DMA1,LL_DMA_STREAM_1, 1);                               // ���ý���������ݳ���
			// LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_1, (uint32_t) &DMA_RX_BUF);           // �����ڴ��ַ��Ҳ�������ý��յ�����Ҫ����
			// LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);  // ��������DMA

			/* �������������ݴ�ӡ */
			//printf("%s", DMA_RX_BUF);
			//printf("TC1 RX Length:%d\r\n", DMA_RX_BUF_MAX - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_1));
			
			/* ��ӡDMA������ */
			//printf("DMA data full !\r\n");
		}
	}	

	//? ���ݴ������
	if (LL_DMA_IsActiveFlag_TE1(DMA1)) {
		LL_DMA_ClearFlag_TE1(DMA1);  

		/* ��ӡ���������־ */
    	printf("DMA data transfer failed !\r\n");
	}
}

//-----------------------------------------------------------------
// void TJC_DMA_SendString_n(const char *string, uint16_t size)
//-----------------------------------------------------------------
//
// ��������: ����ָ�������ַ�����TJC_USART7�˿�
// ��ڲ���1: char* string: �������ַ���
// ��ڲ���2: uint8_t size: �����ַ�������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendString_n(const char *string, uint16_t size){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
	strncpy(DMA_TX_BUF, string, size);

	//ʧ��DMA���ڷ���������
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
	// ����DMA���ͳ���
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, size);
	// ���÷���DMA
    LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_0, (uint32_t) &DMA_TX_BUF);
	
	#if 0	// DEBUG
		printf_s_d("S1 %s", DMA_TX_BUF);
		printf_s_d("MemoryAddr: 0x%08x\t", LL_DMA_GetMemoryAddress(DMA1, LL_DMA_STREAM_0));
		printf_s_d("PeriphAddr: 0x%08x\t\r\n", LL_DMA_GetPeriphAddress(DMA1, LL_DMA_STREAM_0));

		printf_s_d("DMA_TX_BUF: 0x%08x\t", &DMA_TX_BUF);
		printf_s_d("U1->TDR:    0x%08x\t\r\n", (uint32_t)&(TJC_HDMI_UART->TDR));
	#endif
	
	//ʹ��DMA���ڷ���������
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
}
//-----------------------------------------------------------------
// void TJC_DMA_SendString_s(char *format, ...)
//-----------------------------------------------------------------
//
// ��������: ���͸�ʽ���ַ�����TJC_USART7�˿�,����ʽ������
// ��ڲ���1: char *format: �����͸�ʽ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendString_s(char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
// ��������: ���͸�ʽ���ַ�����TJC_USART7�˿�,�޸�ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendString_sp(char *format){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
// ��������: ����ָ�TJC_USART7�˿ڣ�����ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendCmd_s(char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
// ��������: ����ָ�TJC_USART7�˿ڣ��޸�ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendCmd_sp(char *format){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����main page�е�t0�ؼ���ʾ�ַ�����ָ������
// ��ڲ���1: char* string: ����ʾ�ַ���
// ��ڲ���2: uint8_t size: ��ʾ�ַ�������
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_MAIN_n(char *string, uint8_t size){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����main page�е�t0�ؼ���ʾ�ַ���������ʽ������
// ��ڲ���1: char* format: ����ʾ��ʽ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_MAIN_s(char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����show page�е�barx�ؼ���ʾ�ַ�����ָ������
// ��ڲ���1: uint8_t WindowIndex: ����������0~12��
// ��ڲ���2: char* string: ����ʾ�ַ���
// ��ڲ���3: uint8_t size: ��ʾ�ַ�������
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_SHOW_n(uint8_t WindowIndex, char *string, uint8_t size){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
// ��������: ����ָ�TJC_USART7�˿�,���ڴ�����main page�е�t0��barx�ؼ���ʾ�ַ���������ʽ������
// ��ڲ���1: uint8_t WindowIndex: ����������0~12��
// ��ڲ���2: char* format: ����ʾ��ʽ���ַ���
// ��ڲ���3: ... �� ��ʽ���������
// ע������: ��
//
//-----------------------------------------------------------------
void TJC_DMA_SendTxt_SHOW_s(uint8_t WindowIndex, char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(TJC_HDMI_UART)); //�ȴ��������
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
