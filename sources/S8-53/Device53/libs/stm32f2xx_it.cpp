#include "usbd_desc.h"
#include "defines.h"
#include "VCP/VCP.h"
#include "Log.h"
#include <usbd_cdc_interface.h>


#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NMI_Handler()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HardFault_Handler()
{
    while (1)
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MemManage_Handler()
{
    while (1)
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BusFault_Handler()
{
    while (1)
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void UsageFault_Handler()
{
    while (1)
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SVC_Handler()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DebugMon_Handler()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PendSV_Handler()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SysTick_Handler()
{
    HAL_IncTick();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ADC_IRQHandler()
{
    HAL_ADC_IRQHandler(&handleADC);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/// This interrupt call soft NSS for spi (see Hardware::SPIforPanel.c::PanelInit() and HAL_GPIO_EXTI_Callback().
void EXTI0_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/// See Hardware::SPIforPanel.c::HAL_SPI_RxCpltCallback().
void SPI1_IRQHandler()
{
    HAL_SPI_IRQHandler(&handleSPI);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OTG_HS_IRQHandler()
{
    HAL_HCD_IRQHandler(&handleHCD);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DMA1_Stream5_IRQHandler()
{
    HAL_DMA_IRQHandler(handleDAC.DMA_Handle1);
}

#ifdef __cplusplus
}
#endif
