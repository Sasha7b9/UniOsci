#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hardware
{
public:

    void Init();
private:
    void SystemClock_Config(void);
    void MX_GPIO_Init(void);
    void MX_ADC3_Init(void);
    void MX_DAC_Init(void);
    void MX_SPI4_Init(void);
    void MX_USART3_UART_Init(void);
    void MX_USB_OTG_FS_PCD_Init(void);
    void MX_USB_OTG_HS_PCD_Init(void);
};


void _Error_Handler(char *, int);

#define ERROR_HANDLER() _Error_Handler((char *)__FILE__, __LINE__)
