#include "defines.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageTrig
 *  @{
 */

extern const Page pTrig;
extern const Page mainPage;
extern const Page mpAutoFind;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPress_Mode(bool active)
{
    fpga.Stop(false);
    if (!START_MODE_IS_SINGLE)
    {
        fpga.Start();
    }
}

DEF_CHOICE_3(       mcMode, pTrig,                                                                                             //--- СИНХР - Режим ---
    START_MODE, FuncActive, OnPress_Mode, FuncDraw,
    "Режим", "Mode"
    ,
    "Задаёт режим запуска:\n"
    "1. \"Авто\" - запуск происходит автоматически.\n"
    "2. \"Ждущий\" - запуск происходит по уровню синхронизации.\n"
    "3. \"Однократный\" - запуск происходит по достижении уровня синхронизации один раз. Для следующего измерения нужно нажать кнопку ПУСК/СТОП."
    ,
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP."
    ,
    "Авто ",       "Auto",
    "Ждущий",      "Wait",
    "Однократный", "Single"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool active)
{
    fpga.SetTrigSource(TRIGSOURCE);
}

DEF_CHOICE_3(       mcSource,                                                                                               //--- СИНХР - Источник ---
    pTrig,
    TRIGSOURCE, FuncActive, OnChanged_Source, FuncDraw,
    "Источник", "Source",
    "Выбор источника сигнала синхронизации.",
    "Synchronization signal source choice.",
    "Канал 1", "Channel 1",
    "Канал 2", "Channel 2",
    "Внешний", "External"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool active)
{
    fpga.SetTrigPolarity(TRIG_POLARITY);
}

DEF_CHOICE_2(       mcPolarity,                                                                                           //--- СИНХР - Полярность ---
    pTrig,
    TRIG_POLARITY, FuncActive, OnChanged_Polarity, FuncDraw,
    "Полярность", "Polarity"
    ,
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса."
    ,
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut."
    ,
    "Фронт", "Front",
    "Срез",  "Back"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Input(bool active)
{
    fpga.SetTrigInput(TRIG_INPUT);
}

DEF_CHOICE_4(       mcInput,                                                                                                    //--- СИНХР - Вход ---
    pTrig,
    TRIG_INPUT, FuncActive, OnChanged_Input, FuncDraw,
    "Вход", "Input"
    ,
    "Выбор связи с источником синхронизации:\n"
    "1. \"ПС\" - полный сигнал.\n"
    "2. \"АС\" - закрытый вход.\n"
    "3. \"ФНЧ\" - фильтр низких частот.\n"
    "4. \"ФВЧ\" - фильтр высоких частот."
    ,
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"AS\" - a gated entrance.\n"
    "3. \"LPF\" - low-pass filter.\n"
    "4. \"HPF\" - high-pass filter frequency."
    ,
    "ПС",  "Full",
    "АС",  "AC",
    "ФНЧ", "LPF",
    "ФВЧ", "HPF"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcAutoFind_Mode,                                                                                   //--- СИНХР - ПОИСК - Режим ---
    mpAutoFind,
    TRIG_MODE_FIND, FuncActive, FuncChangedChoice, FuncDraw,
    "Режим", "Mode"
    ,
    "Выбор режима автоматического поиска синхронизации:\n"
    "1. \"Ручной\" - поиск производится по нажатию кнопки \"Найти\" или по удержанию в течение 0.5с кнопки СИНХР, если "
    "установлено \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\".\n"
    "2. \"Автоматический\" - поиск производится автоматически."
    ,
    "Selecting the automatic search of synchronization:\n"
    "1. \"Hand\" - search is run on pressing of the button \"Find\" or on deduction during 0.5s the СИНХР button if it is "
    "established \"SERVICE\x99Mode long СИНХР\x99 Autolevel\".\n"
    "2. \"Auto\" - the search is automatically."
    ,
    "Ручной",         "Hand",
    "Автоматический", "Auto"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_AutoFind_Search(void)
{
    return TRIG_MODE_FIND_IS_HAND;
}

static void OnPress_AutoFind_Search(void)
{
    fpga.FindAndSetTrigLevel();
}

DEF_BUTTON(         mbAutoFind_Search,                                                                                 //--- СИНХР - ПОИСК - Найти ---
    "Найти", "Search",
    "Производит поиск уровня синхронизации.",
    "Runs for search synchronization level.",
    mpAutoFind, IsActive_AutoFind_Search, OnPress_AutoFind_Search, FuncDraw
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         mpAutoFind,                                                                                                   // СИНХР - ПОИСК ///
    Page_TrigAuto, &pTrig, FuncActive, EmptyPressPage,
    "ПОИСК", "SEARCH",
    "Управление автоматическим поиском уровня синхронизации.",
    "Office of the automatic search the trigger level.",
    mcAutoFind_Mode,    // СИНХР - ПОИСК - Режим
    mbAutoFind_Search   // СИНХР - ПОИСК - Найти    
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pTrig,                                                                                                                // СИНХР ///
    Page_Trig, &mainPage, FuncActive, EmptyPressPage,
    "СИНХР", "TRIG",
    "Содержит настройки синхронизации.",
    "Contains synchronization settings.",
    mcMode,     // СИНХР - Режим
    mcSource,   // СИНХР - Источник
    mcPolarity, // СИНХР - Полярность
    mcInput,    // СИНХР - Вход
    mpAutoFind  // СИНХР - ПОИСК
);


/** @}  @}
 */
