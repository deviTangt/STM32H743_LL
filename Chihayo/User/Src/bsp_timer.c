#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
//*******************************// include _h files    //************************************//
#include "bsp_timer.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

__attribute__((section("0x24070000"))) __IO uint32_t bspTick;		//! �������������ʹ��volatile���Σ�����uwTick
//-----------------------------------------------------------------
// inline void bspTick_mark_x_set()			% x = 1, 2, 3
//-----------------------------------------------------------------
uint32_t bspTick_mark_0;
uint32_t bspTick_mark_1;
uint32_t bspTick_mark_2;
uint32_t bspTick_mark_3;
uint32_t bspTick_mark_4;

//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void bsp_Timer_Config_Init(void)
//-----------------------------------------------------------------
//
// ��������: ��ʼ���弶֧�ְ���ʱ��ϵͳ����
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void bsp_Timer_Config_Init(void){
	//? ��ʼ��ʱ��
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);

	//? �����ж�
	NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),10, 0));
	NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);

	//? ���ö�ʱ���ṹ�� 1
	LL_TIM_InitTypeDef TIM_InitStruct             = {0};
	                   TIM_InitStruct.Prescaler   = 239;                    // Ԥ��Ƶ
	                   TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;  // ����ģʽ�����ϼ���
	                   TIM_InitStruct.Autoreload  = 65535;                  // �Զ���װ��ֵ
	LL_TIM_Init(TIM14, &TIM_InitStruct);

	//? ��������
	LL_TIM_DisableARRPreload(TIM14);                     // �����Զ�װ��ģʽ
	LL_TIM_SetTriggerOutput (TIM14, LL_TIM_TRGO_RESET);  // ��λ����
	LL_TIM_DisableMasterSlaveMode(TIM14);

	//? ��ʼ���弶֧�ְ���ʱ��
	#if __HARDWARE_CONFIG__BSP_TIMER_ENABLE__ // begin of __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
		bsp_Timer_Init(); //! Enable BspTick Timer
	#endif // end of __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
}
//-----------------------------------------------------------------
// inline void bsp_Timer_Init(void)
//-----------------------------------------------------------------
//
// ��������: ��ʼ���弶֧�ְ���ʱ��
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void bsp_Timer_Init(void){
	LL_TIM_EnableIT_UPDATE(bsp_IMx);			//ʹ�ܶ�ʱ�����ϼ���
	LL_TIM_ClearFlag_UPDATE(bsp_IMx);			//������ϼ��������־λ
	LL_TIM_EnableCounter(bsp_IMx);				//ʹ�ܶ�ʱ����ʼ����
}

//-----------------------------------------------------------------
// inline uint32_t Get_SystemTimer(void)
//-----------------------------------------------------------------
//
// ��������: ��ȡ��ʱ��ʱ��
// ��ڲ���1: ��
// �� �� ֵ: ʵʱʱ��
// ע������: ��
//
//-----------------------------------------------------------------
inline uint32_t Get_SystemTimer(void){
	return bsp_IMx->CNT + (bspTick << 16);
}
//-----------------------------------------------------------------
// inline void Update_SystemTick(void)
//-----------------------------------------------------------------
//
// ��������: ���¶�ʱ��ʱ��
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ���˺������붨ʱ���жϷ�����TIM14_IRQHandler(void)��
//
//-----------------------------------------------------------------
inline void Update_SystemTick(void)
{
	//if(LL_TIM_IsActiveFlag_UPDATE(bsp_IMx) == SET){	//�ж϶�ʱ���Ƿ����
		LL_TIM_ClearFlag_UPDATE(bsp_IMx);				//������ϼ��������־λ
		bspTick++;								//����
	//}
}
//-----------------------------------------------------------------
// inline void delay_us(uint32_t cnt)
//-----------------------------------------------------------------
//
// ��������: ΢�뼶���������ʱ
// ��ڲ���1: ��ʱʱ��(us)
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void delay_us(uint32_t cnt){
	uint32_t temp = cnt + Get_SystemTimer();
	while(Get_SystemTimer() < temp);
}
//-----------------------------------------------------------------
// inline void delay_ms(uint32_t cnt)
//-----------------------------------------------------------------
//
// ��������: ���뼶���������ʱ
// ��ڲ���1: ��ʱʱ��(ms)
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void delay_ms(uint32_t cnt){
	uint32_t temp = cnt * 1000 + Get_SystemTimer();	
	while(Get_SystemTimer() < temp);
}
//-----------------------------------------------------------------
// inline void bspTick_mark_0_set()
//-----------------------------------------------------------------
//
// ��������: ��¼bspTick�ϵ�0ʱ��
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark_0_set(){
	bspTick_mark_0 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_1_set()
//-----------------------------------------------------------------
//
// ��������: ��¼bspTick�ϵ�1ʱ��
// ��ڲ���1: ��ʱʱ��(ms)
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark_1_set(){
	bspTick_mark_1 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_2_set()
//-----------------------------------------------------------------
//
// ��������: ��¼bspTick�ϵ�2ʱ��
// ��ڲ���1: ��ʱʱ��(ms)
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark_2_set(){
	bspTick_mark_2 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_dif_show(uint8_t SHOW_Windows)
//-----------------------------------------------------------------
//
// ��������: ��ʾbspTick�ϵ�1��2֮��ʱ���
// ��ڲ���1: ��ʾ����
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark_dif_show(uint8_t SHOW_Windows){
	printf_s(SHOW_Windows, "Tick Dif:%2d.%03ds %03dus", bsp_mark_dif_sec, bsp_mark_dif_ms, bsp_mark_dif_us);
}
//-----------------------------------------------------------------
// inline void bspTick_mark_dif_show_s(uint8_t SHOW_Windows, char *string)
//-----------------------------------------------------------------
//
// ��������: ��ʾbspTick�ϵ�1��2֮��ʱ���, �Զ�����ʾ���
// ��ڲ���1: ��ʾ����
// ��ڲ���1: ��ʾ���
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark_dif_show_s(uint8_t SHOW_Windows, char *string){
	printf_s(SHOW_Windows, "%s:%2d.%03ds %03dus", string, bsp_mark_dif_sec, bsp_mark_dif_ms, bsp_mark_dif_us);
}
//-----------------------------------------------------------------
// inline void bspTick_mark_3_set()
//-----------------------------------------------------------------
//
// ��������: ��¼bspTick�ϵ�3ʱ��
// ��ڲ���1: ��ʱʱ��(ms)
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark_3_set(){
	bspTick_mark_3 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_4_set()
//-----------------------------------------------------------------
//
// ��������: ��¼bspTick�ϵ�4ʱ��
// ��ڲ���1: ��ʱʱ��(ms)
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark_4_set(){
	bspTick_mark_4 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark34_dif_show(uint8_t SHOW_Windows)
//-----------------------------------------------------------------
//
// ��������: ��ʾbspTick�ϵ�3��4֮��ʱ���
// ��ڲ���1: ��ʾ����
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark34_dif_show(uint8_t SHOW_Windows){
	printf_s(SHOW_Windows, "Tick34 Dif:%2d.%03ds %03dus", bsp_mark34_dif_sec, bsp_mark34_dif_ms, bsp_mark34_dif_us);
}
//-----------------------------------------------------------------
// inline void bspTick_mark34_dif_show_s(uint8_t SHOW_Windows, char *string)
//-----------------------------------------------------------------
//
// ��������: ��ʾbspTick�ϵ�3��4֮��ʱ���, �Զ�����ʾ���
// ��ڲ���1: ��ʾ����
// ��ڲ���1: ��ʾ���
// �� �� ֵ: ��
// ע������: �ޣ�
//
//-----------------------------------------------------------------
inline void bspTick_mark34_dif_show_s(uint8_t SHOW_Windows, char *string){
	printf_s(SHOW_Windows, "%s:%2d.%03ds %03dus", string, bsp_mark34_dif_sec, bsp_mark34_dif_ms, bsp_mark34_dif_us);
}

//*******************************// end_c               //************************************//
#endif


#if 0 //// stm32h7xx_it.c�滻
/**
  * @brief This function handles TIM8 trigger and commutation interrupts and TIM14 global interrupt.
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 0 */
  #if __HARDWARE_CONFIG__BSP_TIMER_ENABLE__ // begin of __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM14_IRQHandler_BSP_Timer_UPDATE
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM7->CNT reached the maximum counter value 0xffff,
    //                             update the BspTimerCnt.
    // Detected Case: TIM7 Up Overflow
    // Returned Value: update BspTimerCnt
    // Notice: None
    //
    //-----------------------------------------------------------------
    Update_SystemTick();
  #endif // end of __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
  /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 1 */

  /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 1 */
}
#endif

