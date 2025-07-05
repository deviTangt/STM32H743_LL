/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
    /* USER CODE BEGIN LL_EXTI_LINE_3 */
  #if __HARDWARE_CONFIG__KEY_ENABLE__ // begin of __HARDWARE_CONFIG__KEY_ENABLE__
    //-----------------------------------------------------------------
    //? EXTI3_IRQHandler_KEY_1_DETECT
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Upon KEY_1 pressed, then turn the lighting state of LED.
    // Detected Case: Low state of KEY_1
    // Returned Value: The lighting state of LED 
    // Notice: None
    //
    //-----------------------------------------------------------------
    EXTI3_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__KEY_ENABLE__
    /* USER CODE END LL_EXTI_LINE_3 */
  }
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */
  if (LL_DMA_IsActiveFlag_TC0(DMA1)) {
    LL_DMA_ClearFlag_TC0(DMA1);  // 
    //printf_s_d("DMA INT0\r\n");
  }
  if (LL_DMA_IsActiveFlag_TE0(DMA1)) {
    LL_DMA_ClearFlag_TE0(DMA1);  // 
    printf_s_d("DMA ERR0\r\n");
  }
  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */
  #if __HARDWARE_CONFIG__TJC_RX_ENABLE__ & __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
    //-----------------------------------------------------------------
    //? DMA1_Stream1_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Get Received Data 
    // Detected Case: RXNE
    // Returned Value: Received Data
    // Notice: None
    //
    //-----------------------------------------------------------------
    DMA1_Stream1_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__
  /* USER CODE END DMA1_Stream1_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */
  
  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */
  #if __HARDWARE_CONFIG__DMA_ADC1_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
    //-----------------------------------------------------------------
    //? DMA1_Stream2_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: ADC
    // Detected Case: ADC DMA Finish
    // Returned Value: None
    // Notice: None
    //
    //-----------------------------------------------------------------
    DMA1_Stream2_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
  /* USER CODE END DMA1_Stream2_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */
  
  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */
  #if __HARDWARE_CONFIG__DMA_ADC2_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC2_ENABLE__
    //-----------------------------------------------------------------
    //? DMA1_Stream3_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: ADC
    // Detected Case: ADC DMA Finish
    // Returned Value: None
    // Notice: None
    //
    //-----------------------------------------------------------------
    DMA1_Stream3_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__DMA_ADC2_ENABLE__
  /* USER CODE END DMA1_Stream3_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */
  #if __HARDWARE_CONFIG__DMA_ADC3_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
    //-----------------------------------------------------------------
    //? DMA1_Stream4_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: ADC
    // Detected Case: ADC DMA Finish
    // Returned Value: None
    // Notice: None
    //
    //-----------------------------------------------------------------
    DMA1_Stream4_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
  /* USER CODE END DMA1_Stream4_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */
  if (LL_DMA_IsActiveFlag_TC5(DMA1)) {
    LL_DMA_ClearFlag_TC5(DMA1);  // 
    //printf_s_d("DMA INT0\r\n");
  }
  if (LL_DMA_IsActiveFlag_TE5(DMA1)) {
    LL_DMA_ClearFlag_TE5(DMA1);  // 
    printf_s_d("DMA ERR5\r\n");
  }
  /* USER CODE END DMA1_Stream5_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */
  #if __HARDWARE_CONFIG__TJC_RX_ENABLE__ & __HARDWARE_CONFIG__TJC_DMA_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
    //-----------------------------------------------------------------
    //? DMA1_Stream1_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Get Received Data 
    // Detected Case: RXNE
    // Returned Value: Received Data
    // Notice: None
    //
    //-----------------------------------------------------------------
    //DMA1_Stream1_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__
  /* USER CODE END DMA1_Stream6_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

  /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_5) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
    /* USER CODE BEGIN LL_EXTI_LINE_5 */
  #if __HARDWARE_CONFIG__KEY_ENABLE__ // begin of __HARDWARE_CONFIG__KEY_ENABLE__
    //-----------------------------------------------------------------
    //? EXTI5_IRQHandler_KEY_2_DETECT
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Upon KEY_2 pressed, then turn the lighting state of LED.
    // Detected Case: Low state of KEY_2
    // Returned Value: The lighting state of LED 
    // Notice: None
    //
    //-----------------------------------------------------------------
    EXTI9_5_IRQHandler_Func();
  #endif
    /* USER CODE END LL_EXTI_LINE_5 */
  }
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

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

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER2_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM2_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM2->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM3 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM2_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER2_ENABLE__
  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER3_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM3_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM3->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM3 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM3_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER3_ENABLE__
  /* USER CODE END TIM3_IRQn 0 */
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

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

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  #if __HARDWARE_CONFIG__FPGA_RX_ENABLE__ // begin of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
    //-----------------------------------------------------------------
    //? USART1_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Get Received Data 
    // Detected Case: RXNE
    // Returned Value: Received Data
    // Notice: None
    //
    //-----------------------------------------------------------------
    USART1_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__FPGA_RX_ENABLE__
  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM8 break interrupt and TIM12 global interrupt.
  */
void TIM8_BRK_TIM12_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_BRK_TIM12_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER12_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER12_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM12_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM12->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM12 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM12_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER12_ENABLE__
  /* USER CODE END TIM8_BRK_TIM12_IRQn 0 */
  /* USER CODE BEGIN TIM8_BRK_TIM12_IRQn 1 */

  /* USER CODE END TIM8_BRK_TIM12_IRQn 1 */
}

/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */
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
  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}

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

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER5_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER5_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM5_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM5->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM5 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM5_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER5_ENABLE__
  /* USER CODE END TIM5_IRQn 0 */
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}

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

/**
  * @brief This function handles UART7 global interrupt.
  */
void UART7_IRQHandler(void)
{
  /* USER CODE BEGIN UART7_IRQn 0 */
  /* USER CODE END UART7_IRQn 0 */
  /* USER CODE BEGIN UART7_IRQn 1 */
  #if __HARDWARE_CONFIG__TJC_RX_ENABLE__ // begin of __HARDWARE_CONFIG__TJC_RX_ENABLE__
    //-----------------------------------------------------------------
    //? UART7_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Get Received Data 
    // Detected Case: RXNE
    // Returned Value: Received Data
    // Notice: None
    //
    //-----------------------------------------------------------------
    UART7_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__TJC_RX_ENABLE__
  /* USER CODE END UART7_IRQn 1 */
}

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

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */
  #if __HARDWARE_CONFIG__USER_TIMER_ENABLE__ & __HARDWARE_CONFIG__USER_TIMER16_ENABLE__ // begin of __HARDWARE_CONFIG__USER_TIMER16_ENABLE__
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    //? TIM16_IRQHandler_Func
    //-----------------------------------------------------------------
    //
    // Interrupt Excute Function: Once TIM16->CNT reached the maximum counter value,
    //                             excute certain function.
    // Detected Case: TIM16 Up Overflow
    // Returned Value: excute certain function
    // Notice: None
    //
    //-----------------------------------------------------------------
    TIM16_IRQHandler_Func();
  #endif // end of __HARDWARE_CONFIG__USER_TIMER16_ENABLE__
  /* USER CODE END TIM16_IRQn 0 */
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}

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

/**
  * @brief This function handles ADC3 global interrupt.
  */
void ADC3_IRQHandler(void)
{
  /* USER CODE BEGIN ADC3_IRQn 0 */

  /* USER CODE END ADC3_IRQn 0 */
  /* USER CODE BEGIN ADC3_IRQn 1 */

  /* USER CODE END ADC3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
