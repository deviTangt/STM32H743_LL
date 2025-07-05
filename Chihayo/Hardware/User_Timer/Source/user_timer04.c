#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER4_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer04.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer4_call_cnt = 0;

uint32_t sample_ad_value[300];
uint16_t sample_ad_cnt;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer4_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// ��������: timer4��ʱ�����ó�ʼ��
// ��ڲ���1: uint16_t _Prescaler:  Ԥ��Ƶֵ
// ��ڲ���2: uint32_t _Autoreload: ����ϵ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer4_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? ��ʼ��ʱ��
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

	//? �����ж�
	NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM4_IRQn);

	//? ���ö�ʱ���ṹ�� 100kHz
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // �Զ���װ��ֵ      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // ʱ�ӷ�Ƶ
	LL_TIM_Init(TIM4, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload(TIM4);                                // �����Զ�װ��ģʽ
	LL_TIM_SetClockSource    (TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);  // ����ʱ��Դ���ڲ�����
	LL_TIM_SetTriggerOutput  (TIM4, LL_TIM_TRGO_RESET);            // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM4);

  //? UPDATE��־λ
	#if __HARDWARE_CONFIG__TIMER4_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER4_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM4);  // ������ϼ��������־λ
		LL_TIM_EnableIT_UPDATE (TIM4);  // ʹ�ܶ�ʱ�����ϼ����ж�
  #endif // end of __HARDWARE_CONFIG__TIMER4_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer4_start()
//-----------------------------------------------------------------
//
// ��������: ������ʱ��4
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer4_start(){
    if(__HARDWARE_CONFIG__TIMER4_INT_UPDATE_ENABLE__)
      LL_TIM_ClearFlag_UPDATE(TIM4);  // ������ϼ��������־λ
    LL_TIM_EnableCounter   (TIM4);  // ʹ�ܶ�ʱ����ʼ����
}
//-----------------------------------------------------------------
// inline void timer4_stop()
//-----------------------------------------------------------------
//
// ��������: �رն�ʱ��4
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer4_stop(){
    if(__HARDWARE_CONFIG__TIMER4_INT_UPDATE_ENABLE__)
      LL_TIM_ClearFlag_UPDATE(TIM4);   // ������ϼ��������־λ
    LL_TIM_DisableCounter   (TIM4);  // �رն�ʱ������
}

//-----------------------------------------------------------------
// inline void TIM4_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: ��ʱ��4�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM4_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void TIM4_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM4) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(TIM4);				//������ϼ��������־λ
		
    // if (sample_ad_cnt < fft_adc_n){
    //   sample_ad_value[sample_ad_cnt ++] = DMA_ADC1_RX_BUF[0];
    //   timer4_call_cnt ++;
    // }
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__


#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER4_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM4_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM4->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM4 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM4_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER4_ENABLE__
  /* USER CODE END TIM4_IRQn 0 */
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}
#endif
