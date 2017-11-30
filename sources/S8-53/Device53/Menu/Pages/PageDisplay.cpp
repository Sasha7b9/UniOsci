#include "Menu/MenuItems.h"
#include "defines.h"
#include "Menu/Menu.h"
#include "Tables.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "FPGA/DataStorage.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"


/** @addtogroup Menu
 *  @{
 *  @defgroup PageDisplay
 *  @{
 */

extern const Page mainPage;
extern const Page pDisplay;
extern const Page mspAccumulation;
extern const Page mspAveraging;
extern const Page mspGrid;
extern const Page mspSettings_Colors;
extern const Page mspSettings;

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMapping, pDisplay,                                                                               //--- ������� - ����������� ---
    MODE_DRAW_SIGNAL, FuncActive, FuncChangedChoice, FuncDraw,
    "�����������", "View",
    "����� ����� ����������� �������.",
    "Sets the display mode signal.",
    "������", "Vector",
    "�����",  "Points"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_9(   mcAccumulation_Number,                                                                    // ������� - ���������� - ���������� ---
    mspAccumulation,
    ENUM_ACCUM, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Number"
    ,
    "����� ������������ ���������� ��������� �������� �� ������. ���� � ��������� \"�����\" ������� \"�������������\", ����� ��������� ������ "
    "�������� ������ \"��������\"."
    "\"�������������\" - ������ ��������� ������� �� ������� �� ��� ���, ���� �� ����� ������ ������ \"��������\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed.",
    DISABLE_RU,      DISABLE_EN,
    "2",             "2",
    "4",             "4",
    "8",             "8",
    "16",            "16",
    "32",            "32",
    "64",            "64",
    "128",           "128",
    "�������������", "Infinity"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcAccumulation_Mode,                                                                           // ������� - ���������� - ����� ---
    mspAccumulation,
    MODE_ACCUM, FuncActive, FuncChangedChoice, FuncDraw,
    "�����", "Mode"
    ,
    "1. \"����������\" - ����� ���������� ��������� ���������� ��������� ���������� ������� �������. ���� ����� ������, ����� ������ �� ������� "
    "��� ���������� ������� ���������� ���������.\n"
    "2. \"�� ����������\" - �� ������� ������ ��������� �������� ��� ������� (� ������ �������� ������) ���������� ���������. ����������� �������� "
    "������� �������������� � ������������� ���������� ��������� ���������� ��������� ��� ���������� ������."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the display. This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory.",
    "�� ����������", "Not to dump",
    "����������",    "Dump"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accumulation_Clear(void)
{
    return ENUM_ACCUM_IS_INFINITY;
}

void OnPress_Accumulation_Clear(void)
{
    display.Redraw();
}

DEF_BUTTON(mcAccumulation_Clear,                                                                       // ������� - ���������� - �������� ///
           "��������", "Clear",
           "������� ����� �� ����������� ��������.",
           "Clears the screen of the saved-up signals.",
           mspAccumulation, IsActive_Accumulation_Clear, OnPress_Accumulation_Clear, FuncDraw
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accumulation(void)
{
    return SET_TBASE > TBase_50ns;
}

DEF_PAGE_3(         mspAccumulation,                                                                                       // ������� - ���������� ///
    Page_DisplayAccumulation, &pDisplay, IsActive_Accumulation, EmptyPressPage,
    "����������", "ACCUMULATION",
    "��������� ������ ����������� ��������� �������� �� ������.",
    "Mode setting signals to display the last screen.",
    mcAccumulation_Number, // ������� - ���������� - ����������
    mcAccumulation_Mode,   // ������� - ���������� - �����
    mcAccumulation_Clear   // ������� - ���������� - ��������
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(  mcAveraging_Number,                                                                       // ������� - ���������� - ���������� ---
    mspAveraging, ENUM_AVE, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Number",
    "����� ���������� ��������� ���������, �� ������� ������������ ����������.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU, DISABLE_EN,
    "2",        "2",
    "4",        "4",
    "8",        "8",
    "16",       "16",
    "32",       "32",
    "64",       "64",
    "128",      "128",
    "256",      "256",
    "512",      "512"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcAveraging_Mode,                                                                              // ������� - ���������� - ����� ---
    mspAveraging, MODE_AVE, FuncActive, FuncChangedChoice, FuncDraw,
    "�����", "Mode"
    ,
    "1. \"�����\" - ������ ����� ����������, ����� � ������� ��������� ������ ��������� �������.\n"
    "2. \"��������������\" - ��������������� ����� ����������. ����� ����� ������������, ����� ������ ���������� ��������� �������, ��� ����� "
    "����������� � ������."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory.",
    "�����",          "Accurately",
    "��������������", "Around"
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Averaging(void)
{
    return true;
}

DEF_PAGE_2(         mspAveraging,                                                                                          // ������� - ���������� ///
    Page_DisplayAverage, &pDisplay, IsActive_Averaging, EmptyPressPage,
    "����������", "AVERAGE",
    "��������� ������ ���������� �� ��������� ����������.",
    "Settings of the mode of averaging on the last measurements.",
    mcAveraging_Number, // ������� - ���������� - ����������
    mcAveraging_Mode    // ������� - ���������� - �����
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_MinMax(void)
{
    return SET_TBASE > TBase_50ns;
}

static void OnChanged_MinMax(bool active)
{
}

DEF_CHOICE_8(       mcMinMax,                                                                                                // ������� - ��� ���� ---
    pDisplay, ENUM_MIN_MAX, IsActive_MinMax, OnChanged_MinMax, FuncDraw,
    "��� ����", "Min Max"
    ,
    "����� ���������� ��������� ���������, �� ������� �������� ��������������� �����, ��������� �������� � ��������� ���������."
    ,
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under ",
    DISABLE_RU, DISABLE_EN,
    "2",        "2",
    "4",        "4",
    "8",        "8",
    "16",       "16",
    "32",       "32",
    "64",       "64",
    "128",      "128"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(  mcSmoothing,                                                                                          // ������� - ����������� ---
    pDisplay,  SMOOTHING, FuncActive, FuncChangedChoice, FuncDraw,
    "�����������", "Smoothing",
    "������������� ���������� ����� ��� ������� ����������� �� �������� ������ �������.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    DISABLE_RU, DISABLE_EN,
    "2 �����",  "2 points",
    "3 �����",  "3 points",
    "4 �����",  "4 points",
    "5 �����",  "5 points",
    "6 �����",  "6 points",
    "7 �����",  "7 points",
    "8 �����",  "8 points",
    "9 �����",  "9 points",
    "10 �����", "10 points"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_RefreshFPS(bool active)
{
    fpga.SetNumSignalsInSec(sDisplay_NumSignalsInS());
}


DEF_CHOICE_5(       mcRefreshFPS,                                                                                      // ������� - ������� ������ ---
    pDisplay, ENUM_SIGNALS_IN_SEC, FuncActive, OnChanged_RefreshFPS, FuncDraw,
    "������� ������", "Refresh rate",
    "����� ������������ ����� ��������� � ������� ������.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5",  "5",
    "2",  "2",
    "1",  "1"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       mcGrid_Type,                                                                                          // ������� - ����� - ��� ---
    mspGrid, TYPE_GRID, FuncActive, FuncChangedChoice, FuncDraw,
    "���", "Type",
    "����� ���� �����.",
    "Choice like grid.",
    "��� 1", "Type 1",
    "��� 2", "Type 2",
    "��� 3", "Type 3",
    "��� 4", "Type 4"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
ColorType colorTypeGrid = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::Grid()};

void OnChanged_Grid_Brightness(void)
{
    colorTypeGrid.SetBrightness(BRIGHTNESS_GRID / 1e2f);
}

static void BeforeDraw_Grid_Brightness(void)
{
    colorTypeGrid.Init(false);
    BRIGHTNESS_GRID = (int16)(colorTypeGrid.brightness * 100.0f);
}

DEF_GOVERNOR(       mgGrid_Brightness,                                                                                // ������� - ����� - ������� ---
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the grid.",
    mspGrid, BRIGHTNESS_GRID, 0, 100,
    FuncActive, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         mspGrid,                                                                                                    // ������� - ����� ///
    Page_DisplayGrid, &pDisplay, FuncActive, EmptyPressPage,
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate grid.",
    mcGrid_Type,      // ������� - ����� - ���
    mgGrid_Brightness // ������� - ����� - �������
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcTypeShift,                                                                                             // ������� - �������� ---
    pDisplay, LINKING_RSHIFT, FuncActive, FuncChangedChoice, FuncDraw,
    "��������", "Offset"
    ,
    "����� ����� ��������� �������� �� ���������\n1. \"����������\" - ����������� ���������� ��������.\n2. \"�������\" - ����������� ��������� "
    "�������� �� ������."
    ,
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of "
    "the offset on the screen.",
    "����������", "Voltage",
    "�������",    "Divisions"
);

// ������� - ��������� - ����� - �������� ����� ------------------------------------------------------------------------------------------------------
DEF_CHOICE_2
(
    mcSettings_Colors_Scheme, mspSettings_Colors,
    COLOR_SCHEME, FuncActive, FuncChangedChoice, FuncDraw,
    "�������� �����", "Color scheme",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2"
);

//---------------------------------------------------------------------------------------------------------- ������� - ��������� - ����� - ����� 1 ---
static ColorType colorT1 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::DATA_A};

static const GovernorColor mgcSettings_Colors_ChannelA =
{
    Item_GovernorColor, &mspSettings_Colors, 0,
    {
        "����� 1", "Channel 1", "",  ""
    },
    &colorT1
};

//---------------------------------------------------------------------------------------------------------- ������� - ��������� - ����� - ����� 2 ---
static ColorType colorT2 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::DATA_B};

static const GovernorColor mgcSettings_Colors_ChannelB =
{
    Item_GovernorColor, &mspSettings_Colors, 0,
    {
        "����� 2", "Channel 2", "", ""
    },
    &colorT2
};

//------------------------------------------------------------------------------------------------------------ ������� - ��������� - ����� - ����� ---
static const GovernorColor mgcSettings_Colors_Grid =
{
    Item_GovernorColor, &mspSettings_Colors, 0,
    {
        "�����", "Grid",
        "������������� ���� �����",
        "Sets the grid color"
    },
    &colorTypeGrid
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4(         mspSettings_Colors,                                                                             // ������� - ��������� - ����� ///
    Page_ServiceDisplayColors, &mspSettings, FuncActive, EmptyPressPage,
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    mcSettings_Colors_Scheme,    // ������� - ��������� - ����� - �������� �����
    mgcSettings_Colors_ChannelA, // ������� - ��������� - ����� - ����� 1
    mgcSettings_Colors_ChannelB, // ������� - ��������� - ����� - ����� 2
    mgcSettings_Colors_Grid      // ������� - ��������� - ����� - �����
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_Brightness(void)
{
    painter.SetBrightnessDisplay(BRIGHTNESS);
}

DEF_GOVERNOR(       mgSettings_Brightness,                                                                        // ������� - ��������� - ������� ---
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    mspSettings, BRIGHTNESS, 0, 100,
    FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mgSettings_Levels,                                                                             // ������� - ��������� - ������ ---
    "������", "Levels",
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� ������ ��������",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    mspSettings, TIME_SHOW_LEVELS, 0, 125,
    FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mgSettings_TimeMessages,                                                                        // ������� - ��������� - ����� ---
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    mspSettings, TIME_MESSAGES, 1, 99,
    FuncActive, FuncChanged, FuncBeforeDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcSettings_ShowStringNavigation,                                                          // ������� - ��������� - ������ ���� ---
    mspSettings, SHOW_STRING_NAVIGATION, FuncActive, FuncChangedChoice, FuncDraw,
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����",
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������",   "Hide",
    "����������", "Show"
);

// ������� - ��������� - ���. ������� ----------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_ShowAltMarkers(bool)
{
    display.ChangedRShiftMarkers();
}

DEF_CHOICE_3
(
    mcSettings_ShowAltMarkers,
    mspSettings,
    ALT_MARKERS, FuncActive, OnChanged_Settings_ShowAltMarkers, FuncDraw,
    "���. �������", "Alt. markers"
    ,
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� ��� ������ �������������"
    ,
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level",
    "��������",   "Hide",
    "����������", "Show",
    "����",       "Auto"
);

// ������� - ��������� - �������� --------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AutoHide(bool autoHide)
{
    menu.SetAutoHide(autoHide);
}

DEF_CHOICE_6
(
    mcSettings_AutoHide, mspSettings,
    MENU_AUTO_HIDE, FuncActive, OnChanged_Settings_AutoHide, FuncDraw,
    "��������", "Hide",
    "��������� ����� ���������� ������� ������ ��� �������� �����, �� ��������� �������� ���� ������������� ��������� � ������",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "�������",      "Never",
    "����� 5 ���",  "Through 5 s",
    "����� 10 ���", "Through 10 s",
    "����� 15 ���", "Through 15 s",
    "����� 30 ���", "Through 30 s",
    "����� 60 ���", "Through 60 s"
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         mspSettings,                                                                                            // ������� - ��������� ///
    Page_ServiceDisplay, &pDisplay, FuncActive, EmptyPressPage,
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    mspSettings_Colors,              // ������� - ��������� - �����
    mgSettings_Brightness,           // ������� - ��������� - �������
    mgSettings_Levels,               // ������� - ��������� - ������
    mgSettings_TimeMessages,         // ������� - ��������� - �����
    mcSettings_ShowStringNavigation, // ������� - ��������� - ������ ����
    mcSettings_ShowAltMarkers,       // ������� - ��������� - ���. �������
    mcSettings_AutoHide              // ������� - ��������� - ��������
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_9(         pDisplay,                                                                                                           // ������� ///
    Page_Display, &mainPage, FuncActive, EmptyPressPage,
    "�������", "DISPLAY",
    "�������� ��������� ����������� �������.",
    "Contains settings of display of the display.",
    mcMapping,       // ������� - �����������
    mspAccumulation, // ������� - ����������
    mspAveraging,    // ������� - ����������
    mcMinMax,        // ������� - ��� ����
    mcSmoothing,     // ������� - �����������
    mcRefreshFPS,    // ������� - ������� ������
    mspGrid,         // ������� - �����
    mcTypeShift,     // ������� - ��������
    mspSettings      // ������� - ���������
);


/** @}  @}
 */
