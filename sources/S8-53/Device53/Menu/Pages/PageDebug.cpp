#include "defines.h"
#include "Menu/Pages/Definition.h"
#include "FlashDrive/FlashDrive.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGATypes.h"
#include "Hardware/FSMC.h"
#include "Hardware/Sound.h"
#include "Hardware/FLASH.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Log.h"
#include "Utils/Dictionary.h"


/** @addtogroup Menu
 *  @{
 *  @defgroup PageDebug
 *  @{
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// � ���� ��������� ����� ��������� ������ ��������� ������ ��� �������� �������� ppSerialNumer
typedef struct
{
    int number;     ///< ��������������, ���������� �����.
    int year;       ///< ��������������, ���.
    int curDigt;    ///< ��������������, ������� (0) ��� ����� (1) ��������� ����� ���������.
} StructForSN;


extern const Page mainPage;
extern const Page mpConsole_Registers;
extern const Page pDebug;
extern const Page mpConsole;
extern const Page mpADC;
extern const Page mpADC_Balance;
extern const Page mpADC_Stretch;
extern const Page mpADC_AltRShift;
extern const Page mpRandomizer;
extern const Page ppSerialNumber;

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_ShowAll, mpConsole_Registers,                          //--- ������� - ������� - �������� - ���������� ��� ---
    IS_SHOW_REGISTERS_ALL, FuncActive, FuncChangedChoice, FuncDraw,
    "���������� ���", "Show all",
    "���������� ��� ��������, ���������� � ��������",
    "To show all values transferred in registers",
    "���", "No",
    "��",  "Yes"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Console_Registers(void)
{
    return !IS_SHOW_REGISTERS_ALL;
}

DEF_CHOICE_2(       mcConsole_Registers_RD_FL,                                                          //--- ������� - ������� - �������� - RD_FL ---
    mpConsole_Registers,
    set.debug.showRegisters.flag, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_RShiftA,                                                     //--- ������� - ������� - �������� - U ��. 1� ---
    mpConsole_Registers,
    set.debug.showRegisters.rShiftA, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U ��. 1�", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_RShiftB,                                                     //--- ������� - ������� - �������� - U ��. 2� ---
    mpConsole_Registers,
    set.debug.showRegisters.rShiftB, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U ��. 2�", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_TrigLev,                                                     //--- ������� - ������� - �������� - U �����. ---
    mpConsole_Registers,
    set.debug.showRegisters.trigLev, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U �����.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_RangeA,                                                   //--- ������� - ������� - �������� - �����/��� 1 ---
    mpConsole_Registers,
    set.debug.showRegisters.range[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����/��� 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_RangeB,                                                   //--- ������� - ������� - �������� - �����/��� 2 ---
    mpConsole_Registers,
    set.debug.showRegisters.range[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����/��� 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_TrigParam,                                              //--- ������� - ������� - �������� - �����. �����. ---
    mpConsole_Registers,
    set.debug.showRegisters.trigParam, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����. �����.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_ChanParamA,                                             //--- ������� - ������� - �������� - �����. ���. 1 ---
    mpConsole_Registers,
    set.debug.showRegisters.chanParam[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����. ���. 1",  "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_ChanParamB,                                             //--- ������� - ������� - �������� - �����. ���. 2 ---
    mpConsole_Registers,
    set.debug.showRegisters.chanParam[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����. ���. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_TBase,                                                      //--- ������� - ������� - �������� - �����/��� ---
    mpConsole_Registers,
    set.debug.showRegisters.tBase, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "�����/���", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_Registers_tShift,                                                         //--- ������� - ������� - �������� - � ��. ---
    mpConsole_Registers,
    set.debug.showRegisters.tShift, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "� ��.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_12(        mpConsole_Registers,                                                                           // ������� - ������� - �������� ///
    static,
    Page_DebugShowRegisters,  &mpConsole, FuncActive, EmptyPressPage,
    "��������", "REGISTERS",
    "",
    "",
    mcConsole_Registers_ShowAll,    // ������� - ������� - �������� - ���������� ���
    mcConsole_Registers_RD_FL,      // ������� - ������� - �������� - RD_FL
    mcConsole_Registers_RShiftA,    // ������� - ������� - �������� - U ��. 1�
    mcConsole_Registers_RShiftB,    // ������� - ������� - �������� - U ��. 2�
    mcConsole_Registers_TrigLev,    // ������� - ������� - �������� - U �����.
    mcConsole_Registers_RangeA,     // ������� - ������� - �������� - �����/��� 1
    mcConsole_Registers_RangeB,     // ������� - ������� - �������� - �����/��� 2
    mcConsole_Registers_TrigParam,  // ������� - ������� - �������� - �����. �����.
    mcConsole_Registers_ChanParamA, // ������� - ������� - �������� - �����. ���. 1
    mcConsole_Registers_ChanParamB, // ������� - ������� - �������� - �����. ���. 2
    mcConsole_Registers_TBase,      // ������� - ������� - �������� - �����/���
    mcConsole_Registers_tShift      // ������� - ������� - �������� - � ��.
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int16 shiftADCA;
static int16 shiftADCB;

static void Draw_ADC_Balance_Mode(int x, int y)
{
    int8 shift[2][3] =
    {
        {0, (int8)SET_BALANCE_ADC_A, (int8)BALANCE_ADC_A},
        {0, (int8)SET_BALANCE_ADC_B, (int8)BALANCE_ADC_B}
    };

    shiftADCA = shift[0][BALANCE_ADC_TYPE];
    shiftADCB = shift[1][BALANCE_ADC_TYPE];
}

static void OnChanged_ADC_Balance_Mode(bool active)
{
    Draw_ADC_Balance_Mode(0, 0);

    fpga.WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shiftADCA, false);
    fpga.WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shiftADCB, false);
}

DEF_CHOICE_3(       mcADC_Balance_Mode,                                                                       //--- ������� - ��� - ������ - ����� ---
    mpADC_Balance,
    BALANCE_ADC_TYPE, FuncActive, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Balance_ShiftA(void)
{
    BALANCE_ADC_A = shiftADCA;
    fpga.WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)BALANCE_ADC_A, false);
}

static bool IsActive_ADC_Balance_Shift(void)
{
    return BALANCE_ADC_TYPE_IS_HAND;
}

DEF_GOVERNOR(       mgADC_Balance_ShiftA,                                                                //--- ������� - ��� - ������ - �������� 1 ---
    "�������� 1", "Offset 1",
    "",
    "",
    mpADC_Balance, shiftADCA, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftA, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Balance_ShiftB(void)
{
    BALANCE_ADC_B = shiftADCB;
    fpga.WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)BALANCE_ADC_B, false);
}

DEF_GOVERNOR(       mgADC_Balance_ShiftB,                                                                //--- ������� - ��� - ������ - �������� 2 ---
    "�������� 2", "Offset 2",
    "",
    "",
    mpADC_Balance, shiftADCB, -125, 125, IsActive_ADC_Balance_Shift, OnChanged_ADC_Balance_ShiftB, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         mpADC_Balance,                                                                                       // ������� - ��� - ������ ///
    static,
    Page_DebugADCbalance, &mpADC, FuncActive, EmptyPressPage,
    "������", "BALANCE",
    "",
    "",
    mcADC_Balance_Mode,     // ������� - ��� - ������ - �����
    mgADC_Balance_ShiftA,   // ������� - ��� - ������ - �������� 1
    mgADC_Balance_ShiftB    // ������� - ��� - ������ - �������� 2
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LoadStretchADC(Channel chan)
{
    if (DEBUG_STRETCH_ADC_TYPE_IS_DISABLED)
    {
        fpga.WriteToHardware(chan == A ? WR_CAL_A : WR_CAL_B, 0x80, true);
    }
    else if (DEBUG_STRETCH_ADC_TYPE_IS_HAND)
    {
        fpga.WriteToHardware(chan == A ? WR_CAL_A : WR_CAL_B, (uint8)DEBUG_STRETCH_ADC(chan), true);
    }
    else if (DEBUG_STRETCH_ADC_TYPE_IS_SETTINGS)
    {
        fpga.LoadKoeffCalibration(chan);
    }
}

static void OnChanged_ADC_Stretch_Mode(bool active)
{
    if (active)
    {
        LoadStretchADC(A);
        LoadStretchADC(B);
    }
}

DEF_CHOICE_3(       mcADC_Stretch_Mode,                                                                     //--- ������� - ��� - �������� - ����� ---
    mpADC_Stretch,
    DEBUG_STRETCH_ADC_TYPE, FuncActive, OnChanged_ADC_Stretch_Mode, FuncDraw,
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_Stretch_ADC(void)
{
    return DEBUG_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_ADC_A(void)
{
    fpga.WriteToHardware(WR_CAL_A, (uint8)DEBUG_STRETCH_ADC_A, true);
}

DEF_GOVERNOR(       mgADC_Stretch_ADC_A,                                                                //--- ������� - ��� - �������� - �����. 1� ---
    "�����. 1�", "Koeff. 1ch",
    "",
    "",
    mpADC_Stretch, DEBUG_STRETCH_ADC_A, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_A, FuncBeforeDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Stretch_ADC_B(void)
{
    fpga.WriteToHardware(WR_CAL_B, (uint8)DEBUG_STRETCH_ADC_B, true);
}

DEF_GOVERNOR(       mgADC_Stretch_ADC_B,                                                                //--- ������� - ��� - �������� - �����. 2� ---
    "�����. 2�", "Koeff. 2ch",
    "",
    "",
    mpADC_Stretch, DEBUG_STRETCH_ADC_B, 0, 255, IsActive_ADC_Stretch_ADC, OnChanged_ADC_Stretch_ADC_B, FuncBeforeDraw
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         mpADC_Stretch,                                                                                     // ������� - ��� - �������� ///
    static,
    Page_DebugADCstretch, &mpADC, FuncActive, EmptyPressPage,
    "��������", "STRETCH",
    "",
    "",
    mcADC_Stretch_Mode,  // ������� - ��� - �������� - �����
    mgADC_Stretch_ADC_A, // ������� - ��� - �������� - �����. 1�
    mgADC_Stretch_ADC_B  // ������� - ��� - �������� - �����. 2�
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ADC_AltRShift_Reset(void)
{
    for (int chan = 0; chan < 2; chan++)
    {
        for (int mode = 0; mode < 2; mode++)
        {
            for (int range = 0; range < RangeSize; range++)
            {
                set.chan[chan].rShiftAdd[range][mode] = 0;
            }
        }
    }
    fpga.SetRShift(A, SET_RSHIFT_A);
    fpga.SetRShift(B, SET_RSHIFT_B);
}

DEF_BUTTON(         mbADC_AltRShift_Reset,                                                                  //--- ������� - ��� - ��� ���� - ����� ---
    "�����", "Reset",
    "", "",
    mpADC_AltRShift, FuncActive, OnPress_ADC_AltRShift_Reset, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_AltRShift_A(void)
{
    fpga.SetRShift(A, SET_RSHIFT_A);
}

DEF_GOVERNOR(       mbADC_AltRShift_2mV_DC_A,                                                      //--- ������� - ��� - ��� ���� - �� 1� 2�� ���� ---
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    mpADC_AltRShift, set.chan[A].rShiftAdd[Range_2mV][ModeCouple_DC], -100, 100,
    FuncActive, OnChanged_ADC_AltRShift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_AltRShift_B(void)
{
    fpga.SetRShift(B, SET_RSHIFT_B);
}

DEF_GOVERNOR(       mbADC_AltRShift_2mV_DC_B,                                                      //--- ������� - ��� - ��� ���� - �� 2� 2�� ���� ---
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    mpADC_AltRShift, set.chan[B].rShiftAdd[Range_2mV][ModeCouple_DC], -100, 100,
    FuncActive, OnChanged_ADC_AltRShift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mbADC_AltRShift_5mV_DC_A,                                                      //--- ������� - ��� - ��� ���� - �� 1� 5�� ���� ---
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    mpADC_AltRShift, set.chan[A].rShiftAdd[Range_5mV][ModeCouple_DC], -100, 100,
    FuncActive, OnChanged_ADC_AltRShift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mbADC_AltRShift_5mV_DC_B,                                                         // ������� - ��� - ��� ���� - �� 2� 5�� ���� ---
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    mpADC_AltRShift, set.chan[B].rShiftAdd[Range_5mV][ModeCouple_DC], -100, 100,
    FuncActive, OnChanged_ADC_AltRShift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mbADC_AltRShift_10mV_DC_A,                                                       // ������� - ��� - ��� ���� - �� 1� 10�� ���� ---
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    mpADC_AltRShift, set.chan[A].rShiftAdd[Range_10mV][ModeCouple_DC], -100, 100,
    FuncActive, OnChanged_ADC_AltRShift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mbADC_AltRShift_10mV_DC_B,                                                       // ������� - ��� - ��� ���� - �� 2� 10�� ���� ---
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    mpADC_AltRShift, set.chan[B].rShiftAdd[Range_10mV][ModeCouple_DC], -100, 100,
    FuncActive, OnChanged_ADC_AltRShift_B, FuncBeforeDraw
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7(         mpADC_AltRShift,                                                                                  /// ������� - ��� - ��� ���� ///
    static,
    Page_DebugADCaltShift, &mpADC, FuncActive, EmptyPressPage,
    "��� ����", "ADD RSHFIT",
    "",
    "",
    mbADC_AltRShift_Reset,     // ������� - ��� - ��� ���� - �����
    mbADC_AltRShift_2mV_DC_A,  // ������� - ��� - ��� ���� - �� 1� 2�� ����
    mbADC_AltRShift_2mV_DC_B,  // ������� - ��� - ��� ���� - �� 2� 2�� ����
    mbADC_AltRShift_5mV_DC_A,  // ������� - ��� - ��� ���� - �� 1� 5�� ����
    mbADC_AltRShift_5mV_DC_B,  // ������� - ��� - ��� ���� - �� 2� 5�� ����
    mbADC_AltRShift_10mV_DC_A, // ������� - ��� - ��� ���� - �� 1� 10�� ����
    mbADC_AltRShift_10mV_DC_B  // ������� - ��� - ��� ���� - �� 2� 10�� ����
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         mpADC,                                                                                                        // ������� - ��� ///
    static,
    Page_DebugADC, &pDebug, FuncActive, EmptyPressPage,
    "���", "ADC",
    "",
    "",
    mpADC_Balance,  // ������� - ��� - ������
    mpADC_Stretch,  // ������� - ��� - ��������
    mpADC_AltRShift // ������� - ��� - ��� ����
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Randomizer_SamplesForGates(void)
{
    fpga.SetNumberMeasuresForGates(NUM_MEAS_FOR_GATES);
}

DEF_GOVERNOR(       mgRandomizer_SamplesForGates,                                                             // ������� - ����-��� - ���-�/������ ---
    "���-�/������", "Samples/gates",
    "",
    "",
    mpRandomizer, NUM_MEAS_FOR_GATES, 1, 2500,
    FuncActive, OnChanged_Randomizer_SamplesForGates, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Randomizer_AltTShift0(void)
{
    fpga.SetDeltaTShift(ADD_SHIFT_T0);
}

DEF_GOVERNOR(       mgRandomizer_AltTShift0,                                                                   // ������� - ����-��� - tShift ���. ---
    "tShift ���.", "tShift alt.",
    "",
    "",
    mpRandomizer, ADD_SHIFT_T0, 0, 510,
    FuncActive, OnChanged_Randomizer_AltTShift0, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mgRandomizer_Average,                                                                          // ������� - ����-��� - ������. ---
    "������.", "Average",
    "",
    "",
    mpRandomizer, NUM_AVE_FOR_RAND, 1, 32,
    FuncActive, FuncChanged, FuncBeforeDraw
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         mpRandomizer,                                                                                            // ������� - ����-��� ///
    static,
    Page_DebugRandomizer, &pDebug, FuncActive, EmptyPressPage,
    "����-���", "RANDOMIZER",
    "",
    "",
    mgRandomizer_SamplesForGates, // ������� - ����-��� - ���-�/������
    mgRandomizer_AltTShift0,      // ������� - ����-��� - tShift ���.
    mgRandomizer_Average          // ������� - ����-��� - ������.
);





//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_SizeSettings(int x, int y)
{
    painter.DrawFormatText(x + 5, y + 21, Color::BLACK, "������ %d", sizeof(Settings));
}

static int8 temp = 0;

DEF_CHOICE_2(       mcSizeSettings, pDebug,                                                                          // ������� - ������ ��������  ---
    temp, FuncActive, FuncChangedChoice, OnDraw_SizeSettings,
    "������ ��������", "Size settings",
    "����� ������� ��������� Settings",
    "Show size of struct Settings",
    "������", "Size",
    "������", "Size"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SaveFirmware(void)
{
    return gBF.flashDriveIsConnected;
}

static void OnPress_SaveFirmware(void)
{
    StructForWrite structForWrite;

    flashDrive.OpenNewFileForWrite("S8-53.bin", &structForWrite);

    uint8 *address = (uint8*)0x08020000;
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        flashDrive.WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    flashDrive.CloseFile(&structForWrite);

    display.ShowWarningGood(FirmwareSaved);
}

DEF_BUTTON(         mbSaveFirmware,                                                                                    // ������� - ����. �������� ---
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    pDebug, IsActive_SaveFirmware, OnPress_SaveFirmware, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Exit(void)
{
    display.RemoveAddDrawFunction();
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
    ppSerialNumber,  FuncActive, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Save(void)
{
    ACCESS_EXTRAMEM(StructForSN, s);

    char stringSN[20];

    snprintf(stringSN, 19, "%02d %04d", s->number, s->year);

    if (!OTP_SaveSerialNumber(stringSN))
    {
        display.ShowWarningBad(FullyCompletedOTP);
    }
}

static void Draw_SerialNumber_Save(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   bSerialNumber_Save,                                                                               // ������� - �/� - ��������� ---
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

    painter.DrawRectangle(x0, y0, width, height, Color::Fill());
    painter.FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, Color::Back());

    int deltaX = 10;

    ACCESS_EXTRAMEM(StructForSN, s);

    bool selNumber = s->curDigt == 0;

    char buffer[20];
    snprintf(buffer, 19, "%02d", s->number);

    Color colorText = Color::Fill();
    Color colorBackground = Color::Back();

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
        colorText = Color::Fill();
        colorBackground = Color::Back();
    }

    snprintf(buffer, 19, "%04d", s->year);

    painter.SetColor(colorText);
    painter.DrawTextOnBackground(x + 5, y, buffer, colorBackground);

    // ������ ������� ���������� �� ���������� ����� � OTP-������ ��� ������

    int allShots = OTP_GetSerialNumber(buffer);

    painter.SetColor(Color::Fill());

    painter.DrawFormatText(x0 + deltaX, y0 + 130, "������� ���������� ����� %s", buffer[0] == 0 ? "-- ----" : buffer);

    painter.DrawFormatText(x0 + deltaX, y0 + 100, "�������� ����� ��� %d �������", allShots);
}

static void OnPress_SerialNumber()
{
    menu.OpenPageAndSetItCurrent(PageSB_Debug_SerialNumber);
    display.SetAddDrawFunction(Draw_EnterSerialNumber);
    MALLOC_EXTRAMEM(StructForSN, s);
    s->number = 01;
    s->year = 2017;
    s->curDigt = 0;
}

static void OnRegSet_SerialNumber(int angle)
{
    typedef int(*pFunc)(int*, int, int);

    typedef void (Math::*pFuncRIII)(int&, int, int);

    pFuncVpIII p = angle > 0 ? CircleIncrease<int> : CircleDecrease<int>;

    ACCESS_EXTRAMEM(StructForSN, s);

    if (s->curDigt == 0)
    {
        p(&s->number, 1, 99);
    }
    else
    {
        p(&s->year, 2014, 2050);
    }
    sound.GovernorChangedValue();
}

DEF_PAGE_SB(        ppSerialNumber,                                                                                               // ������� - �/� ///
    PageSB_Debug_SerialNumber, &pDebug, FuncActive,
    OnPress_SerialNumber, FuncDrawPage, OnRegSet_SerialNumber,
    "�/�", "S/N",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    &bSerialNumber_Exit,     // ������� - �/� - �����
    &bSerialNumber_Change,   // ������� - �/� - �������
    0,
    0,
    0,
    &bSerialNumber_Save      // ������� - �/� - ���������
);



//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcStats,                                                                                            //--- ������� - ���������� ---
    pDebug,
    SHOW_STATS, FuncActive, FuncChangedChoice, FuncDraw,
    "����������", "Statistics"
    ,
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ "
    "��������"
    ,
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "�� ����������", "Hide",
    "����������", "Show"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       mgConsole_NumStrings,                                                                    //--- ������� - ������� - ����� ����� ---
    "����� �����", "Number strings",
    "",
    "",
    mpConsole, NUM_STRINGS, 0, 33, FuncActive, FuncChanged, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcConsole_SizeFont,                                                                       // ������� - ������� - ������ ������ ---
    mpConsole,
    SIZE_FONT_CONSOLE, FuncActive, FuncChangedChoice, FuncDraw,
    "������ ������", "Size font",
    "",
    "",
    "5", "5",
    "8", "8"
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3(         mpConsole,                                                                                                // ������� - ������� ///
    static,
    Page_DebugConsole, &pDebug, FuncActive, EmptyPressPage,
    "�������", "CONSOLE",
    "",
    "",
    mgConsole_NumStrings, // ������� - ������� - ����� �����
    mcConsole_SizeFont,   // ������� - ������� - ������ ������
    mpConsole_Registers   // ������� - ������� - ��������
);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_6(         pDebug,                                                                                                             // ������� ///
    ,
    Page_Debug, &mainPage, FuncActive, EmptyPressPage,
    "�������", "DEBUG",
    "",
    "",
    mcStats,        // ������� - ����������
    mpConsole,      // ������� - �������
    mpADC,          // ������� - ���
    mpRandomizer,   // ������� - ����-���
    mcSizeSettings, // ������� - ������ ��������
    mbSaveFirmware  // ������� - ����. ��������
);

/** @}  @}
 */
