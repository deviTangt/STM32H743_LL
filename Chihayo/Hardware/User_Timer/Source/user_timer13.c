#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER13_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer13.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer13_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer13_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// ��������: timer13��ʱ�����ó�ʼ��
// ��ڲ���1: uint16_t _Prescaler:  Ԥ��Ƶֵ
// ��ڲ���2: uint32_t _Autoreload: ����ϵ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer13_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? ��ʼ��ʱ��
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM13);

	//? �����ж�
	NVIC_SetPriority(TIM8_UP_TIM13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

	//? ���ö�ʱ���ṹ�� 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // �Զ���װ��ֵ      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // ʱ�ӷ�Ƶ
	LL_TIM_Init(TIM13, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload(TIM13);                                // �����Զ�װ��ģʽ
	LL_TIM_SetClockSource    (TIM13, LL_TIM_CLOCKSOURCE_INTERNAL);  // ����ʱ��Դ���ڲ�����
	LL_TIM_SetTriggerOutput  (TIM13, LL_TIM_TRGO_RESET);            // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM13);

  //? UPDATE��־λ
	#if __HARDWARE_CONFIG__TIMER13_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER13_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM13);  // ������ϼ��������־λ
		LL_TIM_EnableIT_UPDATE (TIM13);  // ʹ�ܶ�ʱ�����ϼ����ж�
  #endif // end of __HARDWARE_CONFIG__TIMER13_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer13_start()
//-----------------------------------------------------------------
//
// ��������: ������ʱ��13
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer13_start(){
  if(__HARDWARE_CONFIG__TIMER13_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM13);  // ������ϼ��������־λ
  LL_TIM_EnableCounter   (TIM13);  // ʹ�ܶ�ʱ����ʼ����
}
//-----------------------------------------------------------------
// inline void timer13_stop()
//-----------------------------------------------------------------
//
// ��������: �رն�ʱ��13
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer13_stop(){
  if(__HARDWARE_CONFIG__TIMER13_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM13);   // ������ϼ��������־λ
  LL_TIM_DisableCounter   (TIM13);  // �رն�ʱ������
}

//-----------------------------------------------------------------
// inline void TIM13_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: ��ʱ��13�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM13_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void TIM13_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM13) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(TIM13);				//������ϼ��������־λ
		
		timer13_call_cnt ++;
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__


#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER8_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM8_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM8->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM3 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM8_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER8_ENABLE__

  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER13_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM13_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM13->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM13 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM13_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER13_ENABLE__
  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}
#endif
