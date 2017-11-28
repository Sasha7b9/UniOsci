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
extern const char chanInputRu[] =   "1. \"Вкл\" - выводить сигнал на экран.\n"
                                    "2. \"Откл\" - не выводить сигнал на экран.";
extern const char chanInputEn[] =   "1. \"Enable\" - signal output to the screen.\n"
                                    "2. \"Disable\" - no output to the screen.";

extern const char chanCoupleRu[] =  "Задаёт вид связи с источником сигнала.\n"
                                    "1. \"Пост\" - открытый вход.\n"
                                    "2. \"Перем\" - закрытый вход.\n"
                                    "3. \"Земля\" - вход соединён с землёй.";
extern const char chanCoupleEn[] =  "Sets a type of communication with a signal source.\n"
                                    "1. \"AC\" - open input.\n"
                                    "2. \"DC\" - closed input.\n"
                                    "3. \"Ground\" - input is connected to the ground.";

extern const char chanFiltrRu[] = "Включает/отключает фильтр для ограничения полосы пропускания.";
extern const char chanFiltrEn[] = "Includes/switches-off the filter for restriction of a pass-band.";

extern const char chanInverseRu[] = "При \"Вкл\" сигнал на экране будет симметрично отражён относительно U = 0В.";
extern const char chanInverseEn[] = "When \"Enable\" signal on the screen will be reflected symmetrically with respect to U = 0V.";

extern const char chanMultiplierRu[] = "Ослабление сигнала:\n\"x1\" - сигнал не ослабляется.\n\"x10\" - сигнал ослабляется в 10 раз";
extern const char chanMultiplierEn[] = "Attenuation: \n\"x1\" - the signal is not attenuated.\n\"x10\" - the signal is attenuated by 10 times";

//--------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Вход ---
void OnChanged_InputA(bool active)
{
    panel.EnableLEDChannelA(sChannel_Enabled(A));
}

DEF_CHOICE_2
(
    mcInputA, pChanA,
    SET_ENABLED_A, FuncActive, OnChanged_InputA, FuncDraw,
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Связь ---
void OnChanged_CoupleA(bool active)
{
    fpga.SetModeCouple(A, SET_COUPLE_A);
}

DEF_CHOICE_3
(
    mcCoupleA, pChanA,
    SET_COUPLE_A, FuncActive, OnChanged_CoupleA, FuncDraw,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "AC",
    "Перем", "DC",
    "Земля", "Ground"
);

//------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Фильтр ---
void OnChanged_FiltrA(bool active)
{
    fpga.EnableChannelFiltr(A, FILTR_A);
}

DEF_CHOICE_2
(
    mcFiltrA, pChanA,
    FILTR_A, FuncActive, OnChanged_FiltrA, FuncDraw,
    "Фильтр", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Инверсия ---
static void OnChanged_InverseA(bool active)
{
    fpga.SetRShift(A, SET_RSHIFT_A);
}

DEF_CHOICE_2
(
    mcInverseA, pChanA,
    SET_INVERSE_A, FuncActive, OnChanged_InverseA, FuncDraw,
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//---------------------------------------------------------------------------------------------------------------------------- КАНАЛ 1 - Множитель ---
DEF_CHOICE_2
(
    mcMultiplierA, pChanA,
    SET_DIVIDER(A), FuncActive, FuncChangedChoice, FuncDraw,
    "Множитель", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "х1",  "x1",
    "x10", "x10"
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// КАНАЛ 1 ///
extern const Page mainPage;

DEF_PAGE_5
(
    pChanA, ,
    Page_ChannelA, &mainPage, FuncActive, EmptyPressPage,
    "КАНАЛ 1", "CHANNEL 1",
    "Содержит настройки канала 1.",
    "Contains settings of the channel 1.",
    mcInputA,       // КАНАЛ 1 - Вход
    mcCoupleA,      // КАНАЛ 1 - Связь
    mcFiltrA,       // КАНАЛ 1 - Фильтр
    mcInverseA,     // КАНАЛ 1 - Инверсия
    mcMultiplierA   // КАНАЛ 1 - Множитель
);

//--------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Вход ---
void OnChanged_InputB(bool active)
{
    panel.EnableLEDChannelB(sChannel_Enabled(B));
}

DEF_CHOICE_2
(
    mcInputB, pChanB,
    SET_ENABLED_B, FuncActive, OnChanged_InputB, FuncDraw,
    "Вход", "Input",
    chanInputRu,
    chanInputEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Связь ---
void OnChanged_CoupleB(bool active)
{
    fpga.SetModeCouple(B, SET_COUPLE_B);
}

DEF_CHOICE_3
(
    mcCoupleB, pChanB,
    SET_COUPLE_B, FuncActive, OnChanged_CoupleB, FuncDraw,
    "Связь", "Couple",
    chanCoupleRu,
    chanCoupleEn,
    "Пост",  "AC",
    "Перем", "DC",
    "Земля", "Ground"
);

//------------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Фильтр ---
void OnChanged_FiltrB(bool active)
{
    fpga.EnableChannelFiltr(B, FILTR_B);
}

DEF_CHOICE_2
(
    mcFiltrB, pChanB,
    FILTR_B, FuncActive, OnChanged_FiltrB, FuncDraw,
    "Фильтр", "Filtr",
    chanFiltrRu,
    chanFiltrEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Инверсия ---
static void OnChanged_InverseB(bool active)
{
    fpga.SetRShift(B, SET_RSHIFT_B);
}

DEF_CHOICE_2
(
    mcInverseB, pChanB,
    SET_INVERSE_B, FuncActive, OnChanged_InverseB, FuncDraw,
    "Инверсия", "Inverse",
    chanInverseRu,
    chanInverseEn,
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//---------------------------------------------------------------------------------------------------------------------------- КАНАЛ 2 - Множитель ---
DEF_CHOICE_2
(
    mcMultiplierB, pChanB,
    SET_DIVIDER(B), FuncActive, FuncChangedChoice, FuncDraw,
    "Множитель", "Divider",
    chanMultiplierRu,
    chanMultiplierEn,
    "х1",  "x1",
    "x10", "x10"
);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// КАНАЛ 2 ///
DEF_PAGE_5
(
    pChanB, ,
    Page_ChannelB, &mainPage, FuncActive, EmptyPressPage,
    "КАНАЛ 2", "CHANNEL 2",
    "Содержит настройки канала 2.",
    "Contains settings of the channel 2.",
    mcInputB,       // КАНАЛ 2 - Вход
    mcCoupleB,      // КАНАЛ 2 - Связь
    mcFiltrB,       // КАНАЛ 2 - Фильтр
    mcInverseB,     // КАНАЛ 2 - Инверсия
    mcMultiplierB   // КАНАЛ 2 - Множитель
);


/** @}  @}
 */
