#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern ColorType colorTypeGrid;

extern const           Page pDisplay;
extern const         Choice cViewMode;                          ///< ДИСПЛЕЙ - Отображение
extern const          Page ppAccum;                             ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ
static bool         IsActive_Accum(void);
extern const         Choice cAccum_Num;                         ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество
extern const         Choice cAccum_Mode;                        ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим
extern const         Button bAccum_Clear;                       ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить
static bool         IsActive_Accum_Clear(void);
       void          OnPress_Accum_Clear(void);
extern const          Page ppAverage;                           ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ
static bool         IsActive_Average(void);
extern const         Choice cAverage_Num;                       ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество
extern const         Choice cAverage_Mode;                      ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим
extern const         Choice cMinMax;                            ///< ДИСПЛЕЙ - Мин Макс
static bool         IsActive_MinMax(void);
static void        OnChanged_MinMax(bool active);
extern const         Choice cSmoothing;                         ///< ДИСПЛЕЙ - Сглаживание
extern const         Choice cRefreshFPS;                        ///< ДИСПЛЕЙ - Частота обновл
static void        OnChanged_RefreshFPS(bool active);
extern const          Page ppGrid;                              ///< ДИСПЛЕЙ - СЕТКА
extern const         Choice cGrid_Type;                         ///< ДИСПЛЕЙ - СЕТКА - Тип
extern const       Governor gGrid_Brightness;                   ///< ДИСПЛЕЙ - СЕТКА - Яркость
       void        OnChanged_Grid_Brightness(void);
static void       BeforeDraw_Grid_Brightness(void);
extern const         Choice cScaleYtype;                        ///< ДИСПЛЕЙ - Смещение
extern const          Page ppDisplaySettings;                          ///< ДИСПЛЕЙ - НАСТРОЙКИ
extern const         Page pppSettings_Colors;                   ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА
extern const         Choice cSettings_Colors_Scheme;            ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема
extern const GovernorColor gcSettings_Colors_ChannelA;          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1
extern const GovernorColor gcSettings_Colors_ChannelB;          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2
extern const GovernorColor gcSettings_Colors_Grid;              ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка
extern const         Choice cSettings_Colors_Background;        ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон
static void        OnChanged_Settings_Colors_Background(bool active);
       void PageService_InitGlobalColors(void);
extern const       Governor gSettings_Brightness;               ///< ДИСПЛЕЙ - НАСТРОЙКИ - Яркость
static void        OnChanged_Settings_Brightness(void);
extern const       Governor gSettings_Levels;                   ///< ДИСПЛЕЙ - НАСТРОЙКИ - Уровни
extern const       Governor gSettings_Time;                     ///< ДИСПЛЕЙ - НАСТРОЙКИ - Время
extern const         Choice cSettings_StringNavigation;         ///< ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню
extern const         Choice cSettings_AltMarkers;               ///< ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры
static void        OnChanged_Settings_AltMarkers(bool active);
extern const         Choice cSettings_AutoHide;                 ///< ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать
static void        OnChanged_Settings_AutoHide(bool active);

// ДИСПЛЕЙ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageDisplay_Init(void)
{
    OnChanged_Settings_Colors_Background(true);   // Заносим значения в гувернёры цветов
}

extern const Page mainPage;

DEF_PAGE_9(     pDisplay, ,
    Page_Display, &mainPage, FuncActive, EmptyPressPage,
    "ДИСПЛЕЙ", "DISPLAY",
    "Содержит настройки отображения дисплея.",
    "Contains settings of display of the display.",
    cViewMode,      // ДИСПЛЕЙ - Отображение
    ppAccum,        // ДИСПЛЕЙ - НАКОПЛЕНИЕ
    ppAverage,      // ДИСПЛЕЙ - УСРЕДНЕНИЕ
    cMinMax,        // ДИСПЛЕЙ - Мин Макс
    cSmoothing,     // ДИСПЛЕЙ - Сглаживание
    cRefreshFPS,    // ДИСПЛЕЙ - Частота обновл
    ppGrid,         // ДИСПЛЕЙ - СЕТКА
    cScaleYtype,    // ДИСПЛЕЙ - Смещение
    ppDisplaySettings      // ДИСПЛЕЙ - НАСТРОЙКИ
);

//-------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - Отображение ---
DEF_CHOICE_2
(
    cViewMode, pDisplay,
    MODE_DRAW_SIGNAL, FuncActive, FuncChangedChoice, FuncDraw,
    "Отображение", "View",
    "Задаёт режим отображения сигнала.",
    "Sets the display mode signal.",
    "Вектор",  "Vector",
    "Точки",   "Points"
);

//--------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАКОПЛЕНИЕ ---
static bool IsActive_Accum(void)
{
    return SET_TBASE > TBase_20ns;
}

DEF_PAGE_3(     ppAccum, static,
    Page_Display_Accum, &pDisplay, IsActive_Accum, EmptyPressPage,
    "НАКОПЛЕНИЕ", "ACCUMULATION",
    "Настройки режима отображения последних сигналов на экране.",
    "Mode setting signals to display the last screen.",
    cAccum_Num,     // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество
    cAccum_Mode,    // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим
    bAccum_Clear    // ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить
);

//-------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество ---
DEF_CHOICE_REG_9
(
    cAccum_Num, ppAccum,
    ENUM_ACCUM, FuncActive, FuncChangedChoice, FuncDraw,
    "Количество", "Number"
    ,
    "Задаёт максимальное количество последних сигналов на экране. Если в настройке \"Режим\" выбрано \"Бесконечность\", экран очищается только "
    "нажатием кнопки \"Очистить\"."
    "\"Бесконечность\" - каждое измерение остаётся на дисплее до тех пор, пока не будет нажата кнопка \"Очистить\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed.",
    DISABLE_RU, DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128",
    "Бесконечность", "Infinity"
);

//------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим ---
DEF_CHOICE_2
(
    cAccum_Mode, ppAccum,
    MODE_ACCUM, FuncActive, FuncChangedChoice, FuncDraw,
    "Режим", "Mode"
    ,
    "1. \"Сбрасывать\" - после накопления заданного количества измерения происходит очистка дисплея. Этот режим удобен, когда памяти не хватает "
    "для сохранения нужного количества измерений.\n"
    "2. \"Не сбрасывать\" - на дисплей всегда выводится заданное или меньшее (в случае нехватки памяти) количество измерений. Недостатком является "
    "меньшее быстродействие и невозможность накопления заданного количества измерений при недостатке памяти."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the display. This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory.",    
    "Не сбрасывать",   "Not to dump",
    "Сбрасывать",      "Dump"
);

//---------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить ---
static bool IsActive_Accum_Clear(void)
{
    return ENUM_ACCUM != ENumAccum_1 && !MODE_ACCUM_NO_RESET;
}

void OnPress_Accum_Clear(void)
{
    NEED_FINISH_DRAW = 1;
}

DEF_BUTTON
(
    bAccum_Clear,
    "Очистить", "Clear",
    "Очищает экран от накопленных сигналов.",
    "Clears the screen of the saved-up signals.",
    ppAccum, IsActive_Accum_Clear, OnPress_Accum_Clear, FuncDraw
);

//--------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - УСРЕДНЕНИЕ ---
static bool IsActive_Average(void)
{
    return true;
}

DEF_PAGE_2(     ppAverage, static,
    Page_Display_Average, &pDisplay, IsActive_Average, EmptyPressPage,
    "УСРЕДНЕНИЕ", "AVERAGE",
    "Настройки режима усреднения по последним измерениям.",
    "Settings of the mode of averaging on the last measurements.",
    cAverage_Num,   // ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество
    cAverage_Mode   // ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим
);

//-------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество ---
DEF_CHOICE_REG_10
(
    cAverage_Num, ppAverage,
    ENUM_AVE, FuncActive, FuncChangedChoice, FuncDraw,
    "Количество", "Number",
    "Задаёт количество последних измерений, по которым производится усреднение.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU, DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128",
    "256", "256",
    "512", "512"    
);

//------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим ---
DEF_CHOICE_2
(
    cAverage_Mode, ppAverage,
    MODE_AVERAGING, FuncActive, FuncChangedChoice, FuncDraw,
    "Режим", "Mode"
    ,
    "1. \"Точно\" - точный режим усреднения, когда в расчёте участвуют только последние сигналы.\n"
    "2. \"Приблизительно\" - приблизительный режим усреднения. Имеет смысл использовать, когда задано количество измерений большее, чем может "
    "поместиться в памяти.",
    
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory.",
    
    "Точно",           "Accurately",
    "Приблизительно",  "Around"    
);

//----------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - Мин Макс ---
static bool IsActive_MinMax(void)
{
    return !IN_RANDOM_MODE && !IN_P2P_MODE;
}

static void OnChanged_MinMax(bool)
{
    /*
    int maxMeasures = DS_NumberAvailableEntries();  
    int numMinMax = sDisplay_NumberMinMax();

    if (maxMeasures < numMinMax)
    {
        display.ShowWarningWithNumber(ExcessValues, maxMeasures);
    }
    */
}

DEF_CHOICE_8
(
    cMinMax, pDisplay,
    ENUM_MIN_MAX, IsActive_MinMax, OnChanged_MinMax, FuncDraw,
    "Мин Макс", "Min Max",
    "Задаёт количество последних измерений, по которым строятся ограничительные линии, огибающие минимумы и максимумы измерений.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under "
    "construction.",

    DISABLE_RU,DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128"
);

//-------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - Сглаживание ---
DEF_CHOICE_REG_10
(
    cSmoothing, pDisplay,
    ENUM_SMOOTHING, FuncActive, FuncChangedChoice, FuncDraw,
    "Сглаживание", "Smoothing",
    "Устанавливает количество точек для расчёта сглаженного по соседним точкам сигнала.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    
    DISABLE_RU, DISABLE_EN,
    "2 точки", "2 points",
    "3 точки", "3 points",
    "4 точки", "4 points",
    "5 точек", "5 points",
    "6 точек", "6 points",
    "7 точек", "7 points",
    "8 точек", "8 points",
    "9 точек", "9 points",
    "10 точек", "10 points"
);

//----------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - Частота обновл ---
static void OnChanged_RefreshFPS(bool)
{
    FPGA_SetENumSignalsInSec(NUM_SIGNALS_IN_SEC);
}

DEF_CHOICE_5
(
    cRefreshFPS, pDisplay,
    ENUM_SIGNALS_IN_SEC, FuncActive, OnChanged_RefreshFPS, FuncDraw,
    "Частота обновл", "Refresh rate",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "Sets the maximum number of the shots removed in a second.",
    "25",  "25",
    "10",  "10",
    "5",   "5",
    "2",   "2",
    "1",   "1"
);

//-------------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - СЕТКА ---
DEF_PAGE_2(     ppGrid, static,
    Page_Display_Grid, &pDisplay, FuncActive, EmptyPressPage,
    "СЕТКА", "GRID",
    "Содержит настройки отображения координатной сетки.",
    "Contains settings of display of a coordinate grid.",
    cGrid_Type,         // ДИСПЛЕЙ - СЕТКА - Тип
    gGrid_Brightness    // ДИСПЛЕЙ - СЕТКА - Яркость
);

//-------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - СЕТКА - Тип ---
DEF_CHOICE_4
(
    cGrid_Type, ppGrid,
    TYPE_GRID, FuncActive, FuncChangedChoice, FuncDraw,
    "Тип", "Type",
    "Выбор типа сетки.",
    "Choice like grid.",
     "Тип 1", "Type 1",
     "Тип 2", "Type 2",
     "Тип 3", "Type 3",
     "Тип 4", "Type 4"
);

//---------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - СЕТКА - Яркость ---
void OnChanged_Grid_Brightness(void)
{
    colorTypeGrid.SetBrightness(BRIGHTNESS_GRID / 100.0f);
}


static void BeforeDraw_Grid_Brightness(void)
{
    colorTypeGrid.Init(false);
    BRIGHTNESS_GRID = (int16)(colorTypeGrid.brightness * 100.0f);
}

DEF_GOVERNOR
(
    gGrid_Brightness,
    "Яркость", "Brightness",
    "Устанавливает яркость сетки.",
    "Adjust the brightness of the grid.",
    ppGrid, BRIGHTNESS_GRID, 0, 100, FuncActive, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
);

//----------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - Смещение ---
DEF_CHOICE_2
(
    cScaleYtype, pDisplay,
    LINKING_RSHIFT, FuncActive, FuncChangedChoice, FuncDraw,
    "Смещение", "Оffset",
    "Задаёт режим удержания смещения по вертикали\n1. \"Напряжение\" - сохраняется наряжение смещения.\n2. \"Деления\" - сохраняется положение "
    "смещения на экране.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of "
    "the offset on the screen.",
    "Напряжение", "Voltage",
    "Деления", "Divisions"
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ДИСПЛЕЙ - НАСТРОЙКИ ///
DEF_PAGE_7(         ppDisplaySettings,
    static,
    Page_Display_Settings, &pDisplay, FuncActive, EmptyPressPage,
    "НАСТРОЙКИ", "SETTINGS",
    "Дополнительные настройки дисплея",
    "Additional display settings",
    pppSettings_Colors,         // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА
    gSettings_Brightness,       // ДИСПЛЕЙ - НАСТРОЙКИ - Яркость
    gSettings_Levels,           // ДИСПЛЕЙ - НАСТРОЙКИ - Уровни
    gSettings_Time,             // ДИСПЛЕЙ - НАСТРОЙКИ - Время
    cSettings_StringNavigation, // ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню
    cSettings_AltMarkers,       // ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры
    cSettings_AutoHide          // ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать,
);

//-------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА ---
/// \todo Добавить дополнительные цвета 1-го и 2-го каналов
DEF_PAGE_5(     pppSettings_Colors, static,
    Page_Display_Settings_Colors, &ppDisplaySettings, FuncActive, EmptyPressPage,
    "ЦВЕТА", "COLORS",
    "Выбор цветов дисплея",
    "The choice of colors display",
    cSettings_Colors_Scheme,    // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема
    gcSettings_Colors_ChannelA, // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1
    gcSettings_Colors_ChannelB, // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2
    gcSettings_Colors_Grid,     // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка
    cSettings_Colors_Background // ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон
    //&mgcColorChannelAalt,
    //&mgcColorChannelBalt,
    //&mgcColorMenu1,
    //&mgcColorMenu2,
    //&mgcColorMenu3
    //&mgcColorMathem,
    //&mgcColorFFT,
    //&mcServDisplInverse
);

//--------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема ---
DEF_CHOICE_2
(
    cSettings_Colors_Scheme, pppSettings_Colors,
    set.serv_ColorScheme, FuncActive, FuncChangedChoice, FuncDraw,
    "Цветовая схема", "Color scheme",
    "Режим работы калибратора",
    "Mode of operation of the calibrator",
    "Схема 1", "Scheme 1",
    "Схема 2", "Scheme 2"
);

// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1 -------------------------------------------------------------------------------------------------------------
static ColorType colorT1 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::DATA_A};
static const GovernorColor gcSettings_Colors_ChannelA =
{
    Item_GovernorColor, &pppSettings_Colors, 0,
    {
        "Канал 1", "Channel 1",
        "",
        ""
    },
    &colorT1
};

// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2 -------------------------------------------------------------------------------------------------------------
static ColorType colorT2 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::DATA_B};
static const GovernorColor gcSettings_Colors_ChannelB =
{
    Item_GovernorColor, &pppSettings_Colors, 0,
    {
        "Канал 2", "Channel 2",
        "",
        ""
    },
    &colorT2
};

// ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка ---------------------------------------------------------------------------------------------------------------
ColorType colorTypeGrid = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::GRID};
static const GovernorColor gcSettings_Colors_Grid =
{
    Item_GovernorColor, &pppSettings_Colors, 0,
    {
        "Сетка", "Grid",
        "Устанавливает цвет сетки",
        "Sets the grid color"
    },
    &colorTypeGrid
};

//-------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон ---
static void OnChanged_Settings_Colors_Background(bool)
{
    PageService_InitGlobalColors();

    gcSettings_Colors_ChannelA.ct->color = gColorChan[A];
    gcSettings_Colors_ChannelB.ct->color = gColorChan[B];
    gcSettings_Colors_Grid.ct->color = gColorGrid;

    gcSettings_Colors_ChannelA.ct->Init(true);
    gcSettings_Colors_ChannelB.ct->Init(true);
    gcSettings_Colors_Grid.ct->Init(true);
}

void PageService_InitGlobalColors(void)
{
    gColorBack = BACKGROUND_BLACK ? Color::BLACK : Color::WHITE;
    gColorFill = BACKGROUND_BLACK ? Color::WHITE : Color::BLACK;
    gColorGrid = BACKGROUND_BLACK ? Color::GRID : Color::GRID_WHITE;
    gColorChan[A] = BACKGROUND_BLACK ? Color::DATA_A : Color::DATA_WHITE_ACCUM_A;
    gColorChan[B] = BACKGROUND_BLACK ? Color::DATA_B : Color::DATA_WHITE_ACCUM_B;
    gColorChan[A_B] = gColorChan[MathCh] = BACKGROUND_BLACK ? Color::WHITE : Color::BLACK;
}

DEF_CHOICE_2
(
    cSettings_Colors_Background, pppSettings_Colors,
    BACKGROUND, FuncActive, OnChanged_Settings_Colors_Background, FuncDraw,
    "Фон", "Background",
    "Выбор цвета фона",
    "Choice of color of a background",
    "Чёрный", "Black",
    "Белый", "White"
);

//------------------------------------------------------------------------------------------------------------------ ДИСПЛЕЙ - НАСТРОЙКИ - Яркость ---
static void OnChanged_Settings_Brightness(void)
{
    painter.SetBrightnessDisplay(BRIGHTNESS_DISPLAY);
}

DEF_GOVERNOR
(
    gSettings_Brightness,
    "Яркость", "Brightness",
    "Установка яркости свечения дисплея",
    "Setting the brightness of the display",
    ppDisplaySettings, BRIGHTNESS_DISPLAY, 0, 100,
    FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - Уровни ---
DEF_GOVERNOR
(
    gSettings_Levels,
    "Уровни", "Levels",
    "Задаёт время, в течение которого после поворота ручки сещения напряжения на экране остаётся вспомогательная метка уровня смещения",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    ppDisplaySettings, TIME_SHOW_LEVELS, 0, 125,
    FuncActive, FuncChanged, FuncBeforeDraw
);

//-------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - Время ---
DEF_GOVERNOR
(
    gSettings_Time,
    "Время", "Time",
    "Установка времени, в течение которого сообщения будут находиться на экране",
    "Set the time during which the message will be on the screen",
    ppDisplaySettings, TIME_MESSAGES, 1, 99,
    FuncActive, FuncChanged, FuncBeforeDraw
);

//-------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню ---
DEF_CHOICE_3
(
    cSettings_StringNavigation, ppDisplaySettings,
    SHOW_STRING_NAVI, FuncActive, FuncChangedChoice, FuncDraw,
    "Строка меню", "Path menu",
    "При выборе \nПоказывать\n слева вверху экрана выводится полный путь до текущей страницы меню", /// \todo Исправить перевод
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "Временно", "Temporary",    /// \todo Исправить перевод
    "Всегда", "All",
    "Никогда", "None"
);

//------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры ---
static void OnChanged_Settings_AltMarkers(bool active)
{
    display.ChangedRShiftMarkers(active);
}

DEF_CHOICE_3
(
    cSettings_AltMarkers, ppDisplaySettings,
    ALT_MARKERS, FuncActive, OnChanged_Settings_AltMarkers, FuncDraw,
    "Доп. маркеры", "Alt. markers",
    "Устанавливает режим отображения дополнительных маркеров уровней смещения и синхронизации:\n"
    "\"Скрывать\" - дополнительные маркеры не показываются,\n"
    "\"Показывать\" - дополнительные маркеры показываются всегда,\n"
    "\"Авто\" - дополнительные маркеры показываются в течение 5 сек после поворота ручки смещения канала по напряжению или уровня синхронизации",
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level",    
    
    "Скрывать", "Hide",
    "Показывать", "Show",
    "Авто", "Auto"
);

//----------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать ---
static void OnChanged_Settings_AutoHide(bool autoHide)
{
    menu.SetAutoHide(autoHide);
}

DEF_CHOICE_6
(
    cSettings_AutoHide, ppDisplaySettings,
    MENU_AUTO_HIDE, FuncActive, OnChanged_Settings_AutoHide, FuncDraw,
    "Скрывать", "Hide",
    "Установка после последнего нажатия кнопки или поворота ручки, по истечении которого меню автоматически убирается с экрана",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "Никогда",      "Never",
    "Через 5 сек",  "Through 5 s",
    "Через 10 сек", "Through 10 s",
    "Через 15 сек", "Through 15 s",
    "Через 30 сек", "Through 30 s",
    "Через 60 сек", "Through 60 s"
);

/*
//----------------------------------------------------------------------------------------------------------------------------------------------------
// СЕРВИС - ДИСПЛЕЙ - ЦВЕТА - МЕНЮ Область
static ColorType colorT6 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::MENU_FIELD };
const GovernorColor mgcColorMenu3 = { Item_GovernorColor, &pppSettings_Colors, { "Меню Область", "Menu Field" },
{
    "",
    "",
},
0, &colorT6
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
// СЕРВИС - ДИСПЛЕЙ - ЦВЕТА - МЕНЮ Заголовок
static ColorType colorT7 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, COLOR_MENU_TITLE };
const GovernorColor mgcColorMenu1 = { Item_GovernorColor, &pppSettings_Colors, { "Mеню Заголовок", "Menu Title" },
{
    "",
    ""
},
0, &colorT7, OnChangedColorMenuTitle
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
// СЕРВИС - ДИСПЛЕЙ - ЦВЕТА - МЕНЮ Пункт
static ColorType colorT8 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, COLOR_MENU_ITEM };
const GovernorColor mgcColorMenu2 = { Item_GovernorColor, &pppSettings_Colors, { "Меню Пункт", "Menu Item" },
{
    "",
    ""
},
0, &colorT8, OnChangedColorMenu2Item
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
// СЕРВИС - ДИСПЛЕЙ - ЦВЕТА - Инверсия
const Choice mcServDisplInverse =
{
    Item_Choice, &pppSettings_Colors, { "Инверсия", "Inverse" },
    {
        "",
        ""
    },
    0,
    { { DISABLE_RU, DISABLE_EN },
    { ENABLE_RU, ENABLE_EN },
    },
    0
};
*/
