/*
 * Name        drv_OLED_Display.h
 * Author      Maksim Golikov (SW developer)
 * Created on: 03 APLIL, 2018
 * Description driver for control OLED display ssd1351 (header file)
 */

#ifndef DRV_OLED_DISPLAY_H_
#define DRV_OLED_DISPLAY_H_

#include "stdint.h"
#include "stm32f1xx_hal.h"


enum{
COLOR_WHITE		=	0xFFFF,
COLOR_BLACK		=	0x0000,
COLOR_RED		=	0xF800,
COLOR_PURP		=	0xC815,
COLOR_GREEN		=	0x07E0,
COLOR_BLUE		=	0x04FF,
COLOR_YELLOW	=	0xFFE0,
COLOR_SIEMENS	=	0x06F7,
COLOR_MAGENTA	=	0xf81f,
COLOR_LMAGENTA	=	0xfc1f,
COLOR_TURQUOISE	=	0x36b9,
COLOR_DGREEN	=	0x7e00
};


void drv_OLED_Display_Init(SPI_HandleTypeDef *spi,
						   GPIO_TypeDef *_portDI, GPIO_TypeDef *_portSCK, GPIO_TypeDef *_portCS, GPIO_TypeDef *_portDC, GPIO_TypeDef *_portRES,
                           uint8_t _pinDI, uint8_t _pinSCK, uint8_t _pinCS, uint8_t _pinDC, uint8_t _pinRES);



void drv_OLED_Display_Clear();

void drv_OLED_Display_FillScreen(uint16_t color);

void drv_OLED_Display_FillRectangle(uint8_t x_st, uint8_t y_st,
		                            uint8_t x_end, uint8_t y_end,
								    uint16_t color);

void drv_OLED_Display_DrawPixel(uint8_t x, uint8_t y, uint16_t color);


void drv_OLED_Display_DrawPictire(uint8_t x_st,  uint8_t y_st,
		                          uint8_t width, uint8_t heigh,
								  uint16_t *ptrPicture, uint16_t color);


void  drv_OLED_Display_DrawImage(uint8_t x_st,  uint8_t y_st,
                                 uint8_t width, uint8_t heigh,
		                         uint16_t *ptrPicture);


void drv_OLED_Display_SetIntensity(uint8_t intensity);


#endif /* DRV_OLED_DISPLAY_H_ */
