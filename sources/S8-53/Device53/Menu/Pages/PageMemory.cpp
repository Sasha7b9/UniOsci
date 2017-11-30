#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "PageMemory.h"
#include "Menu/Pages/Definition.h"
#include "../FileManager.h"
#include "FPGA/FPGA.h"
#include "FPGA/DataStorage.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/font/Font.h"
#include "Display/Painter.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/CommonFunctions.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/FLASH.h"
#include "Hardware/Sound.h"
#include "Log.h"
#include "Tables.h"
#include "Utils/Dictionary.h"
#include <string.h>
#include <stdio.h>
#include "stub.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageMemory
 *  @{
 */


extern const Page pMemory;
extern const Page mainPage;
extern const Page mspMemLast;
extern const Page mspMemInt;
extern const Page mpSetName;
extern const Page mspSetMask;
extern const Page mspFileManager;
extern const Page mpSetName;
extern const Page mspMemoryExt;

extern void PressSB_SetName_Exit();

static void DrawSetMask();  // ��� ������� ������, ����� ������ ����� ������� �����.
static void DrawSetName();  // ��� ������� ������, ����� ����� ������ ��� ����� ��� ����������


void ChangeC_Memory_NumPoints(bool active)
{
    if(sMemory_GetNumPoints(false) == 281)
    {
        SHIFT_IN_MEMORY = 0;
    }
    else
    {
        if(TPOS_IS_LEFT)
        {
            SHIFT_IN_MEMORY = 0;
        }
        else if(TPOS_IS_CENTER)
        {
            SHIFT_IN_MEMORY = sMemory_GetNumPoints(false) / 2 - grid.Width() / 2;
        }
        else if(TPOS_IS_RIGHT)
        {
            SHIFT_IN_MEMORY = sMemory_GetNumPoints(false) - grid.Width() - 2;
        }
    }
    fpga.SetTShift(TSHIFT);
}

// ������� �� ������ - ����� �� - �����
bool IsActiveMemoryExtSetMask()
{                       
    return FILE_NAMING_MODE_IS_MASK;
}


void DrawSB_MemLastSelect(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 3, y + 2, set.memory.strMemoryLast.isActiveModeSelect ? '\x2a' : '\x28');
    painter.SetFont(TypeFont_8);
}

void DrawSB_MemLast_Prev(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x20');
    painter.SetFont(TypeFont_8);
}

void DrawSB_MemLast_Next(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x64');
    painter.SetFont(TypeFont_8);
}

void PressSB_MemLastSelect()
{
    set.memory.strMemoryLast.isActiveModeSelect = !set.memory.strMemoryLast.isActiveModeSelect;
}

void PressSB_MemLast_Next()
{
    CircleIncrease<int16>(&gMemory.currentNumLatestSignal, 0, dataStorage.AllDatas() - 1);
}

void PressSB_MemLast_Prev()
{
    CircleDecrease<int16>(&gMemory.currentNumLatestSignal, 0, dataStorage.AllDatas() - 1);
}

static void RotateSB_MemLast(int angle)
{
    if (dataStorage.AllDatas() > 1)
    {
        sound.RegulatorSwitchRotate();
    }
    if (Sign(angle) > 0)
    {
        PressSB_MemLast_Next();
    }
    else
    {
        PressSB_MemLast_Prev();
    }
}

static void DrawSB_MemExtSetNameSave(int x, int y)
{
    if (gBF.flashDriveIsConnected == 1)
    {
        painter.SetFont(TypeFont_UGO2);
        painter.Draw4SymbolsInRect(x + 2, y + 1, '\x42');
        painter.SetFont(TypeFont_8);
    }
}

static void PressSB_MemExtSetNameSave()
{
    if (gBF.flashDriveIsConnected == 1)
    {
        PressSB_SetName_Exit();
        gMemory.needForSaveToFlashDrive = 1;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbMemLastPrev,                                                                           //--- ������ - ��������� - ���������� ---
    "����������", "Previous",
    "������� � ����������� �������",
    "Go to the previous signal",
    mspMemLast, FuncActive, PressSB_MemLast_Prev, DrawSB_MemLast_Prev

);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbMemLastNext,                                                                            //--- ������ - ��������� - ��������� ---
    "���������", "Next",
    "������� � ���������� �������",
    "Go to the next signal",
    mspMemLast, FuncActive, PressSB_MemLast_Next, DrawSB_MemLast_Next
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PressSB_MemLast_IntEnter()
{
    menu.OpenPageAndSetItCurrent(PageSB_Memory_Internal);
    MODE_WORK = ModeWork_ROM;
    FLASH_GetData(gMemory.currentNumIntSignal, &gDSmemInt, &gData0memInt, &gData1memInt);
    gMemory.exitFromIntToLast = 1;
}

void DrawSB_MemLast_IntEnter(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x40');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbMemLastIntEnter,                                                                         //--- ������ - ��������� - ����� �� ---
    "����� ��", "Internal storage",
    "������� ��� ������, ����� ��������� ������ �� ���������� ������������ ����������",
    "Press this button to keep a signal in an internal memory",
    mspMemLast, FuncActive, PressSB_MemLast_IntEnter, DrawSB_MemLast_IntEnter
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_MemLast_SaveToFlash()
{
    gMemory.exitFromModeSetNameTo = RETURN_TO_LAST_MEM;
    Memory_SaveSignalToFlashDrive();
}

void DrawSB_MemLast_SaveToFlash(int x, int y)
{
    if (gBF.flashDriveIsConnected == 1)
    {
        painter.SetFont(TypeFont_UGO2);
        painter.Draw4SymbolsInRect(x + 2, y + 1, '\x42');
        painter.SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON(   sbMemLastSaveToFlash,                                                                     //--- ������ - ��������� - ��������� ---
    "���������", "Save",
    "������ ���������� �������� ��� ������������� ������� ��. ��������� ��������� ������ �� ������ ��",
    "Click this button to save the signal on the external FLASH",
    mspMemLast, FuncActive, PressSB_MemLast_SaveToFlash, DrawSB_MemLast_SaveToFlash
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbSetNameSave,
    "���������", "Save",
    "���������� �� ���� ��� �������� ������",
    "Saving to flashdrive with the specified name",
    mpSetName, FuncActive, PressSB_MemExtSetNameSave, DrawSB_MemExtSetNameSave
);

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{   
    static const pFuncVpI8I8I8 func[3] =
    {
        CircleDecrease<int8>,
        CircleDecrease<int8>,
        CircleIncrease<int8>
    };

    painter.ResetFlash();
    if (INDEX_SYMBOL > maxIndex)
    {
        INDEX_SYMBOL = maxIndex - 1;
    }
    func[Sign(angle) + 1]((int8 *)&INDEX_SYMBOL, 0, maxIndex - 1);
    sound.RegulatorSwitchRotate();

}

static void OnMemExtSetMaskRegSet(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PressSB_SetMask_Backspace()
{
    int size = strlen(FILE_NAME_MASK);
    if (size > 0)
    {
        if (size > 1 && FILE_NAME_MASK[size - 2] == 0x07)
        {
            FILE_NAME_MASK[size - 2] = '\0';
        }
        else
        {
            FILE_NAME_MASK[size - 1] = '\0';
        }
    }
}

void DrawSB_SetMask_Backspace(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbSetMaskBackspace,                                                                                    //--- ����� - Backspace ---
    "Backspace", "Backspace",
    "������� ��������� �������� ������",
    "Deletes the last entered symbol",
    mspSetMask, FuncActive, PressSB_SetMask_Backspace, DrawSB_SetMask_Backspace
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_SetMask_Delete()
{
    FILE_NAME_MASK[0] = '\0';
}

static void DrawSB_SetMask_Delete(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbSetMaskDelete,                                                                                         //--- ����� - ������� ---
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered symbols",
    mspSetMask, FuncActive, PressSB_SetMask_Delete, DrawSB_SetMask_Delete
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_SetMask_Insert()
{
    int index = INDEX_SYMBOL;
    int size = strlen(FILE_NAME_MASK);
    if (size == MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        return;
    }
    if (index < 0x41)
    {
        FILE_NAME_MASK[size] = symbolsAlphaBet[index][0];
        FILE_NAME_MASK[size + 1] = '\0';
    }
    else
    {
        index -= 0x40;
        if (index == 0x07)  // ��� %nN - ��������� ������
        {
            if (size < MAX_SYMBOLS_IN_FILE_NAME - 2 && size > 0)
            {
                if (FILE_NAME_MASK[size - 1] >= 0x30 && FILE_NAME_MASK[size - 1] <= 0x39) // ���� ����� ������� �����
                {
                    FILE_NAME_MASK[size] = FILE_NAME_MASK[size - 1] - 0x30;
                    FILE_NAME_MASK[size - 1] = 0x07;
                    FILE_NAME_MASK[size + 1] = '\0';
                }
            }
        }
        else
        {
            FILE_NAME_MASK[size] = index;
            FILE_NAME_MASK[size + 1] = '\0';
        }
    }
}

static void DrawSB_SetMask_Insert(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_INSERT);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbSetMaskInsert,                                                                                        //--- ����� - �������� ---
    "��������", "Insert",
    "��������� ��������� ������",
    "Inserts the chosen symbol",
    mspSetMask, FuncActive, PressSB_SetMask_Insert, DrawSB_SetMask_Insert
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_SetName_Backspace()
{
    int size = strlen(FILE_NAME);
    if (size > 0)
    {
        FILE_NAME[size - 1] = '\0';
    }
}

static void DrawSB_SetName_Backspace(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbSetNameBackspace,                                                                                //--- ��� ����� - Backspace ---
    "Backspace", "Backspace",
    "������� ��������� ������",
    "Delete the last character",
    mpSetName, FuncActive, PressSB_SetName_Backspace, DrawSB_SetName_Backspace
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_SetName_Delete()
{
    FILE_NAME[0] = '\0';
}

static void DrawSB_SetName_Delete(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbSetNameDelete,                                                                                     //--- ��� ����� - ������� ---
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered characters",
    mpSetName, FuncActive, PressSB_SetName_Delete, DrawSB_SetName_Delete
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_SetName_Insert()
{
    int size = strlen(FILE_NAME);
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        FILE_NAME[size] = symbolsAlphaBet[INDEX_SYMBOL][0];
        FILE_NAME[size + 1] = '\0';
    }
}

static void DrawSB_SetName_Insert(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x26');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbSetNameInsert,                                                                                    //--- ��� ����� - �������� ---
    "��������", "Insert",
    "������ ��������� ������",
    "Print the next character",
    mpSetName, FuncActive, PressSB_SetName_Insert, DrawSB_SetName_Insert
);

void DrawSB_MemExtNewFolder(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 1, y, '\x46');
    painter.SetFont(TypeFont_8);
}

void DrawSB_FM_LevelDown(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    painter.SetFont(TypeFont_8);
}

void DrawSB_FM_LevelUp(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    painter.SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_FM_Tab(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x6e');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbFileManagerTab,                                                                                          //--- ������� - Tab ---
    "Tab", "Tab",
    "������� ����� ���������� � �������",
    "The transition between the directories and files",
    mspFileManager, FuncActive, PressSB_FM_Tab, DrawSB_FM_Tab
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbFileManagerLevelDown,                                                                                  //--- ������� - ����� ---
    "�����", "Enter",
    "������� � ��������� �������",
    "Transition to the chosen catalog",
    mspFileManager, FuncActive, PressSB_FM_LevelDown, DrawSB_FM_LevelDown
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbFileManagerLevelUp,                                                                                   //--- ������� - ������ ---
    "�����", "Leave",
    "������� � ������������ �������",
    "Transition to the parental catalog",
    mspFileManager, FuncActive, PressSB_FM_LevelUp, DrawSB_FM_LevelUp
);

static void SaveSignalToIntMemory(void)
{
    if (gMemory.exitFromIntToLast == 1)
    {
        if  (gDSmemLast != 0)
        {
            FLASH_SaveData(gMemory.currentNumIntSignal, gDSmemLast, gData0memLast, gData1memLast);
            FLASH_GetData(gMemory.currentNumIntSignal, &gDSmemInt, &gData0memInt, &gData1memInt);
            display.ShowWarningGood(SignalIsSaved);
        }
    }
    else
    {
        if (gDSet != 0)
        {
            FLASH_SaveData(gMemory.currentNumIntSignal, gDSet, gData0, gData1);
            FLASH_GetData(gMemory.currentNumIntSignal, &gDSet, &gData0memInt, &gData1memInt);
            display.ShowWarningGood(SignalIsSaved);
        }
    }
}

static void DrawMemoryWave(int num, bool exist)
{
    char buffer[20];
    
    int x = grid.Left() + 2 + num * 12;
    int y = grid.FullBottom() - 10;
    int width = 12;
    painter.FillRegion(x, y, width, 10, num == gMemory.currentNumIntSignal ? Color::FLASH_10 : Color::Back());
    painter.DrawRectangle(x, y, width, 10, Color::Fill());
    painter.SetColor(num == gMemory.currentNumIntSignal ? Color::FLASH_01 : Color::Fill());
    if (exist)
    {
        painter.DrawText(x + 2, y + 1, su.Int2String(num + 1, false, 2, buffer));
    }
    else
    {
        painter.DrawText(x + 3, y + 1, "\x88");
    }
}

static void FuncAdditionDrawingSPageMemoryInt()
{
    // ������ �������� ��������� ������

    bool exist[MAX_NUM_SAVED_WAVES] = {false};

    FLASH_GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave(i, exist[i]);
    }
}

static void FuncOnRegSetMemInt(int delta)
{
    sound.RegulatorSwitchRotate();
    if (delta < 0)
    {
        CircleDecrease<int8>(&gMemory.currentNumIntSignal, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    else if (delta > 0)
    {
        CircleIncrease<int8>(&gMemory.currentNumIntSignal, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    FLASH_GetData(gMemory.currentNumIntSignal, &gDSmemInt, &gData0memInt, &gData1memInt);
    painter.ResetFlash();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_MemInt_ShowSignalAlways()
{
    gMemory.showAlways = (gMemory.showAlways == 0) ? 1 : 0;
}

static void DrawSB_MemInt_ShowSignalAllways_Yes(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x66');
    painter.SetFont(TypeFont_8);
}

static void DrawSB_MemInt_ShowSignalAllways_No(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x68');
    painter.SetFont(TypeFont_8);
}

static void DrawSB_MemInt_ShowSignalAlways(int x, int y)
{
    if (gMemory.showAlways == 0)
    {
        DrawSB_MemInt_ShowSignalAllways_No(x, y);
    }
    else
    {
        DrawSB_MemInt_ShowSignalAllways_Yes(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_2(   sbMemIntShowSignalAlways,                                                  //--- ������ - ����� �� - ���������� ������ ---
    "���������� ������", "To show always",
    "��������� ������ ���������� ��������� ���������� ������ ������ ��������",
    "Allows to show always the chosen kept signal over the current",
    mspMemInt, FuncActive, PressSB_MemInt_ShowSignalAlways, DrawSB_MemInt_ShowSignalAlways,
    DrawSB_MemInt_ShowSignalAllways_Yes, "���������� ��������� ������ �� ���������� ������ ������ ��������",
                                         "to show the chosen signal from internal memory over the current",
    DrawSB_MemInt_ShowSignalAllways_No,  "������ �� ���������� ������ ����� ������ � ������ ������ � ���������� ������������ �����������",
                                         "the signal from internal memory is visible only in an operating mode with an internal memory"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_MemInt_ModeShow_Direct(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_MEM_INT_SHOW_DIRECT);
    painter.SetFont(TypeFont_8);
}

static void DrawSB_MemInt_ModeShow_Saved(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_MEM_INT_SHOW_SAVED);
    painter.SetFont(TypeFont_8);
}

static void DrawSB_MemInt_ModeShow_Both(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_MEM_INT_SHOW_BOTH);
    painter.SetFont(TypeFont_8);
}

static void PressSB_MemInt_ModeShow()
{
    CircleIncrease<int8>((int8 *)&MODE_SHOW_MEMINT, 0, 2);
}

static void DrawSB_MemInt_ModeShow(int x, int y)
{
    ModeShowIntMem mode = MODE_SHOW_MEMINT;

    if (mode == ModeShowIntMem_Direct)
    {
        DrawSB_MemInt_ModeShow_Direct(x, y);
    }
    else if (mode == ModeShowIntMem_Saved)
    {
        DrawSB_MemInt_ModeShow_Saved(x, y);
    }
    else if (mode == ModeShowIntMem_Both)
    {
        DrawSB_MemInt_ModeShow_Both(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_3(   sbMemIntModeShow,                                                                //--- ������ - ����� �� - ��� ������� ---
    "��� �������", "Type of a signal",
    "���������� ���������� ��� ������� ������ � ������ ����� ��",
    "Show recorded or current signal in mode Internal Memory",
    mspMemInt, FuncActive, PressSB_MemInt_ModeShow, DrawSB_MemInt_ModeShow,
    DrawSB_MemInt_ModeShow_Direct, "�� ������� ������� ������",     "on the display current signal",
    DrawSB_MemInt_ModeShow_Saved,  "�� ������� ���������� ������", "on the display the kept signal",
    DrawSB_MemInt_ModeShow_Both,   "�� ������� ��� �������",        "on the display the both signals"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PressSB_MemInt_SaveToIntMemory()
{
    SaveSignalToIntMemory();
}

void DrawSB_MemInt_SaveToIntMemory(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbMemIntSave,                                                                              //--- ������ - ����� �� - ��������� ---
    "���������", "Save",
    "��������� ������ �� ���������� ������������ ����������",
    "To keep a signal in an internal memory",
    mspMemInt, FuncActive, PressSB_MemInt_SaveToIntMemory, DrawSB_MemInt_SaveToIntMemory
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PressSB_MemInt_SaveToFlashDrive()
{
    gMemory.exitFromModeSetNameTo = RETURN_TO_INT_MEM;
    Memory_SaveSignalToFlashDrive();
}

void DrawSB_MemInt_SaveToFlashDrive(int x, int y)
{
    if (gBF.flashDriveIsConnected == 1)
    {
        painter.SetFont(TypeFont_UGO2);
        painter.Draw4SymbolsInRect(x + 2, y + 1, '\x42');
        painter.SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON(   sbMemIntSaveToFlash,                                                                      //--- ������ - ����� �� - ��������� ---
    "���������", "Save",
    "��������� ������ �� ������",
    "Save signal to flash drive",
    mspMemInt, FuncActive, PressSB_MemInt_SaveToFlashDrive, DrawSB_MemInt_SaveToFlashDrive
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_SetName_Exit()
{
    display.RemoveAddDrawFunction();
    if (gMemory.exitFromModeSetNameTo == RETURN_TO_DISABLE_MENU)
    {
        menu.ShortPressOnPageItem(menu.PagePointerFromName(Page_SB_MemExtSetName), 0);
    }
    else if (gMemory.exitFromModeSetNameTo == RETURN_TO_LAST_MEM)
    {
        menu.OpenPageAndSetItCurrent(PageSB_Memory_Last);
    }
    else if (gMemory.exitFromModeSetNameTo == RETURN_TO_INT_MEM)
    {
        menu.OpenPageAndSetItCurrent(PageSB_Memory_Internal);
    }
    gMemory.exitFromModeSetNameTo = RETURN_TO_DISABLE_MENU;
}

DEF_SMALL_BUTTON(   sbExitSetName,                                                                                         //--- ��� ����� - ����� ---
    "�����", "Exit",
    "����� �� ����������",
    "Failure to save",
    mpSetName, FuncActive, PressSB_SetName_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PressSB_MemInt_Exit()
{
    FLASH_GetData(gMemory.currentNumIntSignal, &gDSmemInt, &gData0memInt, &gData1memInt);
    if (gMemory.exitFromIntToLast == 1)
    {
        menu.OpenPageAndSetItCurrent(PageSB_Memory_Last);
        MODE_WORK = ModeWork_RAM;
        gMemory.exitFromIntToLast = 0;
    }
    else
    {
        menu.ShortPressOnPageItem(menu.PagePointerFromName(PageSB_Memory_Internal), 0);
    }
}

DEF_SMALL_BUTTON_EXIT(  sbExitMemInt,                                                                              //--- ������ - ����� �� - ����� ---
    mspMemInt, FuncActive, PressSB_MemInt_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressMemoryExtMask(void)
{
    menu.OpenPageAndSetItCurrent(Page_SB_MemExtSetMask);
    display.SetAddDrawFunction(DrawSetMask);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncActiveMemoryNumPoinst(void)
{
    return PEAKDET_IS_DISABLE;
}

DEF_CHOICE_3(       mcMemoryNumPoints,                                                                                        //--- ������ - ����� ---
    pMemory, ENUM_POINTS, FuncActiveMemoryNumPoinst, ChangeC_Memory_NumPoints, FuncDraw,
    "�����", "Points"
    ,
    "����� ���������� �������� ��� ����������� ��������. "
    "��� ���������� ���������� �������� ����������� ���������� ���������� � ������ ��������."
    ,
    "Choice of number of counting for the saved signals. "
    "At increase in number of counting the quantity of the signals kept in memory decreases.",
    "281",  "281",
    "512",  "512",
    "1024", "1024"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMemoryExtName,                                                                           //--- ������ - ����� �� - ��� ����� ---
    mspMemoryExt, FILE_NAMING_MODE, FuncActive, FuncChangedChoice, FuncDraw,
    "��� �����", "File name"
    ,
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������"
    ,
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually"
    ,
    "�� �����", "Mask",
    "�������",  "Manually"
);
    
//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMemoryExtAutoConnectFlash,                                                         //--- ������ - ����� �� - ��������������� ---
    mspMemoryExt, FLASH_AUTOCONNECT, FuncActive, FuncChangedChoice, FuncDraw,
    "���������.", "AutoConnect",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMemoryExtModeBtnMemory,                                                              //--- ������ - ����� �� - ��� �� ������ ---
    mspMemoryExt, MODE_BTN_MEMORY, FuncActive, FuncChangedChoice, FuncDraw,
    "��� �� ������", "Mode btn MEMORY",
    "\"Me��\" - ������� ������ ������ ��������� �������� ������\n\"����������\" - ������� ������ ������ ��������� ���� �� ������� ��",
    "\"Menu\" - pressing the ������ button opens the MEMORY page\n\"Save\"- pressing the MEMORY button saves the file to an external memory",
    "����",       "Menu",
    "����������", "Save"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcMemoryExtModeSave,                                                                   //--- ������ - ����� �� - ��������� ��� ---
    mspMemoryExt, MODE_SAVE_SIGNAL, FuncActive, FuncChangedChoice, FuncDraw,
    "��������� ���", "Save as"
    ,
    "���� ������ ������� \"�����������\", ������ ����� ������� � ������� �������� � ����������� ����� � ����������� BMP\n"
    "���� ������ ������� \"�����\", ������ ����� ������� � ������� �������� � ��������� ���� � ����� � ���������� TXT"
    ,
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension BMP\n"
    "If you select \"Text\", the signal will be stored in the current directory as a text file with the extension TXT"
    ,
    "�����������", "Image",
    "�����",       "Text"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------   
void DrawSetName()
{
    int x0 = grid.Left() + 40;
    int y0 = GRID_TOP + 60;
    int width = grid.Width() - 80;
    int height = 80;

    painter.DrawRectangle(x0, y0, width, height, Color::Fill());
    painter.FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, Color::Back());

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while (symbolsAlphaBet[index][0] != ' ')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }

    // ������ ������ ����� � ������
    position = 0;
    while (symbolsAlphaBet[index][0] != 'a')
    {
        DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while (symbolsAlphaBet[index][0] != '%')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    int x = painter.DrawText(x0 + deltaX, y0 + 65, FILE_NAME, Color::Fill());
    painter.FillRegion(x, y0 + 65, 5, 8, Color::FLASH_10);
}

static void DrawFileMask(int x, int y)
{
    char *ch = FILE_NAME_MASK;

    painter.SetColor(Color::Fill());
    while (*ch != '\0')
    {
        if (*ch >= 32)
        {
            x = painter.DrawChar(x, y, *ch);
        }
        else
        {
            if (*ch == 0x07)
            {
                x = painter.DrawChar(x, y, '%');
                x = painter.DrawChar(x, y, (char)(0x30 | *(ch + 1)));
                x = painter.DrawChar(x, y, 'N');
                ch++;
            }
            else
            {
                x = painter.DrawText(x, y, symbolsAlphaBet[*ch + 0x40]);
            }
        }
        ch++;
    }
    painter.FillRegion(x, y, 5, 8, Color::FLASH_10);
}

void DrawSetMask()
{
    int x0 = grid.Left() + 40;
    int y0 = GRID_TOP + 20;
    int width = grid.Width() - 80;
    int height = 160;

    painter.DrawRectangle(x0, y0, width, height, Color::Fill());
    painter.FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, Color::Back());

    int index = 0;
    int position = 0;
    int deltaX = 10;
    int deltaY0 = 5;
    int deltaY = 12;

    // ������ ������� ����� ����������� ��������
    while(symbolsAlphaBet[index][0] != ' ')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0);
        index++;
        position++;
    }
    
    // ������ ������ ����� � ������
    position = 0;
    while(symbolsAlphaBet[index][0] != 'a')
    {
        DrawStr(index, x0 + deltaX + 50 + position * 7, y0 + deltaY0 + deltaY);
        index++;
        position++;
    }

    // ������ ������ ����� ����� ��������
    position = 0;
    while(symbolsAlphaBet[index][0] != '%')
    {
        DrawStr(index, x0 + deltaX + position * 7, y0 + deltaY0 + deltaY * 2);
        index++;
        position++;
    }

    // ������ ������ �����������
    position = 0;
    while (index < (sizeof(symbolsAlphaBet) / 4))
    {
        DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static const char* strings[] =
    {
        "%y - ���, %m - �����, %d - ����",
        "%H - ����, %M - ������, %S - �������",
        "%nN - ���������� �����, ���",
        "n - ����������� ���������� ������ ��� N"
    };

    deltaY--;
    painter.SetColor(Color::Fill());
    for(int i = 0; i < sizeof(strings) / 4; i++)
    {
        painter.DrawText(x0 + deltaX, y0 + 100 + deltaY * i, strings[i]);
    }
}

void Memory_SaveSignalToFlashDrive()
{
    if (gBF.flashDriveIsConnected == 1)
    {
        if (FILE_NAMING_MODE_IS_HAND)
        {
            menu.OpenPageAndSetItCurrent(Page_SB_MemExtSetName);
            display.SetAddDrawFunction(DrawSetName);
        }
        else
        {
            gMemory.needForSaveToFlashDrive = 1;
        }
    }
    else
    {
        gMemory.exitFromModeSetNameTo = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_MemLast_Exit()
{
    MODE_WORK = ModeWork_Dir;
    if (gMemory.runningFPGAbeforeSmallButtons == 1)
    {
        fpga.Start();
        gMemory.runningFPGAbeforeSmallButtons = 0;
    }
    display.RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON_EXIT(  sbExitMemLast,                                                                            //--- ������ - ��������� - ����� ---
    mspMemLast, FuncActive, PressSB_MemLast_Exit, DrawSB_Exit
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPressMemoryLatest()
{
    gMemory.currentNumLatestSignal = 0;
    gMemory.runningFPGAbeforeSmallButtons = fpga.IsRunning() ? 1 : 0;
    fpga.Stop(false);
    MODE_WORK = ModeWork_RAM;
}

static void FuncDrawingAdditionSPageMemoryLast()
{
    char buffer[20];

    int width = 40;
    int height = 10;
    painter.FillRegion(grid.Right() - width, GRID_TOP, width, height, Color::Back());
    painter.DrawRectangle(grid.Right() - width, GRID_TOP, width, height, Color::Fill());
    painter.DrawText(grid.Right() - width + 2, GRID_TOP + 1, su.Int2String(gMemory.currentNumLatestSignal + 1, false, 3, buffer));
    painter.DrawText(grid.Right() - width + 17, GRID_TOP + 1, "/");
    painter.DrawText(grid.Right() - width + 23, GRID_TOP + 1, su.Int2String(dataStorage.AllDatas(), false, 3, buffer));
}

DEF_PAGE_SB(        mspMemLast,                                                                                              // ������ - ��������� ///
    PageSB_Memory_Last, &pMemory, FuncActive, OnPressMemoryLatest, FuncDrawingAdditionSPageMemoryLast, RotateSB_MemLast,
    "���������", "LATEST",
    "������� � ����� ������ � ���������� ����������� ���������",
    "Transition to an operating mode with the last received signals",
    &sbExitMemLast,
    0,
    &sbMemLastNext,
    &sbMemLastPrev,
    &sbMemLastIntEnter,
    &sbMemLastSaveToFlash
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_SetMask_Exit()
{
    display.RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON_EXIT(  sbExitSetMask,                                                                                         //--- ����� - ����� ---
    mspSetMask, FuncActive, PressSB_SetMask_Exit, DrawSB_Exit
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_SB(        mspSetMask,                                                                                       // ������ - ����� �� - ����� ///
    Page_SB_MemExtSetMask, &mspMemoryExt, IsActiveMemoryExtSetMask, OnPressMemoryExtMask, FuncDrawPage, OnMemExtSetMaskRegSet,
    "�����", "MASK",
    "����� ����� ����� ��� ��������������� ���������� ������",
    "Input mode mask for automatic file naming",
    &sbExitSetMask,
    &sbSetMaskDelete,
    0,
    0,
    &sbSetMaskBackspace,
    &sbSetMaskInsert
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_FM_Exit()
{
    display.SetDrawMode(DrawMode_Auto, 0);
    display.RemoveAddDrawFunction();
}

DEF_SMALL_BUTTON(   sbExitFileManager,                                                                                 // ������ - ������� - ����� ///
    "�����", "Exit",
    "��������� ��������� ������� � �������� �������� � �����",
    "Save the selected directory as the current one and exit",
    mspFileManager, FuncActive, PressSB_FM_Exit, DrawSB_Exit
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OnPress_Drive_Manager()
{
    menu.OpenPageAndSetItCurrent(PageSB_Memory_Drive_Manager);
    display.SetDrawMode(DrawMode_Hand, FM_Draw);
    gBF.needRedrawFileManager = 1;
}

static bool FuncOfActiveExtMemFolder()
{
    return gBF.flashDriveIsConnected == 1;
}

DEF_PAGE_SB(        mspFileManager,                                                                                            // ������ - ������� ///
    PageSB_Memory_Drive_Manager, &mspMemoryExt, FuncOfActiveExtMemFolder, OnPress_Drive_Manager, FuncDrawPage, FM_RotateRegSet,
    "�������", "DIRECTORY",
    "��������� ������ � �������� ������� ������������� ����������",
    "Provides access to the file system of the connected drive",
    &sbExitFileManager,
    &sbFileManagerTab,
    0,
    0,
    &sbFileManagerLevelUp,
    &sbFileManagerLevelDown
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_6(         mspMemoryExt,                                                                                             // ������ - ����� �� ///
    Page_MemoryExt, &pMemory, FuncActive, FuncPress, 
    "����� ��", "EXT STORAGE",
    "������ � ������� ������������ �����������.",
    "Work with external storage device.",
    mspFileManager,
    mcMemoryExtName,
    mspSetMask,
    mcMemoryExtModeSave,
    mcMemoryExtModeBtnMemory,
    mcMemoryExtAutoConnectFlash
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPressMemoryInt()
{
    menu.OpenPageAndSetItCurrent(PageSB_Memory_Internal);
    MODE_WORK = ModeWork_ROM;
    FLASH_GetData(gMemory.currentNumIntSignal, &gDSmemInt, &gData0memInt, &gData1memInt);
}

DEF_PAGE_SB(        mspMemInt,                                                                                                // ������ - ����� �� ///
    PageSB_Memory_Internal, &pMemory, FuncActive, OnPressMemoryInt, FuncAdditionDrawingSPageMemoryInt, FuncOnRegSetMemInt,
    "����� ��", "INT STORAGE",
    "������� � ����� ������ � ���������� �������",
    "Transition to an operating mode with internal memory",
    (void*)&sbExitMemInt,
    (void*)&sbMemIntShowSignalAlways,
    (void*)&sbMemIntModeShow,
    (void*)0,
    (void*)&sbMemIntSave,
    (void*)&sbMemIntSaveToFlash
);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnMemExtSetNameRegSet(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4 - 7);
}

// �������� ���������� ��� ��������� ������ ������ ������� ����� ����� ����� ����������� �� ������
DEF_PAGE_SB(        mpSetName,                                                                                                        // ��� ����� ///
    Page_SB_MemExtSetName, 0, FuncActive, FuncPress, FuncDrawPage, OnMemExtSetNameRegSet,
    "", "", "", "",
    &sbExitSetName,
    &sbSetNameDelete,
    0,
    &sbSetNameBackspace,
    &sbSetNameInsert,
    &sbSetNameSave
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4(         pMemory,                                                                                                             // ������ ///
    Page_Memory, &mainPage, FuncActive, FuncPress,
    "������", "MEMORY",
    "������ � ������� � ���������� �������.",
    "Working with external and internal memory.",
    mcMemoryNumPoints,
    mspMemLast,
    mspMemInt,
    mspMemoryExt
);


/** @}  @}
 */
