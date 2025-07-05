#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER15_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER15_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer15.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer15_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer15_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// ��������: timer15��ʱ�����ó�ʼ��
// ��ڲ���1: uint16_t _Prescaler:  Ԥ��Ƶֵ
// ��ڲ���2: uint32_t _Autoreload: ����ϵ��
// �� �� ֵ: ��
// ע������: ������APB2�����ϣ���һ�㶨ʱ����ͬ
//
//-----------------------------------------------------------------
inline void timer15_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? ��ʼ��ʱ��
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM15);

	//? �����ж�
	NVIC_SetPriority(TIM15_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM15_IRQn);

	//? ���ö�ʱ���ṹ�� 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // �Զ���װ��ֵ      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // ʱ�ӷ�Ƶ
	LL_TIM_Init(TIM15, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload(TIM15);                                // �����Զ�װ��ģʽ
	LL_TIM_SetClockSource    (TIM15, LL_TIM_CLOCKSOURCE_INTERNAL);  // ����ʱ��Դ���ڲ�����
	LL_TIM_SetTriggerOutput  (TIM15, LL_TIM_TRGO_RESET);            // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM15);

  //? UPDATE��־λ
	#if __HARDWARE_CONFIG__TIMER15_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER15_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM15);  // ������ϼ��������־λ
		LL_TIM_EnableIT_UPDATE (TIM15);  // ʹ�ܶ�ʱ�����ϼ����ж�
  #endif // end of __HARDWARE_CONFIG__TIMER15_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer15_start()
//-----------------------------------------------------------------
//
// ��������: ������ʱ��15
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer15_start(){
  if(__HARDWARE_CONFIG__TIMER15_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM15);  // ������ϼ��������־λ
  LL_TIM_EnableCounter   (TIM15);  // ʹ�ܶ�ʱ����ʼ����
}
//-----------------------------------------------------------------
// inline void timer15_stop()
//-----------------------------------------------------------------
//
// ��������: �رն�ʱ��15
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer15_stop(){
  if(__HARDWARE_CONFIG__TIMER15_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM15);   // ������ϼ��������־λ
  LL_TIM_DisableCounter   (TIM15);  // �رն�ʱ������
}

//-----------------------------------------------------------------
// inline void TIM15_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: ��ʱ��15�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM15_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void TIM15_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM15) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(TIM15);				//������ϼ��������־λ
		
		timer15_call_cnt ++;
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER15_ENABLE__


#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM15 global interrupt.
  */
void TIM15_IRQHandler(void)
{
  /* USER CODE BEGIN TIM15_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER15_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER15_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM15_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM15->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM15 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM15_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER15_ENABLE__
  /* USER CODE END TIM15_IRQn 0 */
  /* USER CODE BEGIN TIM15_IRQn 1 */

  /* USER CODE END TIM15_IRQn 1 */
}
#endif
