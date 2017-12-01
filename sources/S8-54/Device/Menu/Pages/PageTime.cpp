#include "defines.h"
#include "FPGA/FPGA.h"
#include "PageMemory.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


extern const Page mainPage;
extern const Page pTime;


//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Sample(void)
{
    return IN_RANDOM_MODE && !START_MODE_SINGLE;
}

DEF_CHOICE_2(       cSample,                                                                                             //--- ��������� - ������� ---
    "�������", "Sampling",
    "\"��������\" - \n"
    "\"�������������\" -",
    "\"Real\" - \n"
    "\"Equals\" - ",
    "�������� �����", "Real",
    "�������������",  "Equals",
    SAMPLE_TYPE, pTime, IsActive_Sample, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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
    if (active)
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

DEF_CHOICE_2(       cPeakDet,                                                                                            //--- ��������� - ��� ��� ---
    "��� ���", "Pic deat",
    "��������/��������� ������� ��������.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_PEAKDET, pTime, IsActive_PeakDet, OnChanged_PeakDet, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_TPos(bool active)
{
    OnChanged_Points(active);
    FPGA_SetTShift(SET_TSHIFT);
}

DEF_CHOICE_3(       cTPos,                                                                                                    //--- ��������� - �� ---
    "\x7b", "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "����",  "Left",
    "�����", "Center",
    "�����", "Right",
    TPOS, pTime, FuncActive, OnChanged_TPos, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cDivRole,                                                                                       //--- ��������� - �-��� ��/��� ---
    "�-��� ��/���", "Funct Time/DIV"
    ,
    "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
    "1. \"�����\" - ��������� �������� �� �������.\n"
    "2. \"������\" - ����������� �� ������."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory."
    ,
    "�����",  "Time",
    "������", "Memory",
    TIME_DIVXPOS, pTime, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cShiftXtype,                                                                                        //--- ��������� - �������� ---
    "��������", "Offset"
    ,
    "����� ����� ��������� �������� �� �����������\n1. \"�����\" - ����������� ���������� �������� � ��������.\n2. \"�������\" - ����������� "
    "��������� ������� �� ������."
    ,
    "Sets the mode of retaining the horizontal displacement\n1. \"Time\" - saved the asbolutic offset in seconds.\n2. \"Divisions\" - retained "
    "the position of the offset on the screen."
    ,
    "�����",   "Time",
    "�������", "Divisions",
    LINKING_TSHIFT, pTime, FuncActive, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pTime,                                                                                                            // ��������� ///
    "���������", "SCAN",
    "�������� ��������� ��������.",
    "Contains scan settings.",
    cSample,        // ��������� - �������
    cPeakDet,       // ��������� - ��� ���
    cTPos,          // ��������� - ��
    cDivRole,       // ��������� - �-��� ��/���
    cShiftXtype,    // ��������� - ��������
    &mainPage, FuncActive, EmptyPressPage
);

const Page * pointerPageTime = &pTime;
