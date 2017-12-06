/*
*************** �������� ����� ****************************
*/
#include "main.h"
#include "globals.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include "Hardware/FLASH.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FILE_NAME "S8-53.bin"

typedef void(*pFunction)(void);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainStruct *ms;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Upgrade(void);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    pFunction JumpToApplication;

    ms = (MainStruct *)malloc(sizeof(MainStruct));
    ms->percentUpdate = 0.0f;

    Hardware_Init();

    Settings_Load();

    Timer_PauseOnTime(250);

    ms->state = State_Start;

    Display_Init();

    Timer_Enable(kTemp, 10, Display_Update);

    uint timeStart = HAL_GetTick();

    drive.Init();

    while (HAL_GetTick() - timeStart < TIME_WAIT && !drive.Update())
    {
    }

    if ((ms->drive.connection && ms->drive.active == 0) ||  // ���� ����� ����������, �� � �������� ��������� ������-�� �� �������
        (ms->drive.active && ms->state != State_Mount))     // ��� ������� � �������� ���������, �� ������-�� �� ���������� ������� ������������
    {
        free(ms);
        NVIC_SystemReset();
    }

    if (ms->state == State_Mount)                           // ��� ��������, ��� ���� ������ �������������
    {
        if (drive.FileExist(FILE_NAME))                    // ���� �� ����� ���������� ��������
        {
            ms->state = State_RequestAction;

            while (1)
            {
                PanelButton button = Panel_PressedButton();
                if (button == B_F1)
                {
                    ms->state = State_Upgrade;
                    Upgrade();
                    break;
                }
                else if (button == B_F5)
                {
                    ms->state = State_Ok;
                    break;
                }
            }
        }
        else
        {
            ms->state = State_NotFile;
        }
    }
    else if (ms->state == State_WrongFlash) // ���� �� ������� ��������������
    {
        Timer_PauseOnTime(5000);
    }

    ms->state = State_Ok;

    Panel_DeInit();

    Timer_Disable(kTemp);

    while (Display_IsRun())
    {
    }

    Display_Update();

    HAL_DeInit();

    free(ms);

    __disable_irq();
    // ������ ��������� �� �������� ���������
    JumpToApplication = (pFunction)(*(__IO uint*)(MAIN_PROGRAM_START_ADDRESS + 4));
    __set_MSP(*(__IO uint*)MAIN_PROGRAM_START_ADDRESS);
    __enable_irq();
    JumpToApplication();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Upgrade(void)
{
    const int sizeSector = 1 * 1024;

    uint8 buffer[sizeSector];

    FLASH_Prepare();

    int size = drive.OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = ADDR_SECTOR_PROGRAM_0;

    while (size)
    {
        int readedBytes = drive.ReadFromFile(sizeSector, buffer);
        FLASH_WriteData(address, buffer, readedBytes);
        size -= readedBytes;
        address += readedBytes;

        ms->percentUpdate = 1.0f - (float)size / fullSize;
    }

    drive.CloseOpenedFile();
}


/*! \page page1 �������� ������ ����������
    \verbatim
    �� ������ 0x08000000 ��������� ���������.
    ����� ������ �� ��������� ������� ������.
        ���� ������ ����:
            ��������� ������� ����� S8-53.bin. ���� ������� �������, ������� ��������� "�� USB-����� ���������� ������ ����������� �����������. ����������?"
            ���� ������ "��":
                1. ��������� �������:
                    5 - 0x08020000
                    6 - 0x08040000
                    7 - 0x08060000
                2. �� �� ����� ������������ ���������� ����� S8-53.bin
        ���� ������ �������������� �� �������:
            ������� ��������� "�� ������� �������������� ������. ���������, ��� �� ��� �������� ������� fat32"
    ����� ����������� ������� �� ������, ���������� � 0x0802004
    \endverbatim
*/
