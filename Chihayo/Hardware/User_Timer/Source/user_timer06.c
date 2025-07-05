#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER6_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER6_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer06.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer6_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer6_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// ��������: timer6��ʱ�����ó�ʼ��
// ��ڲ���1: uint16_t _Prescaler:  Ԥ��Ƶֵ
// ��ڲ���2: uint32_t _Autoreload: ����ϵ��s
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer6_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? ��ʼ��ʱ��
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

	//? �����ж�
	NVIC_SetPriority(TIM6_DAC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//? ���ö�ʱ���ṹ�� 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // �Զ���װ��ֵ      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // ʱ�ӷ�Ƶ
	LL_TIM_Init(TIM6, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload(TIM6);                                // �����Զ�װ��ģʽ
	LL_TIM_SetClockSource    (TIM6, LL_TIM_CLOCKSOURCE_INTERNAL);  // ����ʱ��Դ���ڲ�����
	LL_TIM_SetTriggerOutput  (TIM6, LL_TIM_TRGO_RESET);            // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM6);

  //? UPDATE��־λ
	#if __HARDWARE_CONFIG__TIMER6_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER6_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM6);  // ������ϼ��������־λ
		LL_TIM_EnableIT_UPDATE (TIM6);  // ʹ�ܶ�ʱ�����ϼ����ж�
  #endif // end of __HARDWARE_CONFIG__TIMER6_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer6_start()
//-----------------------------------------------------------------
//
// ��������: ������ʱ��6
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer6_start(){
  if(__HARDWARE_CONFIG__TIMER6_INT_UPDATE_ENABLE__) 
    LL_TIM_ClearFlag_UPDATE(TIM6);  // ������ϼ��������־λ
  LL_TIM_EnableCounter   (TIM6);  // ʹ�ܶ�ʱ����ʼ����
}
//-----------------------------------------------------------------
// inline void timer6_stop()
//-----------------------------------------------------------------
//
// ��������: �رն�ʱ��6
// ��ڲ���1: ��
// �� �� ֵ: ��SS
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer6_stop(){
  if(__HARDWARE_CONFIG__TIMER6_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM6);   // ������ϼ��������־λ
  LL_TIM_DisableCounter   (TIM6);  // �رն�ʱ������
}

//-----------------------------------------------------------------
// inline void TIM6_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: ��ʱ��6�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM6_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void TIM6_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM6) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(TIM6);				//������ϼ��������־λ
		
		timer6_call_cnt ++;
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER6_ENABLE__



#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM6 global interrupt, DAC1_CH1 and DAC1_CH2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER6_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER6_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM6_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM6->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM6 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM6_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER6_ENABLE__
  /* USER CODE END TIM6_DAC_IRQn 0 */
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}
#endif
