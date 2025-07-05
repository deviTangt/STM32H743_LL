#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER1_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer01.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer1_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer1_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//s
// ��������: timer1��ʱ�����ó�ʼ��
// ��ڲ���1: uint16_t _Prescaler:  Ԥ��Ƶֵ
// ��ڲ���2: uint32_t _Autoreload: ����ϵ��
// �� �� ֵ: ��
// ע������: ������APB2�����ϣ���һ�㶨ʱ����ͬ
//
//-----------------------------------------------------------------
inline void timer1_config_init(uint16_t _Prescaler, uint32_t _Autoreload){ // ���벶��
	//? ��ʼ��ʱ��
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

	//? �����ж�
	NVIC_SetPriority(TIM1_UP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM1_UP_IRQn);

	//? ���ö�ʱ���ṹ�� 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // �Զ���װ��ֵ      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // ʱ�ӷ�Ƶ
	LL_TIM_Init(TIM1, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload     (TIM1);                                // �����Զ�װ��ģʽ
	LL_TIM_SetClockSource         (TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);  // ����ʱ��Դ���ڲ�����
	LL_TIM_SetTriggerOutput       (TIM1, LL_TIM_TRGO_RESET);            // ��λ����
	LL_TIM_SetTriggerOutput2     (TIM1, LL_TIM_TRGO2_RESET);            // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM1);                                // ��������ģʽ

  //? UPDATE��־λ
  #if __HARDWARE_CONFIG__TIMER1_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER1_INT_UPDATE_ENABLE__
    LL_TIM_ClearFlag_UPDATE(TIM1);  // ������ϼ��������־λ
    LL_TIM_EnableIT_UPDATE (TIM1);  // ʹ�ܶ�ʱ�����ϼ����ж�
  #endif // end of __HARDWARE_CONFIG__TIMER1_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer1_start()
//-----------------------------------------------------------------
//
// ��������: ������ʱ��1
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer1_start(){
    if(__HARDWARE_CONFIG__TIMER1_INT_UPDATE_ENABLE__)
      LL_TIM_ClearFlag_UPDATE(TIM1);  // ������ϼ��������־λ
    LL_TIM_EnableCounter   (TIM1);  // ʹ�ܶ�ʱ����ʼ����
}
//-----------------------------------------------------------------
// inline void timer1_stop()
//-----------------------------------------------------------------
//
// ��������: �رն�ʱ��1
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer1_stop(){
    if(__HARDWARE_CONFIG__TIMER1_INT_UPDATE_ENABLE__)
      LL_TIM_ClearFlag_UPDATE(TIM1);   // ������ϼ��������־λ
    LL_TIM_DisableCounter   (TIM1);  // �رն�ʱ������
}

//-----------------------------------------------------------------
// inline void TIM1_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: ��ʱ��1�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM1_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void TIM1_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM1) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(TIM1);				//������ϼ��������־λ
		
        timer1_call_cnt ++;
	}
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__


#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER1_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM1_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM1->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM3 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM1_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER1_ENABLE__
  /* USER CODE END TIM1_UP_IRQn 0 */
  /* USER CODE BEGIN TIM1_UP_IRQn 1 */

  /* USER CODE END TIM1_UP_IRQn 1 */
}
#endif
