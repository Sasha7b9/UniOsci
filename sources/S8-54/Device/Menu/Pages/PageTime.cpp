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
extern const  Choice cSample;               ///< РАЗВЕРТКА - Выборка
static bool  IsActive_Sample(void);
extern const  Choice cPeakDet;              ///< РАЗВЕРТКА - Пик дет
static bool  IsActive_PeakDet(void);
       void OnChanged_PeakDet(bool active);
extern const  Choice cTPos;                 ///< РАЗВЕРТКА - То
       void OnChanged_TPos(bool active);
extern const  Choice cDivRole;              ///< РАЗВЕРТКА - Ф-ция ВР/ДЕЛ
extern const  Choice cShiftXtype;           ///< РАЗВЕРТКА - Смещение

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// РАЗВЕРТКА ///
DEF_PAGE_5(     pTime, ,
    Page_Time, &mainPage, FuncActive, EmptyPressPage,
    "РАЗВЕРТКА", "SCAN",
    "Содержит настройки развёртки.",
    "Contains scan settings.",
    cSample,    // РАЗВЕРТКА - Выборка
    cPeakDet,   // РАЗВЕРТКА - Пик дет
    cTPos,      // РАЗВЕРТКА - То
    cDivRole,   // РАЗВЕРТКА - Ф-ция ВР/ДЕЛ
    cShiftXtype // РАЗВЕРТКА - Смещение
);

const Page * pointerPageTime = &pTime;

//---------------------------------------------------------------------------------------------------------------------------- РАЗВЕРТКА - Выборка ---
static bool IsActive_Sample(void)
{
    return IN_RANDOM_MODE && !START_MODE_SINGLE;
}

DEF_CHOICE_2
(
    cSample, pTime,
    SAMPLE_TYPE, IsActive_Sample, FuncChangedChoice, FuncDraw,
    "Выборка", "Sampling",
    "\"Реальная\" - \n"
    "\"Эквивалентная\" -",
    "\"Real\" - \n"
    "\"Equals\" - ",

    "Реальное время", "Real",
    "Эквивалентная",  "Equals"
);

//---------------------------------------------------------------------------------------------------------------------------- РАЗВЕРТКА - Пик дет ---
static bool IsActive_PeakDet(void)
{
    if (FPGA_POINTS_32k)                       // При 32к точек на канал мы не можем включать пиковый детектор
    {
        return false;
    }

    if (FPGA_POINTS_16k && SET_ENABLED_B) // При 16к точках на канал мы можем работать только с одним каналом
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
    "Пик дет", "Pic deat",
    "Включает/выключает пиковый детектор.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------------------------------------- РАЗВЕРТКА - То ---
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
    "Задаёт точку привязки нулевого смещения по времени к экрану - левый край, центр, правый край.", 
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "Лево",  "Left",
    "Центр", "Center",
    "Право", "Right"
);

//----------------------------------------------------------------------------------------------------------------------- РАЗВЕРТКА - Ф-ция ВР/ДЕЛ ---
DEF_CHOICE_2
(
    cDivRole, pTime,
    TIME_DIVXPOS, FuncActive, FuncChangedChoice, FuncDraw,
    "Ф-ция ВР/ДЕЛ", "Funct Time/DIV",
    "Задаёт функцию для ручки ВРЕМЯ/ДЕЛ: в режиме сбора информации (ПУСК/СТОП в положении ПУСК):\n"
    "1. \"Время\" - изменение смещения по времени.\n"
    "2. \"Память\" - перемещение по памяти.",
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory.",
    "Время",  "Time",
    "Память", "Memory"
);

//--------------------------------------------------------------------------------------------------------------------------- РАЗВЕРТКА - Смещение ---
DEF_CHOICE_2
(
    cShiftXtype, pTime,
    LINKING_TSHIFT, FuncActive, FuncChangedChoice, FuncDraw,
    "Смещение", "Offset",
    "Задаёт режим удержания смещения по горизонтали\n1. \"Время\" - сохраняется абсолютное смещение в секундах.\n2. \"Деления\" - сохраняется "
    "положение мещения на экране.",
    "Sets the mode of retaining the horizontal displacement\n1. \"Time\" - saved the asbolutic offset in seconds.\n2. \"Divisions\" - retained "
    "the position of the offset on the screen.",
    "Время",   "Time",
    "Деления", "Divisions"
);
