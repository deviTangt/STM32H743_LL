#ifndef __LCD_INIT_H // begin of __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 1  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif



//-----------------LCD�˿ڶ���---------------- 

#define LCD_SCLK_Clr() LL_GPIO_ResetOutputPin(TFTLCD_SCL_GPIO_Port, TFTLCD_SCL_Pin);
#define LCD_SCLK_Set() LL_GPIO_SetOutputPin(TFTLCD_SCL_GPIO_Port, TFTLCD_SCL_Pin);

#define LCD_MOSI_Clr() LL_GPIO_ResetOutputPin(TFTLCD_SDA_GPIO_Port, TFTLCD_SDA_Pin);
#define LCD_MOSI_Set() LL_GPIO_SetOutputPin(TFTLCD_SDA_GPIO_Port, TFTLCD_SDA_Pin);

#define LCD_RES_Clr()  LL_GPIO_ResetOutputPin(TFTLCD_RES_GPIO_Port, TFTLCD_RES_Pin);
#define LCD_RES_Set()  LL_GPIO_SetOutputPin(TFTLCD_RES_GPIO_Port, TFTLCD_RES_Pin);

#define LCD_DC_Clr()   LL_GPIO_ResetOutputPin(TFTLCD_DC_GPIO_Port, TFTLCD_DC_Pin);
#define LCD_DC_Set()   LL_GPIO_SetOutputPin(TFTLCD_DC_GPIO_Port, TFTLCD_DC_Pin);
 		     
#define LCD_CS_Clr()   LL_GPIO_ResetOutputPin(TFTLCD_CS_GPIO_Port, TFTLCD_CS_Pin);
#define LCD_CS_Set()   LL_GPIO_SetOutputPin(TFTLCD_CS_GPIO_Port, TFTLCD_CS_Pin);

#define LCD_BLK_Clr()  LL_GPIO_ResetOutputPin(TFTLCD_BL_GPIO_Port, TFTLCD_BL_Pin);
#define LCD_BLK_Set()  LL_GPIO_SetOutputPin(TFTLCD_BL_GPIO_Port, TFTLCD_BL_Pin);




void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(uint8_t dat);//ģ��SPIʱ��
void LCD_WR_DATA8(uint8_t dat);//д��һ���ֽ�
void LCD_WR_DATA(uint16_t dat);//д�������ֽ�
void LCD_WR_REG(uint8_t dat);//д��һ��ָ��
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��

#endif // end of __LCD_INIT_H




