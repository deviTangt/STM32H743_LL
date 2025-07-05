#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__VISUALIZE_SHOW_ENABLE__	// begin of __HARDWARE_CONFIG__VISUALIZE_SHOW_ENABLE__
//*******************************// include _h files    //************************************//
#include "visualize_show.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//
//*******************************// define function     //************************************//

//-----------------------------------------------------------------
// inline void display_show_init()
//-----------------------------------------------------------------
//
// ��������: ��ʼ����ӡ���ӻ�������Ϣ
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
inline void display_show_init(){

}

//-----------------------------------------------------------------
// void visualize_show()
//-----------------------------------------------------------------
//
// ��������: ��ӡ���ӻ�������Ϣ
// ��ڲ���1: ��
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void visualize_show(){
    //printf_s(25, "ADC1_RX:%dmV", DMA_ADC1_RX_BUF[0] * 3300 / 0xffff);

    //? ���ݽ���
    if (__HARDWARE_CONFIG__TJC_RX_ENABLE__)
      TJC_USART_RX_HANDLER();
    if (__HARDWARE_CONFIG__FPGA_RX_ENABLE__)
      FPGA_RX_HANDLER();

    //? dma adc����
    #if __HARDWARE_CONFIG__DMA_ADC1_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
      //printf_s(5, "dma_cnt:%d,%0d,%0d", dma_adc1_finish_cnt / 1000000, dma_adc1_finish_cnt / 1000 % 1000, dma_adc1_finish_cnt % 1000);
      for (uint8_t i = 0;i < 1;i ++){
        printf_s(15 + i, "AD1_val %d:%dmV", i, (DMA_ADC1_RX_BUF[i] * 3300) >> 16);
      }
    #endif // end of __HARDWARE_CONFIG__DMA_ADC1_ENABLE__
    #if __HARDWARE_CONFIG__DMA_ADC2_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC2_ENABLE__
      //printf_s(7, "dma_cnt:%d,%0d,%0d", dma_adc2_finish_cnt / 1000000, dma_adc2_finish_cnt / 1000 % 1000, dma_adc2_finish_cnt % 1000);
      for (uint8_t i = 0;i < 1;i ++){
        printf_s(8 + i, "AD2_val %d:%dmV", i, (DMA_ADC2_RX_BUF[i] * 3300) >> 16);
      }
    #endif // end of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
    #if __HARDWARE_CONFIG__DMA_ADC3_ENABLE__ // begin of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__
      //printf_s(9, "dma_cnt:%d,%0d,%0d", dma_adc3_finish_cnt / 1000000, dma_adc3_finish_cnt / 1000 % 1000, dma_adc3_finish_cnt % 1000);
      for (uint8_t i = 0;i < 1;i ++){
        printf_s(10 + i, "AD3_val %d:%dmV", i, (DMA_ADC3_RX_BUF[i] * 3300) >> 16);
      }
    #endif // end of __HARDWARE_CONFIG__DMA_ADC3_ENABLE__

    static uint32_t it = 0;
    printf_f("{{����������Ȼ��%d}}", it++);
    
    //printf_s(15, "timer12_call_cnt:%d", timer12_call_cnt);
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__VISUALIZE_SHOW_ENABLE__
