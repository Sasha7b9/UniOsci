#include "defines.h"
#include "Log.h"
#include "Settings/SettingsTypes.h"
#include "Menu/MenuItems.h"
#include "Panel/Panel.h"
#include "FPGA/FPGA.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageChannels
 *  @{
 */

extern const Page pChanA;
extern const Page pChanB;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const char chanInputRu[] =   "1. \"���\" - �������� ������ �� �����.\n"
                                    "2. \"����\" - �� �������� ������ �� �����.";
extern const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

extern const char chanCoupleRu[] =  "����� ��� ����� � ���������� �������.\n"
                                    "1. \"����\" - �������� ����.\n"
                                    "2. \"�����\" - �������� ����.\n"
                                    "3. \"�����\" - ���� ������� � �����.";
extern const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

extern const char chanFiltrRu[] = "��������/��������� ������ ��� ����������� ������ �����������.";
extern const char chanFiltrEn[] = "Includes/switches-off the filter for restriction of a pass-band.";

extern const char chanInverseRu[] = "��� \"���\" ������ �� ������ ����� ����������� ������ ������������ U = 0�.";
extern const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

extern const char chanMultiplierRu[] = "���������� �������:\n\"x1\" - ������ �� �����������.\n\"x10\" - ������ ����������� � 10 ���";
extern const char chanMultiplierEn[] = "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";

//--------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ���� ---
void OnChanged_InputA(bool active)
{
    panel.EnableLEDChannelA(sChannel_Enabled(A));
}

DEF_CHOICE_2
(
    mcInputA, pChanA,
    SET_ENABLED_A, FuncActive, OnChanged_InputA, FuncDraw,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ����� ---
void OnChanged_CoupleA(bool active)
{
    fpga.SetModeCouple(A, SET_COUPLE_A);
}

DEF_CHOICE_3
(
    mcCoupleA, pChanA,
    SET_COUPLE_A, FuncActive, OnChanged_CoupleA, FuncDraw,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground"
);

//------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ������ ---
void OnChanged_FiltrA(bool active)
{
    fpga.EnableChannelFiltr(A, FILTR_A);
}

DEF_CHOICE_2
(
    mcFiltrA, pChanA,
    FILTR_A, FuncActive, OnChanged_FiltrA, FuncDraw,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- ����� 1 - �������� ---
static void OnChanged_InverseA(bool active)
{
    fpga.SetRShift(A, SET_RSHIFT_A);
}

DEF_CHOICE_2
(
    mcInverseA, pChanA,
    SET_INVERSE_A, FuncActive, OnChanged_InverseA, FuncDraw,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//---------------------------------------------------------------------------------------------------------------------------- ����� 1 - ��������� ---
DEF_CHOICE_2
(
    mcMultiplierA, pChanA,
    SET_DIVIDER(A), FuncActive, FuncChangedChoice, FuncDraw,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1",  "x1",
    "x10", "x10"
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����� 1 ///
extern const Page mainPage;

DEF_PAGE_5
(
    pChanA, ,
    Page_ChannelA, &mainPage, FuncActive, EmptyPressPage,
    "����� 1", "CHANNEL 1",
    "�������� ��������� ������ 1.",
    "Contains settings of the channel 1.",
    mcInputA,       // ����� 1 - ����
    mcCoupleA,      // ����� 1 - �����
    mcFiltrA,       // ����� 1 - ������
    mcInverseA,     // ����� 1 - ��������
    mcMultiplierA   // ����� 1 - ���������
);

//--------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ���� ---
void OnChanged_InputB(bool active)
{
    panel.EnableLEDChannelB(sChannel_Enabled(B));
}

DEF_CHOICE_2
(
    mcInputB, pChanB,
    SET_ENABLED_B, FuncActive, OnChanged_InputB, FuncDraw,
    "����", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ����� ---
void OnChanged_CoupleB(bool active)
{
    fpga.SetModeCouple(B, SET_COUPLE_B);
}

DEF_CHOICE_3
(
    mcCoupleB, pChanB,
    SET_COUPLE_B, FuncActive, OnChanged_CoupleB, FuncDraw,
    "�����", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "����",  "AC",
    "�����", "DC",
    "�����", "Ground"
);

//------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ������ ---
void OnChanged_FiltrB(bool active)
{
    fpga.EnableChannelFiltr(B, FILTR_B);
}

DEF_CHOICE_2
(
    mcFiltrB, pChanB,
    FILTR_B, FuncActive, OnChanged_FiltrB, FuncDraw,
    "������", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- ����� 2 - �������� ---
static void OnChanged_InverseB(bool active)
{
    fpga.SetRShift(B, SET_RSHIFT_B);
}

DEF_CHOICE_2
(
    mcInverseB, pChanB,
    SET_INVERSE_B, FuncActive, OnChanged_InverseB, FuncDraw,
    "��������", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//---------------------------------------------------------------------------------------------------------------------------- ����� 2 - ��������� ---
DEF_CHOICE_2
(
    mcMultiplierB, pChanB,
    SET_DIVIDER(B), FuncActive, FuncChangedChoice, FuncDraw,
    "���������", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "�1",  "x1",
    "x10", "x10"
);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����� 2 ///
DEF_PAGE_5
(
    pChanB, ,
    Page_ChannelB, &mainPage, FuncActive, EmptyPressPage,
    "����� 2", "CHANNEL 2",
    "�������� ��������� ������ 2.",
    "Contains settings of the channel 2.",
    mcInputB,       // ����� 2 - ����
    mcCoupleB,      // ����� 2 - �����
    mcFiltrB,       // ����� 2 - ������
    mcInverseB,     // ����� 2 - ��������
    mcMultiplierB   // ����� 2 - ���������
);


/** @}  @}
 */
