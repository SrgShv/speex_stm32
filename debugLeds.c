
#include "debugLeds.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_gpio.h"
#include "main.h"


void initGPIO_LED(void)
{
    GPIO_InitTypeDef gpio;

    // Включаем тактирование
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &gpio);
}

void debugBlinkBlueLed(const int divScaler)
{
    static int bcntr = 0;
    static char bflg = 0;

    if(++bcntr >= divScaler)
    {
        bcntr = 0;
        if(bflg == 0)
        {
            bflg = 1;
            //GPIO_SetBits(GPIOD, GPIO_Pin_15);
            GPIOD->BSRRL = GPIO_Pin_15;
        }
        else
        {
            bflg = 0;
            //GPIO_ResetBits(GPIOD, GPIO_Pin_15);
            GPIOD->BSRRH = GPIO_Pin_15;
        };
    };
}

void debugBlinkRedLed(const int divScaler)
{
	static int rcntr = 0;
	static char rflg = 0;

	if(++rcntr >= divScaler)
    {
        rcntr = 0;
        if(rflg == 0)
        {
            rflg = 1;
            //GPIO_SetBits(GPIOD, GPIO_Pin_14);
				GPIOD->BSRRL = GPIO_Pin_14;
        }
        else
        {
            rflg = 0;
            //GPIO_ResetBits(GPIOD, GPIO_Pin_14);
				GPIOD->BSRRH = GPIO_Pin_14;
        };
    };
}

void debugBlinkOrangeLed(const int divScaler)
{
	static int ocntr = 0;
	static char oflg = 0;

	if(++ocntr >= divScaler)
    {
        ocntr = 0;
        if(oflg == 0)
        {
            oflg = 1;
            //GPIO_SetBits(GPIOD, GPIO_Pin_13);
				GPIOD->BSRRL = GPIO_Pin_13;
        }
        else
        {
            oflg = 0;
            //GPIO_ResetBits(GPIOD, GPIO_Pin_13);
				GPIOD->BSRRH = GPIO_Pin_13;
        };
    };
}

void debugBlinkGreenLed(const int divScaler)
{
	static int gcntr = 0;
	static char gflg = 0;

	if(++gcntr >= divScaler)
    {
        gcntr = 0;
        if(gflg == 0)
        {
            gflg = 1;
            //GPIO_SetBits(GPIOD, GPIO_Pin_12);
				GPIOD->BSRRL = GPIO_Pin_12;
        }
        else
        {
            gflg = 0;
            //GPIO_ResetBits(GPIOD, GPIO_Pin_12);
				GPIOD->BSRRH = GPIO_Pin_12;
        };
    };
}
