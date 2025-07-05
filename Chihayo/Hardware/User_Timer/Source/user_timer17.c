#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER17_ENABLE__	// begin of __HARDWARE_CONFIG__USER_TIMER17_ENABLE__
//*******************************// include _h files    //************************************//
#include "user_timer17.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

uint32_t timer17_call_cnt = 0;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void timer17_config_init(uint16_t _Prescaler, uint32_t _Autoreload)
//-----------------------------------------------------------------
//
// ��������: timer17��ʱ�����ó�ʼ��
// ��ڲ���1: uint16_t _Prescaler:  Ԥ��Ƶֵ
// ��ڲ���2: uint32_t _Autoreload: ����ϵ��
// �� �� ֵ: ��
// ע������: ������APB2�����ϣ���һ�㶨ʱ����ͬ
//
//-----------------------------------------------------------------
inline void timer17_config_init(uint16_t _Prescaler, uint32_t _Autoreload){
	//? ��ʼ��ʱ��
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM17);

	//? �����ж�
	NVIC_SetPriority(TIM17_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 2));
    NVIC_EnableIRQ(TIM17_IRQn);

	//? ���ö�ʱ���ṹ�� 1000us
	LL_TIM_InitTypeDef TIM_InitStruct               = {0};
	                   TIM_InitStruct.Prescaler     = _Prescaler;                 // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;      // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload    = _Autoreload;                // �Զ���װ��ֵ      
	                   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  // ʱ�ӷ�Ƶ
	LL_TIM_Init(TIM17, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload(TIM17);                                // �����Զ�װ��ģʽ
	LL_TIM_SetClockSource    (TIM17, LL_TIM_CLOCKSOURCE_INTERNAL);  // ����ʱ��Դ���ڲ�����
	LL_TIM_SetTriggerOutput  (TIM17, LL_TIM_TRGO_RESET);            // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM17);

  //? UPDATE��־λ
	#if __HARDWARE_CONFIG__TIMER17_INT_UPDATE_ENABLE__ // begin of __HARDWARE_CONFIG__TIMER17_INT_UPDATE_ENABLE__
		LL_TIM_ClearFlag_UPDATE(TIM17);  // ������ϼ��������־λ
		LL_TIM_EnableIT_UPDATE (TIM17);  // ʹ�ܶ�ʱ�����ϼ����ж�
  #endif // end of __HARDWARE_CONFIG__TIMER17_INT_UPDATE_ENABLE__
}
//-----------------------------------------------------------------
// inline void timer17_start()
//-----------------------------------------------------------------
//
// ��������: ������ʱ��17
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer17_start(){
  if(__HARDWARE_CONFIG__TIMER17_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM17);  // ������ϼ��������־λ
  LL_TIM_EnableCounter   (TIM17);  // ʹ�ܶ�ʱ����ʼ����
}
//-----------------------------------------------------------------
// inline void timer17_stop()
//-----------------------------------------------------------------
//
// ��������: �رն�ʱ��17
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void timer17_stop(){
  if(__HARDWARE_CONFIG__TIMER17_INT_UPDATE_ENABLE__)
    LL_TIM_ClearFlag_UPDATE(TIM17);   // ������ϼ��������־λ
  LL_TIM_DisableCounter   (TIM17);  // �رն�ʱ������
}

//-----------------------------------------------------------------
// inline void TIM17_IRQHandler_Func(void)
//-----------------------------------------------------------------
//
// ��������: ��ʱ��17�жϷ�����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM17_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void TIM17_IRQHandler_Func(void){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM17) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(TIM17);				//������ϼ��������־λ
		
		timer17_call_cnt ++;
	}
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__USER_TIMER17_ENABLE__


#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM17 global interrupt.
  */
void TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM17_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER17_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER17_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM17_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM17->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM17 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM17_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER17_ENABLE__
  /* USER CODE END TIM17_IRQn 0 */
  /* USER CODE BEGIN TIM17_IRQn 1 */

  /* USER CODE END TIM17_IRQn 1 */
}
#endif

