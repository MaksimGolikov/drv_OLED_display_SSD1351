/*
 * Name        drv_OLED_Display.c
 * Author      Maksim Golikov (SW developer)
 * Created on: 03 APLIL, 2018
 * Description driver for control OLED display ssd1351 (source file)
 */

#include "DRV/drv_OLED_Display.h"
#include "DRV/drv_OLED_DisplayCommand.h"



#define SEND_COMMAND   0
#define SEND_DATA      1

#define SCREEN_WIDTH  0x7F
#define SCREEN_HEIGH  0x7F


typedef struct {
	GPIO_TypeDef *port;
	uint32_t      pin;
}Connect_t;

typedef struct{
	SPI_HandleTypeDef  *spiConfigure;

	Connect_t DI;
	Connect_t SCK;
	Connect_t CS;
	Connect_t DC;
	Connect_t RES;

}displayControl_t;


displayControl_t display;



void SendCommand(uint8_t command);
void SendData(uint8_t data);
void ResetDisplay(uint8_t state);






void drv_OLED_Display_Init(SPI_HandleTypeDef *spi,
		                   GPIO_TypeDef *_portDI, GPIO_TypeDef *_portSCK, GPIO_TypeDef *_portCS, GPIO_TypeDef *_portDC, GPIO_TypeDef *_portRES,
		                   uint8_t _pinDI, uint8_t _pinSCK, uint8_t _pinCS, uint8_t _pinDC, uint8_t _pinRES){
	display.spiConfigure = spi;

	display.DI.port     = _portDI;
	display.SCK.port    = _portSCK;
	display.CS.port     = _portCS;
	display.DC.port     = _portDC;
	display.RES.port    = _portRES;
	display.DI.pin      = GPIO_PIN_0 << _pinDI;
	display.SCK.pin     = GPIO_PIN_0 << _pinSCK;
	display.CS.pin      = GPIO_PIN_0 << _pinCS;
	display.DC.pin      = GPIO_PIN_0 << _pinDC;
	display.RES.pin     = GPIO_PIN_0 << _pinRES;



	//Initialize display process
	ResetDisplay(GPIO_PIN_RESET);
	HAL_Delay(100);
	ResetDisplay(GPIO_PIN_SET);
	HAL_Delay(100);
	ResetDisplay(GPIO_PIN_RESET);
	HAL_Delay(100);
	ResetDisplay(GPIO_PIN_SET);
	HAL_Delay(100);


    SendCommand(COMMAND_COMMAND_LOCK);
    SendData(0x12);
    SendCommand(COMMAND_COMMAND_LOCK);
    SendData(0xB1);

    SendCommand(COMMAND_DISPLAY_OFF);
    SendCommand(COMMAND_CLOCK_DIV);
    SendData(0xF0);

    SendCommand(COMMAND_MUXRATIO);
    SendData(0x7F);

    SendCommand(COMMAND_DISPLAY_OFFSET);
    SendData(0x00);

    SendCommand(COMMAND_STARTLINE);
    SendData(0x00);

    SendCommand(COMMAND_SET_REMAP);
    SendData(0x74);

    SendCommand(COMMAND_SET_GPIO);
    SendData(0x00);

    SendCommand(COMMAND_FUNCTION_SELECT);
    SendData(0xC1);

    SendCommand(COMMAND_SET_VSL);
    SendData(0xA0);
    SendData(0xB5);
    SendData(0x55);

    SendCommand(COMMAND_CONTRAST_ABC);
	SendData(0xC8);  //Color A //8a
	SendData(0xA8);  //Color B //51
	SendData(0xC8);  //Color C //8a

	SendCommand(COMMAND_CONTRAST_MASTER);
	SendData(0x0F);

	SendCommand(COMMAND_SETGRAY);
	SendData(0x05);
	SendData(0x06);
	SendData(0x07);
	SendData(0x08);
	SendData(0x09);
	SendData(0x0a);
	SendData(0x0b);
	SendData(0x0c);
	SendData(0x0D);
	SendData(0x0E);
	SendData(0x0F);
	SendData(0x10);
	SendData(0x11);
	SendData(0x12);
	SendData(0x13);
	SendData(0x14);
	SendData(0x15);
	SendData(0x16);
	SendData(0x18);
	SendData(0x1a);
	SendData(0x1b);
	SendData(0x1C);
	SendData(0x1D);
	SendData(0x1F);
	SendData(0x21);
	SendData(0x23);
	SendData(0x25);
	SendData(0x27);
	SendData(0x2A);
	SendData(0x2D);
	SendData(0x30);
	SendData(0x33);
	SendData(0x36);
	SendData(0x39);
	SendData(0x3C);
	SendData(0x3F);
	SendData(0x42);
	SendData(0x45);
	SendData(0x48);
	SendData(0x4C);
	SendData(0x50);
	SendData(0x54);
	SendData(0x58);
	SendData(0x5C);
	SendData(0x60);
	SendData(0x64);
	SendData(0x68);
	SendData(0x6C);
	SendData(0x70);
	SendData(0x74);
	SendData(0x78);
	SendData(0x7D);
	SendData(0x82);
	SendData(0x87);
	SendData(0x8C);
	SendData(0x91);
	SendData(0x96);
	SendData(0x9B);
	SendData(0xA0);
	SendData(0xA5);
	SendData(0xAA);
	SendData(0xAF);
	SendData(0xB4);


	SendCommand(COMMAND_PRECHARGE);
	SendData(0x32);

	SendCommand(COMMAND_PRECHARGE_LEVEL);
	SendData(0x16);

	SendCommand(COMMAND_PRECHARGE2);
	SendData(0x01);
	SendData(0x09);
	SendData(0x0F);

	SendCommand(COMMAND_VCOMH);
	SendData(0x05);

	SendCommand(COMMAND_NORMAL_DISPLAY);

	SendCommand(COMMAND_SET_COLUMN);
	SendData(0x00);
	SendData(SCREEN_WIDTH);

	SendCommand(COMMAND_SET_ROW);
	SendData(0x00);
	SendData(SCREEN_HEIGH);

	SendCommand(COMMAND_DISPLAY_ON);
}


void drv_OLED_Display_Clear(){
	SendCommand(COMMAND_SET_COLUMN);
	SendData(0x00);
	SendData(SCREEN_WIDTH);

	SendCommand(COMMAND_SET_ROW);
	SendData(0x00);
	SendData(SCREEN_HEIGH);

	SendCommand(COMMAND_WRITE_RAM);
    uint16_t pixelsCount = SCREEN_HEIGH * SCREEN_WIDTH;

    for(uint16_t pixel = 0; pixel < pixelsCount; ++pixel){
    	SendData(0xFF);
    	SendData(0xFF);
    }
}


void drv_OLED_Display_FillScreen(uint16_t color){
	SendCommand(COMMAND_SET_COLUMN);
	SendData(0x00);
	SendData(SCREEN_WIDTH);

	SendCommand(COMMAND_SET_ROW);
	SendData(0x00);
	SendData(SCREEN_HEIGH);

	SendCommand(COMMAND_WRITE_RAM);
	uint16_t pixelsCount = SCREEN_HEIGH * SCREEN_WIDTH;

	for (uint16_t pixel = 0; pixel < pixelsCount; ++pixel) {
		SendData(color >> 8);
		SendData(color);
	}
}


void drv_OLED_Display_FillRectangle(uint8_t x_st,  uint8_t y_st,
		                            uint8_t x_end, uint8_t y_end,
									uint16_t color) {
	if ((x_st < x_end) && (y_st < y_end)) {
		SendCommand(COMMAND_SET_COLUMN);
		SendData(x_st);
		SendData(x_end);

		SendCommand(COMMAND_SET_ROW);
		SendData(y_st);
		SendData(y_end);

		SendCommand(COMMAND_WRITE_RAM);
		uint16_t pixelsCount = (x_end - x_st) * (y_end - y_st);

		for (uint16_t pixel = 0; pixel < pixelsCount; ++pixel) {
			SendData(color >> 8);
			SendData(color);
		}
	}
}


void drv_OLED_Display_DrawPixel(uint8_t x, uint8_t y, uint16_t color){
	SendCommand(COMMAND_SET_COLUMN);
	SendData(x);
	SendData(SCREEN_WIDTH - 1);

	SendCommand(COMMAND_SET_ROW);
	SendData(y);
	SendData(SCREEN_HEIGH - 1);

	SendCommand(COMMAND_WRITE_RAM);
	SendData(color >> 8);
	SendData(color);
}


void drv_OLED_Display_DrawPictire(uint8_t x_st,  uint8_t y_st,
		                          uint8_t width, uint8_t heigh,
								  uint8_t *ptrPicture, uint16_t color){
	if ((width > 0) && (heigh > 0)){

		SendCommand(COMMAND_SET_COLUMN);
		SendData( x_st );
		SendData( x_st + heigh );

		SendCommand(COMMAND_SET_ROW);
		SendData( y_st );
		SendData( y_st + width );

		SendCommand(COMMAND_WRITE_RAM);
		for(uint8_t x = 0; x < width; ++x){
			for(uint8_t y = 0; y < heigh; ++y){
				uint8_t dot = *(ptrPicture + (y * width) + x);
				if (dot > 0) {
					uint8_t x_position = x_st + x;
					uint8_t y_position = y_st + y;
					drv_OLED_Display_DrawPixel(x_position, y_position, color);
				}
			}
		}

	}
}


void  drv_OLED_Display_DrawImage(uint8_t x_st,  uint8_t y_st,
                                 uint8_t width, uint8_t heigh,
		                         uint16_t *ptrPicture){
	if ((width > 0) && (heigh > 0)){

			SendCommand(COMMAND_SET_COLUMN);
			SendData( x_st );
			SendData( x_st + width );

			SendCommand(COMMAND_SET_ROW);
			SendData( y_st );
			SendData( y_st + heigh );

			SendCommand(COMMAND_WRITE_RAM);
			for(uint8_t x = 0; x < heigh; ++x){
				for(uint8_t y = 0; y < width; ++y){
					uint16_t dotColor = *(ptrPicture + (y * width) + x);
					uint8_t x_position = x_st + x;
					uint8_t y_position = y_st + y;
					drv_OLED_Display_DrawPixel(x_position, y_position, dotColor);
				}
			}

		}
}








void SendCommand(uint8_t command){
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 0);
	HAL_GPIO_WritePin(display.DC.port, display.DC.pin, SEND_COMMAND);
	HAL_SPI_Transmit(display.spiConfigure, &command, 1, 100);
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 1);
}

void SendData(uint8_t data){
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 0);
	HAL_GPIO_WritePin(display.DC.port, display.DC.pin, SEND_DATA);
	HAL_SPI_Transmit(display.spiConfigure, &data, 1, 100);
	HAL_GPIO_WritePin(display.CS.port, display.CS.pin, 1);
}

void ResetDisplay(uint8_t state){
	uint8_t pinState = 0;
	if(state){
		pinState = 1;
	}
	HAL_GPIO_WritePin(display.RES.port, display.RES.pin, pinState);
}

