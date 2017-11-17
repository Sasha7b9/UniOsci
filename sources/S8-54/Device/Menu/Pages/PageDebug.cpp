#include "Log.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/FLASH.h"
#include "Hardware/Sound.h"
#include "Menu/MenuDrawing.h"
#include "Menu/MenuFunctions.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsDebug.h"
#include "Utils/Debug.h"
#include "Utils/Dictionary.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/CommonFunctions.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void LoadTShift(void);
extern const      Page pDebug;
extern const     Page ppConsole;                            ///< ������� - �������
extern const  Governor gConsole_NumStrings;                 ///< ������� - ������� - ������ ������
extern const    Choice cConsole_SizeFont;                   ///< ������� - ������� - ������ ������
extern const    Choice cConsole_ModeStop;                   ///< ������� - ������� - ���. ��������
extern const    Page pppConsole_Registers;                  ///< ������� - ������� - ��������
extern const    Choice cConsole_Registers_ShowAll;          ///< ������� - ������� - �������� - ���������� ���
extern const    Choice cConsole_Registers_RD_FL;            ///< ������� - ������� - �������� - RD_FL
static bool    IsActive_Console_Registers(void);
extern const    Choice cConsole_Registers_RShiftA;          ///< ������� - ������� - �������� - U ��. 1�
extern const    Choice cConsole_Registers_RShiftB;          ///< ������� - ������� - �������� - U ��. 2�
extern const    Choice cConsole_Registers_TrigLev;          ///< ������� - ������� - �������� - U �����.
extern const    Choice cConsole_Registers_RangeA;           ///< ������� - ������� - �������� - �����/��� 1
extern const    Choice cConsole_Registers_RangeB;           ///< ������� - ������� - �������� - �����/��� 2
extern const    Choice cConsole_Registers_TrigParam;        ///< ������� - ������� - �������� - �����. �����.
extern const    Choice cConsole_Registers_ChanParamA;       ///< ������� - ������� - �������� - �����. ���. 2
extern const    Choice cConsole_Registers_ChanParamB;       ///< ������� - ������� - �������� - �����. ���. 2
extern const    Choice cConsole_Registers_TBase;            ///< ������� - ������� - �������� - �����/���
extern const    Choice cConsole_Registers_TShift;           ///< ������� - ������� - �������� - � ��.
extern const    Button bConsole_SizeSettings;               ///< ������� - ������� - ������ ��������
static void        Draw_Console_SizeSettings(int x, int y); 
extern const     Page ppADC;                                ///< ������� - ���
extern const    Page pppADC_Balance;                        ///< ������� - ��� - ������
extern const    Choice cADC_Balance_Mode;                   ///< ������� - ��� - ������ - �����
static void   OnChanged_ADC_Balance_Mode(bool active);
static void        Draw_ADC_Balance_Mode(int x, int y);
extern const  Governor gADC_Balance_ShiftA;                 ///< ������� - ��� - ������ - �������� 1
static bool    IsActive_ADC_Balance_ShiftAB(void);
static void   OnChanged_ADC_Balance_ShiftA(void);
extern const  Governor gADC_Balance_ShiftB;                 ///< ������� - ��� - ������ - �������� 2 
static void   OnChanged_ADC_Balance_ShiftB(void);
extern const    Page pppADC_Stretch;                        ///< ������� - ��� - ��������
extern const    Choice cADC_Stretch_Mode;                   ///< ������� - ��� - �������� - �����
       void   OnChanged_ADC_Stretch_Mode(bool active);
extern const  Governor gADC_Stretch_A;                      ///< ������� - ��� - �������� - �������� 1�
static bool    IsActive_ADC_StretchAB(void);
static void   OnChanged_ADC_Stretch_A(void);
extern const  Governor gADC_Stretch_B;                      ///< ������� - ��� - �������� - �������� 2�
static void   OnChanged_ADC_Stretch_B(void);
extern const  Governor gADC_Stretch_Ak20mV;                 ///< ������� - ��� - �������� - 20��/1� 1�
extern const  Governor gADC_Stretch_Ak50mV;                 ///< ������� - ��� - �������� - 50�� 1� 
extern const  Governor gADC_Stretch_Ak100mV;                ///< ������� - ��� - �������� - 100��/5� 1�
extern const  Governor gADC_Stretch_Ak2V;                   ///< ������� - A�� - �������� - 2� 1�
extern const  Governor gADC_Stretch_Bk20mV;                 ///< ������� - ��� - �������� - 20��/1� 2�
extern const  Governor gADC_Stretch_Bk50mV;                 ///< ������� - ��� - �������� - 50�� 2� 
extern const  Governor gADC_Stretch_Bk100mV;                ///< ������� - ��� - �������� - 100��/5� 2�
extern const  Governor gADC_Stretch_Bk2V;                   ///< ������� - ��� - �������� - 2� 2�
extern const    Page pppADC_Shift;                          ///< ������� - ��� - ��� ����
extern const    Button bADC_Shift_Reset;                    ///< ������� - ��� - ��� ���� - �����
static void     OnPress_ADC_Shift_Reset(void);
extern const  Governor gADC_Shift_A2mV;                     ///< ������� - ��� - ��� ���� - �� 1� 2�� ����
static void   OnChanged_ADC_Shift_A(void);
extern const  Governor gADC_Shift_B2mV;                     ///< ������� - ��� - ��� ���� - �� 2� 2�� ����
static void   OnChanged_ADC_Shift_B(void);
extern const  Governor gADC_Shift_A5mV;                     ///< ������� - ��� - ��� ���� - �� 1� 5�� ����
extern const  Governor gADC_Shift_B5mV;                     ///< ������� - ��� - ��� ���� - �� 2� 5�� ����
extern const  Governor gADC_Shift_A10mV;                    ///< ������� - ��� - ��� ���� - �� 1� 10�� ����
extern const  Governor gADC_Shift_B10mV;                    ///< ������� - ��� - ��� ���� - �� 2� 10�� ����
extern const     Page ppRand;                               ///< ������� - ����-���
extern const  Governor gRand_NumMeasures;                   ///< ������� - ����-��� - ���-�/������
static void   OnChanged_Rand_NumMeasures(void);
extern const  Governor gRand_NumAverage;                    ///< ������� - ����-��� - ������.
extern const  Governor gRand_NumSmooth;                     ///< ������� - ����-��� - �����������
extern const    Choice cRand_ShowInfo;                      ///< ������� - ����-��� - ����������
extern const    Choice gRand_ShowStat;                      ///< ������� - ����-��� - ����������
extern const  Governor gRand_TimeCompensation;              ///< ������� - ����-��� - ����������� ��������
static void   OnChanged_Rand_TimeCompensation(void);
extern const  Governor gRand_AddTimeShift;                  ///< ������� - ����-��� - ��������
static void   OnChanged_Rand_AddTimeShift(void);
extern const  Governor gRand_Pretriggered;                  ///< ������� - ����-��� - ����������
static void   OnChanged_Rand_Pretriggered(void);
extern const     Page ppChannels;                           ///< ������� - ������
extern const    Choice cChannels_BandwidthA;                ///< ������� - ������ - ������ 1
static void   OnChanged_Channels_BandwidthA(bool active);
extern const    Choice cChannels_BandwidthB;                ///< ������� - ������ - ������ 2
static void   OnChanged_Channels_BandwidthB(bool active);
extern const    Choice cStats;                              ///< ������� - ����������
extern const    Choice cDisplayOrientation;                 ///< ������� - ����������
       void   OnChanged_DisplayOrientation(bool);
extern const    Choice cEMS;                                ///< ������� - ����� ���
static void   OnChanged_EMS(bool);
extern const Governor mgPred;                               ///< ������� - ����������
static void   OnChanged_Pred(void);
extern const Governor mgPost;                               ///< ������� - �����������
static void   OnChanged_Post(void);
extern const     Page ppSettings;                           ///< ������� - ���������
static void     OnPress_Settings(void);
extern const   SButton bSettings_Exit;                      ///< ������� - ��������� - �����
static void     OnPress_Settings_Exit(void);
extern const    Button bSaveFirmware;                       ///< ������� - ����. ��������
static bool    IsActive_SaveFirmware(void);
static void     OnPress_SaveFirmware(void);
extern const     Page ppSerialNumber;                       ///< ������� - �/�
static void     OnPress_SerialNumber(void);
static void        Draw_EnterSerialNumber(void);
static void    OnRegSet_SerialNumber(int);
extern const   SButton bSerialNumber_Exit;                  ///< ������� - �/� - �����
static void     OnPress_SerialNumber_Exit(void);
extern const   SButton bSerialNumber_Change;                ///< ������� - �/� - �������
static void     OnPress_SerialNumber_Change(void);
static void        Draw_SerialNumber_Change(int, int);
extern const   SButton bSerialNumber_Save;                  ///< ������� - �/� - ���������
static void     OnPress_SerialNumber_Save(void);
static void        Draw_SerialNumber_Save(int, int);
extern const    Button bEraseData;                          ///< ������� - ������� ������
static void     OnPress_EraseData(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// � ���� ��������� ����� ��������� ������ ��������� ������ ��� �������� �������� ppSerialNumer
typedef struct
{
    int number;     ///< ��������������, ���������� �����.
    int year;       ///< ��������������, ���.
    int curDigt;    ///< ��������������, ������� (0) ��� ����� (1) ��������� ����� ���������.
} StructForSN;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� ///
DEF_PAGE_13(    pDebug, ,
    Page_Debug, &mainPage, FuncActive, EmptyPressPage,
    "�������", "DEBUG",
    "",
    "",
    ppConsole,              // ������� - �������
    ppADC,                  // ������� - ���
    ppRand,			        // ������� - ����-���
    ppChannels,		        // ������� - ������
    cStats,			        // ������� - ����������
    cDisplayOrientation,    // ������� - ����������
    cEMS,                   // ������� - ����� ���
    mgPred,			        // ������� - ����������
    mgPost,			        // ������� - �����������
    ppSettings,		        // ������� - ���������
    bSaveFirmware,          // ������� - ����. ��������
    ppSerialNumber,         // ������� - �/�
    bEraseData              // ������� - ������� ������
);

const Page * pointerPageDebug = &pDebug;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ������� ///
DEF_PAGE_5(     ppConsole, static,
    Page_Debug_Console, &pDebug, FuncActive, EmptyPressPage,
    "�������", "CONSOLE",
    "",
    "",
    gConsole_NumStrings,    // ������� - ������� - ����� �����
    cConsole_SizeFont,      // ������� - ������� - ������ ������
    cConsole_ModeStop,      // ������� - ������� - ���. ��������
    pppConsole_Registers,   // ������� - ������� - ��������
    bConsole_SizeSettings   // ������� - ������� - ������ ��������
);

//---------------------------------------------------------------------------------------------------------------- ������� - ������� - ����� ����� ---
DEF_GOVERNOR
(
    gConsole_NumStrings,
    "����� �����", "Number strings",
    "",
    "",
    ppConsole, CONSOLE_NUM_STRINGS, 0, 33, FuncActive, FuncChanged, FuncBeforeDraw
);

//-------------------------------------------------------------------------------------------------------------- ������� - ������� - ������ ������ ---
DEF_CHOICE_2
(
    cConsole_SizeFont, ppConsole,
    set.dbg_SizeFont, FuncActive, FuncChangedChoice, FuncDraw,
    "������ ������", "Size font",
    "",
    "",
    "5", "5",
    "8", "8"    
);

//-------------------------------------------------------------------------------------------------------------- ������� - ������� - ���. �������� ---
DEF_CHOICE_2
(
    cConsole_ModeStop, ppConsole,
    MODE_PAUSE_CONSOLE, FuncActive, FuncChangedChoice, FuncDraw,
    "���. ��������", "Mode stop",
    "������������� ����������� ������������ ������ � ������� ���� ������� �� ������ ����/����",
    "It provides the ability to pause the output to the console by pressing the ����/���� button",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ������� - �������� ///
DEF_PAGE_12(    pppConsole_Registers, static,
    Page_Debug_Console_Registers, &ppConsole, FuncActive, EmptyPressPage,
    "��������", "REGISTERS",
    "",
    "",
    cConsole_Registers_ShowAll,     // ������� - ������� - �������� - ���������� ���
    cConsole_Registers_RD_FL,       // ������� - ������� - �������� - RD_FL
    cConsole_Registers_RShiftA,     // ������� - ������� - �������� - U ��. 1�
    cConsole_Registers_RShiftB,     // ������� - ������� - �������� - U ��. 2�
    cConsole_Registers_TrigLev,     // ������� - ������� - �������� - U �����
    cConsole_Registers_RangeA,      // ������� - ������� - �������� - �����/��� 1
    cConsole_Registers_RangeB,      // ������� - ������� - �������� - �����/��� 2
    cConsole_Registers_TrigParam,   // ������� - ������� - �������� - �����. �����.
    cConsole_Registers_ChanParamA,  // ������� - ������� - �������� - �����. ���. 1
    cConsole_Registers_ChanParamB,  // ������� - ������� - �������� - �����. ���. 2
    cConsole_Registers_TBase,       // ������� - ������� - �������� - �����/���
    cConsole_Registers_TShift       // ������� - ������� - �������� - � ��.
);

//-------------------------------------------------------------------------------------------------- ������� - ������� - �������� - ���������� ��� ---
DEF_CHOICE_2
(
    cConsole_Registers_ShowAll, pppConsole_Registers,
    DBG_SHOW_ALL, FuncActive, FuncChangedChoice, FuncDraw,
    "���������� ���", "Show all",
    "���������� ��� ��������, ���������� � ��������",
    "To show all values transferred in registers",
    "���", "No",
    "��",  "Yes"
);

//----------------------------------------------------------------------------------------------------------- ������� - ������� - �������� - RD_FL ---
static bool IsActive_Console_Registers(void)
{
    return DBG_SHOW_ALL;
}

DEF_CHOICE_2
(
    cConsole_Registers_RD_FL, pppConsole_Registers,
    DBG_SHOW_FLAG, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------- ������� - ������� - �������� - U ��. 1� ---
DEF_CHOICE_2
(
    cConsole_Registers_RShiftA, pppConsole_Registers,
    set.dbg_ShowRShift[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------- ������� - ������� - �������� - U ��. 2� ---
DEF_CHOICE_2
(
    cConsole_Registers_RShiftB, pppConsole_Registers,
    set.dbg_ShowRShift[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------- ������� - ������� - �������� - U �����. ---
DEF_CHOICE_2
(
    cConsole_Registers_TrigLev, pppConsole_Registers,
    set.dbg_ShowTrigLev, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------- ������� - ������� - �������� - �����/��� 1 ---
DEF_CHOICE_2
(
    cConsole_Registers_RangeA, pppConsole_Registers,
    set.dbg_ShowRange[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------- ������� - ������� - �������� - �����/��� 2 ---
DEF_CHOICE_2
(
    cConsole_Registers_RangeB, pppConsole_Registers,
    set.dbg_ShowRange[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------- ������� - ������� - �������� - �����. �����. ---
DEF_CHOICE_2
(
    cConsole_Registers_TrigParam, pppConsole_Registers,
    set.dbg_ShowTrigParam, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------- ������� - ������� - �������� - �����. ���. 2 ---
DEF_CHOICE_2
(
    cConsole_Registers_ChanParamA, pppConsole_Registers,
    set.dbg_ShowChanParam[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����. ���. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------- ������� - ������� - �������� - �����. ���. 2 ---
DEF_CHOICE_2
(
    cConsole_Registers_ChanParamB, pppConsole_Registers,
    set.dbg_ShowChanParam[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//------------------------------------------------------------------------------------------------------- ������� - ������� - �������� - �����/��� ---
DEF_CHOICE_2
(
    cConsole_Registers_TBase, pppConsole_Registers,
    set.dbg_ShowTBase, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------- ������� - ������� - �������� - � ��. ---
DEF_CHOICE_2
(
    cConsole_Registers_TShift, pppConsole_Registers,
    set.dbg_ShowTShift, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//------------------------------------------------------------------------------------------------------------ ������� - ������� - ������ �������� ---
static void Draw_Console_SizeSettings(int x, int y)
{
    char buffer[30];
    sprintf(buffer, "����.�����. %d", sizeof(Settings));
    painter.DrawText(x + 6, y + 13, buffer, gColorBack);
}

DEF_BUTTON
(
    bConsole_SizeSettings,
    "", "",
    "���������� ������� ������ ��������� ��� ���������� ��������",
    "Displays the current size of the structure to save settings",
    ppConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ��� ///
DEF_PAGE_3(     ppADC, static,
    Page_Debug_ADC, &pDebug, FuncActive, EmptyPressPage,
    "���", "ADC",
    "",
    "",
    pppADC_Balance, // ������� - ��� - ������
    pppADC_Stretch, // ������� - ��� - ��������
    pppADC_Shift    // ������� - ��� - ��� ����
);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ��� - ������ ///
DEF_PAGE_3(     pppADC_Balance, static,
    Page_Debug_ADC_Balance, &ppADC, FuncActive, EmptyPressPage,
    "������", "BALANCE",
    "",
    "",
    cADC_Balance_Mode,      // ������� - ��� - ������ - �����
    gADC_Balance_ShiftA,    // ������� - ��� - ������ - �������� 1
    gADC_Balance_ShiftB     // ������� - ��� - ������ - �������� 2
);

//----------------------------------------------------------------------------------------------------------------- ������� - ��� - ������ - ����� ---
static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);
}

static int16 shiftADCA;
static int16 shiftADCB;

static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, SET_BALANCE_ADC_A, (int8)NRST_BALANCE_ADC_A},
        {0, SET_BALANCE_ADC_B, (int8)NRST_BALANCE_ADC_B}
    };

    shiftADCA = shift[A][NRST_BALANCE_ADC_TYPE];
    shiftADCB = shift[B][NRST_BALANCE_ADC_TYPE];
}

DEF_CHOICE_3
(
    cADC_Balance_Mode, pppADC_Balance,
    NRST_BALANCE_ADC_TYPE, FuncActive, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual"
);

//------------------------------------------------------------------------------------------------------------ ������� - ��� - ������ - �������� 1 ---
static bool IsActive_ADC_Balance_ShiftAB(void)
{
    return NRST_BALANCE_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Balance_ShiftA(void)
{
    NRST_BALANCE_ADC_A = shiftADCA;
}

DEF_GOVERNOR
(
    gADC_Balance_ShiftA,
    "�������� 1", "Offset 1",
    "",
    "",
    pppADC_Balance, shiftADCA, -125, 125, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftA, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------ ������� - ��� - ������ - �������� 2 ---
static void OnChanged_ADC_Balance_ShiftB(void)
{
    NRST_BALANCE_ADC_B = shiftADCB;
}

DEF_GOVERNOR
(
    gADC_Balance_ShiftB,
    "�������� 2", "Offset 2",
    "",
    "",
    pppADC_Balance, shiftADCB, -125, 125, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftB, FuncBeforeDraw
);

static const Choice emptyChoice = {Item_Choice};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ��� - �������� ///
DEF_PAGE_15(    pppADC_Stretch, static,
    Page_Debug_ADC_Stretch, &ppADC, FuncActive, EmptyPressPage,
    "��������", "STRETCH",
    "������������� ����� � �������� �������� (��� ������� ������)",
    "Sets mode and the value of stretching (manual mode)",
    cADC_Stretch_Mode,      // ������� - ��� - �������� - �����
    gADC_Stretch_A,         // ������� - ��� - �������� - �������� 1�
    gADC_Stretch_B,         // ������� - ��� - �������� - �������� 2�
    emptyChoice,
    emptyChoice,
    gADC_Stretch_Ak20mV,    // ������� - ��� - �������� - 20��/1� 1�
    gADC_Stretch_Ak50mV,    // ������� - ��� - �������� - 50�� 1� 
    gADC_Stretch_Ak100mV,   // ������� - ��� - �������� - 100��/5� 1�
    gADC_Stretch_Ak2V,      // ������� - A�� - �������� - 2� 1�
    emptyChoice,
    gADC_Stretch_Bk20mV,    // ������� - ��� - �������� - 20��/1� 2�
    gADC_Stretch_Bk50mV,    // ������� - ��� - �������� - 50�� 2� 
    gADC_Stretch_Bk100mV,   // ������� - ��� - �������� - 100��/5� 2�
    gADC_Stretch_Bk2V,      // ������� - ��� - �������� - 2� 2�
    emptyChoice
);

//--------------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - ����� ---
static int16 stretchA;
static int16 stretchB;  

void OnChanged_ADC_Stretch_Mode(bool)
{
    if (NRST_STRETCH_ADC_TYPE_IS_DISABLE)
    {
        stretchA = NRST_STRETCH_ADC_A(StretchADC_Disable) = 0;
        stretchB = NRST_STRETCH_ADC_B(StretchADC_Disable) = 0;
    }
    else
    {
        stretchA = NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE);
        stretchB = NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE);
    }
}

DEF_CHOICE_3
(
    cADC_Stretch_Mode, pppADC_Stretch,
    NRST_STRETCH_ADC_TYPE, FuncActive, OnChanged_ADC_Stretch_Mode, FuncDraw,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual"
);

//--------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - �������� 1� ---
static bool IsActive_ADC_StretchAB(void)
{
    return NRST_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_A(void)
{
    NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE) = stretchA;
}

DEF_GOVERNOR
(
    gADC_Stretch_A,
    "�������� 1�", "Stretch 1ch",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    "Sets the manual stretching of the first channel.\n1 = 0.0001",
    pppADC_Stretch, stretchA, -10000, 10000, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_A, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - �������� 2� ---
static void OnChanged_ADC_Stretch_B(void)
{
    NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE) = stretchB;
}

DEF_GOVERNOR
(
    gADC_Stretch_B,
    "�������� 2�", "Stretch 2ch",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    "Sets the manual stretching of the second channel.\n1 = 0.0001",
    pppADC_Stretch, stretchB, -10000, 10000, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_B, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - 20��/1� 1� ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak20mV,
    "20��/1� 1�", "20mV/1V 1k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_20mV_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - 50�� 1� ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak50mV,
    "50�� 1�", "50mV 1k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_50mV_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - 100��/5� 1� ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak100mV,
    "100��/5� 1�", "100mV/5V 1ch",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_100mV_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - 2� 1� ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak2V,
    "2� 1�", "2V 1ch",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_2V_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - 20��/1� 2� ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk20mV,
    "20��/1� 2�", "20mV/1V 2k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_20mV_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------- ������� - ��� - �������� - ��� ���� 50�� 2� ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk50mV,
    "50�� 2�", "50mV 2k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_50mV_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - 100��/5� 2� ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk100mV,
    "100��/5� 2�", "100mV/5V 2k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_100mV_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------- ������� - ��� - �������� - 2� 2� ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk2V,
    "2� 2�", "2V 2ch",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_2V_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ��� - ��� ���� ///
DEF_PAGE_7(     pppADC_Shift, static,
    Page_Debug_ADC_Shift, &ppADC, FuncActive, EmptyPressPage,
    "��� ����", "ADD RSHFIT",
    "",
    "",
    bADC_Shift_Reset,   // ������� - ��� - ��� ���� - �����
    gADC_Shift_A2mV,    // ������� - ��� - ��� ���� - �� 1� 2�� ����
    gADC_Shift_B2mV,    // ������� - ��� - ��� ���� - �� 2� 2�� ����
    gADC_Shift_A5mV,    // ������� - ��� - ��� ���� - �� 1� 5�� ����
    gADC_Shift_B5mV,    // ������� - ��� - ��� ���� - �� 2� 5�� ����
    gADC_Shift_A10mV,   // ������� - ��� - ��� ���� - �� 1� 10�� ����
    gADC_Shift_B10mV    // ������� - ��� - ��� ���� - �� 2� 10�� ����
);

// ������� - ��� - ��� ���� - ����� ------------------------------------------------------------------------------------------------------------------
static void OnPress_ADC_Shift_Reset(void)
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < 3; range++)
        {
            RSHIFT_ADD_STABLE(ch, range) = 0;
        }
    }
    FPGA_SetRShift(A, SET_RSHIFT_A);
    FPGA_SetRShift(B, SET_RSHIFT_B);
}

DEF_BUTTON(
    bADC_Shift_Reset,
    "�����", "Reset",
    "",
    "",    
    pppADC_Shift, FuncActive, OnPress_ADC_Shift_Reset, FuncDraw);

//------------------------------------------------------------------------------------------------------ ������� - ��� - ��� ���� - �� 1� 2�� ���� ---
static void OnChanged_ADC_Shift_A(void)
{
    FPGA_SetRShift(A, SET_RSHIFT_A);
}

DEF_GOVERNOR
(
    gADC_Shift_A2mV,
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_A(Range_2mV), -100, 100, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ ������� - ��� - ��� ���� - �� 2� 2�� ���� ---
static void OnChanged_ADC_Shift_B(void)
{
    FPGA_SetRShift(B, SET_RSHIFT_B);
}

DEF_GOVERNOR
(
    gADC_Shift_B2mV,
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_B(Range_2mV), -100, 100, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ ������� - ��� - ��� ���� - �� 1� 5�� ���� ---
DEF_GOVERNOR
(
    gADC_Shift_A5mV,
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_A(Range_5mV), -100, 100, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ ������� - ��� - ��� ���� - �� 2� 5�� ���� ---
DEF_GOVERNOR
(
    gADC_Shift_B5mV,
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_B(Range_5mV), -100, 100, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------- ������� - ��� - ��� ���� - �� 1� 10�� ���� ---
DEF_GOVERNOR
(
    gADC_Shift_A10mV,
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_A(Range_10mV), -100, 100, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------- ������� - ��� - ��� ���� - �� 2� 10�� ���� ---
DEF_GOVERNOR
(
    gADC_Shift_B10mV,
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_B(Range_10mV), -100, 100, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ����-��� ///
DEF_PAGE_8(     ppRand, static,
    Page_Debug_Rand, &pDebug, FuncActive, EmptyPressPage,
    "����-���", "RANDOMIZER",
    "",
    "",
    gRand_NumAverage,       // ������� - ����-��� - ������.
    gRand_NumSmooth,        // ������� - ����-��� - �����������
    gRand_NumMeasures,      // ������� - ����-��� - ���-�/������
    cRand_ShowInfo,         // ������� - ����-��� - ����������
    gRand_ShowStat,         // ������� - ����-��� - ����������
    gRand_TimeCompensation, // ������� - ����-��� - ����������� ��������
    gRand_AddTimeShift,     // ������� - ����-��� - ��������
    gRand_Pretriggered      // ������� - �A��-��� - ����������
);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ������ ///
DEF_PAGE_2(     ppChannels, static,
    Page_Debug_Channels, &pDebug, FuncActive, EmptyPressPage,
    "������", "CHANNELS",
    "",
    "",
    cChannels_BandwidthA,   // ������� - ������ - ������ 1
    cChannels_BandwidthB    // ������� - ������ - ������ 2
);

//-------------------------------------------------------------------------------------------------------------------- ������� - ������ - ������ 1 ---
static void OnChanged_Channels_BandwidthA(bool)
{
    FPGA_SetBandwidth(A);
}

DEF_CHOICE_7
(
    cChannels_BandwidthA, ppChannels,
    BANDWIDTH_DEBUG(A), FuncActive, OnChanged_Channels_BandwidthA, FuncDraw,
    "������ 1", "Bandwidth 1",
    "����� ����� ������� ������, ������� ����� ����������� � �����1-������ ��� ������ �������� ������",
    "Here you can select the bandwidth, which will operate in CHANNEL1-Bandwidth when set to Full",
    "������", "Full",
    "20���", "20MHz",
    "100���", "100MHz",
    "200���", "200MHz",
    "350���", "350MHz",
    "650���", "650MHz",
    "750���", "750MHz"
);

//-------------------------------------------------------------------------------------------------------------------- ������� - ������ - ������ 2 ---
static void OnChanged_Channels_BandwidthB(bool)
{
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_7
(
    cChannels_BandwidthB, ppChannels,
    BANDWIDTH_DEBUG(B), FuncActive, OnChanged_Channels_BandwidthB, FuncDraw,
    "������ 2", "Bandwidth 2",
    "����� ����� ������� ������, ������� ����� ����������� � �����2-������ ��� ������ �������� ������",
    "Here you can select the bandwidth, which will operate in CHANNEL2-Bandwidth when set to Full",
    "������", "Full",
    "20���", "20MHz",
    "100���", "100MHz",
    "200���", "200MHz",
    "350���", "350MHz",
    "650���", "650MHz",
    "750���", "750MHz"
);

//-------------------------------------------------------------------------------------------------------------- ������� - ����-��� - ���-�/������ ---
static void OnChanged_Rand_NumMeasures(void)
{
    FPGA_SetNumberMeasuresForGates(NUM_MEASURES_FOR_GATES);
}

DEF_GOVERNOR
(
    gRand_NumMeasures,
    "���-�/������", "Samples/gates",
    "",
    "",
    ppRand, NUM_MEASURES_FOR_GATES, 1, 2500, FuncActive, OnChanged_Rand_NumMeasures, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ ������� - ����-��� - ����������� �������� ---
static void OnChanged_Rand_TimeCompensation(void)
{
    FPGA_SetDeltaTShift(TIME_COMPENSATION);
}

DEF_GOVERNOR
(
    gRand_TimeCompensation,
    "����������� ��������", "Compenstaion time",
    "���������� ����������� �������� ��� 40 ��",
    "",
    ppRand, TIME_COMPENSATION, 0, 510, FuncActive, OnChanged_Rand_TimeCompensation, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------------ ������� - ����-��� - �������� ---
static void OnChanged_Rand_AddTimeShift(void)
{
    FPGA_SetTShift(SET_TSHIFT);
}

int16 addShift = 0;

DEF_GOVERNOR
(
    gRand_AddTimeShift,
    "��� ��������", "Add shift",
    "���������� ������� ��� �������� tShift",
    "",
    ppRand, addShift, -100, 100, FuncActive, OnChanged_Rand_AddTimeShift, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------------------- ������� - ����-��� - ���������� ---
static void OnChanged_Rand_Pretriggered(void)
{
    LoadTShift();
}

DEF_GOVERNOR
(
    gRand_Pretriggered,
    "����������", "Pretiggered",
    "�������� �����������, ������� ������� � ������������",
    "",
    ppRand, PRETRIGGERED, 0, 30000, FuncActive, OnChanged_Rand_Pretriggered, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------------- ������� - ����-��� - ������. ---
DEF_GOVERNOR
(
    gRand_NumAverage,
    "������.", "Average",
    "",
    "",
    ppRand, NRST_NUM_AVE_FOR_RAND, 1, 32, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------- ������� - ����-��� - ����������� ---
DEF_GOVERNOR
(
    gRand_NumSmooth,
    "�����������", "Smoothing",
    "",
    "",
    ppRand, NRST_NUM_SMOOTH_FOR_RAND, 1, 10, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------------------- ������� - ���������� ---
static int16 pred;

DEF_GOVERNOR
(
    mgPred,
    "����������", "",
    "", "",
    pDebug, pred, 0, 15000, FuncActive, OnChanged_Pred, FuncBeforeDraw
);

// ������� - ����������� -----------------------------------------------------------------------------------------------------------------------------
static int16 post;

DEF_GOVERNOR
(
    mgPost,
    "�����������", "",
    "", "",
    pDebug, post, 0, 15000, FuncActive, OnChanged_Post, FuncBeforeDraw
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - ��������� ///
DEF_PAGE_SB(    ppSettings, static,
    PageSB_Debug_Settings, &pDebug, FuncActive,
    OnPress_Settings, EmptyDrawPage, FuncRegSetPage,
    "���������", "SETTINGS",
    "�������� ���������� � ����������",
    "Show settings information",
    &bSettings_Exit,    // ������� - ��������� - �����
    0,
    0,
    0,
    0,
    0
);

//-------------------------------------------------------------------------------------------------------------------- ������� - ��������� - ����� ---
static void OnPress_Settings_Exit(void)
{
    display.SetDrawMode(DrawMode_Auto, 0);
}

DEF_SMALL_BUTTON
(
    bSettings_Exit,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button to return to the previous menu",
    ppSettings, FuncActive, OnPress_Settings_Exit, DrawSB_Exit
);

static void DebugShowSetInfo_Draw(void)
{
    painter.BeginScene(gColorBack);
    painter.DrawRectangle(0, 0, 319, 239, gColorFill);

    int x0 = 30;
    int y0 = 25;
    int dY = 10;
    int y = y0 - dY;

#define Y_AND_INCREASE (y += dY, y)
#define DRAW_TEXT(str)                  painter.DrawText(x0, Y_AND_INCREASE, str);
#define DRAW_FORMAT(str, value)         painter.DrawFormatText(x0, Y_AND_INCREASE, str, value)
#define DRAW_FORMAT2(str, val1, val2)   painter.DrawFormatText(x0, Y_AND_INCREASE, str, val1, val2);

    //Painter_DrawFormatText(x0, Y_AND_INCREASE, "������ �������� ��������� %d", sizeof(set));
    DRAW_FORMAT("������ �������� ��������� : %d", sizeof(set));
    painter.DrawText(x0, Y_AND_INCREASE, "�������������� ���������:");
    int x = painter.DrawText(x0, Y_AND_INCREASE, "rShiftAdd :") + 5;

    int ddY = 0;

    for (int type = 0; type < 2; type++)
    {
        for (int ch = 0; ch < 2; ch++)
        {
            for (int range = 0; range < RangeSize; range++)
            {
                painter.DrawFormatText(x + range * 20, y + dY * ddY, "%d", NRST_RSHIFT_ADD(ch, range, type));
            }
            ddY++;
        }
    }

    y += dY * 3;

    DRAW_FORMAT("correctionTime : %d", NRST_CORRECTION_TIME);
    DRAW_FORMAT2("balanceADC : %d %d", NRST_BALANCE_ADC_A, NRST_BALANCE_ADC_B);
    DRAW_FORMAT("numAveForRand : %d", NRST_NUM_AVE_FOR_RAND);

    pString s[3] = {"���������", "��������� �������������", "������ �������"};
    DRAW_FORMAT("balanceADCtype : %s", (NRST_BALANCE_ADC_TYPE < 3 ? s[NRST_BALANCE_ADC_TYPE] : "!!! ������������ �������� !!!"));
    DRAW_FORMAT("stretchADCtype : %s", (NRST_STRETCH_ADC_TYPE < 3 ? s[NRST_STRETCH_ADC_TYPE] : "!!! ������������ �������� !!!"));
  
    x = painter.DrawText(x0, Y_AND_INCREASE, "stretchADC :") + 5;

    for (int ch = 0; ch < 2; ch++)
    {
        for (int num = 0; num < 3; num++)
        {
            painter.DrawFormatText(x + num * 20, y + dY * ch, "%d", NRST_STRETCH_ADC(ch, num));
        }
    }

    y += dY;

#define DRAW_STRETCH(name) DRAW_FORMAT2(#name " : %d %d", set.nrst_##name[0], set.nrst_##name[1])

    DRAW_STRETCH(AddStretch20mV);
    DRAW_STRETCH(AddStretch50mV);
    DRAW_STRETCH(AddStretch100mV);
    DRAW_STRETCH(AddStretch2V);

    DRAW_FORMAT("numSmoothForRand : %d", NRST_NUM_SMOOTH_FOR_RAND);

    Menu_Draw();
    painter.EndScene();
}

static void OnPress_Settings(void)
{
    display.SetDrawMode(DrawMode_Auto, DebugShowSetInfo_Draw);
}

static void OnChanged_Pred(void)
{
    gPred = ~pred;
    static char buffer[30];
    LOG_WRITE("pred %d %s", pred, strUtils.Hex16toString((uint16)gPred, buffer));
}

static void OnChanged_Post(void)
{
    gPost = (uint16)~post;
    static char buffer[30];
    LOG_WRITE("post %d %s", post, strUtils.Hex16toString(gPost, buffer));
}

//---------------------------------------------------------------------------------------------------------------- ������� - ����-��� - ���������� ---
DEF_CHOICE_2
(
    cRand_ShowInfo, ppRand,
    SHOW_RAND_INFO, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Information",
    "���������� ���������� � ������� �������������",
    "To show information on randomizer gate",
    "�� ����������", "Hide",
    "����������",    "Show"
);

//---------------------------------------------------------------------------------------------------------------- ������� - ����-��� - ���������� ---
DEF_CHOICE_2
(
    gRand_ShowStat, ppRand,
    SHOW_RAND_STAT, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Statistics",
    "���������� ������ ����������",
    "Statistics show schedule",
    "�� ����������", "Hide",
    "����������",    "Show"
);

//---------------------------------------------------------------------------------------------------------------------------- ������� - ����� ��� ---
static void OnChanged_EMS(bool)
{
    FPGA_SetBandwidth(A);
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_2
(
    cEMS, pDebug,
    MODE_EMS, FuncActive, OnChanged_EMS, FuncDraw,
    "����� ���", "EMS mode",
    "������������� �������� ������ 20���, ����������� �� 4-� ������, ���������� �� 8-�� ������",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------------------------------- ������� - ���������� ---
void OnChanged_DisplayOrientation(bool)
{
    display.SetOrientation(DISPLAY_ORIENTATION);
}

DEF_CHOICE_2
(
    cDisplayOrientation, pDebug,
    DISPLAY_ORIENTATION, FuncActive, OnChanged_DisplayOrientation, FuncDraw,
    "����������", "DisplayOrientation",
    "������������� ���������� �������",
    "Sets display orientation",
    "������",   "Direct",
    "��������", "Back"
);

//--------------------------------------------------------------------------------------------------------------------------- ������� - ���������� ---
DEF_CHOICE_2
(
    cStats, pDebug,
    SHOW_STAT, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Statistics",
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ ��������",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "�� ����������", "Hide",
    "����������",    "Show"
);

// ������� - ����. �������� --------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(
    bSaveFirmware,
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    pDebug, IsActive_SaveFirmware, OnPress_SaveFirmware, FuncDraw);

static bool IsActive_SaveFirmware(void)
{
    return FDRIVE_IS_CONNECTED;
}

static void OnPress_SaveFirmware(void)
{
    display.FuncOnWaitStart(DICT(DSaveFirmware), false);

    StructForWrite structForWrite;

    FDrive_OpenNewFileForWrite("S8-54.bin", &structForWrite);

    uint8 *address = (uint8 *)0x08020000;
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive_WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive_CloseFile(&structForWrite);

    display.FuncOnWaitStop();

    display.ShowWarning(FirmwareSaved);
}

// ������� - ������� ������ --------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(
    bEraseData,
    "������� �����", "Erase data",
    "������� ���������� ������ �� ����",
    "Erase all saved datas from EEPROM",
    pDebug, FuncActive, OnPress_EraseData, FuncDraw);

static void OnPress_EraseData(void)
{
    display.FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    FLASH_DeleteAllData();
    display.FuncOnWaitStop();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� - �/� ///
DEF_PAGE_SB(    ppSerialNumber, static,
    PageSB_Debug_SerialNumber, &pDebug, FuncActive,
    OnPress_SerialNumber, EmptyDrawPage, OnRegSet_SerialNumber,
    "�/�", "S/N",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    &bSerialNumber_Exit,    // ������� - �/� - �����
    &bSerialNumber_Change,  // ������� - �/� - �������
    0,
    0,
    0,
    &bSerialNumber_Save     // ������� - �/� - ���������
);

static void OnPress_SerialNumber(void)
{
    display.SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number = 01;
    s->year = 2017;
    s->curDigt = 0;
}

static void Draw_EnterSerialNumber(void)
{
    int x0 = grid.Left() + 40;
    int y0 = GRID_TOP + 20;
    int width = grid.Width() - 80;
    int height = 160;

    painter.DrawRectangle(x0, y0, width, height, gColorFill);
    painter.FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, gColorBack);

    int deltaX = 10;

    ACCESS_EXTRAMEM(StructForSN, s);
    
    bool selNumber = s->curDigt == 0;

    char buffer[20];
    snprintf(buffer, 19, "%02d", s->number);

    Color colorText = gColorFill;
    Color colorBackground = gColorBack;

    if (selNumber)
    {
        colorText = Color::FLASH_01;
        colorBackground = Color::FLASH_10;
    }

    int y = y0 + 50;

    painter.SetColor(colorText);
    int x = painter.DrawTextOnBackground(x0 + deltaX, y, buffer, colorBackground);

    colorText = Color::FLASH_01;
    colorBackground = Color::FLASH_10;

    if (selNumber)
    {
        colorText = gColorFill;
        colorBackground = gColorBack;
    }

    snprintf(buffer, 19, "%04d", s->year);

    painter.SetColor(colorText);
    painter.DrawTextOnBackground(x + 5, y, buffer, colorBackground);

    // ������ ������� ���������� �� ���������� ����� � OTP-������ ��� ������

    int allShots = OTP_GetSerialNumber(buffer);

    painter.DrawFormText(x0 + deltaX, y0 + 130, gColorFill, "������� ���������� ����� %s", buffer[0] == 0 ? "-- ----" : buffer);

    painter.DrawFormText(x0 + deltaX, y0 + 100, gColorFill, "�������� ����� ��� %d �������", allShots);
}

static void OnRegSet_SerialNumber(int angle)
{
    typedef int (*pFunc)(int *, int, int);

    pFuncMathPIII p = (angle > 0) ? (&Math::CircleIncrease<int>) : (&Math::CircleDecrease<int>);

    ACCESS_EXTRAMEM(StructForSN, s);

    if (s->curDigt == 0)
    {
        (math.*p)(&s->number, 1, 99);
    }
    else
    {
        (math.*p)(&s->year, 2016, 2050);
    }
    Sound_GovernorChangedValue();
}

//-------------------------------------------------------------------------------------------------------------------------- ������� - �/� - ����� ---
static void OnPress_SerialNumber_Exit(void)
{
    OnPressSB_Exit();
    FREE_EXTRAMEM();
}

DEF_SMALL_BUTTON
(
    bSerialNumber_Exit,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button to return to the previous menu",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Exit, DrawSB_Exit
);


//----------------------------------------------------------------------------------------------------------------------- ������� - �/� - �������� ---
static void OnPress_SerialNumber_Change(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);
    ++s->curDigt;
    s->curDigt %= 2;
    painter.ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_TAB);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bSerialNumber_Change,
    "��������", "Insert",
    "��������� �������� ������",
    "Inserts the chosen symbol",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
);

// ������� - �/� - ��������� -------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON
(
    bSerialNumber_Save,
    "���������", "Save",
    "���������� �������� ����� � OTP",
    "Records the serial number in OTP",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
);

static void OnPress_SerialNumber_Save(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);

    char stringSN[20];

    snprintf(stringSN, 19, "%02d %04d", s->number, s->year);

    if (!OTP_SaveSerialNumber(stringSN))
    {
        display.ShowWarning(FullyCompletedOTP);
    }
}

static void Draw_SerialNumber_Save(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    painter.SetFont(TypeFont_8);
}


































/*
void OnPressDebugDisable(void)
{
    ShowMenu(false);
    SetMenuPageDebugActive(false);
    SetMenuPosActItem(Page_Main, 0);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void OnDegubConsoleViewChanged(bool active)
{
    display.SetPauseForConsole(CONSOLE_IN_PAUSE);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressDebugConsoleUp(void)
{
    display.OneStringUp();
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressDebugConsoleDown(void)
{
    display.OneStringDown();
}

        /// ������� - ��� - ���. ����. ���. - ��������
        const Governor mgDebugADCaltShift =
        {
            Item_Governor, &mspDebugADCaltShift,
            {
                "��������", "Value"
            },
            {
                "",
                ""
            },
            0,
            &set.debug.altShift, -2, 2, 0
        };

bool sIsShowReg_RShift1(void)
{
    return set.debug.showRegisters.rShiftB || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TrigLev(void)
{
    return set.debug.showRegisters.trigLev || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_Range(Channel ch)
{
    return set.debug.showRegisters.range[ch] || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TrigParam(void)
{
    return set.debug.showRegisters.trigParam || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_ChanParam(Channel ch)
{
    return set.debug.showRegisters.chanParam[ch] || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TShift(void)
{
    return set.debug.showRegisters.tShift || set.debug.showRegisters.all;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool sIsShowReg_TBase(void)
{
    return set.debug.showRegisters.tBase || set.debug.showRegisters.all;
}


const Page mspDebugADCaltShift =    // ������� - ��� - ��� ���� ���
{
    Item_Page, &ppADC,
    {
        "��� ���� ���", "ALT SHIFT MEM"
    },
    {
        "",
        ""
    },
    0, Page_Debug_ADC_AltShift,
    {
        (void *)&mgDebugADCaltShift
    }
};
*/
