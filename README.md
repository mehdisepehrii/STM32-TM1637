# STM32-TM1637
Tm1637 library for stm32
using this library to run 6 7segments and 16keys with stm32 and tm1637

In TM1637.c there is some configuration that you have to change!
Ex(Configuration):
#define CLK_PORT GPIOA
#define DIO_PORT GPIOA
#define CLK_PIN GPIO_PIN_1
#define DIO_PIN GPIO_PIN_2
#define CLK_PORT_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE
#define DIO_PORT_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE
#define SegmentNo 4


There are 3 functions that you need to know!
1. tm1637Init();
It uses once in codes before any other code for tm1637 to initialization.
2.tm1637SetBrightness(int);
This function is using to set the brightness of 7segments.(0-8)
3.void tm1637Display(char []);
To display charectors and numbers and floats on 7segments.
4.tm1637ReadKey();
This function returns the decimal of the key that has been pressed.
