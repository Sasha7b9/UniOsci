#include "Log.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/FLASH.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void LoadTShift(void);

extern const PageBase mainPage;
extern const PageBase pDebug;
extern const PageBase ppConsole;
extern const PageBase pppConsole_Registers;
extern const PageBase ppADC;
extern const PageBase pppADC_Balance;
extern const PageBase pppADC_Stretch;
extern const PageBase pppADC_Shift;
extern const PageBase ppRand;
extern const PageBase ppChannels;
extern const PageBase ppSettings;
extern const PageBase ppSerialNumber;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// � ���� ��������� ����� ��������� ������ ��������� ������ ��� �������� �������� ppSerialNumer
typedef struct
{
    int number;     ///< ��������������, ���������� �����.
    int year;       ///< ��������������, ���.
    int curDigt;    ///< ��������������, ������� (0) ��� ����� (1) ��������� ����� ���������.
} StructForSN;

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gConsole_NumStrings,                                                                     //--- ������� - ������� - ����� ����� ---
    "����� �����", "Number strings",
    "",
    "",
    CONSOLE_NUM_STRINGS, 0, 33, ppConsole, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_SizeFont,                                                                     //--- ������� - ������� - ������ ������ ---
    "������ ������", "Size font",
    "",
    "",
    "5", "5",
    "8", "8",
    set.dbg_SizeFont, ppConsole, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_ModeStop,                                                                     //--- ������� - ������� - ���. �������� ---
    "���. ��������", "Mode stop",
    "������������� ����������� ������������ ������ � ������� ���� ������� �� ������ ����/����",
    "It provides the ability to pause the output to the console by pressing the ����/���� button",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    MODE_PAUSE_CONSOLE, ppConsole, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cConsole_Registers_ShowAll,                                                    //--- ������� - ������� - �������� - ���������� ��� ---
    "���������� ���", "Show all",
    "���������� ��� ��������, ���������� � ��������",
    "To show all values transferred in registers",
    "���", "No",
    "��", "Yes",
    DBG_SHOW_ALL, pppConsole_Registers, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Console_Registers(void)
{
    return DBG_SHOW_ALL;
}

DEF_CHOICE_2(       cConsole_Registers_RD_FL,                                                           //--- ������� - ������� - �������� - RD_FL ---
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    DBG_SHOW_FLAG, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RShiftA,                                                      //--- ������� - ������� - �������� - U ��. 1� ---
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRShift[A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RShiftB,                                                      //--- ������� - ������� - �������� - U ��. 2� ---
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRShift[B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TrigLev,                                                      //--- ������� - ������� - �������� - U �����. ---
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTrigLev, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RangeA,                                                    //--- ������� - ������� - �������� - �����/��� 1 ---
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRange[A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_RangeB,                                                    //--- ������� - ������� - �������� - �����/��� 2 ---
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowRange[B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TrigParam,                                               //--- ������� - ������� - �������� - �����. �����. ---
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTrigParam, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_ChanParamA,                                              //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowChanParam[A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_ChanParamB,                                              //--- ������� - ������� - �������� - �����. ���. 2 ---
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowChanParam[B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TBase,                                                       //--- ������� - ������� - �������� - �����/��� ---
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTBase, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cConsole_Registers_TShift,                                                          //--- ������� - ������� - �������� - � ��. ---
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    set.dbg_ShowTShift, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_12(        pppConsole_Registers,                                                                          // ������� - ������� - �������� ///
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Console_SizeSettings(int x, int y)
{
    char buffer[30];
    sprintf(buffer, "����.�����. %d", sizeof(Settings));
    painter.DrawText(x + 6, y + 13, buffer, gColorBack);
}

DEF_BUTTON(     bConsole_SizeSettings,                                                                   //--- ������� - ������� - ������ �������� ---
    "", "",
    "���������� ������� ������ ��������� ��� ���������� ��������",
    "Displays the current size of the structure to save settings",
    ppConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         ppConsole,                                                                                                // ������� - ������� ///
    "�������", "CONSOLE",
    "",
    "",
    gConsole_NumStrings,        // ������� - ������� - ����� �����
    cConsole_SizeFont,          // ������� - ������� - ������ ������
    cConsole_ModeStop,          // ������� - ������� - ���. ��������
    pppConsole_Registers,       // ������� - ������� - ��������
    bConsole_SizeSettings,      // ������� - ������� - ������ ��������
    Page_Debug_Console, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);
}

DEF_CHOICE_3(       cADC_Balance_Mode,                                                                        //--- ������� - ��� - ������ - ����� ---
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual",
    NRST_BALANCE_ADC_TYPE, pppADC_Balance, FuncActive, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_Balance_ShiftAB(void)
{
    return NRST_BALANCE_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Balance_ShiftA(void)
{
    NRST_BALANCE_ADC_A = shiftADCA;
}

DEF_GOVERNOR(       gADC_Balance_ShiftA,                                                                 //--- ������� - ��� - ������ - �������� 1 ---
    "�������� 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftA, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Balance_ShiftB(void)
{
    NRST_BALANCE_ADC_B = shiftADCB;
}

DEF_GOVERNOR(       gADC_Balance_ShiftB,                                                                 //--- ������� - ��� - ������ - �������� 2 ---
    "�������� 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftB, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         pppADC_Balance,                                                                                      // ������� - ��� - ������ ///
    "������", "BALANCE",
    "",
    "",
    cADC_Balance_Mode,      // ������� - ��� - ������ - �����
    gADC_Balance_ShiftA,    // ������� - ��� - ������ - �������� 1
    gADC_Balance_ShiftB,    // ������� - ��� - ������ - �������� 2
    Page_Debug_ADC_Balance, &ppADC, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

DEF_CHOICE_3(       cADC_Stretch_Mode,                                                                      //--- ������� - ��� - �������� - ����� ---
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual",
    NRST_STRETCH_ADC_TYPE, pppADC_Stretch, FuncActive, OnChanged_ADC_Stretch_Mode, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_StretchAB(void)
{
    return NRST_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_A(void)
{
    NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE) = stretchA;
}

DEF_GOVERNOR(       gADC_Stretch_A,                                                                   //--- ������� - ��� - �������� - �������� 1� ---
    "�������� 1�", "Stretch 1ch",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    "Sets the manual stretching of the first channel.\n1 = 0.0001",
    stretchA, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Stretch_B(void)
{
    NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE) = stretchB;
}

DEF_GOVERNOR(       gADC_Stretch_B,                                                                   //--- ������� - ��� - �������� - �������� 2� ---
    "�������� 2�", "Stretch 2ch",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    "Sets the manual stretching of the second channel.\n1 = 0.0001",
    stretchB, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak20mV,                                                               //--- ������� - ��� - �������� - 20��/1� 1� ---
    "20��/1� 1�", "20mV/1V 1k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak50mV,                                                                  //--- ������� - ��� - �������� - 50�� 1� ---
    "50�� 1�", "50mV 1k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak100mV,                                                             //--- ������� - ��� - �������� - 100��/5� 1� ---
    "100��/5� 1�", "100mV/5V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_100mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak2V,                                                                      //--- ������� - ��� - �������� - 2� 1� ---
    "2� 1�", "2V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk20mV,                                                               //--- ������� - ��� - �������� - 20��/1� 2� ---
    "20��/1� 2�", "20mV/1V 2k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk50mV,                                                         //--- ������� - ��� - �������� - ��� ���� 50�� 2� ---
    "50�� 2�", "50mV 2k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk100mV,                                                             //--- ������� - ��� - �������� - 100��/5� 2� ---
    "100��/5� 2�", "100mV/5V 2k",
    "",
    "",
    NRST_ADD_STRETCH_100mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk2V,                                                                      //--- ������� - ��� - �������� - 2� 2� ---
    "2� 2�", "2V 2ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Choice emptyChoice = {Item_Choice, 0, false, Page_NoPage, 0, FuncActive};

DEF_PAGE_15(        pppADC_Stretch,                                                                                    // ������� - ��� - �������� ///
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

DEF_BUTTON(         bADC_Shift_Reset,                                                                        //-- ������� - ��� - ��� ���� - ����� ---
    "�����", "Reset",
    "",
    "",
    pppADC_Shift, FuncActive, OnPress_ADC_Shift_Reset, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_A(void)
{
    FPGA_SetRShift(A, SET_RSHIFT_A);
}

DEF_GOVERNOR(       gADC_Shift_A2mV,                                                               //--- ������� - ��� - ��� ���� - �� 1� 2�� ���� ---
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_B(void)
{
    FPGA_SetRShift(B, SET_RSHIFT_B);
}

DEF_GOVERNOR(       gADC_Shift_B2mV,                                                               //--- ������� - ��� - ��� ���� - �� 2� 2�� ���� ---
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_A5mV,                                                               //--- ������� - ��� - ��� ���� - �� 1� 5�� ���� ---
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_B5mV,                                                               //--- ������� - ��� - ��� ���� - �� 2� 5�� ���� ---
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_A10mV,                                                             //--- ������� - ��� - ��� ���� - �� 1� 10�� ���� ---
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Shift_B10mV,                                                             //--- ������� - ��� - ��� ���� - �� 2� 10�� ���� ---
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         pppADC_Shift,                                                                                      // ������� - ��� - ��� ���� ///
    "��� ����", "ADD RSHFIT",
    "",
    "",
    bADC_Shift_Reset,   // ������� - ��� - ��� ���� - �����
    gADC_Shift_A2mV,    // ������� - ��� - ��� ���� - �� 1� 2�� ����
    gADC_Shift_B2mV,    // ������� - ��� - ��� ���� - �� 2� 2�� ����
    gADC_Shift_A5mV,    // ������� - ��� - ��� ���� - �� 1� 5�� ����
    gADC_Shift_B5mV,    // ������� - ��� - ��� ���� - �� 2� 5�� ����
    gADC_Shift_A10mV,   // ������� - ��� - ��� ���� - �� 1� 10�� ����
    gADC_Shift_B10mV,   // ������� - ��� - ��� ���� - �� 2� 10�� ����
    Page_Debug_ADC_Shift, &ppADC, FuncActive, EmptyPressPage
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         ppADC,                                                                                                        // ������� - ��� ///
    "���", "ADC",
    "",
    "",
    pppADC_Balance, // ������� - ��� - ������
    pppADC_Stretch, // ������� - ��� - ��������
    pppADC_Shift,   // ������� - ��� - ��� ����
    Page_Debug_ADC, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gRand_NumAverage,                                                                           //--- ������� - ����-��� - ������. ---
    "������.", "Average",
    "",
    "",
    NRST_NUM_AVE_FOR_RAND, 1, 32, ppRand, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gRand_NumSmooth,                                                                        //--- ������� - ����-��� - ����������� ---
    "�����������", "Smoothing",
    "",
    "",
    NRST_NUM_SMOOTH_FOR_RAND, 1, 10, ppRand, FuncActive, FuncChanged, FuncBeforeDraw
);

static void OnChanged_Rand_NumMeasures(void)
{
    FPGA_SetNumberMeasuresForGates(NUM_MEASURES_FOR_GATES);
}

DEF_GOVERNOR(       gRand_NumMeasures,                                                                     //--- ������� - ����-��� - ���-�/������ ---
    "���-�/������", "Samples/gates",
    "",
    "",
    NUM_MEASURES_FOR_GATES, 1, 2500, ppRand, FuncActive, OnChanged_Rand_NumMeasures, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cRand_ShowInfo,                                                                          //--- ������� - ����-��� - ���������� ---
    "����������", "Information",
    "���������� ���������� � ������� �������������",
    "To show information on randomizer gate",
    "�� ����������", "Hide",
    "����������", "Show",
    SHOW_RAND_INFO, ppRand, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       gRand_ShowStat,                                                                          //--- ������� - ����-��� - ���������� ---
    "����������", "Statistics",
    "���������� ������ ����������",
    "Statistics show schedule",
    "�� ����������", "Hide",
    "����������", "Show",
    SHOW_RAND_STAT, ppRand, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_TimeCompensation(void)
{
    FPGA_SetDeltaTShift(TIME_COMPENSATION);
}

DEF_GOVERNOR(   gRand_TimeCompensation,                                                            //--- ������� - ����-��� - ����������� �������� ---
    "����������� ��������", "Compenstaion time",
    "���������� ����������� �������� ��� 40 ��",
    "",
    TIME_COMPENSATION, 0, 510, ppRand, FuncActive, OnChanged_Rand_TimeCompensation, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_AddTimeShift(void)
{
    FPGA_SetTShift(SET_TSHIFT);
}

int16 addShift = 0;

DEF_GOVERNOR(       gRand_AddTimeShift,                                                                        //--- ������� - ����-��� - �������� ---
    "��� ��������", "Add shift",
    "���������� ������� ��� �������� tShift",
    "",
    addShift, -100, 100, ppRand, FuncActive, OnChanged_Rand_AddTimeShift, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_Pretriggered(void)
{
    LoadTShift();
}

DEF_GOVERNOR(       gRand_Pretriggered,                                                                      //--- ������� - ����-��� - ���������� ---
    "����������", "Pretiggered",
    "�������� �����������, ������� ������� � ������������",
    "",
    PRETRIGGERED, 0, 30000, ppRand, FuncActive, OnChanged_Rand_Pretriggered, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_8(         ppRand,                                                                                                  // ������� - ����-��� ///
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
    gRand_Pretriggered,     // ������� - �A��-��� - ����������
    Page_Debug_Rand, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Channels_BandwidthA(bool)
{
    FPGA_SetBandwidth(A);
}

DEF_CHOICE_7(       cChannels_BandwidthA,                                                                        //--- ������� - ������ - ������ 1 ---
    "������ 1", "Bandwidth 1",
    "����� ����� ������� ������, ������� ����� ����������� � �����1-������ ��� ������ �������� ������",
    "Here you can select the bandwidth, which will operate in CHANNEL1-Bandwidth when set to Full",
    "������", "Full",
    "20���", "20MHz",
    "100���", "100MHz",
    "200���", "200MHz",
    "350���", "350MHz",
    "650���", "650MHz",
    "750���", "750MHz",
    BANDWIDTH_DEBUG(A), ppChannels, FuncActive, OnChanged_Channels_BandwidthA, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Channels_BandwidthB(bool)
{
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_7(       cChannels_BandwidthB,                                                                        //--- ������� - ������ - ������ 2 ---
    "������ 2", "Bandwidth 2",
    "����� ����� ������� ������, ������� ����� ����������� � �����2-������ ��� ������ �������� ������",
    "Here you can select the bandwidth, which will operate in CHANNEL2-Bandwidth when set to Full",
    "������", "Full",
    "20���",  "20MHz",
    "100���", "100MHz",
    "200���", "200MHz",
    "350���", "350MHz",
    "650���", "650MHz",
    "750���", "750MHz",
    BANDWIDTH_DEBUG(B), ppChannels, FuncActive, OnChanged_Channels_BandwidthB, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppChannels,                                                                                                // ������� - ������ ///
    "������", "CHANNELS",
    "",
    "",
    cChannels_BandwidthA,   // ������� - ������ - ������ 1
    cChannels_BandwidthB,   // ������� - ������ - ������ 2
    Page_Debug_Channels, &pDebug, FuncActive, EmptyPressPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cStats,                                                                                             //--- ������� - ���������� ---
    "����������", "Statistics",
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ "
    "��������",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "�� ����������", "Hide",
    "����������",    "Show",
    SHOW_STAT, pDebug, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnChanged_DisplayOrientation(bool)
{
    display.SetOrientation(DISPLAY_ORIENTATION);
}

DEF_CHOICE_2(       cDisplayOrientation,                                                                                //--- ������� - ���������� ---
    "����������", "DisplayOrientation",
    "������������� ���������� �������",
    "Sets display orientation",
    "������",   "Direct",
    "��������", "Back",
    DISPLAY_ORIENTATION, pDebug, FuncActive, OnChanged_DisplayOrientation, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_EMS(bool)
{
    FPGA_SetBandwidth(A);
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_2(       cEMS,                                                                                                //--- ������� - ����� ��� ---
    "����� ���", "EMS mode",
    "������������� �������� ������ 20���, ����������� �� 4-� ������, ���������� �� 8-�� ������",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    MODE_EMS, pDebug, FuncActive, OnChanged_EMS, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int16 pred;

static void OnChanged_Pred(void)
{
    gPred = ~pred;
    static char buffer[30];
    LOG_WRITE("pred %d %s", pred, Hex16toString((uint16)gPred, buffer));
}

DEF_GOVERNOR(       mgPred,                                                                                             //--- ������� - ���������� ---
    "����������", "",
    "", "",
    pred, 0, 15000, pDebug, FuncActive, OnChanged_Pred, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int16 post;

static void OnChanged_Post(void)
{
    gPost = (uint16)~post;
    static char buffer[30];
    LOG_WRITE("post %d %s", post, Hex16toString(gPost, buffer));
}

DEF_GOVERNOR(       mgPost,                                                                                            //--- ������� - ����������� ---
    "�����������", "",
    "", "",
    post, 0, 15000, pDebug, FuncActive, OnChanged_Post, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Settings_Exit(void)
{
    display.SetDrawMode(DrawMode_Auto, 0);
}

DEF_SMALL_BUTTON_EXIT(  bSettings_Exit,                                                                          //--- ������� - ��������� - ����� ---
    ppSettings, FuncActive, OnPress_Settings_Exit, DrawSB_Exit
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    menu.Draw();
    painter.EndScene();
}

static void OnPress_Settings(void)
{
    display.SetDrawMode(DrawMode_Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_SB(        ppSettings,                                                                                             // ������� - ��������� ///
    "���������", "SETTINGS",
    "�������� ���������� � ����������",
    "Show settings information",
    &bSettings_Exit,            // ������� - ��������� - �����
    0,
    0,
    0,
    0,
    0,
    PageSB_Debug_Settings, &pDebug, FuncActive, OnPress_Settings, FuncDrawPage, FuncRegSetPage
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

DEF_BUTTON(         bSaveFirmware,                                                                                  //--- ������� - ����. �������� ---
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    pDebug, IsActive_SaveFirmware, OnPress_SaveFirmware, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Exit(void)
{
    OnPressSB_Exit();
    FREE_EXTRAMEM();
}

DEF_SMALL_BUTTON_EXIT(  bSerialNumber_Exit,                                                                            //--- ������� - �/� - ����� ---
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

DEF_SMALL_BUTTON(   bSerialNumber_Change,                                                                           //--- ������� - �/� - �������� ---
    "��������", "Insert",
    "��������� �������� ������",
    "Inserts the chosen symbol",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

DEF_SMALL_BUTTON(   bSerialNumber_Save,                                                                            //--- ������� - �/� - ��������� ---
    "���������", "Save",
    "���������� �������� ����� � OTP",
    "Records the serial number in OTP",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    painter.DrawFormText(x0 + deltaX, y0 + 130, gColorFill, "������� ����������� ����� %s", buffer[0] == 0 ? "-- ----" : buffer);

    painter.DrawFormText(x0 + deltaX, y0 + 100, gColorFill, "�������� ����� ��� %d �������", allShots);
}

static void OnPress_SerialNumber(void)
{
    display.SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number = 01;
    s->year = 2017;
    s->curDigt = 0;
}

static void OnRegSet_SerialNumber(int angle)
{
    typedef int(*pFunc)(int *, int, int);

    pFuncVpIII p = (angle > 0) ? (CircleIncrease<int>) : (CircleDecrease<int>);

    ACCESS_EXTRAMEM(StructForSN, s);

    if (s->curDigt == 0)
    {
        p(&s->number, 1, 99);
    }
    else
    {
        p(&s->year, 2016, 2050);
    }
    sound.GovernorChangedValue();
}

DEF_PAGE_SB(        ppSerialNumber,                                                                                               // ������� - �/� ///
    "�/�", "S/N",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    &bSerialNumber_Exit,            // ������� - �/� - �����
    &bSerialNumber_Change,          // ������� - �/� - �������
    0,
    0,
    0,
    &bSerialNumber_Save,            // ������� - �/� - ���������
    PageSB_Debug_SerialNumber, &pDebug, FuncActive, OnPress_SerialNumber, FuncDrawPage, OnRegSet_SerialNumber
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_EraseData(void)
{
    display.FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    FLASH_DeleteAllData();
    display.FuncOnWaitStop();
}

DEF_BUTTON(         bEraseData,                                                                                     //--- ������� - ������� ������ ---
    "������� �����", "Erase data",
    "������� ����������� ������ �� ����",
    "Erase all saved datas from EEPROM",
    pDebug, FuncActive, OnPress_EraseData, FuncDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_13(        pDebug,                                                                                                             // ������� ///
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
