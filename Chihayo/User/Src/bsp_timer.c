#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
//*******************************// include _h files    //************************************//
#include "bsp_timer.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//

__attribute__((section("0x24070000"))) __IO uint32_t bspTick;		//! 定义计数变量，使用volatile修饰，类似uwTick
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
// 函数功能: 初始化板级支持包定时器系统配置
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void bsp_Timer_Config_Init(void){
	//? 初始化时钟
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM14);

	//? 配置中断
	NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),10, 0));
	NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);

	//? 配置定时器结构体 1
	LL_TIM_InitTypeDef TIM_InitStruct             = {0};
	                   TIM_InitStruct.Prescaler   = 239;                    // 预分频
	                   TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;  // 计数模式：向上计数
	                   TIM_InitStruct.Autoreload  = 65535;                  // 自动重装载值
	LL_TIM_Init(TIM14, &TIM_InitStruct);

	//? 其他配置
	LL_TIM_DisableARRPreload(TIM14);                     // 启动自动装载模式
	LL_TIM_SetTriggerOutput (TIM14, LL_TIM_TRGO_RESET);  // 复位更新
	LL_TIM_DisableMasterSlaveMode(TIM14);

	//? 初始化板级支持包定时器
	#if __HARDWARE_CONFIG__BSP_TIMER_ENABLE__ // begin of __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
		bsp_Timer_Init(); //! Enable BspTick Timer
	#endif // end of __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
}
//-----------------------------------------------------------------
// inline void bsp_Timer_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: 初始化板级支持包定时器
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void bsp_Timer_Init(void){
	LL_TIM_EnableIT_UPDATE(bsp_IMx);			//使能定时器向上计数
	LL_TIM_ClearFlag_UPDATE(bsp_IMx);			//清除向上计数溢出标志位
	LL_TIM_EnableCounter(bsp_IMx);				//使能定时器开始计数
}

//-----------------------------------------------------------------
// inline uint32_t Get_SystemTimer(void)
//-----------------------------------------------------------------
//
// 函数功能: 获取定时器时间
// 入口参数1: 无
// 返 回 值: 实时时间
// 注意事项: 无
//
//-----------------------------------------------------------------
inline uint32_t Get_SystemTimer(void){
	return bsp_IMx->CNT + (bspTick << 16);
}
//-----------------------------------------------------------------
// inline void Update_SystemTick(void)
//-----------------------------------------------------------------
//
// 函数功能: 更新定时器时间
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 将此函数加入定时器中断服务函数TIM14_IRQHandler(void)中
//
//-----------------------------------------------------------------
inline void Update_SystemTick(void)
{
	//if(LL_TIM_IsActiveFlag_UPDATE(bsp_IMx) == SET){	//判断定时器是否溢出
		LL_TIM_ClearFlag_UPDATE(bsp_IMx);				//清除向上计数溢出标志位
		bspTick++;								//计数
	//}
}
//-----------------------------------------------------------------
// inline void delay_us(uint32_t cnt)
//-----------------------------------------------------------------
//
// 函数功能: 微秒级软件堵塞延时
// 入口参数1: 延时时数(us)
// 返 回 值: 无
// 注意事项: 无
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
// 函数功能: 毫秒级软件堵塞延时
// 入口参数1: 延时时数(ms)
// 返 回 值: 无
// 注意事项: 无：
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
// 函数功能: 记录bspTick断点0时间
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark_0_set(){
	bspTick_mark_0 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_1_set()
//-----------------------------------------------------------------
//
// 函数功能: 记录bspTick断点1时间
// 入口参数1: 延时时数(ms)
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark_1_set(){
	bspTick_mark_1 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_2_set()
//-----------------------------------------------------------------
//
// 函数功能: 记录bspTick断点2时间
// 入口参数1: 延时时数(ms)
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark_2_set(){
	bspTick_mark_2 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_dif_show(uint8_t SHOW_Windows)
//-----------------------------------------------------------------
//
// 函数功能: 显示bspTick断点1和2之间时间差
// 入口参数1: 显示窗口
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark_dif_show(uint8_t SHOW_Windows){
	printf_s(SHOW_Windows, "Tick Dif:%2d.%03ds %03dus", bsp_mark_dif_sec, bsp_mark_dif_ms, bsp_mark_dif_us);
}
//-----------------------------------------------------------------
// inline void bspTick_mark_dif_show_s(uint8_t SHOW_Windows, char *string)
//-----------------------------------------------------------------
//
// 函数功能: 显示bspTick断点1和2之间时间差, 自定义显示语句
// 入口参数1: 显示窗口
// 入口参数1: 显示语句
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark_dif_show_s(uint8_t SHOW_Windows, char *string){
	printf_s(SHOW_Windows, "%s:%2d.%03ds %03dus", string, bsp_mark_dif_sec, bsp_mark_dif_ms, bsp_mark_dif_us);
}
//-----------------------------------------------------------------
// inline void bspTick_mark_3_set()
//-----------------------------------------------------------------
//
// 函数功能: 记录bspTick断点3时间
// 入口参数1: 延时时数(ms)
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark_3_set(){
	bspTick_mark_3 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark_4_set()
//-----------------------------------------------------------------
//
// 函数功能: 记录bspTick断点4时间
// 入口参数1: 延时时数(ms)
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark_4_set(){
	bspTick_mark_4 = bsp_us;
}
//-----------------------------------------------------------------
// inline void bspTick_mark34_dif_show(uint8_t SHOW_Windows)
//-----------------------------------------------------------------
//
// 函数功能: 显示bspTick断点3和4之间时间差
// 入口参数1: 显示窗口
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark34_dif_show(uint8_t SHOW_Windows){
	printf_s(SHOW_Windows, "Tick34 Dif:%2d.%03ds %03dus", bsp_mark34_dif_sec, bsp_mark34_dif_ms, bsp_mark34_dif_us);
}
//-----------------------------------------------------------------
// inline void bspTick_mark34_dif_show_s(uint8_t SHOW_Windows, char *string)
//-----------------------------------------------------------------
//
// 函数功能: 显示bspTick断点3和4之间时间差, 自定义显示语句
// 入口参数1: 显示窗口
// 入口参数1: 显示语句
// 返 回 值: 无
// 注意事项: 无：
//
//-----------------------------------------------------------------
inline void bspTick_mark34_dif_show_s(uint8_t SHOW_Windows, char *string){
	printf_s(SHOW_Windows, "%s:%2d.%03ds %03dus", string, bsp_mark34_dif_sec, bsp_mark34_dif_ms, bsp_mark34_dif_us);
}

//*******************************// end_c               //************************************//
#endif


#if 0 //// stm32h7xx_it.c替换
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

