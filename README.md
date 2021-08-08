# STM32-TM1637
Tm1637 library for stm32<br>
using this library to run 6 7segments and 16keys with stm32 and tm1637

<font color="green">In TM1637.c there is some configuration that you have to change!</font><br>
Ex(Configuration):<br>
#define CLK_PORT GPIOA<br>
#define DIO_PORT GPIOA<br>
#define CLK_PIN GPIO_PIN_1<br>
#define DIO_PIN GPIO_PIN_2<br>
#define CLK_PORT_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE<br>
#define DIO_PORT_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE<br>
#define SegmentNo 4(set the number of segments that are conected to tm1637)<br>


There are 4 functions that you need to know!
1. tm1637Init();<br>
It uses once in codes before any other code for tm1637 to initialization.<br>

2. tm1637SetBrightness(int);<br>
This function is using to set the brightness of 7segments.(0-8)<br>

3. void tm1637Display(char []);<br>
To display charectors and numbers and floats on 7segments.<br>

4. tm1637ReadKey();<br>
This function returns the decimal of the key that has been pressed.<br>
