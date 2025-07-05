#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__UART_FPGA_ENABLE__	// begin of __HARDWARE_CONFIG__UART_FPGA_ENABLE__
//*******************************// include _h files    //************************************//
#include "uart_fpga.h"
//*******************************// define parameters   //************************************//

static uint32_t i;
static char String_Temp[STRING_FPGA_MAX_LENGTH];
#if __HARDWARE_CONFIG__FPGA_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__
	char DMA_FPGA_TX_BUF[DMA_FPGA_TX_BUF_MAX] __attribute__ ((section ("DMA_FPGA_TX_BUF"),  zero_init));
	char DMA_FPGA_RX_BUF[DMA_FPGA_RX_BUF_MAX] __attribute__ ((section ("DMA_FPGA_RX_BUF"),  zero_init));

	uint16_t DMA_FPGA_RX_Length = 0;
	static uint8_t  FPGA_RX     = 0;
	uint32_t FPGA_RX_tick       = 0, FPGA_RX_tick_pre = 0;
#endif // end of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__

//*******************************// parameters          //************************************//
//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void FPGA_USART_Config_Init()
//-----------------------------------------------------------------
//
// ��������: ��ʼ��FPGA UART�������ü�UART�˿�
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void FPGA_USART_Config_Init(){
	//? ʱ������
	LL_RCC_SetUSARTClockSource(LL_RCC_USART16_CLKSOURCE_PCLK2);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

	//? GPIO���� 
	//? PB14   ------> USART1_TX
	//? PB15   ------> USART1_RX
	LL_GPIO_InitTypeDef GPIO_InitStruct            = {0};
	                    GPIO_InitStruct.Pin        = FPGA_TX_Pin;
	                    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
	                    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
	                    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	                    GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
	                    GPIO_InitStruct.Alternate  = LL_GPIO_AF_4;
	LL_GPIO_Init(FPGA_TX_GPIO_Port, &GPIO_InitStruct);
	                    GPIO_InitStruct.Pin        = FPGA_RX_Pin;
	                    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
	                    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
	                    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	                    GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
	                    GPIO_InitStruct.Alternate  = LL_GPIO_AF_4;
	LL_GPIO_Init(FPGA_RX_GPIO_Port, &GPIO_InitStruct);

	//? USART DMA����
	#if __HARDWARE_CONFIG__FPGA_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__
		//? USART DMA TX����
		LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_5, LL_DMAMUX1_REQ_USART1_TX);  
		LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_5, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); 
		LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_5, LL_DMA_PRIORITY_HIGH);
		LL_DMA_SetMode(DMA1, LL_DMA_STREAM_5, LL_DMA_MODE_NORMAL);
		LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_5, LL_DMA_PERIPH_NOINCREMENT);
		LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_5, LL_DMA_MEMORY_INCREMENT);
		LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_5, LL_DMA_PDATAALIGN_BYTE);
		LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_5, LL_DMA_MDATAALIGN_BYTE);
		LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_5);

		//? USART DMA RX����
		#if __HARDWARE_CONFIG__FPGA_RX_ENABLE__ & 0 // begin of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
			LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_6, LL_DMAMUX1_REQ_USART1_RX);
			LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
			LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_6, LL_DMA_PRIORITY_HIGH);
			LL_DMA_SetMode(DMA1, LL_DMA_STREAM_6, LL_DMA_MODE_CIRCULAR);
			LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_6, LL_DMA_PERIPH_NOINCREMENT);
			LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_6, LL_DMA_MEMORY_INCREMENT);
			LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_6, LL_DMA_PDATAALIGN_BYTE);
			LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_6, LL_DMA_MDATAALIGN_BYTE);
			LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_6);
		#endif // end of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
	#endif // __HARDWARE_CONFIG__FPGA_DMA_ENABLE__ // end of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__

	//? USART �ж�����
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 1));
	NVIC_EnableIRQ(USART1_IRQn);

	//? USART�ṹ������
	LL_USART_InitTypeDef USART_InitStruct                     = {0};
	                     USART_InitStruct.PrescalerValue      = LL_USART_PRESCALER_DIV1;   // USARTԤ��Ƶϵ����1
	                     USART_InitStruct.BaudRate            = 115200;                    // USART������
	                     USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;     // USART����bit����8bit
	                     USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;       // USARTֹͣλ����1bit
	                     USART_InitStruct.Parity              = LL_USART_PARITY_NONE;      // USART��żУ�飺��
	                     USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;  // USART���䷽��TX/RX˫��
	                     USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;   // USARTӲ��������
	                     USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;  // USART������ϵ����16
	LL_USART_Init(USART1, &USART_InitStruct);

	//? FIFO����
	LL_USART_DisableFIFO(USART1);
	LL_USART_SetTXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_ConfigAsyncMode(USART1);


	//? RXNE�ж�ʹ��
	#if __HARDWARE_CONFIG__FPGA_RX_ENABLE__ & (~__HARDWARE_CONFIG__FPGA_DMA_ENABLE__ | 1)// begin of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
		FPGA_UART->RDR = 0x00;
		CLEAR_BIT(USART1->ISR, USART_ISR_RXNE_RXFNE);
		LL_USART_EnableIT_RXNE_RXFNE(USART1);
	#endif // end of __HARDWARE_CONFIG__FPGA_RX_ENABLE__

	//? DMA����
	#if __HARDWARE_CONFIG__FPGA_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__
		DMA_FPGA_UART_Init();
	#endif // end of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__

	//? USARTʹ��
	LL_USART_Enable(USART1);

	// �ȴ�USART��ʼ�����
	while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
	{
	}
}

//-----------------------------------------------------------------
// inline void USART1_IRQHandler_Func()
//-----------------------------------------------------------------
//
// ��������: USART1�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void USART1_IRQHandler_Func(){
	#if __HARDWARE_CONFIG__FPGA_RX_ENABLE__  // begin of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
		if (LL_USART_IsActiveFlag_RXNE_RXFNE(USART1)){
			FPGA_RX = LL_USART_ReceiveData8(USART1);
			DMA_FPGA_RX_BUF[DMA_FPGA_RX_Length ++] = FPGA_RX;

			FPGA_RX_tick_pre = bsp_us;
		}

		//! �������ORE�ж� ����RXNE�жϻ��Զ�����USART OverRun Error Flag�жϴ����жϷ�����
		if (LL_USART_IsActiveFlag_ORE(USART1)){ 
			LL_USART_ClearFlag_ORE(USART1);
		}

		if (LL_USART_IsActiveFlag_PE(USART1)){
			LL_USART_DisableIT_PE(USART1);
		}
	#endif // end of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
}

//-----------------------------------------------------------------
// inline void FPGA_RX_HANDLER()
//-----------------------------------------------------------------
//
// ��������: ��FPGA���ڽ��յ������ݽ��д���
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void FPGA_RX_HANDLER(){
	FPGA_RX_tick = bsp_us;
	//? ���ն��зǿ��ҽ��ռ������
	if (DMA_FPGA_RX_Length && FPGA_RX_tick > FPGA_RX_tick_pre + FPGA_RX_HANDLE_TIME){
		DMA_FPGA_RX_BUF[DMA_FPGA_RX_Length] = '\0';
		printf_s(23, "RX: [len-%02d]", DMA_FPGA_RX_Length);
		printf_s(24, "|%s|", DMA_FPGA_RX_BUF);

		DMA_FPGA_RX_Length = 0;
	}
} 



//-----------------------------------------------------------------
// inline void FPGA_SendByte(uint8_t Byte)
//-----------------------------------------------------------------
//
// ��������: ����1�ֽڵ�FPGA_USART5�˿�
// ��ڲ���1: uint8_t Byte: �������ֽ�
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void FPGA_SendByte(uint8_t Byte){
    while(!LL_USART_IsActiveFlag_TC(FPGA_UART)); //�ȴ��������
    LL_USART_TransmitData8(FPGA_UART, (uint8_t)Byte);
}
//-----------------------------------------------------------------
// inline void FPGA_SendString_n(char* string, uint8_t size)
//-----------------------------------------------------------------
//
// ��������: ����ָ�������ַ�����FPGA_USART5�˿�
// ��ڲ���1: char* string: �������ַ���
// ��ڲ���2: uint8_t size: �����ַ�������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void FPGA_SendString_n(char* string, uint8_t size){
    for (i = 0;i < size;i ++){
        while(!LL_USART_IsActiveFlag_TC(FPGA_UART)); //�ȴ��������
        LL_USART_TransmitData8(FPGA_UART, string[i]);
    }
}
//-----------------------------------------------------------------
// void FPGA_SendString_s(char *format, ...)
//-----------------------------------------------------------------
//
// ��������: ���͸�ʽ���ַ�����FPGA_USART5�˿�,����ʽ������
// ��ڲ���1: char *format: �����͸�ʽ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void FPGA_SendString_s(char *format, ...){
	va_list arg;
	va_start(arg, format);
	vsprintf(String_Temp, format, arg);
	va_end(arg);
	for (i = 0;String_Temp[i] != '\0'; i ++){
        while(!LL_USART_IsActiveFlag_TC(FPGA_UART)); //�ȴ��������
        LL_USART_TransmitData8(FPGA_UART, String_Temp[i]);
    }
}
//-----------------------------------------------------------------
// inline void FPGA_SendString_sp(char *format)
//-----------------------------------------------------------------
//
// ��������: ���͸�ʽ���ַ�����FPGA_USART5�˿�,�޸�ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void FPGA_SendString_sp(char *format){
	for (i = 0;format[i] != '\0'; i ++){
        while(!LL_USART_IsActiveFlag_TC(FPGA_UART)); //�ȴ��������
        LL_USART_TransmitData8(FPGA_UART, format[i]);
    }
}



#if __HARDWARE_CONFIG__FPGA_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__
//-----------------------------------------------------------------
// inline void DMA_FPGA_UART_Init()
//-----------------------------------------------------------------
//
// ��������: ����FPGA USART DMAת�ƹ���
// ��ڲ���1: ��
// ע������: DMA_FPGA_TX_BUF �� DMA_RX_BUF ���õ�ַʱ�ǵÿ������ֲ�.STMH743��Ĭ�ϵ�ַ��DTCM��0x20000000����֧��DMA1ת�ˣ�
//			�����ַ����AXI SRAM��0x24000000��
//
//-----------------------------------------------------------------
inline void DMA_FPGA_UART_Init(){
	//? ��������
	// ʹ��DMA USART����
	LL_USART_EnableDMAReq_TX(FPGA_UART);

	// ���DMA��������ж��봫������жϱ�־λ 
	LL_DMA_ClearFlag_TC5(DMA1);  // TCIF ����ڧ�ѧ֧� ��ݧѧԧ� - ���� ����������������������
	LL_DMA_ClearFlag_TE5(DMA1);  // TELF

	// ʹ��DMA��������ж��봫������ж�
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_5);	// TCIE
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_5);	// TEIE

	//? �����ж����ȼ�
	NVIC_SetPriority(DMA1_Stream5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 1));
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    
	// ����DMA�����ַ
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_5, (uint32_t) &FPGA_UART->TDR);

	//? ��������
	#if __HARDWARE_CONFIG__FPGA_RX_ENABLE__ & 0 // begin of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
		// ʹ��DMA USART����
		LL_USART_EnableDMAReq_RX(FPGA_UART); //���ѧ֧� �է�ҧ�� �ߧ� �٧ѧ���� DMA FPGA_UART (��֧�֧էѧ�� �� ���ڧ֧�)

		// ���DMA��������ж��봫������жϱ�־λ 
		LL_DMA_ClearFlag_TC1(DMA1);  // TCIF
		LL_DMA_ClearFlag_TE1(DMA1);  // TEIF
		// ʹ��DMA��������ж��봫������ж�
		LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_6);  // TCIE
		LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_6);  // TEIE

		//? �����ж����ȼ�
		NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
		NVIC_EnableIRQ(DMA1_Stream1_IRQn);

		LL_DMA_SetDataLength    (DMA1,LL_DMA_STREAM_6, DMA_RX_BUF_MAX);               // ���ý���������ݳ���
		LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_6, (uint32_t)&DMA_RX_BUF);       // �����ڴ��ַ��Ҳ�������ý��յ�����Ҫ����
		LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_6, (uint32_t) &FPGA_UART->RDR);  // ����DMA�����ַ

		LL_USART_ClearFlag_IDLE(USART1);
		// ʹ��DMA�����ж�
		LL_USART_EnableIT_IDLE(USART1);

		//ʹ�ܽ���DMA STREAM
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);  // ��������DMA
	 #endif // end of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
}

//-----------------------------------------------------------------
// inline void DMA1_Stream5_IRQHandler_Func()
//-----------------------------------------------------------------
//
// ��������: DMA1_Stream5�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void DMA1_Stream5_IRQHandler_Func(){
	//? ����UART����
	if (LL_DMA_IsActiveFlag_TC5(DMA1)) {
		LL_DMA_ClearFlag_TC5(DMA1); 
		if (DMA_RX_BUF_MAX - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_5) > 0){  

			// printf("DMA FPGA_RX:%c\r\n", DMA_RX_BUF[0]);
			// //LL_DMA_SetDataLength    (DMA1,LL_DMA_STREAM_1, 1);                               // ���ý���������ݳ���
			// LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_1, (uint32_t) &DMA_RX_BUF);           // �����ڴ��ַ��Ҳ�������ý��յ�����Ҫ����
			// LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);  // ��������DMA

			/* �������������ݴ�ӡ */
			//printf("%s", DMA_RX_BUF);
			//printf("TC1 FPGA_RX Length:%d\r\n", DMA_RX_BUF_MAX - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_1));
			
			/* ��ӡDMA������ */
			//printf("DMA data full !\r\n");
		}
	}	

	//? ���ݴ������
	if (LL_DMA_IsActiveFlag_TE5(DMA1)) {
		LL_DMA_ClearFlag_TE5(DMA1);  

		/* ��ӡ���������־ */
    	printf("DMA data transfer failed !\r\n");
	}
}

//-----------------------------------------------------------------
// void FPGA_DMA_SendString_n(const char *string, uint16_t size)
//-----------------------------------------------------------------
//
// ��������: ����ָ�������ַ�����FPGA_USART5�˿�
// ��ڲ���1: char* string: �������ַ���
// ��ڲ���2: uint8_t size: �����ַ�������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void FPGA_DMA_SendString_n(const char *string, uint16_t size){
	while(!LL_USART_IsActiveFlag_TC(FPGA_UART)); //�ȴ��������
	strncpy(DMA_FPGA_TX_BUF, string, size);

	//ʧ��DMA���ڷ���������
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
	// ����DMA���ͳ���
  	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, size);
	// ���÷���DMA
  	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_5, (uint32_t) &DMA_FPGA_TX_BUF);
	
	//ʹ��DMA���ڷ���������
  	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
}
//-----------------------------------------------------------------
// void FPGA_DMA_SendString_s(char *format, ...)
//-----------------------------------------------------------------
//
// ��������: ���͸�ʽ���ַ�����FPGA_USART5�˿�,����ʽ������
// ��ڲ���1: char *format: �����͸�ʽ���ַ���
// ��ڲ���2: ... �� ��ʽ���������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void FPGA_DMA_SendString_s(char *format, ...){
	while(!LL_USART_IsActiveFlag_TC(FPGA_UART)); //�ȴ��������
	va_list arg;
	va_start(arg, format);
	vsprintf(DMA_FPGA_TX_BUF, format, arg);
	va_end(arg);

	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, strlen(DMA_FPGA_TX_BUF));
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_5, (uint32_t) &DMA_FPGA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5); 
}
//-----------------------------------------------------------------
// void FPGA_DMA_SendString_sp(char *format)
//-----------------------------------------------------------------
//
// ��������: ���͸�ʽ���ַ�����FPGA_USART5�˿�,�޸�ʽ������
// ��ڲ���1: char *format: ������ָ���ַ���
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void FPGA_DMA_SendString_sp(char *format){
	while(!LL_USART_IsActiveFlag_TC(FPGA_UART)); //�ȴ��������
	strncpy(DMA_FPGA_TX_BUF, format, strlen(format));

	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, strlen(format));
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_STREAM_5, (uint32_t) &DMA_FPGA_TX_BUF);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5); 
}
#endif	// end of __HARDWARE_CONFIG__FPGA_DMA_ENABLE__

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__UART_FPGA_ENABLE__
