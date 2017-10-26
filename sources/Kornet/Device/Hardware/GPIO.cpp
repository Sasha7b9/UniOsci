#include "GPIO.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GPIO gpio;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO::SetInputPullDown(GPIO_TypeDef *gpio, uint numPin)
{
    gpio->MODER &= ~(3U << (numPin * 2));

    gpio->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (numPin * 2U));
    gpio->PUPDR |= GPIO_PULLDOWN << (numPin * 2U);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GPIO::SetOutputPP_PullDown(GPIO_TypeDef *gpio, uint numPin)
{
    gpio->MODER &= ~(GPIO_MODER_MODER0 << (numPin * 2));
    gpio->MODER |= (GPIO_MODE_OUTPUT_PP << (numPin * 2));

    gpio->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (numPin * 2U));
    gpio->PUPDR |= GPIO_PULLDOWN << (numPin * 2U);
}
