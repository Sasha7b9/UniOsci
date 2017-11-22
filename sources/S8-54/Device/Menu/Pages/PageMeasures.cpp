#include "defines.h"
#include "Menu/MenuItems.h"
#include "Utils/Measures.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include "Menu/Pages/Definition.h"
#include "Hardware/Sound.h"
#include "FPGA/fpgaExtensions.h"
#include "Utils/Dictionary.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int8 posActive = 0;                 ///< Позиция активного измерения (на котором курсор)
bool pageChoiceIsActive = false;    ///< Если true - раскрыта страница выбора измерения
int8 posOnPageChoice = 0;           ///< Позиция курсора на странице выбора измерения

extern const Page pMeasures;
extern const Page ppFreqMeter;
extern const Page ppTune;

//------------------------------------------------------------------------------------------------------------ ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер ---
static void OnChanged_FreqMeter_Enable(bool)
{
    FreqMeter_Init();
}

DEF_CHOICE_2
(
    cFreqMeter_Enable, ppFreqMeter,
    FREQ_METER_ENABLED, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "Частотомер", "Freq meter",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//--------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F ---
DEF_CHOICE_3
(
    cFreqMeter_TimeF, ppFreqMeter,
    FREQ_METER_TIMECOUNTING, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "Время счёта F", "Time calc F",
    "Позволяет выбрать точность измерения частоты - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of frequency - the more time, the accuracy more time of measurement is more",
    "100мс", "100ms",
    "1с", "1s",
    "10с", "10ms"
);

//--------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени ---
DEF_CHOICE_4
(
    cFreqMeter_FreqClc, ppFreqMeter,
    FREQ_METER_FREQ_CLC, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "Метки времени", "Timestamps",
    "Выбор частоты следования счётных импульсов",
    "Choice of frequency of following of calculating impulses",
    "100кГц", "10MHz",
    "1МГц", "200MHz",
    "10МГц", "10MHz",
    "100МГц", "100MHz"
);

//------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов ---
DEF_CHOICE_3
(
    cFreqMeter_NumPeriods, ppFreqMeter,
    FREQ_METER_NUM_PERIODS, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw,
    "Кол-во периодов", "Num periods",
    "Позволяет выбрать точность измерения периода - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of period - the more time, the accuracy more time of measurement is more",
    "1", "1",
    "10", "10",
    "100", "100"    
);

//------------------------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - Показывать ---
DEF_CHOICE_2
(
    cIsShow, pMeasures,
    SHOW_MEASURES, FuncActive, FuncChangedChoice, FuncDraw,
    "Показывать", "Show",
    "Выводить или не выводить измерения на экран",
    "Output or output measurements on screen",
    "Нет", "No",
    "Да", "Yes"
);

//------------------------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - Количество ---
static bool IsActive_Number(void)
{
    return SHOW_MEASURES;
}

DEF_CHOICE_7
(
    cNumber, pMeasures,
    NUM_MEASURES, IsActive_Number, FuncChangedChoice, FuncDraw,
    "Количество", "Number"
    ,
    "Устанавливает максимальное количество выводимых измерений:\n"
    "\"1\" - одно измерение\n"
    "\"2\" - два измерения\n"
    "\"1х5\" - 1 строка с пятью измерениями\n"
    "\"2х5\" - 2 строки с пятью измерениями в каждой\n"
    "\"3х5\" - 3 строки с пятью измерениями в каждой\n"
    "\"6x1\" - 6 строк по одному измерению в каждой\n"
    "\"6х2\" - 6 строк по два измерения в каждой"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n",
    "1", "1",
    "2", "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2"
);

//----------------------------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - Каналы ---
static bool IsActive_Channels(void)
{
    return SHOW_MEASURES;
}

DEF_CHOICE_3
(
    cChannels, pMeasures,
    SOURCE_MEASURE, IsActive_Channels, FuncChangedChoice, FuncDraw,
    "Каналы", "Channels",
    "По каким каналам выводить измерения",
    "Which channels to output measurement",
    "1", "1",
    "2", "2",
    "1 и 2", "1 and 2"
);

//-------------------------------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - Вид ---
static bool IsActive_Mode(void)
{
    return SHOW_MEASURES;
}

DEF_CHOICE_2
(
    cMode, pMeasures,
    set.meas_ModeViewSignals, IsActive_Mode, FuncChangedChoice, FuncDraw,
    "Вид", "View",
    "Уменьшать или нет зону вывода сигнала для исключения перекрытия его результами измерений",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "Как есть",    "As is",
    "Уменьшать",   "Reduce"
);

//------------------------------------------------------------------------------------------------------------------ ИЗМЕРЕНИЯ - НАСТРОИТЬ - Выход ---
DEF_SMALL_BUTTON
(
    bTune_Exit,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button to return to the previous menu",
    ppTune, FuncActive, OnPressSB_Exit, DrawSB_Exit
);

//---------------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Маркеры ---
static void OnPress_Tune_Markers(void)
{
    measures.ShortPressOnSmallButonMarker();
}

static void Draw_Tune_Markers(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x60');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bTune_Markers,
    "Маркер", "Marker",
    "Позволяет установить маркеры для визуального контроля измерений",
    "Allows to establish markers for visual control of measurements",
    ppTune, FuncActive, OnPress_Tune_Markers, Draw_Tune_Markers
);

//-------------------------------------------------------------------------------------------------------------- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Настройка ---
static void OnPress_Tune_Settings(void)
{
    pageChoiceIsActive = !pageChoiceIsActive;
    if (pageChoiceIsActive)
    {
        posOnPageChoice = (int8)MEASURE(posActive);
    }
}

static void Draw_Tune_Settings(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bTune_Settings,
    "Настройка", "Setup",
    "Позволяет выбрать необходимые измерения",
    "Allows to choose necessary measurements",
    ppTune, FuncActive, OnPress_Tune_Settings, Draw_Tune_Settings
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ИЗМЕРЕНИЯ - НАСТРОИТЬ ///
static bool IsActive_Tune(void)
{
    return SHOW_MEASURES;
}

static void OnRegSet_Tune(int angle)
{
    static const int8 step = 3;
    static int8 currentAngle = 0;
    currentAngle += (int8)angle;
    if (currentAngle < step && currentAngle > -step)
    {
        return;
    }
    if (pageChoiceIsActive)
    {
        posOnPageChoice += (int8)Sign(currentAngle);
        Sound_RegulatorSwitchRotate();
        if (posOnPageChoice < 0)
        {
            posOnPageChoice = Meas_NumMeasures - 1;
        }
        else if (posOnPageChoice == Meas_NumMeasures)
        {
            posOnPageChoice = 0;
        }
        MEASURE(posActive) = (Meas)posOnPageChoice;
        painter.ResetFlash();
    }
    else
    {
        int row = 0;
        int col = 0;
        measures.GetActive(&row, &col);
        col += Sign(currentAngle);
        if (col < 0)
        {
            col = measures.NumCols() - 1;
            row--;
            if (row < 0)
            {
                row = measures.NumRows() - 1;
            }
        }
        else if (col == measures.NumCols())
        {
            col = 0;
            row++;
            if (row >= measures.NumRows())
            {
                row = 0;
            }
        }
        measures.SetActive(row, col);
        Sound_RegulatorSwitchRotate();
    }
    currentAngle = 0;
}

DEF_PAGE_SB(    ppTune, static,
    PageSB_Measures_Tune, &pMeasures, IsActive_Tune,
    EmptyPressPage, EmptyDrawPage, OnRegSet_Tune,
    "НАСТРОИТЬ", "CONFIGURE",
    "Переход в режми точной настройки количества и видов измерений",
    "Transition to rezhm of exact control of quantity and types of measurements",
    &bTune_Exit,
    0,
    0,
    0,
    &bTune_Markers,
    &bTune_Settings
);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ИЗМЕРЕНИЯ - ЧАСТОТОМЕР ///
DEF_PAGE_4(     ppFreqMeter, static,
    Page_Service_FreqMeter, &pMeasures, FuncActive, EmptyPressPage,
    "ЧАСТОТОМЕР", "FREQ METER",
    "",
    "",
    cFreqMeter_Enable,     // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер
    cFreqMeter_TimeF,      // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F
    cFreqMeter_FreqClc,    // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени
    cFreqMeter_NumPeriods  // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ИЗМЕРЕНИЯ ///
DEF_PAGE_6(     pMeasures, ,
    Page_Measures, &mainPage, FuncActive, EmptyPressPage,
    "ИЗМЕРЕНИЯ", "MEASURES",
    "Автоматические измерения",
    "Automatic measurements",
    ppFreqMeter,   // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР
    cIsShow,       // ИЗМЕРЕНИЯ - Показывать
    cNumber,       // ИЗМЕРЕНИЯ - Количество
    cChannels,     // ИЗМЕРЕНИЯ - Каналы
    ppTune,        // ИЗМЕРЕНИЯ - НАСТРОИТЬ
    cMode          // ИЗМЕРЕНИЯ - Вид
);

const Page * pointerPageMeasures = &pMeasures;
