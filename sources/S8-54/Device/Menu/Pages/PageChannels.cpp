#include "defines.h"
#include "Display/Display.h"
#include "FPGA/fpga.h"
#include "FPGA/fpgaExtensions.h"
#include "Log.h"
#include "Menu/MenuItems.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "Menu/Pages/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const    Page pChanA;
extern const  Choice cChanA_Input;                      ///< ����� 1 - ����
       void OnChanged_ChanA_Input(bool active);
extern const  Choice cChanA_Couple;                     ///< ����� 1 - �����
static void OnChanged_ChanA_Couple(bool active);
extern const  Choice cChanA_Bandwidth;                  ///< ����� 1 - ������
static void OnChanged_ChanA_Bandwidth(bool active);
extern const  Choice cChanA_Resistance;                 ///< ����� 1 - �� ����
static void OnChanged_ChanA_Resistance(bool active);
extern const  Choice cChanA_Inverse;                    ///< ����� 1 - ��������
static void OnChanged_ChanA_Inverse(bool active);
extern const  Choice cChanA_Divider;                    ///< ����� 1 - ��������
extern const  Button bChanA_Balance;                    ///< ����� 1 - �������������
static void   OnPress_ChanA_Balance(void);
extern const    Page pChanB;
extern const  Choice cChanB_Input;
static bool  IsActive_ChanB_Input(void);
      void  OnChanged_ChanB_Input(bool active);
extern const  Choice cChanB_Couple;
static void OnChanged_ChanB_Couple(bool active);
extern const  Choice cChanB_Bandwidth;
static void OnChanged_ChanB_Bandwidth(bool active);
extern const  Choice cChanB_Resistance;
static void OnChanged_ChanB_Resistance(bool active);
extern const  Choice cChanB_Inverse;
static void OnChanged_ChanB_Inverse(bool active);
extern const  Choice cChanB_Divider;
extern const  Button bChanB_Balance;
static void   OnPress_ChanB_Balance(void);


//----------------------------------------------------------------------------------------------------------------------------------------------------
static const char chanInputRu[] =   "1. \"���\" - �������� ������ �� �����.\n"
                                    "2. \"����\" - �� �������� ������ �� �����.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "����� ��� ����� � ���������� �������.\n"
                                    "1. \"����\" - �������� ����.\n"
                                    "2. \"�����\" - �������� ����.\n"
                                    "3. \"�����\" - ���� ������� � �����.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

static const char chanInverseRu[] = "��� \"���\" ������ �� ������ ����� ����������� ������ ������������ U = 0�.";
static const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanDividerRu[] = "���������� �������:\n\"����\" - ������ �� �����������.\n\"x10\" - ������ ����������� � 10 ���";
static const char chanDividerEn[] = "Attenuation: \n\"Off\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����� 1 ///
DEF_PAGE_7(         pChanA,
    ,
    Page_ChannelA, &mainPage, FuncActive, EmptyPressPage,
    "����� 1", "CHANNEL 1",
    "�������� ��������� ������ 1.",
    "Contains settings of the channel 1.",
    cChanA_Input,      // ����� 1 - ����
    cChanA_Couple,     // ����� 1 - �����
    cChanA_Bandwidth,  // ����� 1 - ������
    cChanA_Resistance, // ����� 1 - �� ����
    cChanA_Inverse,    // ����� 1 - ��������
    cChanA_Divider,    // ����� 1 - ��������
    bChanA_Balance     // ����� 1 - �������������
);

//--------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ���� ---
void OnChanged_InputA(bool)
{
    panel.EnableLEDChannel(A, SET_ENABLED_A);
}

DEF_CHOICE_2
(
    cChanA_Input, pChanA,
    SET_ENABLED_A, FuncActive, OnChanged_InputA, FuncDraw,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ����� ---
static void OnChanged_ChanA_Couple(bool)
{
    FPGA_SetModeCouple(A, SET_COUPLE_A);
}

DEF_CHOICE_3
(
    cChanA_Couple, pChanA,
    SET_COUPLE_A, FuncActive, OnChanged_ChanA_Couple, FuncDraw,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����", "AC",
    "�����", "DC",
    "�����", "Ground"
);


//------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ������ ---
static void OnChanged_ChanA_Bandwidth(bool)
{
    FPGA_SetBandwidth(A);
}

DEF_CHOICE_2
(
    cChanA_Bandwidth, pChanA,
    SET_BANDWIDTH_A, FuncActive, OnChanged_ChanA_Bandwidth, FuncDraw,
    "������", "Bandwidth",
    "����� ������ ����������� ������", 
    "Sets the channel bandwidth",
    "������", "Full",
    "20���", "20MHz"
);

//------------------------------------------------------------------------------------------------------------------------ ����� 1 - ������������� ---
static void OnChanged_ChanA_Resistance(bool)
{
    FPGA_SetResistance(A, SET_RESISTANCE_A);
    if (SET_RESISTANCE_A == Resistance_50Om)
    {
        display.ShowWarning(Warn50Ohms);
    }
}

DEF_CHOICE_2
(
    cChanA_Resistance, pChanA,
    SET_RESISTANCE_A, FuncActive, OnChanged_ChanA_Resistance, FuncDraw,
    "�� ����", "Resistance",
    "",
    "",
    "1 ���", "1 Mohm",
    "50 ��", "50 Ohm"
);

//----------------------------------------------------------------------------------------------------------------------------- ����� 1 - �������� ---
static void OnChanged_ChanA_Inverse(bool)
{
    FPGA_SetRShift(A, SET_RSHIFT_A);
}

DEF_CHOICE_2
(
    cChanA_Inverse, pChanA,
    SET_INVERSE_A, FuncActive, OnChanged_ChanA_Inverse, FuncDraw,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- ����� 1 - �������� ---
DEF_CHOICE_2
(
    cChanA_Divider, pChanA,
    SET_DIVIDER_A, FuncActive, FuncChangedChoice, FuncDraw,
    "��������", "Divider",
    chanDividerRu,
    chanDividerEn,
    "����", "Off",
    "1/10", "1/10"
);

//------------------------------------------------------------------------------------------------------------------------ ����� 1 - ������������� ---
static void OnPress_ChanA_Balance(void)
{
    FPGA_BalanceChannel(A);
}

DEF_BUTTON
(
    bChanA_Balance, 
    "�������������", "Balance",
    "������������� �����",
    "Balancing channel",
    pChanA, EmptyFuncBV, OnPress_ChanA_Balance, EmptyFuncVII
)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����� 2 ///
DEF_PAGE_7(         pChanB,
    ,
    Page_ChannelB, &mainPage, FuncActive, EmptyPressPage,
    "����� 2", "CHANNEL 2",
    "�������� ��������� ������ 2.",
    "Contains settings of the channel 2.",
    cChanB_Input,       // ����� 2 - ����
    cChanB_Couple,      // ����� 2 - �����
    cChanB_Bandwidth,   // ����� 2 - ������
    cChanB_Resistance,  // ����� 2 - �������������
    cChanB_Inverse,     // ����� 2 - ��������
    cChanB_Divider,     // ����� 2 - ��������
    bChanB_Balance      // ����� 2 - �������������
);

//--------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ���� ---
static bool IsActive_ChanB_Input(void)
{
    return !FPGA_POINTS_32k;
}

void OnChanged_InputB(bool active)
{
    if (!active)
    {
        display.ShowWarning(TooLongMemory);
        return;
    }

    if (FPGA_POINTS_32k && SET_ENABLED_B)
    {
        SET_ENABLED_B = false;
    }
    panel.EnableLEDChannel(B, SET_ENABLED_B);
}

DEF_CHOICE_2
(
    cChanB_Input, pChanB,
    SET_ENABLED_B, IsActive_ChanB_Input, OnChanged_InputB, FuncDraw,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ����� ---
DEF_CHOICE_3
(
    cChanB_Couple, pChanB,
    SET_COUPLE_B, FuncActive, OnChanged_ChanB_Couple, FuncDraw,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����", "AC",
    "�����", "DC",
    "�����", "Ground"
);

static void OnChanged_ChanB_Couple(bool)
{
    FPGA_SetModeCouple(B, SET_COUPLE_B);
}

//------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ������ ---
static void OnChanged_ChanB_Bandwidth(bool)
{
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_2
(
    cChanB_Bandwidth, pChanB,
    SET_BANDWIDTH_B, FuncActive, OnChanged_ChanB_Bandwidth, FuncDraw,
    "������", "Bandwidth",
    "",
    "",
    "������", "Full", 
    "20���", "20MHz"
);

//------------------------------------------------------------------------------------------------------------------------ ����� 2 - ������������� ---
static void OnChanged_ChanB_Resistance(bool)
{
    FPGA_SetResistance(B, SET_RESISTANCE_B);
    if (SET_RESISTANCE_B == Resistance_50Om)
    {
        display.ShowWarning(Warn50Ohms);
    }
}

DEF_CHOICE_2
(
    cChanB_Resistance, pChanB,
    SET_RESISTANCE_B, FuncActive, OnChanged_ChanB_Resistance, FuncDraw,
    "�� ����", "Resistance",
    "", "",
    "1 ���", "1 Mohm",
    "50 ��", "50 Ohm"
);

//----------------------------------------------------------------------------------------------------------------------------- ����� 2 - �������� ---
static void OnChanged_ChanB_Inverse(bool)
{
    FPGA_SetRShift(B, SET_RSHIFT_B);
}

DEF_CHOICE_2
(
    cChanB_Inverse, pChanB,
    SET_INVERSE_B, FuncActive, OnChanged_ChanB_Inverse, FuncDraw,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- ����� 2 - �������� ---
DEF_CHOICE_2
(
    cChanB_Divider, pChanB,
    SET_DIVIDER_B, FuncActive, FuncChangedChoice, FuncDraw,
    "��������", "Divider",
    chanDividerRu,
    chanDividerEn,
    "����", "Jff",
    "1/10", "1/10"
);

//------------------------------------------------------------------------------------------------------------------------ ����� 2 - ������������� ---
static void OnPress_ChanB_Balance(void)
{
    FPGA_BalanceChannel(B);
}

DEF_BUTTON
(
    bChanB_Balance,
    "�������������", "Balance",
    "������������� �����",
    "Balancing channel",
    pChanB, EmptyFuncBV, OnPress_ChanB_Balance, EmptyFuncVII
);
