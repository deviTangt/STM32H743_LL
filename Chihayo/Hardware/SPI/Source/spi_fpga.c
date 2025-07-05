#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__SPI_FPGA_ENABLE__	// begin of __HARDWARE_CONFIG__SPI_FPGA_ENABLE__
//*******************************// include _h files    //************************************//
#include "spi_fpga.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//
//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void spi_fpga_init()
//-----------------------------------------------------------------
//
// 函数功能: SPI-FPGA通信初始化
// 入口参数1: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
inline void spi_fpga_init(){
    //? SPI时钟配置
    LL_RCC_SetSPIClockSource(LL_RCC_SPI123_CLKSOURCE_PLL1Q);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

    /**SPI2 GPIO Configuration
    PC2_C   ------> SPI2_MISO
    PC3_C   ------> SPI2_MOSI
    PB10    ------> SPI2_SCK
    */
    //? GPIO结构体配置
    LL_GPIO_InitTypeDef GPIO_InitStruct            = {0};
                        GPIO_InitStruct.Pin        = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
                        GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
                        GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
                        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
                        GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
                        GPIO_InitStruct.Alternate  = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
                        GPIO_InitStruct.Pin        = LL_GPIO_PIN_10;
                        GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
                        GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
                        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
                        GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
                        GPIO_InitStruct.Alternate  = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //? SPI结构体配置
    LL_SPI_InitTypeDef SPI_InitStruct                   = {0};
                       SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
                       SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;
                       SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
                       SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_HIGH;
                       SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_2EDGE;
                       SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
                       SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV8;
                       SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
                       SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
                       SPI_InitStruct.CRCPoly           = 0x0;
    LL_SPI_Init(SPI2, &SPI_InitStruct);

    //? Advanced配置
    LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_SetFIFOThreshold(SPI2, LL_SPI_FIFO_TH_01DATA);
    LL_SPI_EnableNSSPulseMgt(SPI2);

    //? 开启SPI
    LL_SPI_Enable(SPI2);
}

void spi_txrx(uint8_t tx, uint8_t *rx){
  #define cs_h()    LL_GPIO_SetOutputPin(SPI2_CS_GPIO_Port, SPI2_CS_Pin)
  #define cs_l()    LL_GPIO_ResetOutputPin(SPI2_CS_GPIO_Port, SPI2_CS_Pin)
  #define sclk_h()  LL_GPIO_SetOutputPin(SPI2_SCL_GPIO_Port, SPI2_SCL_Pin)
  #define sclk_l()  LL_GPIO_ResetOutputPin(SPI2_SCL_GPIO_Port, SPI2_SCL_Pin)
  #define mosi_h()  LL_GPIO_SetOutputPin(SPI2_MOSI_GPIO_Port, SPI2_MOSI_Pin)
  #define mosi_l()  LL_GPIO_ResetOutputPin(SPI2_MOSI_GPIO_Port, SPI2_MOSI_Pin)
  
  sclk_h();
  cs_l();

  uint8_t rd = 0;
  for (uint8_t i = 0;i < 8;i ++){
    sclk_l();
    if (tx & (1 << (7 - i))) mosi_h(); else mosi_l();
    
    delay_us(2);

    if (LL_GPIO_IsInputPinSet(SPI2_MISO_GPIO_Port, SPI2_MISO_Pin) == SET)
      rd |= (1 << (7 - i));
    
    sclk_h();
  }

  *rx = rd;
  cs_h();
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__SPI_FPGA_ENABLE__
