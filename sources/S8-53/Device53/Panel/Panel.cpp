#include "defines.h"
#include "Panel.h"
#include "Controls.h"
#include "FPGA/FPGA.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Hardware/Sound.h"
#include <stdio.h>
#include <string.h>
#include "PanelFunctions.cpp"


Panel panel;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_DATA            20

#define LED_CHAN0_ENABLE    129
#define LED_CHAN0_DISABLE   1
#define LED_CHAN1_ENABLE    130
#define LED_CHAN1_DISABLE   2
#define LED_TRIG_ENABLE     131
#define LED_TRIG_DISABLE    3
#define POWER_OFF           4

static PanelButton pressedKey = B_Empty;
static PanelButton pressedButton = B_Empty;         // ��� ������������ ��� ������������ ������� ������ ��� ����������� ������
static uint16 dataTransmitted[MAX_DATA] = {0x00};
static uint16 numDataForTransmitted = 0;
static uint timePrevPressButton = 0;
static uint timePrevReleaseButton = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void(*funcOnKeyDown[B_NumButtons])(void)    =
{    
    0,
    EmptyFuncVV,    // B_ChannelA
    EmptyFuncVV,    // B_Service
    EmptyFuncVV,    // B_ChannelB
    EmptyFuncVV,    // B_Display
    EmptyFuncVV,    // B_Time
    EmptyFuncVV,    // B_Memory                                         
    EmptyFuncVV,    // B_Sinchro
    StartDown,      // B_Start
    EmptyFuncVV,    // B_Cursors
    EmptyFuncVV,    // B_Measures
    PowerDown,      // B_Power
    EmptyFuncVV,    // B_Help
    EmptyFuncVV,    // B_Menu
    EmptyFuncVV,    // B_F1
    EmptyFuncVV,    // B_F2
    EmptyFuncVV,    // B_F3
    EmptyFuncVV,    // B_F4
    EmptyFuncVV     // B_F5
};

static void (*funcOnKeyUp[B_NumButtons])(void)    =
{
    0,
    EmptyFuncVV,    // B_ChannelA
    EmptyFuncVV,    // B_Service
    EmptyFuncVV,    // B_ChannelB
    EmptyFuncVV,    // B_Display
    EmptyFuncVV,    // B_Time
    EmptyFuncVV,    // B_Memory
    EmptyFuncVV,    // B_Sinchro
    EmptyFuncVV,    // B_Start
    EmptyFuncVV,    // B_Cursors
    EmptyFuncVV,    // B_Measures
    EmptyFuncVV,    // B_Power
    EmptyFuncVV,    // B_Help
    EmptyFuncVV,    // B_Menu
    EmptyFuncVV,    // B_F1
    EmptyFuncVV,    // B_F2
    EmptyFuncVV,    // B_F3
    EmptyFuncVV,    // B_F4
    EmptyFuncVV     // B_F5
};

static void (*funcOnLongPressure[B_NumButtons])(void)    =
{
    0,
    ChannelALong,   // B_ChannelA
    EmptyFuncVV,    // B_Service
    ChannelBLong,   // B_ChannelB
    EmptyFuncVV,    // B_Display
    TimeLong,       // B_Time
    EmptyFuncVV,    // B_Memory
    TrigLong,       // B_Sinchro
    EmptyFuncVV,    // B_Start
    EmptyFuncVV,    // B_Cursors
    EmptyFuncVV,    // B_Measures
    EmptyFuncVV,    // B_Power
    HelpLong,       // B_Help
    MenuLong,       // B_Menu
    F1Long,         // B_F1
    F2Long,         // B_F2
    F3Long,         // B_F3
    F4Long,         // B_F4
    F5Long          // B_F5
};

static void (*funculatorLeft[R_Set + 1])(void)    =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RangeALeft,     // R_RangeA
    RShiftALeft,    // R_RShiftA
    RangeBLeft,     // R_RangeB
    RShiftBLeft,    // R_RShiftB
    TBaseLeft,      // R_TBase
    TShiftLeft,     // R_TShift
    TrigLevLeft,    // R_TrigLev
    SetLeft         // R_Set
};
static void (*funculatorRight[R_Set + 1])(void) =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    RangeARight,    // R_RangeA
    RShiftARight,   // R_RShiftA
    RangeBRight,    // R_RangeB
    RShiftBRight,   // R_RShiftB
    TBaseRight,     // R_TBase
    TShiftRight,    // R_TShift
    TrigLevRight,   // R_TrigLev
    SetRight        // R_Set
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelButton ButtonIsRelease(uint16 command)
{
    PanelButton button = B_Empty;
    if(command < B_NumButtons && command > B_Empty)
    {
        if(gTimeMS - timePrevReleaseButton > 100)
        {
            button = (PanelButton)command;
            timePrevReleaseButton = gTimeMS;
        }
    }
    return button;
}

PanelButton ButtonIsPress(uint16 command)
{
    PanelButton button = B_Empty;
    if (((command & 0x7f) < B_NumButtons) && ((command & 0x7f) > B_Empty))
    {
        if(gTimeMS - timePrevPressButton > 100)
        {
            button = (PanelButton)(command & 0x7f);
            timePrevPressButton = gTimeMS;
        }
    }
    return button;
}

PanelRegulator RegulatorLeft(uint16 command)
{
    if(command >= 20 && command <= 27)
    {
        return (PanelRegulator)command;
    }
    return R_Empty;
}

PanelRegulator RegulatorRight(uint16 command)
{
    if(((command & 0x7f) >= 20) && ((command & 0x7f) <= 27))
    {
        return (PanelRegulator)(command & 0x7f);
    }
    return R_Empty;
}

void OnTimerPressedKey()
{
    if(pressedKey != B_Empty)
    {
        void (*func)(void) = funcOnLongPressure[pressedKey];
        menu.ReleaseButton(pressedKey);
        if(func != 0)
        {
            func();
        }
        pressedKey = B_Empty;
    }
    Timer_Disable(kPressKey);
}

bool Panel::ProcessingCommandFromPIC(uint16 command)
{
    static int allRecData = 0;

    PanelButton releaseButton = ButtonIsRelease(command);
    PanelButton pressButton = ButtonIsPress(command);
    PanelRegulator regLeft = RegulatorLeft(command);
    PanelRegulator regRight = RegulatorRight(command);

    if (pressButton)
    {
        pressedButton = pressButton;
    }

    if(gBF.panelIsRunning == 0)
    {
        return true;
    }

    if(command != 0)
    {
        allRecData++;
    }
    else
    {
        allRecData++;
        allRecData--;
    }

    if(releaseButton)
    {
        menu.ReleaseButton(releaseButton);
        funcOnKeyUp[releaseButton]();
        if(pressedKey != B_Empty)
        {
            menu.ShortPressureButton(releaseButton);
            pressedKey = B_Empty;
        }
    }
    else if(pressButton)
    {
        funcOnKeyDown[pressButton]();
        menu.PressButton(pressButton);
        pressedKey = pressButton;
        Timer_Enable(kPressKey, 500, OnTimerPressedKey);
    }
    else if(regLeft)
    {
        /*
        if (set.memory.modeWork == ModeWork_Dir || regLeft == R_Set || regLeft == R_TShift)
        {
        */
            funculatorLeft[regLeft]();
        //}
    }
    else if(regRight)
    {
        /*
        if (set.memory.modeWork == ModeWork_Dir || regRight == R_Set || regRight == R_TShift)
        {
        */
            funculatorRight[regRight]();
        //}
    }

    if ((command > R_Set && command < (B_Empty + 1 + 128)) || (command > (R_Set + 128)))
    {
        if(Settings_DebugModeEnable())
        {
            static int errRecData = 0;
            errRecData++;
            float percent = (float)errRecData / allRecData * 100.0f;
            char buffer[100];
            buffer[0] = 0;
            sprintf(buffer, "%5.3f", percent);
            strcat(buffer, "%");
            LOG_ERROR("������ SPI - %s %d/%d, command = %d", buffer, errRecData, allRecData, (int)command);
        }
        return false;
    }
    return true;
}

void Panel::EnableLEDChannelA(bool enable)
{
    panel.TransmitData(enable ? LED_CHANA_ENABLE : LED_CHANA_DISABLE);
}

void Panel::EnableLEDChannelB(bool enable)
{
    panel.TransmitData(enable ? LED_CHANB_ENABLE : LED_CHANB_DISABLE);
}

void Panel::EnableLEDTrig(bool enable)
{
    static uint timeEnable = 0;
    static bool first = true;
    static bool fired = false;
    if(first)
    {
        panel.TransmitData(LED_TRIG_DISABLE);
        display.EnableTrigLabel(false);
        timeEnable = gTimeMS;
        first = false;
    }

    if(enable)
    {
        timeEnable = gTimeMS;
    }

    if(enable != fired)
    {
        if(enable)
        {
            panel.TransmitData(LED_TRIG_ENABLE);
            display.EnableTrigLabel(true);
            fired = true;
        }
        else if(gTimeMS - timeEnable > 100)
        {
            panel.TransmitData(LED_TRIG_DISABLE);
            display.EnableTrigLabel(false);
            fired = false;
        }
    }
}

void Panel::TransmitData(uint16 data)
{
    if(numDataForTransmitted >= MAX_DATA)
    {
        LOG_WRITE("�� ���� ������� � ������ - ���� �����");
    }
    else
    {
        dataTransmitted[numDataForTransmitted] = data;
        numDataForTransmitted++;
    }
}

uint16 Panel::NextData()
{
    if (numDataForTransmitted > 0)
    {
        numDataForTransmitted--;
        return dataTransmitted[numDataForTransmitted];
    }
    return 0;
}

void Panel::Disable()
{
    gBF.panelIsRunning = 0;
}

void Panel::Enable()
{
    gBF.panelIsRunning = 1;
}

/*
    SPI1
    56  - PG0 - ����������� NSS 
    41  - PA5 - SCK
    42  - PA6 - MISO
    135 - PB5 - MOSI
*/



void Panel::Init()
{
    __SPI1_CLK_ENABLE();

    GPIO_InitTypeDef isGPIOA_B =
    {
        GPIO_PIN_5 | GPIO_PIN_6,    // GPIO_Pin
        GPIO_MODE_AF_PP,            // GPIO_Mode
        GPIO_PULLDOWN,
        GPIO_SPEED_FAST,            // GPIO_Speed
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA_B);
    
    isGPIOA_B.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &isGPIOA_B);
    
    if (HAL_SPI_Init(&handleSPI) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(SPI1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    
    // ������ �������� ����������� NSS (PG0).

    GPIO_InitTypeDef isGPIOG =
    {
        GPIO_PIN_0,                 // GPIO_Pin
        GPIO_MODE_IT_RISING,        // GPIO_Mode
        GPIO_NOPULL
    };      
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    // �������� ���������
    isGPIOG.Pin = GPIO_PIN_12;
    isGPIOG.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIOG.Speed = GPIO_SPEED_HIGH;
    isGPIOG.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOG, &isGPIOG);

    panel.EnableLEDRegSet(false);
}

void Panel::EnableLEDRegSet(bool enable)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

PanelButton Panel::WaitPressingButton()
{
    pressedButton = B_Empty;
    while (pressedButton == B_Empty) {};
    return pressedButton;
}

static uint8 dataSPIfromPanel;

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        HAL_SPI_Receive_IT(&handleSPI, &dataSPIfromPanel, 1);
    }
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* handleSPI)
{
    if (!panel.ProcessingCommandFromPIC(dataSPIfromPanel))
    {
        HAL_SPI_DeInit(handleSPI);
        HAL_SPI_Init(handleSPI);
    }
    uint16 data = panel.NextData();
    SPI1->DR = data;
}
