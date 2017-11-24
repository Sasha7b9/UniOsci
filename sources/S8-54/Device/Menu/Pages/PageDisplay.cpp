#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern ColorType colorTypeGrid;

extern const           Page pDisplay;
extern const         Choice cViewMode;                          ///< ������� - �����������
extern const          Page ppAccum;                             ///< ������� - ����������
static bool         IsActive_Accum(void);
extern const         Choice cAccum_Num;                         ///< ������� - ���������� - ����������
extern const         Choice cAccum_Mode;                        ///< ������� - ���������� - �����
extern const         Button bAccum_Clear;                       ///< ������� - ���������� - ��������
static bool         IsActive_Accum_Clear(void);
       void          OnPress_Accum_Clear(void);
extern const          Page ppAverage;                           ///< ������� - ����������
static bool         IsActive_Average(void);
extern const         Choice cAverage_Num;                       ///< ������� - ���������� - ����������
extern const         Choice cAverage_Mode;                      ///< ������� - ���������� - �����
extern const         Choice cMinMax;                            ///< ������� - ��� ����
static bool         IsActive_MinMax(void);
static void        OnChanged_MinMax(bool active);
extern const         Choice cSmoothing;                         ///< ������� - �����������
extern const         Choice cRefreshFPS;                        ///< ������� - ������� ������
static void        OnChanged_RefreshFPS(bool active);
extern const          Page ppGrid;                              ///< ������� - �����
extern const         Choice cGrid_Type;                         ///< ������� - ����� - ���
extern const       Governor gGrid_Brightness;                   ///< ������� - ����� - �������
       void        OnChanged_Grid_Brightness(void);
static void       BeforeDraw_Grid_Brightness(void);
extern const         Choice cScaleYtype;                        ///< ������� - ��������
extern const          Page ppDisplaySettings;                          ///< ������� - ���������
extern const         Page pppSettings_Colors;                   ///< ������� - ��������� - �����
extern const         Choice cSettings_Colors_Scheme;            ///< ������� - ��������� - ����� - �������� �����
extern const GovernorColor gcSettings_Colors_ChannelA;          ///< ������� - ��������� - ����� - ����� 1
extern const GovernorColor gcSettings_Colors_ChannelB;          ///< ������� - ��������� - ����� - ����� 2
extern const GovernorColor gcSettings_Colors_Grid;              ///< ������� - ��������� - ����� - �����
extern const         Choice cSettings_Colors_Background;        ///< ������� - ��������� - ����� - ���
static void        OnChanged_Settings_Colors_Background(bool active);
       void PageService_InitGlobalColors(void);
extern const       Governor gSettings_Brightness;               ///< ������� - ��������� - �������
static void        OnChanged_Settings_Brightness(void);
extern const       Governor gSettings_Levels;                   ///< ������� - ��������� - ������
extern const       Governor gSettings_Time;                     ///< ������� - ��������� - �����
extern const         Choice cSettings_StringNavigation;         ///< ������� - ��������� - ������ ����
extern const         Choice cSettings_AltMarkers;               ///< ������� - ��������� - ���. �������
static void        OnChanged_Settings_AltMarkers(bool active);
extern const         Choice cSettings_AutoHide;                 ///< ������� - ��������� - ��������
static void        OnChanged_Settings_AutoHide(bool active);

// ������� ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageDisplay_Init(void)
{
    OnChanged_Settings_Colors_Background(true);   // ������� �������� � �������� ������
}

extern const Page mainPage;

DEF_PAGE_9(     pDisplay, ,
    Page_Display, &mainPage, FuncActive, EmptyPressPage,
    "�������", "DISPLAY",
    "�������� ��������� ����������� �������.",
    "Contains settings of display of the display.",
    cViewMode,      // ������� - �����������
    ppAccum,        // ������� - ����������
    ppAverage,      // ������� - ����������
    cMinMax,        // ������� - ��� ����
    cSmoothing,     // ������� - �����������
    cRefreshFPS,    // ������� - ������� ������
    ppGrid,         // ������� - �����
    cScaleYtype,    // ������� - ��������
    ppDisplaySettings      // ������� - ���������
);

//-------------------------------------------------------------------------------------------------------------------------- ������� - ����������� ---
DEF_CHOICE_2
(
    cViewMode, pDisplay,
    MODE_DRAW_SIGNAL, FuncActive, FuncChangedChoice, FuncDraw,
    "�����������", "View",
    "����� ����� ����������� �������.",
    "Sets the display mode signal.",
    "������",  "Vector",
    "�����",   "Points"
);

//--------------------------------------------------------------------------------------------------------------------------- ������� - ���������� ---
static bool IsActive_Accum(void)
{
    return SET_TBASE > TBase_20ns;
}

DEF_PAGE_3(     ppAccum, static,
    Page_Display_Accum, &pDisplay, IsActive_Accum, EmptyPressPage,
    "����������", "ACCUMULATION",
    "��������� ������ ����������� ��������� �������� �� ������.",
    "Mode setting signals to display the last screen.",
    cAccum_Num,     // ������� - ���������� - ����������
    cAccum_Mode,    // ������� - ���������� - �����
    bAccum_Clear    // ������� - ���������� - ��������
);

//-------------------------------------------------------------------------------------------------------------- ������� - ���������� - ���������� ---
DEF_CHOICE_REG_9
(
    cAccum_Num, ppAccum,
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
    DISABLE_RU, DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128",
    "�������������", "Infinity"
);

//------------------------------------------------------------------------------------------------------------------- ������� - ���������� - ����� ---
DEF_CHOICE_2
(
    cAccum_Mode, ppAccum,
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
    "�� ����������",   "Not to dump",
    "����������",      "Dump"
);

//---------------------------------------------------------------------------------------------------------------- ������� - ���������� - �������� ---
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
    "��������", "Clear",
    "������� ����� �� ����������� ��������.",
    "Clears the screen of the saved-up signals.",
    ppAccum, IsActive_Accum_Clear, OnPress_Accum_Clear, FuncDraw
);

//--------------------------------------------------------------------------------------------------------------------------- ������� - ���������� ---
static bool IsActive_Average(void)
{
    return true;
}

DEF_PAGE_2(     ppAverage, static,
    Page_Display_Average, &pDisplay, IsActive_Average, EmptyPressPage,
    "����������", "AVERAGE",
    "��������� ������ ���������� �� ��������� ����������.",
    "Settings of the mode of averaging on the last measurements.",
    cAverage_Num,   // ������� - ���������� - ����������
    cAverage_Mode   // ������� - ���������� - �����
);

//-------------------------------------------------------------------------------------------------------------- ������� - ���������� - ���������� ---
DEF_CHOICE_REG_10
(
    cAverage_Num, ppAverage,
    ENUM_AVE, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Number",
    "����� ���������� ��������� ���������, �� ������� ������������ ����������.",
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

//------------------------------------------------------------------------------------------------------------------- ������� - ���������� - ����� ---
DEF_CHOICE_2
(
    cAverage_Mode, ppAverage,
    MODE_AVERAGING, FuncActive, FuncChangedChoice, FuncDraw,
    "�����", "Mode"
    ,
    "1. \"�����\" - ������ ����� ����������, ����� � ������� ��������� ������ ��������� �������.\n"
    "2. \"��������������\" - ��������������� ����� ����������. ����� ����� ������������, ����� ������ ���������� ��������� �������, ��� ����� "
    "����������� � ������.",
    
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory.",
    
    "�����",           "Accurately",
    "��������������",  "Around"    
);

//----------------------------------------------------------------------------------------------------------------------------- ������� - ��� ���� ---
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
    "��� ����", "Min Max",
    "����� ���������� ��������� ���������, �� ������� �������� ��������������� �����, ��������� �������� � ��������� ���������.",
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

//-------------------------------------------------------------------------------------------------------------------------- ������� - ����������� ---
DEF_CHOICE_REG_10
(
    cSmoothing, pDisplay,
    ENUM_SMOOTHING, FuncActive, FuncChangedChoice, FuncDraw,
    "�����������", "Smoothing",
    "������������� ���������� ����� ��� ������� ����������� �� �������� ������ �������.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points.",
    
    DISABLE_RU, DISABLE_EN,
    "2 �����", "2 points",
    "3 �����", "3 points",
    "4 �����", "4 points",
    "5 �����", "5 points",
    "6 �����", "6 points",
    "7 �����", "7 points",
    "8 �����", "8 points",
    "9 �����", "9 points",
    "10 �����", "10 points"
);

//----------------------------------------------------------------------------------------------------------------------- ������� - ������� ������ ---
static void OnChanged_RefreshFPS(bool)
{
    FPGA_SetENumSignalsInSec(NUM_SIGNALS_IN_SEC);
}

DEF_CHOICE_5
(
    cRefreshFPS, pDisplay,
    ENUM_SIGNALS_IN_SEC, FuncActive, OnChanged_RefreshFPS, FuncDraw,
    "������� ������", "Refresh rate",
    "����� ������������ ����� ��������� � ������� ������.",
    "Sets the maximum number of the shots removed in a second.",
    "25",  "25",
    "10",  "10",
    "5",   "5",
    "2",   "2",
    "1",   "1"
);

//-------------------------------------------------------------------------------------------------------------------------------- ������� - ����� ---
DEF_PAGE_2(     ppGrid, static,
    Page_Display_Grid, &pDisplay, FuncActive, EmptyPressPage,
    "�����", "GRID",
    "�������� ��������� ����������� ������������ �����.",
    "Contains settings of display of a coordinate grid.",
    cGrid_Type,         // ������� - ����� - ���
    gGrid_Brightness    // ������� - ����� - �������
);

//-------------------------------------------------------------------------------------------------------------------------- ������� - ����� - ��� ---
DEF_CHOICE_4
(
    cGrid_Type, ppGrid,
    TYPE_GRID, FuncActive, FuncChangedChoice, FuncDraw,
    "���", "Type",
    "����� ���� �����.",
    "Choice like grid.",
     "��� 1", "Type 1",
     "��� 2", "Type 2",
     "��� 3", "Type 3",
     "��� 4", "Type 4"
);

//---------------------------------------------------------------------------------------------------------------------- ������� - ����� - ������� ---
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
    "�������", "Brightness",
    "������������� ������� �����.",
    "Adjust the brightness of the grid.",
    ppGrid, BRIGHTNESS_GRID, 0, 100, FuncActive, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
);

//----------------------------------------------------------------------------------------------------------------------------- ������� - �������� ---
DEF_CHOICE_2
(
    cScaleYtype, pDisplay,
    LINKING_RSHIFT, FuncActive, FuncChangedChoice, FuncDraw,
    "��������", "�ffset",
    "����� ����� ��������� �������� �� ���������\n1. \"����������\" - ����������� ��������� ��������.\n2. \"�������\" - ����������� ��������� "
    "�������� �� ������.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of "
    "the offset on the screen.",
    "����������", "Voltage",
    "�������", "Divisions"
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ��������� ///
DEF_PAGE_7(         ppDisplaySettings,
    static,
    Page_Display_Settings, &pDisplay, FuncActive, EmptyPressPage,
    "���������", "SETTINGS",
    "�������������� ��������� �������",
    "Additional display settings",
    pppSettings_Colors,         // ������� - ��������� - �����
    gSettings_Brightness,       // ������� - ��������� - �������
    gSettings_Levels,           // ������� - ��������� - ������
    gSettings_Time,             // ������� - ��������� - �����
    cSettings_StringNavigation, // ������� - ��������� - ������ ����
    cSettings_AltMarkers,       // ������� - ��������� - ���. �������
    cSettings_AutoHide          // ������� - ��������� - ��������,
);

//-------------------------------------------------------------------------------------------------------------------- ������� - ��������� - ����� ---
/// \todo �������� �������������� ����� 1-�� � 2-�� �������
DEF_PAGE_5(     pppSettings_Colors, static,
    Page_Display_Settings_Colors, &ppDisplaySettings, FuncActive, EmptyPressPage,
    "�����", "COLORS",
    "����� ������ �������",
    "The choice of colors display",
    cSettings_Colors_Scheme,    // ������� - ��������� - ����� - �������� �����
    gcSettings_Colors_ChannelA, // ������� - ��������� - ����� - ����� 1
    gcSettings_Colors_ChannelB, // ������� - ��������� - ����� - ����� 2
    gcSettings_Colors_Grid,     // ������� - ��������� - ����� - �����
    cSettings_Colors_Background // ������� - ��������� - ����� - ���
    //&mgcColorChannelAalt,
    //&mgcColorChannelBalt,
    //&mgcColorMenu1,
    //&mgcColorMenu2,
    //&mgcColorMenu3
    //&mgcColorMathem,
    //&mgcColorFFT,
    //&mcServDisplInverse
);

//--------------------------------------------------------------------------------------------------- ������� - ��������� - ����� - �������� ����� ---
DEF_CHOICE_2
(
    cSettings_Colors_Scheme, pppSettings_Colors,
    set.serv_ColorScheme, FuncActive, FuncChangedChoice, FuncDraw,
    "�������� �����", "Color scheme",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "����� 1", "Scheme 1",
    "����� 2", "Scheme 2"
);

// ������� - ��������� - ����� - ����� 1 -------------------------------------------------------------------------------------------------------------
static ColorType colorT1 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::DATA_A};
static const GovernorColor gcSettings_Colors_ChannelA =
{
    Item_GovernorColor, &pppSettings_Colors, 0,
    {
        "����� 1", "Channel 1",
        "",
        ""
    },
    &colorT1
};

// ������� - ��������� - ����� - ����� 2 -------------------------------------------------------------------------------------------------------------
static ColorType colorT2 = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::DATA_B};
static const GovernorColor gcSettings_Colors_ChannelB =
{
    Item_GovernorColor, &pppSettings_Colors, 0,
    {
        "����� 2", "Channel 2",
        "",
        ""
    },
    &colorT2
};

// ������� - ��������� - ����� - ����� ---------------------------------------------------------------------------------------------------------------
ColorType colorTypeGrid = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::GRID};
static const GovernorColor gcSettings_Colors_Grid =
{
    Item_GovernorColor, &pppSettings_Colors, 0,
    {
        "�����", "Grid",
        "������������� ���� �����",
        "Sets the grid color"
    },
    &colorTypeGrid
};

//-------------------------------------------------------------------------------------------------------------- ������� - ��������� - ����� - ��� ---
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
    "���", "Background",
    "����� ����� ����",
    "Choice of color of a background",
    "׸����", "Black",
    "�����", "White"
);

//------------------------------------------------------------------------------------------------------------------ ������� - ��������� - ������� ---
static void OnChanged_Settings_Brightness(void)
{
    painter.SetBrightnessDisplay(BRIGHTNESS_DISPLAY);
}

DEF_GOVERNOR
(
    gSettings_Brightness,
    "�������", "Brightness",
    "��������� ������� �������� �������",
    "Setting the brightness of the display",
    ppDisplaySettings, BRIGHTNESS_DISPLAY, 0, 100,
    FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------------- ������� - ��������� - ������ ---
DEF_GOVERNOR
(
    gSettings_Levels,
    "������", "Levels",
    "����� �����, � ������� �������� ����� �������� ����� ������� ���������� �� ������ ������� ��������������� ����� ������ ��������",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    ppDisplaySettings, TIME_SHOW_LEVELS, 0, 125,
    FuncActive, FuncChanged, FuncBeforeDraw
);

//-------------------------------------------------------------------------------------------------------------------- ������� - ��������� - ����� ---
DEF_GOVERNOR
(
    gSettings_Time,
    "�����", "Time",
    "��������� �������, � ������� �������� ��������� ����� ���������� �� ������",
    "Set the time during which the message will be on the screen",
    ppDisplaySettings, TIME_MESSAGES, 1, 99,
    FuncActive, FuncChanged, FuncBeforeDraw
);

//-------------------------------------------------------------------------------------------------------------- ������� - ��������� - ������ ���� ---
DEF_CHOICE_3
(
    cSettings_StringNavigation, ppDisplaySettings,
    SHOW_STRING_NAVI, FuncActive, FuncChangedChoice, FuncDraw,
    "������ ����", "Path menu",
    "��� ������ \n����������\n ����� ������ ������ ��������� ������ ���� �� ������� �������� ����", /// \todo ��������� �������
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "��������", "Temporary",    /// \todo ��������� �������
    "������", "All",
    "�������", "None"
);

//------------------------------------------------------------------------------------------------------------- ������� - ��������� - ���. ������� ---
static void OnChanged_Settings_AltMarkers(bool active)
{
    display.ChangedRShiftMarkers(active);
}

DEF_CHOICE_3
(
    cSettings_AltMarkers, ppDisplaySettings,
    ALT_MARKERS, FuncActive, OnChanged_Settings_AltMarkers, FuncDraw,
    "���. �������", "Alt. markers",
    "������������� ����� ����������� �������������� �������� ������� �������� � �������������:\n"
    "\"��������\" - �������������� ������� �� ������������,\n"
    "\"����������\" - �������������� ������� ������������ ������,\n"
    "\"����\" - �������������� ������� ������������ � ������� 5 ��� ����� �������� ����� �������� ������ �� ���������� ��� ������ �������������",
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level",    
    
    "��������", "Hide",
    "����������", "Show",
    "����", "Auto"
);

//----------------------------------------------------------------------------------------------------------------- ������� - ��������� - �������� ---
static void OnChanged_Settings_AutoHide(bool autoHide)
{
    menu.SetAutoHide(autoHide);
}

DEF_CHOICE_6
(
    cSettings_AutoHide, ppDisplaySettings,
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

/*
//----------------------------------------------------------------------------------------------------------------------------------------------------
// ������ - ������� - ����� - ���� �������
static ColorType colorT6 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::MENU_FIELD };
const GovernorColor mgcColorMenu3 = { Item_GovernorColor, &pppSettings_Colors, { "���� �������", "Menu Field" },
{
    "",
    "",
},
0, &colorT6
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
// ������ - ������� - ����� - ���� ���������
static ColorType colorT7 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, COLOR_MENU_TITLE };
const GovernorColor mgcColorMenu1 = { Item_GovernorColor, &pppSettings_Colors, { "M��� ���������", "Menu Title" },
{
    "",
    ""
},
0, &colorT7, OnChangedColorMenuTitle
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
// ������ - ������� - ����� - ���� �����
static ColorType colorT8 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, COLOR_MENU_ITEM };
const GovernorColor mgcColorMenu2 = { Item_GovernorColor, &pppSettings_Colors, { "���� �����", "Menu Item" },
{
    "",
    ""
},
0, &colorT8, OnChangedColorMenu2Item
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
// ������ - ������� - ����� - ��������
const Choice mcServDisplInverse =
{
    Item_Choice, &pppSettings_Colors, { "��������", "Inverse" },
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
