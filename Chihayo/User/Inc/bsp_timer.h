#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__BSP_TIMER_ENABLE__
#ifndef  __BSP_TIMER_H
#define  __BSP_TIMER_H
#ifdef  __cplusplus
extern "C"{
#endif
//*******************************// include extern .h files //************************************//
#include "main.h"
//*******************************// define statement        //************************************//

#define bsp_IMx                 TIM14				//宏定义方便替换定时器
#define bsp_us                  (bsp_IMx->CNT + (bspTick << 16)) 
#define bsp_ms                  (bsp_us / 1000) 
#define bsp_sec                 (bsp_ms / 1000) 
#define bsp_min                 (bsp_sec / 60) 
#define bsp_hour                (bsp_min / 60) 

#define bsp_mark_dif_us         ((bspTick_mark_2 - bspTick_mark_1) % 1000) 
#define bsp_mark_dif_ms         ((bspTick_mark_2 - bspTick_mark_1) / 1000 % 1000) 
#define bsp_mark_dif_sec        ((bspTick_mark_2 - bspTick_mark_1) / 1000000 % 60) 
#define bsp_mark_dif_min        ((bspTick_mark_2 - bspTick_mark_1) / 60000000 % 60) 
#define bsp_mark_dif_hour       ((bspTick_mark_2 - bspTick_mark_1) / 3600000000 % 24) 

#define bsp_mark34_dif_us       ((bspTick_mark_4 - bspTick_mark_3) % 1000) 
#define bsp_mark34_dif_ms       ((bspTick_mark_4 - bspTick_mark_3) / 1000 % 1000) 
#define bsp_mark34_dif_sec      ((bspTick_mark_4 - bspTick_mark_3) / 1000000 % 60) 
#define bsp_mark34_dif_min      ((bspTick_mark_4 - bspTick_mark_3) / 60000000 % 60) 
#define bsp_mark34_dif_hour     ((bspTick_mark_4 - bspTick_mark_3) / 3600000000 % 24) 

//*******************************// extern parameters       //************************************//

extern __IO uint32_t bspTick;
//-----------------------------------------------------------------
// inline void bspTick_mark_x_set()			% x = 1, 2, 3
//-----------------------------------------------------------------
extern uint32_t bspTick_mark_0;
extern uint32_t bspTick_mark_1;
extern uint32_t bspTick_mark_2;

//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

extern inline void bsp_Timer_Config_Init(void);
extern inline void bsp_Timer_Init(void);
extern inline uint32_t Get_SystemTimer(void);
extern inline void Update_SystemTick(void);
extern inline void delay_ms(uint32_t cnt);
extern inline void delay_us(uint32_t cnt);

extern inline void bspTick_mark_0_set();
extern inline void bspTick_mark_1_set();
extern inline void bspTick_mark_2_set();
extern inline void bspTick_mark_dif_show(uint8_t SHOW_Windows);
extern inline void bspTick_mark_dif_show_s(uint8_t SHOW_Windows, char *string);
extern inline void bspTick_mark_3_set();
extern inline void bspTick_mark_4_set();
extern inline void bspTick_mark34_dif_show(uint8_t SHOW_Windows);
extern inline void bspTick_mark34_dif_show_s(uint8_t SHOW_Windows, char *string);

#ifdef  __cplusplus
}
#endif
//*******************************// end_h                   //************************************//
#endif 
#endif
