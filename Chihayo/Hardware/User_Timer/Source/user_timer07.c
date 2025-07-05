#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER7_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER7_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer07.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer7_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer7_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// ��������: timer7��ʱ�����ó�ʼ��
// ��ڲ���1: uint16_t _Prescaler:  Ԥ��Ƶֵ
// ��ڲ���2: uint32_t _Autoreload: ����ϵ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer7_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? ��ʼ��ʱ��
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);

	//? �����ж�
	NVIC_SetPriority(TIM7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM7_IRQn);

	//? ���ö�ʱ���ṹ�� 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // �Զ���װ��ֵ      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // ʱ�ӷ�Ƶ
	LL_TIM_Init(TIM7, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload(TIM7);                                // �����Զ�װ��ģʽ
	LL_TIM_SetClockSource    (TIM7, LL_TIM_CLOCKSOURCE_INTERNAL);  // ����ʱ��Դ���ڲ�����
	LL_TIM_SetTriggerOutput  (TIM7, LL_TIM_TRGO_RESET);            // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM7);

  //? UPDATE��־λ
	#if __HARDWARE_CONFIG__TIMER7_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER7_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM7);  // ������ϼ��������־λ
		LL_TIM_EnableIT_UPDATE (TIM7);  // ʹ�ܶ�ʱ�����ϼ����ж�
  #endif // end of __HARDWARE_CONFIG__TIMER7_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer7_start()
//-----------------------------------------------------------------
//
// ��������: ������ʱ��7
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer7_start(){
  if(__HARDWARE_CONFIG__TIMER7_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM7);  // ������ϼ��������־λ
  LL_TIM_EnableCounter   (TIM7);  // ʹ�ܶ�ʱ����ʼ����
}
//-----------------------------------------------------------------
// inline void timer7_stop()
//-----------------------------------------------------------------
//
// ��������: �رն�ʱ��7
// ��ڲ���1: ��
// �� �� ֵ: ��SSSSS
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer7_stop(){
  if(__HARDWARE_CONFIG__TIMER7_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM7);   // ������ϼ��������־λ
  LL_TIM_DisableCounter   (TIM7);  // �رն�ʱ������
}

//-----------------------------------------------------------------
// inline void TIM7_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: ��ʱ��7�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM7_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void TIM7_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM7) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(TIM7);				//������ϼ��������־λ
		
		timer7_call_cnt ++;
	}
}



//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER7_ENABLE__


#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER7_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER7_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM7_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM7->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM7 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM7_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER7_ENABLE__
  /* USER CODE END TIM7_IRQn 0 */
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}
#endif
