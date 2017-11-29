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
extern const  Choice cChanA_Input;                      ///< КАНАЛ 1 - Вход
       void OnChanged_ChanA_Input(bool active);
extern const  Choice cChanA_Couple;                     ///< КАНАЛ 1 - Связь
static void OnChanged_ChanA_Couple(bool active);
extern const  Choice cChanA_Bandwidth;                  ///< КАНАЛ 1 - Полоса
static void OnChanged_ChanA_Bandwidth(bool active);
extern const  Choice cChanA_Resistance;                 ///< КАНАЛ 1 - Вх сопр
static void OnChanged_ChanA_Resistance(bool active);
extern const  Choice cChanA_Inverse;                    ///< КАНАЛ 1 - Инверсия
static void OnChanged_ChanA_Inverse(bool active);
extern const  Choice cChanA_Divider;                    ///< КАНАЛ 1 - Делитель
extern const  Button bChanA_Balance;                    ///< КАНАЛ 1 - Балансировать
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
static const char chanInputRu[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                    "2. \"Откл\" - не выводить сигнал на экран.";
static const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

static const char chanCoupleRu[] =  "Задаёт вид связи с источником сигнала.\n"
                                    "1. \"Пост\" - открытый вход.\n"
                                    "2. \"Перем\" - закрытый вход.\n"
                                    "3. \"Земля\" - вход соединён с землёй.";
static const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

static const char chanInverseRu[] = "При \"Вкл\" сигнал на экране будет симметрично отражён относительно U = 0В.";
static const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

static const char chanDividerRu[] = "Ослабление сигнала:\n\"Выкл\" - сигнал не ослабляется.\n\"x10\" - сигнал ослабляется в 10 раз";
static const char chanDividerEn[] = "Attenuation: \n\"Off\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// КАНАЛ 1 ///
DEF_PAGE_7(         pChanA,
    ,
    Page_ChannelA, &mainPage, FuncActive, EmptyPressPage,
    "КАНАЛ 1", "CHANNEL 1",
    "Содержит настройки канала 1.",
    "Contains settings of the channel 1.",
    cChanA_Input,      // КАНАЛ 1 - Вход
    cChanA_Couple,     // КАНАЛ 1 - Связь
    cChanA_Bandwidth,  // КАНАЛ 1 - Полоса
    cChanA_Resistance, // КАНАЛ 1 - Вх сопр
    cChanA_Inverse,    // КАНАЛ 1 - Инверсия
    cChanA_Divider,    // КАНАЛ 1 - Делитель
    bChanA_Balance     // КАНАЛ 1 - Балансировать
);

//--------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Вход ---
void OnChanged_InputA(bool)
{
    panel.EnableLEDChannel(A, SET_ENABLED_A);
}

DEF_CHOICE_2
(
    cChanA_Input, pChanA,
    SET_ENABLED_A, FuncActive, OnChanged_InputA, FuncDraw,
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Связь ---
static void OnChanged_ChanA_Couple(bool)
{
    FPGA_SetModeCouple(A, SET_COUPLE_A);
}

DEF_CHOICE_3
(
    cChanA_Couple, pChanA,
    SET_COUPLE_A, FuncActive, OnChanged_ChanA_Couple, FuncDraw,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост", "AC",
    "Перем", "DC",
    "Земля", "Ground"
);


//------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Полоса ---
static void OnChanged_ChanA_Bandwidth(bool)
{
    FPGA_SetBandwidth(A);
}

DEF_CHOICE_2
(
    cChanA_Bandwidth, pChanA,
    SET_BANDWIDTH_A, FuncActive, OnChanged_ChanA_Bandwidth, FuncDraw,
    "Полоса", "Bandwidth",
    "Задаёт полосу пропускания канала", 
    "Sets the channel bandwidth",
    "Полная", "Full",
    "20МГц", "20MHz"
);

//------------------------------------------------------------------------------------------------------------------------ КАНАЛ 1 - Сопротивление ---
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
    "Вх сопр", "Resistance",
    "",
    "",
    "1 МОм", "1 Mohm",
    "50 Ом", "50 Ohm"
);

//----------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Инверсия ---
static void OnChanged_ChanA_Inverse(bool)
{
    FPGA_SetRShift(A, SET_RSHIFT_A);
}

DEF_CHOICE_2
(
    cChanA_Inverse, pChanA,
    SET_INVERSE_A, FuncActive, OnChanged_ChanA_Inverse, FuncDraw,
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Делитель ---
DEF_CHOICE_2
(
    cChanA_Divider, pChanA,
    SET_DIVIDER_A, FuncActive, FuncChangedChoice, FuncDraw,
    "Делитель", "Divider",
    chanDividerRu,
    chanDividerEn,
    "Выкл", "Off",
    "1/10", "1/10"
);

//------------------------------------------------------------------------------------------------------------------------ КАНАЛ 1 - Балансировать ---
static void OnPress_ChanA_Balance(void)
{
    FPGA_BalanceChannel(A);
}

DEF_BUTTON
(
    bChanA_Balance, 
    "Балансировать", "Balance",
    "Балансировать канал",
    "Balancing channel",
    pChanA, EmptyFuncBV, OnPress_ChanA_Balance, EmptyFuncVII
)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// КАНАЛ 2 ///
DEF_PAGE_7(         pChanB,
    ,
    Page_ChannelB, &mainPage, FuncActive, EmptyPressPage,
    "КАНАЛ 2", "CHANNEL 2",
    "Содержит настройки канала 2.",
    "Contains settings of the channel 2.",
    cChanB_Input,       // КАНАЛ 2 - Вход
    cChanB_Couple,      // КАНАЛ 2 - Связь
    cChanB_Bandwidth,   // КАНАЛ 2 - Полоса
    cChanB_Resistance,  // КАНАЛ 2 - Сопротивление
    cChanB_Inverse,     // КАНАЛ 2 - Инверсия
    cChanB_Divider,     // КАНАЛ 2 - Делитель
    bChanB_Balance      // КАНАЛ 2 - Балансировать
);

//--------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Вход ---
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
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Связь ---
DEF_CHOICE_3
(
    cChanB_Couple, pChanB,
    SET_COUPLE_B, FuncActive, OnChanged_ChanB_Couple, FuncDraw,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост", "AC",
    "Перем", "DC",
    "Земля", "Ground"
);

static void OnChanged_ChanB_Couple(bool)
{
    FPGA_SetModeCouple(B, SET_COUPLE_B);
}

//------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Полоса ---
static void OnChanged_ChanB_Bandwidth(bool)
{
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_2
(
    cChanB_Bandwidth, pChanB,
    SET_BANDWIDTH_B, FuncActive, OnChanged_ChanB_Bandwidth, FuncDraw,
    "Полоса", "Bandwidth",
    "",
    "",
    "Полная", "Full", 
    "20МГц", "20MHz"
);

//------------------------------------------------------------------------------------------------------------------------ КАНАЛ 2 - Сопротивление ---
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
    "Вх сопр", "Resistance",
    "", "",
    "1 МОм", "1 Mohm",
    "50 Ом", "50 Ohm"
);

//----------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Инверсия ---
static void OnChanged_ChanB_Inverse(bool)
{
    FPGA_SetRShift(B, SET_RSHIFT_B);
}

DEF_CHOICE_2
(
    cChanB_Inverse, pChanB,
    SET_INVERSE_B, FuncActive, OnChanged_ChanB_Inverse, FuncDraw,
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Делитель ---
DEF_CHOICE_2
(
    cChanB_Divider, pChanB,
    SET_DIVIDER_B, FuncActive, FuncChangedChoice, FuncDraw,
    "Делитель", "Divider",
    chanDividerRu,
    chanDividerEn,
    "Выкл", "Jff",
    "1/10", "1/10"
);

//------------------------------------------------------------------------------------------------------------------------ КАНАЛ 2 - Балансировать ---
static void OnPress_ChanB_Balance(void)
{
    FPGA_BalanceChannel(B);
}

DEF_BUTTON
(
    bChanB_Balance,
    "Балансировать", "Balance",
    "Балансировать канал",
    "Balancing channel",
    pChanB, EmptyFuncBV, OnPress_ChanB_Balance, EmptyFuncVII
);
