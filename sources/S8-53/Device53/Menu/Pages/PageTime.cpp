#include "defines.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "PageMemory.h"
#include "FPGA/FPGA.h"
#include "Log.h"
#include "Display/Display.h"
#include "Utils/Math.h"
#include "Hardware/FSMC.h"
#include "Display/Grid.h"
#include "Utils/Dictionary.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageTime
 *  @{
 */


extern const Page pTime;
extern const Page mainPage;

// ��������� - ������� -------------------------------------------------------------------------------------------------------------------------------
static const Choice mcSample =
{
    Item_Choice, &pTime, IsActive_Sample,
    {
        "�������", "Sampling"
        ,
    "\"��������\" - \n"
    "\"�������������\" -"
    ,
    "\"Real\" - \n"
    "\"Equals\" - "
    },
    {
        {"�������� �����",  "Real"},
        {"�������������",   "Equals"}
    },
    (int8*)&SAMPLE_TYPE
};

static bool IsActive_Sample(void)
{
    return sTime_RandomizeModeEnabled();
}

DEF_CHOICE_2
(
    mcSample,
    pTime,
    SAMPLE_TYPE, IsActive_Sample, FuncChangedChoice, FuncDraw,
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pTime,                                                                                                            // ��������� ///
    ,
    Page_Time, &mainPage, FuncActive, EmptyPressPage,
    "���������", "SCAN",
    "�������� ��������� ��������.",
    "Contains scan settings.",
    mcSample,       // ��������� - �������
    mcPeakDet,      // ��������� - ��� ���
    mcTPos,         // ��������� - To
    mcSelfRecorder, // ��������� - ���������
    mcDivRole       // ��������� - �-��� ��/���    
);


// ��������� - ��� ��� -------------------------------------------------------------------------------------------------------------------------------
static const Choice mcPeakDet =
{
    Item_Choice, &pTime, IsActive_PeakDet,
    {
        "��� ���", "Pic deat",
        "��������/��������� ������� ��������.",
        "Turns on/off peak detector."
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
        /* , {"�������",   "Average"} */
    },
    (int8*)&SET_PEAKDET, OnChanged_PeakDet
};

static bool IsActive_PeakDet(void)
{
    return (SET_TBASE >= MIN_TBASE_PEC_DEAT);
}

void OnChanged_PeakDet(bool active)
{
    if (active)
    {
        fpga.SetPeackDetMode(SET_PEAKDET);
        fpga.SetTBase(SET_TBASE);
        if (PEAKDET_IS_DISABLE)
        {
            int8 shift[2][3] =
            {
                {0, (int8)SET_BALANCE_ADC_A, (int8)BALANCE_ADC_A},
                {0, (int8)SET_BALANCE_ADC_B, (int8)BALANCE_ADC_B}
            };

            fpga.WriteToHardware(WR_ADD_RSHIFT_DAC1, shift[0][BALANCE_ADC_TYPE], false);
            fpga.WriteToHardware(WR_ADD_RSHIFT_DAC2, shift[1][BALANCE_ADC_TYPE], false);
        }
        else
        {
            fpga.WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������.
            fpga.WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
        }
        if (PEAKDET_IS_DISABLE)
        {
            int centerX = SHIFT_IN_MEMORY + grid.Width() / 2;
            SHIFT_IN_MEMORY = centerX * 2 - grid.Width() / 2;
            ENUM_POINTS = set.time.oldNumPoints;
            ChangeC_Memory_NumPoints(true);
        }
        else if (PEAKDET_IS_ENABLE)
        {
            int centerX = SHIFT_IN_MEMORY + grid.Width() / 2;
            SHIFT_IN_MEMORY = centerX / 2 - grid.Width() / 2;
            Limitation<int16>(&SHIFT_IN_MEMORY, 0, sMemory_GetNumPoints(false) - grid.Width());
            ChangeC_Memory_NumPoints(true);
        }
    }
    else
    {
        display.ShowWarningBad(TooSmallSweepForPeakDet);
    }
}

// ��������� - To ------------------------------------------------------------------------------------------------------------------------------------
static const Choice mcTPos =
{
    Item_Choice, &pTime, 0,
    {
        "\x7b", "\x7b",
        "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
        "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge."
    },
    {
        {"����",    "Left"},
        {"�����",   "Center"},
        {"�����",   "Right"}
    },
    (int8*)&TPOS, OnChanged_TPos
};

void OnChanged_TPos(bool active)
{
    ChangeC_Memory_NumPoints(active);
    fpga.SetTShift(TSHIFT);
}

// ��������� - ��������� -----------------------------------------------------------------------------------------------------------------------------
static const Choice mcSelfRecorder =
{
    Item_Choice, &pTime, IsActive_SelfRecorder,
    {
        "���������", "Self-Recorder",
        "��������/��������� ����� ���������. ���� ����� �������� �� ��������� 20��/��� � ����� ���������.",
        "Turn on/off the recorder. This mode is available for scanning 20ms/div and slower."
    },
    {
        {DISABLE_RU,    DISABLE_EN},
        {ENABLE_RU,     ENABLE_EN}
    },
    (int8*)&SELFRECORDER
};

static bool IsActive_SelfRecorder(void)
{
    return SET_TBASE >= MIN_TBASE_P2P;
}

// ��������� - �-��� ��/��� --------------------------------------------------------------------------------------------------------------------------
static const Choice mcDivRole =
{
    Item_Choice, &pTime, 0,
    {
        "�-��� ��/���", "Func Time/DIV"
        ,
        "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
        "1. \"�����\" - ��������� �������� �� �������.\n"
        "2. \"������\" - ����������� �� ������."
        ,
        "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
        "1. \"Time\" - change the time shift.\n"
        "2. \"Memory\" - moving from memory."
    },
    {
        {"�����",   "Time"},
        {"������",  "Memory"}
    },
    (int8*)&TIME_DIV_XPOS
};


/** @}  @}
 */
