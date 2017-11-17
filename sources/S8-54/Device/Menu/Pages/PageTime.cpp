#include "defines.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "PageMemory.h"
#include "FPGA/FPGAtypes.h"
#include "FPGA/FPGA.h"
#include "Log.h"
#include "Display/Display.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/_Math.h" 
#include "Hardware/FSMC.h"
#include "Display/Grid.h"
#include "Definition.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const    Page pTime;
extern const  Choice cSample;               ///< ��������� - �������
static bool  IsActive_Sample(void);
extern const  Choice cPeakDet;              ///< ��������� - ��� ���
static bool  IsActive_PeakDet(void);
       void OnChanged_PeakDet(bool active);
extern const  Choice cTPos;                 ///< ��������� - ��
       void OnChanged_TPos(bool active);
extern const  Choice cDivRole;              ///< ��������� - �-��� ��/���
extern const  Choice cShiftXtype;           ///< ��������� - ��������

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��������� ///
DEF_PAGE_5(     pTime, ,
    Page_Time, &mainPage, FuncActive, EmptyPressPage,
    "���������", "SCAN",
    "�������� ��������� ��������.",
    "Contains scan settings.",
    cSample,    // ��������� - �������
    cPeakDet,   // ��������� - ��� ���
    cTPos,      // ��������� - ��
    cDivRole,   // ��������� - �-��� ��/���
    cShiftXtype // ��������� - ��������
);

const Page * pointerPageTime = &pTime;

//---------------------------------------------------------------------------------------------------------------------------- ��������� - ������� ---
static bool IsActive_Sample(void)
{
    return IN_RANDOM_MODE && !START_MODE_SINGLE;
}

DEF_CHOICE_2
(
    cSample, pTime,
    SAMPLE_TYPE, IsActive_Sample, FuncChangedChoice, FuncDraw,
    "�������", "Sampling",
    "\"��������\" - \n"
    "\"�������������\" -",
    "\"Real\" - \n"
    "\"Equals\" - ",

    "�������� �����", "Real",
    "�������������",  "Equals"
);

//---------------------------------------------------------------------------------------------------------------------------- ��������� - ��� ��� ---
static bool IsActive_PeakDet(void)
{
    if (FPGA_POINTS_32k)                       // ��� 32� ����� �� ����� �� �� ����� �������� ������� ��������
    {
        return false;
    }

    if (FPGA_POINTS_16k && SET_ENABLED_B) // ��� 16� ������ �� ����� �� ����� �������� ������ � ����� �������
    {
        return false;
    }

    return (SET_TBASE >= MIN_TBASE_PEC_DEAT);
}

void OnChanged_PeakDet(bool active)
{
    if(active)
    {
        FPGA_SetPeackDetMode(SET_PEAKDET);
        FPGA_SetTBase(SET_TBASE);
        OnChanged_Points(true);
    }
    else
    {
        if (FPGA_POINTS_32k)
        {
            display.ShowWarning(NoPeakDet32k);
        }
        else if (FPGA_POINTS_16k && SET_ENABLED_B)
        {
            display.ShowWarning(NoPeakDet16k);
        }
        else
        {
            display.ShowWarning(TooSmallSweepForPeakDet);
        }
    }
}

DEF_CHOICE_2
(
    cPeakDet, pTime,
    SET_PEAKDET, IsActive_PeakDet, OnChanged_PeakDet, FuncDraw,
    "��� ���", "Pic deat",
    "��������/��������� ������� ��������.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------------------------------------- ��������� - �� ---
void OnChanged_TPos(bool active)
{
    OnChanged_Points(active);
    FPGA_SetTShift(SET_TSHIFT);
}

DEF_CHOICE_3
(
    cTPos, pTime,
    TPOS, FuncActive, OnChanged_TPos, FuncDraw,
    "\x7b", "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.", 
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "����",  "Left",
    "�����", "Center",
    "�����", "Right"
);

//----------------------------------------------------------------------------------------------------------------------- ��������� - �-��� ��/��� ---
DEF_CHOICE_2
(
    cDivRole, pTime,
    TIME_DIVXPOS, FuncActive, FuncChangedChoice, FuncDraw,
    "�-��� ��/���", "Funct Time/DIV",
    "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
    "1. \"�����\" - ��������� �������� �� �������.\n"
    "2. \"������\" - ����������� �� ������.",
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory.",
    "�����",  "Time",
    "������", "Memory"
);

//--------------------------------------------------------------------------------------------------------------------------- ��������� - �������� ---
DEF_CHOICE_2
(
    cShiftXtype, pTime,
    LINKING_TSHIFT, FuncActive, FuncChangedChoice, FuncDraw,
    "��������", "Offset",
    "����� ����� ��������� �������� �� �����������\n1. \"�����\" - ����������� ���������� �������� � ��������.\n2. \"�������\" - ����������� "
    "��������� ������� �� ������.",
    "Sets the mode of retaining the horizontal displacement\n1. \"Time\" - saved the asbolutic offset in seconds.\n2. \"Divisions\" - retained "
    "the position of the offset on the screen.",
    "�����",   "Time",
    "�������", "Divisions"
);
