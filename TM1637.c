/*
Author: MehdiSepehri
Email: sepehrimehdi80@gmail.com
Github: 
*/
#include "stm32f1xx_hal.h"
#include "tm1637.h"
#include <stdio.h>

// Configuration.
#define CLK_PORT GPIOA
#define DIO_PORT GPIOA
#define CLK_PIN GPIO_PIN_1
#define DIO_PIN GPIO_PIN_2
#define CLK_PORT_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE
#define DIO_PORT_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE
#define SegmentNo 4


void _tm1637Start(void);
void _tm1637Stop(void);
void _tm1637ReadResult(void);
void _tm1637WriteByte(unsigned char b);
void _tm1637DelayUsec(unsigned int i);
void _tm1637ClkHigh(void);
void _tm1637ClkLow(void);
void _tm1637DioHigh(void);
void _tm1637DioLow(void);
uint8_t _tm1637Convert(char);

void tm1637Init(void)
{
    CLK_PORT_CLK_ENABLE();
    DIO_PORT_CLK_ENABLE();
    GPIO_InitTypeDef g = {0};
    g.Pull = GPIO_PULLUP;
    g.Mode = GPIO_MODE_OUTPUT_OD; // OD = open drain
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    g.Pin = CLK_PIN;
    HAL_GPIO_Init(CLK_PORT, &g);
    g.Pin = DIO_PIN;
    HAL_GPIO_Init(DIO_PORT, &g);
}

uint8_t _tm1637Convert(char l){
	switch(l){
		case '0': return 0x3f;	case '1': return 0x06;	case '2': return 0x5b;
		case '3': return 0x4f;	case '4': return 0x66;	case '5': return 0x6d;
		case '6': return 0x7d;	case '7': return 0x07;	case '8': return 0x7f;
		case '9': return 0x6f;	case 'A': return 0x77;	case 'a': return 0x77;
		case 'B': return 0x7C;	case 'b': return 0x7C;	case 'C': return 0x39;
		case 'c': return 0x58;	case 'D': return 0x5E;	case 'd': return 0x5E;
		case 'E': return 0x79;	case 'e': return 0x79;	case 'F': return 0x71;
		case 'f': return 0x71;	case 'G': return 0x3d;	case 'g': return 0x3d;
		case 'H': return 0x76;	case 'h': return 0x74;	case 'I': return 0x30;
		case 'i': return 0x30;	case 'J': return 0x1E;	case 'j': return 0x1E;
		case 'L': return 0x38;	case 'l': return 0x38;	case 'M': return 0x55;
		case 'm': return 0x55;	case 'N': return 0x37;	case 'n': return 0x54;
		case 'O': return 0x3F;	case 'o': return 0x5C;	case 'P': return 0x73;
		case 'p': return 0x73;	case 'Q': return 0x67;	case 'q': return 0x67;
		case 'R': return 0x50;	case 'r': return 0x50;	case 'S': return 0x6D;
		case 's': return 0x6D;	case 'T': return 0x78;	case 't': return 0x78;
		case 'U': return 0x3E;	case 'u': return 0x1C;	case 'W': return 0x6a;
		case 'w': return 0x6a;	case 'Y': return 0x6E;	case 'y': return 0x6E;
		default: return 0x00;
	}
}
void tm1637Display(char c[]){
	uint8_t digitArr[SegmentNo];
	int mc=0;
    for (int jt = 0; (jt-mc)<SegmentNo; jt++){
				if(c[jt+mc]=='.'){digitArr[jt-1] |= 0x80;mc++;}
				if(c[jt+mc])digitArr[jt] = _tm1637Convert(c[jt+mc]);
				else digitArr[jt] = 0x00; 
    }
		
    _tm1637Start();
    _tm1637WriteByte(0x40);
    _tm1637ReadResult();
    _tm1637Stop();
    _tm1637Start();
    _tm1637WriteByte(0xc0);
    _tm1637ReadResult();
    for (int it = 0; it < SegmentNo; it++) {
        _tm1637WriteByte(digitArr[it]);
        _tm1637ReadResult();
    }
    _tm1637Stop();
}	

uint8_t tm1637ReadKey(){
	uint8_t retval = 0;

    _tm1637Start();	
		_tm1637WriteByte(0x42);
    _tm1637ReadResult();
	

		_tm1637DioHigh();
		_tm1637DelayUsec(1);
		for(uint8_t bit = 0; bit < 8;bit++){
			_tm1637ClkHigh();
			retval <<= 1;
			if(HAL_GPIO_ReadPin(DIO_PORT,DIO_PIN)==1){
				retval |= 0x01;
			}
			_tm1637ClkLow();
			_tm1637DelayUsec(1);
		
		}
		_tm1637DioLow();

		_tm1637Stop();
		
		

	return retval;

}

// Valid brightness values: 0 - 8.
void tm1637SetBrightness(char brightness)
{
    _tm1637Start();
    _tm1637WriteByte(0x87 + brightness);
    _tm1637ReadResult();
    _tm1637Stop();
}

void _tm1637Start(void)
{
    _tm1637ClkHigh();
    _tm1637DioHigh();
    _tm1637DelayUsec(2);
    _tm1637DioLow();
}

void _tm1637Stop(void)
{
    _tm1637ClkLow();
    _tm1637DelayUsec(2);
    _tm1637DioLow();
    _tm1637DelayUsec(2);
    _tm1637ClkHigh();
    _tm1637DelayUsec(2);
    _tm1637DioHigh();
}

void _tm1637ReadResult(void)
{
    _tm1637ClkLow();
    _tm1637DelayUsec(5);
    //while (HAL_GPIO_ReadPin(DIO_PORT,DIO_PIN)==1);
    _tm1637ClkHigh();
    _tm1637DelayUsec(2);
    _tm1637ClkLow();
}

void _tm1637WriteByte(unsigned char b)
{
    for (int i = 0; i < 8; ++i) {
        _tm1637ClkLow();
        if (b & 0x01) {
            _tm1637DioHigh();
        }
        else {
            _tm1637DioLow();
        }
        _tm1637DelayUsec(2);
        b >>= 1;
        _tm1637ClkHigh();
        _tm1637DelayUsec(2);
    }
}

void _tm1637DelayUsec(unsigned int i)
{
    for (; i>0; i--) {
        for (int j = 0; j < 300; ++j) {
					__NOP();
        }
    }
}

void _tm1637ClkHigh(void)
{
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
}

void _tm1637ClkLow(void)
{
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
}

void _tm1637DioHigh(void)
{
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
}

void _tm1637DioLow(void)
{
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
}

