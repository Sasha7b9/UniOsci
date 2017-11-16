#include "PageMemory.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/FLASH.h"
#include "Hardware/Sound.h"
#include "Menu/FileManager.h"
#include "Menu/Menu.h"
#include "Menu/MenuFunctions.h"
#include "Menu/Pages/Definition.h"
#include "Utils/Dictionary.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void PressSB_FM_Tab(void);

extern const      Page pMemory;
extern const    Choice cPoints;                                 ///< ������ - �����
static bool    IsActive_Points(void);
       void   OnChanged_Points(bool active);
extern const     Page ppLast;                                   ///< ������ - ���������
static void     OnPress_Last(void);
static void      OnDraw_Last(void);
static void    OnRegSet_Last(int angle);
extern const   SButton bLast_Exit;                              ///< ������ - ��������� - �����
static void     OnPress_Last_Exit(void);
extern const   SButton bLast_Next;                              ///< ������ - ��������� - ���������
static void     OnPress_Last_Next(void);
static void        Draw_Last_Next(int x, int y);
extern const   SButton bLast_Prev;                              ///< ������ - ��������� - ����������
static void     OnPress_Last_Prev(void);
static void        Draw_Last_Prev(int x, int y);
extern const   SButton bLast_SaveToROM;                         ///< ������ - ��������� - ����� ��
static void     OnPress_Last_SaveToROM(void);
static void        Draw_Last_SaveToROM(int x, int y);
extern const   SButton bLast_SaveToDrive;                       ///< ������ - ��������� - ���������
static void     OnPress_Last_SaveToDrive(void);
static void        Draw_Last_SaveToDrive(int x, int y);
extern const     Page ppInternal;                               ///< ������ - ����� ��
static void     OnPress_Internal(void);
static void      OnDraw_Internal(void);
static void    OnRegSet_Internal(int delta);
extern const   SButton bInternal_Exit;                          ///< ������ - ����� �� - �����
static void     OnPress_Internal_Exit(void);
extern const   SButton bInternal_ShowAlways;                    ///< ������ - ����� �� - ���������� ������
static void     OnPress_Internal_ShowAlways(void);
static void        Draw_Internal_ShowAlways(int x, int y);
static void        Draw_Internal_ShowAlways_Yes(int x, int y);
static void        Draw_Internal_ShowAlways_No(int x, int y);
extern const   SButton bInternal_ModeShow;                      ///< ������ - ����� �� - ��� �������
static void     OnPress_Internal_ModeShow(void);
static void        Draw_Internal_ModeShow(int x, int y);
static void        Draw_Internal_ModeShow_Direct(int x, int y); 
static void        Draw_Internal_ModeShow_Saved(int x, int y);
static void        Draw_Internal_ModeShow_Both(int x, int y);
extern const   SButton bInternal_Delete;                        ///< ������ - ����� �� - �������
static void     OnPress_Internal_Delete(void);
static void        Draw_Internal_Delete(int x, int y);
/*
static const   SButton bInternal_EraseAll;                      ///< ������ - ����� �� - ������� ���
static void     OnPress_Internal_EraseAll(void);
static void        Draw_Internal_EraseAll(int x, int y);
*/
extern const   SButton bInternal_Scale;                         ///< ������ - ����� �� - �������
//static void     OnPress_Internal_Scale(void);
//static void        Draw_Internal_Scale(int x, int y);
//static void        Draw_Internal_Scale_Recalculated(int x, int y);
//static void        Draw_Internal_Scale_Original(int x, int y);
extern const   SButton bInternal_SaveToMemory;                  ///< ������ - ����� �� - ���������
static void     OnPress_Internal_SaveToMemory(void);
static void        Draw_Internal_SaveToMemory(int x, int y);
extern const   SButton bInternal_SaveToDrive;                   ///< ������ - ����� �� - ��������� �� ������
static void     OnPress_Internal_SaveToDrive(void);
static void        Draw_Internal_SaveToDrive(int x, int y);
extern const     Page ppDrive;                                  ///< ������ - ����� ��
extern const    Choice cDrive_Name;                             ///< ������ - ����� �� - ��� �����
extern const    Choice cDrive_SaveAs;                           ///< ������ - ����� �� - ��������� ���
extern const    Choice cDrive_ModeBtnMemory;                    ///< ������ - ����� �� - ����� �� ������
extern const    Choice cDrive_Autoconnect;                      ///< ������ - ����� �� - ��������������
extern const    Page pppDrive_Manager;                          ///< ������ - ����� �� - �������
static bool    IsActive_Drive_Manager(void);
       void     OnPress_Drive_Manager(void);
extern const   SButton bDrive_Manager_Exit;                     ///< ������ - ����� �� - ������� - �����
static void     OnPress_Drive_Manager_Exit(void);
extern const   SButton bDrive_Manager_Tab;                      ///< ������ - ����� �� - ������� - Tab
static void        Draw_Drive_Manager_Tab(int x, int y);
extern const   SButton bDrive_Manager_LevelUp;                  ///< ������ - ����� �� - ������� - ����� �� ��������
static void        Draw_Drive_Manager_LevelUp(int x, int y);
extern const   SButton bDrive_Manager_LevelDown;                ///< ������ - ����� �� - ������� - ����� � �������
static void        Draw_Drive_Manager_LevelDown(int x, int y);
extern const    Page pppDrive_Mask;                             ///< ������ - ����� �� - �����
static bool    IsActive_Drive_Mask(void);
static void     OnPress_Drive_Mask(void);
static void    OnRegSet_Drive_Mask(int angle);
extern const   SButton bDrive_Mask_Exit;                        ///< ������ - ����� �� - ����� - �����
extern const   SButton bDrive_Mask_Delete;                      ///< ������ - ����� �� - ����� - �������
static void     OnPress_Drive_Mask_Delete(void);
static void        Draw_Drive_Mask_Delete(int x, int y);
extern const   SButton bDrive_Mask_Backspace;                   ///< ������ - ����� �� - ����� - Backspace
static void     OnPress_Drive_Mask_Backspace(void);
static void        Draw_Drive_Mask_Backspace(int x, int y);
extern const   SButton bDrive_Mask_Insert;                      ///< ������ - ����� �� - ����� - ��������
static void     OnPress_Drive_Mask_Insert(void);
static void        Draw_Drive_Mask_Insert(int x, int y);
extern const      Page pSetName;                                ///< ��������� �����
static void    OnRegSet_SetName(int angle);
extern const   SButton bSetName_Exit;                           ///< ��������� ����� - �����
static void     OnPress_SetName_Exit(void);
extern const   SButton bSetName_Delete;                         ///< ��������� ����� - �������
static void     OnPress_SetName_Delete(void);
static void        Draw_SetName_Delete(int x, int y);
extern const   SButton bSetName_Backspace;                      ///< ��������� ����� - Backspace
static void     OnPress_SetName_Backspace(void);
static void        Draw_SetName_Backspace(int x, int y);
extern const   SButton bSetName_Insert;                         ///< ��������� ����� - ��������
static void     OnPress_SetName_Insert(void);
static void        Draw_SetName_Insert(int x, int y);
extern const   SButton bSetName_Save;                           ///< ��������� ����� - ���������
static void     OnPress_SetName_Save(void);
static void        Draw_SetName_Save(int x, int y);

static void DrawSetMask(void);  // ��� ������� ������, ����� ������ ����� ������� �����.
static void DrawFileMask(int x, int y);
static void DrawSetName(void);  // ��� ������� ������, ����� ����� ������ ��� ����� ��� ����������

static void DrawMemoryWave(int num, bool exist);
static void SaveSignalToIntMemory(void);
       void OnMemExtSetMaskNameRegSet(int angle, int maxIndex);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ ///
DEF_PAGE_4(     pMemory, ,
    Page_Memory, &mainPage, FuncActive, EmptyPressPage,
    "������", "MEMORY",
    "������ � ������� � ���������� �������.",
    "Working with external and internal memory.",
    cPoints,    // ������ - �����
    ppLast,     // ������ - ���������
    ppInternal, // ������ - ����� ��
    ppDrive     // ������ - ����� ��
);

const Page * pointerPageMemory = &pMemory;

//--------------------------------------------------------------------------------------------------------------------------------- ������ - ����� ---
static bool IsActive_Points(void)
{
    return SET_PEAKDET_DIS;
}

void OnChanged_Points(bool active)
{
    // ���� ������� ������� ��������, �� �� ����� ����������� ������
    if (SET_PEAKDET_EN && !active)
    {
        display.ShowWarning(WrongModePeackDet);
        return;
    }

    // ��������� ��������� 32� ����� ������, ���� ������� ������ �����
    if (FPGA_POINTS_32k && SET_ENABLED_B)
    {
        display.ShowWarning(DisableChannel2);
        FPGA_ENUM_POINTS = FNP_16k;
    }

    int width = grid.Width();
    
    FPGA_Reset();
    
    if (SET_PEAKDET_EN)
    {
        width *= 2;
    }
    if (TPOS_IS_LEFT)
    {
        SHIFT_IN_MEMORY = 0;
    }
    else if (TPOS_IS_CENTER)
    {
        SHIFT_IN_MEMORY = (int16)(NUM_BYTES_SET / 2 - width / 2);
    }
    else if (TPOS_IS_RIGHT)
    {
        SHIFT_IN_MEMORY = (int16)(NUM_BYTES_SET - width - 2);
    }
    
    FPGA_Reset();
    FPGA_SetTShift(SET_TSHIFT);
    FPGA_Reset();
}

pString namesLengthMemory[] = {"512", "1k", "2k", "4k", "8k", "16k", "32k"};

DEF_CHOICE_6
(
    cPoints, pMemory,
    FPGA_ENUM_POINTS, IsActive_Points, OnChanged_Points, FuncDraw,
    "����� ������", "Mem length",
    "����� ���������� �������� ��� ����������� ��������. "
    "��� ���������� ���������� �������� ����������� ���������� ���������� � ������ ��������.",
    "Choice of number of counting for the saved signals. "
    "At increase in number of counting the quantity of the signals kept in memory decreases.",
    namesLengthMemory[0], namesLengthMemory[0],
    namesLengthMemory[1], namesLengthMemory[1],
    namesLengthMemory[2], namesLengthMemory[2],
    namesLengthMemory[3], namesLengthMemory[3],
    namesLengthMemory[4], namesLengthMemory[4],
    namesLengthMemory[5], namesLengthMemory[5]
    //,namesLengthMemory[6], namesLengthMemory[6],
);

//----------------------------------------------------------------------------------------------------------------------------- ������ - ��������� ---
static void OnPress_Last(void)
{
    NUM_RAM_SIGNAL = 0;
    RUN_FPGA_BEFORE_SB = FPGA_IS_RUNNING ? 1u : 0u;
    FPGA_Stop(false);
    MODE_WORK = ModeWork_RAM;
}

static void OnDraw_Last(void)
{
    char buffer[20];

    int width = 40;
    int height = 10;
    painter.FillRegion(grid.Right() - width, GRID_TOP, width, height, gColorBack);
    painter.DrawRectangle(grid.Right() - width, GRID_TOP, width, height, gColorFill);
    painter.DrawText(grid.Right() - width + 2, GRID_TOP + 1, trans.Int2String(NUM_RAM_SIGNAL + 1, false, 3, buffer));
    painter.DrawText(grid.Right() - width + 17, GRID_TOP + 1, "/");
    painter.DrawText(grid.Right() - width + 23, GRID_TOP + 1, trans.Int2String(dS.NumElementsInStorage(), false, 3, buffer));
}

static void OnRegSet_Last(int angle)
{
    if (dS.NumElementsInStorage() > 1)
    {
        Sound_RegulatorSwitchRotate();
    }
    if (math.Sign(angle) > 0)
    {
        OnPress_Last_Next();
    }
    else
    {
        OnPress_Last_Prev();
    }
}

DEF_PAGE_SB(    ppLast, static,
    PageSB_Memory_Last, &pMemory, FuncActive,
    OnPress_Last, OnDraw_Last, OnRegSet_Last,
    "���������", "LATEST",
    "������� � ����� ������ � ���������� ����������� ���������",
    "Transition to an operating mode with the last received signals",
    &bLast_Exit,       // ������ - ��������� - �����
    0,                 
    &bLast_Next,       // ������ - ��������� - ���������
    &bLast_Prev,       // ������ - ��������� - ����������
    &bLast_SaveToROM,  // ������ - ��������� - ����� ��
    &bLast_SaveToDrive // ������ - ��������� - ���������
);

//--------------------------------------------------------------------------------------------------------------------- ������ - ��������� - ����� ---
static void OnPress_Last_Exit(void)
{
    MODE_WORK = ModeWork_Dir;
    if (RUN_FPGA_BEFORE_SB)
    {
        FPGA_Start();
        RUN_FPGA_BEFORE_SB = 0;
    }
    OnPressSB_Exit();
}

DEF_SMALL_BUTTON
(
    bLast_Exit,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button to return to the previous menu",
    ppLast, FuncActive, OnPress_Last_Exit, DrawSB_Exit
);


// ������ - ��������� - ��������� --------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_Next(void)
{
    CircleIncreaseInt16(&NUM_RAM_SIGNAL, 0, (int16)(dS.NumElementsInStorage() - 1));
}

static void Draw_Last_Next(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x64');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bLast_Next,
    "���������", "Next",
    "������� � ���������� �������",
    "Go to the next signal",
    ppLast, FuncActive, OnPress_Last_Next, Draw_Last_Next
);


// ������ - ��������� - ���������� -------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_Prev(void)
{
    CircleDecreaseInt16(&NUM_RAM_SIGNAL, 0, (int16)(dS.NumElementsInStorage() - 1));
}

static void Draw_Last_Prev(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_BACKSPACE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bLast_Prev,
    "����������", "Previous",
    "������� � ����������� �������",
    "Go to the previous signal",
    ppLast, FuncActive, OnPress_Last_Prev, Draw_Last_Prev
);


// ������ - ��������� - ����� �� ---------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_SaveToROM(void)
{
    ppInternal.SetCurrentSB();
    MODE_WORK = ModeWork_ROM;   // ��������� � ������ ����������� ��
    EXIT_FROM_ROM_TO_RAM = 1;   // ���� ������� �� ������������� ��� ����, �����:
                                // 1 - �� ������� ������ ����� �� �������� "����� ��" �������� � "���������", � �� � �������� ����;
                                // 2 - ��� ����, ����� �� �������� "����� ��" �������� �� ��������� ��������� ������, � ��������� �� �������� 
                                // "���������";
                                // 3 - ����� � Data_Load() ������������� ��������������� ���������.
}

static void Draw_Last_SaveToROM(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_ROM);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bLast_SaveToROM,
    "����� ��", "Internal storage",
    "������� ��� ������, ����� ��������� ������ �� ���������� ������������ ����������",
    "Press this button to keep a signal in an internal memory",
    ppLast, FuncActive, OnPress_Last_SaveToROM, Draw_Last_SaveToROM
);


// ������ - ��������� - ��������� --------------------------------------------------------------------------------------------------------------------
static void OnPress_Last_SaveToDrive(void)
{
    EXIT_FROM_SETNAME_TO = RETURN_TO_LAST_MEM;
    Memory_SaveSignalToFlashDrive();
}

static void Draw_Last_SaveToDrive(int x, int y)
{
    if (FDRIVE_IS_CONNECTED)
    {
        painter.SetFont(TypeFont_UGO2);
        painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
        painter.SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON
(
    bLast_SaveToDrive,
    "���������", "Save",
    "������ ���������� �������� ��� ������������� ������� ��. ��������� ��������� ������ �� ������ ��",
    "Click this button to save the signal on the external FLASH",
    ppLast, FuncActive, OnPress_Last_SaveToDrive, Draw_Last_SaveToDrive
);

void Memory_SaveSignalToFlashDrive(void)
{
    if (FDRIVE_IS_CONNECTED)
    {
        if (FILE_NAMING_MODE_MANUAL)
        {
            display.SetAddDrawFunction(DrawSetName);
        }
        else
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }
    }
    else
    {
        EXIT_FROM_SETNAME_TO = 0;
    }
}

static void DrawSetName(void)
{
    int x0 = grid.Left() + 40;
    int y0 = GRID_TOP + 60;
    int width = grid.Width() - 80;
    int height = 80;

    painter.DrawRectangle(x0, y0, width, height, gColorFill);
    painter.FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, gColorBack);

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

    int x = painter.DrawText(x0 + deltaX, y0 + 65, FILE_NAME, gColorFill);
    painter.FillRegion(x, y0 + 65, 5, 8, Color::FLASH_10);
}



//------------------------------------------------------------------------------------------------------------------------------ ������ - ����� �� ---
static void OnPress_Internal(void)
{
    MODE_WORK = ModeWork_ROM;
}

static void OnDraw_Internal(void)
{
    // ������ �������� ��������� ������

    bool exist[MAX_NUM_SAVED_WAVES] = {false};

    FLASH_GetDataInfo(exist);

    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave(i, exist[i]);
    }
}

static void DrawMemoryWave(int num, bool exist)
{
    char buffer[20];

    int x = grid.Left() + 2 + num * 12;
    int y = grid.FullBottom() - 10;
    int width = 12;
    painter.FillRegion(x, y, width, 10, num == NUM_ROM_SIGNAL ? Color::FLASH_10 : gColorBack);
    painter.DrawRectangle(x, y, width, 10, gColorFill);
    painter.SetColor(num == NUM_ROM_SIGNAL ? Color::FLASH_01 : gColorFill);
    if (exist)
    {
        painter.DrawText(x + 2, y + 1, trans.Int2String(num + 1, false, 2, buffer));
    }
    else
    {
        painter.DrawText(x + 3, y + 1, "\x88");
    }
}

static void OnRegSet_Internal(int delta)
{
    Sound_RegulatorSwitchRotate();
    if (delta < 0)
    {
        CircleDecreaseInt8(&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    else if (delta > 0)
    {
        CircleIncreaseInt8(&NUM_ROM_SIGNAL, 0, MAX_NUM_SAVED_WAVES - 1);
    }
    painter.ResetFlash();
}

DEF_PAGE_SB(    ppInternal, static,
    PageSB_Memory_Internal, &pMemory, FuncActive,
    OnPress_Internal, OnDraw_Internal, OnRegSet_Internal,
    "����� ��", "INT STORAGE",
    "������� � ����� ������ � ���������� �������",
    "Transition to an operating mode with internal memory",
    &bInternal_Exit,            // ������ - ����� �� - �����
    &bInternal_ShowAlways,      // ������ - ����� �� - ���������� ������
    &bInternal_ModeShow,        // ������ - ����� �� - ��� �������
    //0,
    //&bInternal_EraseAll,
    //&bInternal_Scale,           // ������ - ����� �� - �������
    &bInternal_Delete,          // ������ - ����� �� - �������
    &bInternal_SaveToMemory,    // ������ - ����� �� - ���������
    &bInternal_SaveToDrive      // ������ - ����� �� - ��������� �� ������
);

// ������ - ����� �� - ����� -------------------------------------------------------------------------------------------------------------------------
static void FuncForInternalExit(void)
{
    ppLast.SetCurrentSB();
}

static void OnPress_Internal_Exit(void)
{
    if (EXIT_FROM_ROM_TO_RAM)
    {
        MODE_WORK = ModeWork_RAM;
        EXIT_FROM_ROM_TO_RAM = 0;
        menu.RunAfterUpdate(FuncForInternalExit);
    }
    else
    {
        MODE_WORK = ModeWork_Dir;
        if (RUN_FPGA_BEFORE_SB)
        {
            FPGA_Start();
            RUN_FPGA_BEFORE_SB = 0;
        }
        OnPressSB_Exit();
        //ShortPressOnPageItem(PagePointerFromName(PageSB_Memory_Internal), 0);
    }
}

DEF_SMALL_BUTTON
(
    bInternal_Exit,
    "�����", "Exit",
    "������ ��� ������ � ���������� ����",
    "Button to return to the previous menu",
    ppInternal, FuncActive, OnPress_Internal_Exit, DrawSB_Exit
);

//---------------------------------------------------------------------------------------------------------- ������ - ����� �� - ���������� ������ ---
static void OnPress_Internal_ShowAlways(void)
{
    ALWAYS_SHOW_ROM_SIGNAL = ALWAYS_SHOW_ROM_SIGNAL ? 0u : 1u;
}

static void Draw_Internal_ShowAlways(int x, int y)
{
    if (ALWAYS_SHOW_ROM_SIGNAL == 0)
    {
        Draw_Internal_ShowAlways_No(x, y);
    }
    else
    {
        Draw_Internal_ShowAlways_Yes(x, y);
    }
}

static void Draw_Internal_ShowAlways_Yes(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x66');
    painter.SetFont(TypeFont_8);
}

static void Draw_Internal_ShowAlways_No(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x68');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON_HINTS_2
(
    bInternal_ShowAlways,
    "���������� ������", "To show always",
    "��������� ������ ���������� ��������� ���������� ������ ������ ��������",
    "Allows to show always the chosen kept signal over the current",
    ppInternal, FuncActive, OnPress_Internal_ShowAlways, Draw_Internal_ShowAlways,
    hintsShowAlways,
    Draw_Internal_ShowAlways_Yes,   "���������� ��������� ������ �� ���������� ������ ������ ��������",
                                    "to show the chosen signal from internal memory over the current",
    Draw_Internal_ShowAlways_No,    "������ �� ���������� ������ ����� ������ � ������ ������ � ���������� ������������ �����������",
                                    "the signal from internal memory is visible only in an operating mode with an internal memory"
);

//---------------------------------------------------------------------------------------------------------------- ������ - ����� �� - ��� ������� ---
static void OnPress_Internal_ModeShow(void)
{
    CircleIncreaseInt8((int8 *)&SHOW_IN_INT, 0, 2);
}

static void Draw_Internal_ModeShow(int x, int y)
{
    if (SHOW_IN_INT_DIRECT)
    {
        Draw_Internal_ModeShow_Direct(x, y);
    }
    else if (SHOW_IN_INT_SAVED)
    {
        Draw_Internal_ModeShow_Saved(x, y);
    }
    else
    {
        Draw_Internal_ModeShow_Both(x, y);
    }
}

static void Draw_Internal_ModeShow_Direct(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x6a');
    painter.SetFont(TypeFont_8);
}


static void Draw_Internal_ModeShow_Saved(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x6c');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON_HINTS_3
(
    bInternal_ModeShow,
    "��� �������", "Type of a signal",
    "���������� ���������� ��� ������� ������ � ������ ����� ��",
    "Show recorded or current signal in mode Internal Memory",
    ppInternal, FuncActive, OnPress_Internal_ModeShow, Draw_Internal_ModeShow,
    hintsModeShow,
    Draw_Internal_ModeShow_Direct,  "�� ������� ������� ������",        "on the display current signal",    
    Draw_Internal_ModeShow_Saved,   "�� ������� ���������� ������",    "on the display the kept signal",
    Draw_Internal_ModeShow_Both,    "�� ������� ��� �������",           "on the display both signals"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Internal_ModeShow_Both(int x, int y)
{
    painter.DrawText(x + 1, y + 5, "���");
    painter.DrawVLine(x + 1, y + 2, y + 14, gColorBack);
    painter.DrawVLine(x + 2, y + 6, y + 11, gColorFill);
}

/*
// ������ - ����� �� - ������� �� -------------------------------------------------------------------------------------------------------------------
static const SButton bInternal_EraseAll =
{
    Item_SmallButton, &ppInternal, 0,
    {
        "������� ��", "Erase all",
        "������� ��� ������ �� ������� �������� ������, ������� ������� �����������. ��������� ��������������",
        "It erases all data from the storage area, including the area inoformatsiya. The total format"
    },
    OnPress_Internal_EraseAll,
    Draw_Internal_EraseAll,
    {
        {
            Draw_Internal_EraseAll,
            "������� ��� ������",
            "Erase all data"
        }
    }
};

static void OnPress_Internal_EraseAll(void)
{
    display.FuncOnWaitStart("������. ���������", "Erase. Wait", false);
    FLASH_DeleteAllData();
    display.FuncOnWaitStop();
}

static void Draw_Internal_EraseAll(int x, int y)
{
    painter.DrawText(x + 5, y + 5, "E");
}
*/

// ������ - ����� �� - ������� -----------------------------------------------------------------------------------------------------------------------
/*
void OnPress_Internal_Scale(void)
{
    CircleIncreaseInt8((int8 *)&MEM_DATA_SCALE, 0, 1);
}
*/

/*
void Draw_Internal_Scale(int x, int y)
{
    if (MEM_DATA_SCALE_RECALC)
    {
        Draw_Internal_Scale_Recalculated(x, y);
    }
    else
    {
        Draw_Internal_Scale_Original(x, y);
    }
}
*/

/*
void Draw_Internal_Scale_Recalculated(int x, int y)
{
    painter.DrawText(x + 8, y + 2, DICT(DM));
    painter.SetFont(TypeFont_5);
    painter.DrawText(x + 5, y + 9, DICT(DABS));
    painter.SetFont(TypeFont_8);
}
*/

/*
void Draw_Internal_Scale_Original(int x, int y)
{
    painter.DrawText(x + 8, y + 2, DICT(DM));
    painter.SetFont(TypeFont_5);
    painter.DrawText(x + 5, y + 9, DICT(DREL));
    painter.SetFont(TypeFont_8);
}
*/

/*
DEF_SMALL_BUTTON_HINTS_2
(
    bInternal_Scale,
    "�������", "Scale",
    "��������� ��� ��� ���������� ������ � ������� ����������",
    "Whether or not to record the recorded signal to the current settings",
    ppInternal, FuncActive, OnPress_Internal_Scale, Draw_Internal_Scale,
    hintsInternal_Scale,
    Draw_Internal_Scale_Recalculated,   "������ ������� � ������� ���������� ������������",
                                        "The signal is given to the current oscilloscope settings",
    Draw_Internal_Scale_Original,       "������ �������� � ����� ����, � ������� ������",
                                        "The signal is drawn in the form in which the readout"
);
*/

// ������ - ����� �� - ��������� � ������ ------------------------------------------------------------------------------------------------------------
static void SaveSignalToIntMemory(void)
{
    // ������� � ��������� DS, DATA_A, DATA_B ������ �� ��� ��� ��������� ��������� ������, � ����������� �� ����, �� ������ ������ ����� � 
    // "������-����� ��"
    reader.ReadFromRAM(EXIT_FROM_ROM_TO_RAM ? NUM_RAM_SIGNAL : 0, 0, false);

    if (DS)                                             // ���� ���� ��� ���������
    {
        FLASH_SaveData(NUM_ROM_SIGNAL, DS, IN_A, IN_B);   // �� ��������� ������ �� DS, DATA_A, DATA_B �� ����� NUM_ROM_SIGNAL � ����
        display.ShowWarning(SignalIsSaved);
    }
}

static void OnPress_Internal_SaveToMemory(void)
{
    display.FuncOnWaitStart(DICT(DStoredInMemory), false);
    SaveSignalToIntMemory();
    display.FuncOnWaitStop();
}

static void Draw_Internal_SaveToMemory(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_SAVE_TO_MEM);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bInternal_SaveToMemory,
    "��������� � ������", "Save to memory",
    "��������� ������ �� ���������� ������������ ����������",
    "To keep a signal in an internal memory",
    ppInternal, FuncActive, OnPress_Internal_SaveToMemory, Draw_Internal_SaveToMemory
);

// ������ - ����� �� - ������� -----------------------------------------------------------------------------------------------------------------------
static void OnPress_Internal_Delete(void)
{
    display.FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    FLASH_DeleteData(NUM_ROM_SIGNAL);
    display.FuncOnWaitStop();
}

static void Draw_Internal_Delete(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bInternal_Delete,
    "�������", "Delete",
    "������� ��������� ������ �� ����������� ������������� ����������",
    "Removes the selected signal from the internal storage device",
    ppInternal, FuncActive, OnPress_Internal_Delete, Draw_Internal_Delete
);

//-------------------------------------------------------------------------------------------------------- ������ - ����� �� - ��������� �� ������ ---
static void OnPress_Internal_SaveToDrive(void)
{
    EXIT_FROM_SETNAME_TO = RETURN_TO_INT_MEM;
    Memory_SaveSignalToFlashDrive();
}

static void Draw_Internal_SaveToDrive(int x, int y)
{
    if (FDRIVE_IS_CONNECTED)
    {
        painter.SetFont(TypeFont_UGO2);
        painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
        painter.SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON
(
    bInternal_SaveToDrive,
    "��������� �� ������", "Save to disk",
    "��������� ������ �� ������",
    "Save signal to flash drive",
    ppInternal, FuncActive, OnPress_Internal_SaveToDrive, Draw_Internal_SaveToDrive
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ - ����� �� ///
DEF_PAGE_6(     ppDrive, static,
    Page_Memory_Drive, &pMemory, FuncActive, EmptyPressPage,
    "����� ��", "EXT STORAGE",
    "������ � ������� ������������ �����������.",
    "Work with external storage device.",
    pppDrive_Manager,       // ������ - ����� �� - �������
    cDrive_Name,            // ������ - ����� �� - ��� �����
    pppDrive_Mask,          // ������ - ����� �� - �����
    cDrive_SaveAs,          // ������ - ����� �� - ��������� ���
    cDrive_ModeBtnMemory,   // ������ - ����� �� - ��� �� ������
    cDrive_Autoconnect      // ������ - ����� �� - ���������������
);

//------------------------------------------------------------------------------------------------------------------ ������ - ����� �� - ��� ����� ---
DEF_CHOICE_2
(
    cDrive_Name, ppDrive,
    FILE_NAMING_MODE, FuncActive, FuncChangedChoice, FuncDraw,
    "��� �����", "File name",
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������",
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually",
    "�� �����", "Mask",
    "�������",  "Manually"
);

//-------------------------------------------------------------------------------------------------------------- ������ - ����� �� - ��������� ��� ---
DEF_CHOICE_2
(
    cDrive_SaveAs, ppDrive,
    MODE_SAVE, FuncActive, FuncChangedChoice, FuncDraw,
    "��������� ���", "Save as",
    "���� ������ ������� \"�����������\", ������ ����� ������� � ������� �������� � ����������� ����� � ����������� BMP\n"
    "���� ������ ������� \"�����\", ������ ����� ������� � ������� �������� � ��������� ���� � ����� � ���������� TXT",
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension BMP\n"
    "If you select \"Text\", the signal will be stored in the current directory as a text file with the extension TXT",
    "�����������", "Image",
    "�����",       "Text"
);

//-------------------------------------------------------------------------------------------------------------- ������ - ����� �� - ��� �� ������ ---
DEF_CHOICE_2
(
    cDrive_ModeBtnMemory, ppDrive,
    MODE_BTN_MEMORY, FuncActive, FuncChangedChoice, FuncDraw,
    "��� �� ������", "Mode btn MEMORY",
    "",
    "",
    "����",       "Menu",
    "����������", "Save"
);

//------------------------------------------------------------------------------------------------------------ ������ - ����� �� - ��������������� ---
DEF_CHOICE_2
(
    cDrive_Autoconnect, ppDrive,
    FLASH_AUTO_CONNECT, FuncActive, FuncChangedChoice, FuncDraw,
    "���������.", "AutoConnect",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ - ����� �� - ������� ///
static bool IsActive_Drive_Manager(void)
{
    return FDRIVE_IS_CONNECTED;
}

void OnPress_Drive_Manager(void)
{
    if (FDRIVE_IS_CONNECTED)
    {
        FDrive_Mount();
        display.SetDrawMode(DrawMode_Auto, FM_Draw);
        FM_NEED_REDRAW = FM_REDRAW_FULL;
    }
    else
    {
        display.ShowWarning(WarnNeedForFlashDrive);
    }
}

DEF_PAGE_SB(    pppDrive_Manager, static,
    PageSB_Memory_Drive_Manager, &ppDrive, IsActive_Drive_Manager,
    OnPress_Drive_Manager, EmptyDrawPage, FM_RotateRegSet,
    "�������", "DIRECTORY",
    "��������� ������ � �������� ������� ������������� ����������",
    "Provides access to the file system of the connected drive",
    &bDrive_Manager_Exit,       // ������ - ����� �� - ������� - �����
    &bDrive_Manager_Tab,        // ������ - ����� �� - ������� - Tab
    0,
    0,
    &bDrive_Manager_LevelUp,    // ������ - ����� �� - ������� - ����� �� ��������
    &bDrive_Manager_LevelDown   // ������ - ����� �� - ������� - ����� � �������
);

//------------------------------------------------------------------------------------------------------------ ������ - ����� �� - ������� - ����� ---
static void OnPress_Drive_Manager_Exit(void)
{
    display.SetDrawMode(DrawMode_Auto, 0);
    OnPressSB_Exit();
}

DEF_SMALL_BUTTON
(
    bDrive_Manager_Exit,
    "�����", "Exit",
    "������ ��� ������ � ���������� ����",
    "Button to return to the previous menu",
    pppDrive_Manager, FuncActive, OnPress_Drive_Manager_Exit, DrawSB_Exit
);

//-------------------------------------------------------------------------------------------------------------- ������ - ����� �� - ������� - Tab ---
static void Draw_Drive_Manager_Tab(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_TAB);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bDrive_Manager_Tab,
    "Tab", "Tab",
    "������� ����� ���������� � �������",
    "The transition between the directories and files",
    pppDrive_Manager, FuncActive, PressSB_FM_Tab, Draw_Drive_Manager_Tab
);

//------------------------------------------------------------------------------------------------ ������ - ����� �� - ������� - ����� �� �������� ---
static void Draw_Drive_Manager_LevelUp(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bDrive_Manager_LevelUp,
    "����� �� ��������", "Leave from directory",
    "������� � ������������ �������",
    "Transition to the parental catalog",
    pppDrive_Manager, FuncActive, PressSB_FM_LevelUp, Draw_Drive_Manager_LevelUp
);

//-------------------------------------------------------------------------------------------------- ������ - ����� �� - ������� - ����� � ������� ---
static void Draw_Drive_Manager_LevelDown(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bDrive_Manager_LevelDown,
    "����� � �������", "Enter in directory",
    "������� � ��������� �������",
    "Transition to the chosen catalog",
    pppDrive_Manager, FuncActive, PressSB_FM_LevelDown, Draw_Drive_Manager_LevelDown
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ - ����� �� - ����� ///
static bool IsActive_Drive_Mask(void)
{
    return FILE_NAMING_MODE_MASK;
}

static void OnPress_Drive_Mask(void)
{
    display.SetAddDrawFunction(DrawSetMask);
}

static void DrawSetMask(void)
{
    int x0 = grid.Left() + 40;
    int y0 = GRID_TOP + 20;
    int width = grid.Width() - 80;
    int height = 160;

    painter.DrawRectangle(x0, y0, width, height, gColorFill);
    painter.FillRegion(x0 + 1, y0 + 1, width - 2, height - 2, gColorBack);

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

    // ������ ������ �����������
    position = 0;
    while (index < (sizeof(symbolsAlphaBet) / 4))
    {
        DrawStr(index, x0 + deltaX + 26 + position * 20, y0 + deltaY0 + deltaY * 3);
        index++;
        position++;
    }

    DrawFileMask(x0 + deltaX, y0 + 65);

    static pString strings[] =
    {
        "%y - ���, %m - �����, %d - ����",
        "%H - ����, %M - ������, %S - �������",
        "%nN - ���������� �����, ���",
        "n - ����������� ���������� ������ ��� N"
    };

    deltaY--;
    painter.SetColor(gColorFill);
    for (int i = 0; i < sizeof(strings) / 4; i++)
    {
        painter.DrawText(x0 + deltaX, y0 + 100 + deltaY * i, strings[i]);
    }
}

static void DrawFileMask(int x, int y)
{
    char *ch = FILE_NAME_MASK;

    painter.SetColor(gColorFill);
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

static void OnRegSet_Drive_Mask(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4);
}

DEF_PAGE_SB(    pppDrive_Mask, static,
    PageSB_Memory_Drive_Mask, &ppDrive, IsActive_Drive_Mask,
    OnPress_Drive_Mask, EmptyDrawPage, OnRegSet_Drive_Mask,
    "�����", "MASK",
    "����� ����� ����� ��� ��������������� ���������� ������",
    "Input mode mask for automatic file naming",
    &bDrive_Mask_Exit,      // ������ - ����� �� - ����� - �����
    &bDrive_Mask_Delete,    // ������ - ����� �� - ����� - �������
    0,
    0,
    &bDrive_Mask_Backspace, // ������ - ����� �� - ����� - Backspace
    &bDrive_Mask_Insert     // ������ - ����� �� - ����� - ��������
);

//-------------------------------------------------------------------------------------------------------------- ������ - ����� �� - ����� - ����� ---
DEF_SMALL_BUTTON
(
    bDrive_Mask_Exit,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button to return to the previous menu",
    pppDrive_Mask, FuncActive, OnPressSB_Exit, DrawSB_Exit
);

//------------------------------------------------------------------------------------------------------------ ������ - ����� �� - ����� - ������� ---
static void OnPress_Drive_Mask_Delete(void)
{
    FILE_NAME_MASK[0] = '\0';
}

static void Draw_Drive_Mask_Delete(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bDrive_Mask_Delete,
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered symbols",
    pppDrive_Mask, FuncActive, OnPress_Drive_Mask_Delete, Draw_Drive_Mask_Delete
);

//---------------------------------------------------------------------------------------------------------- ������ - ����� �� - ����� - Backspace ---
static void OnPress_Drive_Mask_Backspace(void)
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

static void Draw_Drive_Mask_Backspace(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bDrive_Mask_Backspace,
    "Backspace", "Backspace",
    "������� ��������� �������� ������",
    "Deletes the last entered symbol",
    pppDrive_Mask, FuncActive, OnPress_Drive_Mask_Backspace, Draw_Drive_Mask_Backspace
);

//----------------------------------------------------------------------------------------------------------- ������ - ����� �� - ����� - �������� ---
static void OnPress_Drive_Mask_Insert(void)
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
            FILE_NAME_MASK[size] = (char)index;
            FILE_NAME_MASK[size + 1] = '\0';
        }
    }
}

static void Draw_Drive_Mask_Insert(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, SYMBOL_INSERT);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bDrive_Mask_Insert,
    "��������", "Insert",
    "��������� ��������� ������",
    "Inserts the chosen symbol",
    pppDrive_Mask, FuncActive, OnPress_Drive_Mask_Insert, Draw_Drive_Mask_Insert
);

//////////////////////////////////////////////////////////////////////////////////////////////////////// �������� ���������� ��� ����� ����� ����� ///
static void OnRegSet_SetName(int angle)
{
    OnMemExtSetMaskNameRegSet(angle, sizeof(symbolsAlphaBet) / 4 - 7);
}

void OnMemExtSetMaskNameRegSet(int angle, int maxIndex)
{
    int8(*func[3])(int8 *, int8, int8) =
    {
        CircleDecreaseInt8,
        CircleDecreaseInt8,
        CircleIncreaseInt8
    };

    painter.ResetFlash();
    if (INDEX_SYMBOL > maxIndex)
    {
        INDEX_SYMBOL = (int8)(maxIndex - 1);
    }
    func[math.Sign(angle) + 1](&INDEX_SYMBOL, 0, (int8)(maxIndex - 1));
    Sound_RegulatorSwitchRotate();

}

DEF_PAGE_SB(    pSetName, static,
    PageSB_Memory_SetName, 0, FuncActive,
    EmptyPressPage, EmptyDrawPage, OnRegSet_SetName,
    "", "",
    "",
    "",
    &bSetName_Exit,         // ���� ����� ����� - �����
    &bSetName_Delete,       // ���� ����� ����� - �������
    0,
    &bSetName_Backspace,    // ���� ����� ����� - Backspace
    &bSetName_Insert,       // ���� ����� ����� - ��������
    &bSetName_Save          // ���� ����� ����� - ���������
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Exit(void)
{
    OnPressSB_Exit();
    if (EXIT_FROM_SETNAME_TO == RETURN_TO_DISABLE_MENU)
    {
        PagePointerFromName(PageSB_Memory_SetName)->ShortPressOnItem(0);
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_LAST_MEM)
    {
    }
    else if (EXIT_FROM_SETNAME_TO == RETURN_TO_INT_MEM)
    {
    }
    EXIT_FROM_SETNAME_TO = RETURN_TO_DISABLE_MENU;
}

// ����� ��� ������ �� ������ ������� ����� ������������ �������. ������������ ������ ������ �� ����������
DEF_SMALL_BUTTON
(
    bSetName_Exit,
    "�����", "Exit",
    "����� �� ����������",
    "Failure to save",
    pSetName, FuncActive, OnPress_SetName_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Delete(void)
{
    FILE_NAME[0] = '\0';
}

static void Draw_SetName_Delete(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_DELETE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bSetName_Delete,
    "�������", "Delete",
    "������� ��� �������� �������",
    "Deletes all entered characters",
    pSetName, FuncActive, OnPress_SetName_Delete, Draw_SetName_Delete
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Backspace(void)
{
    int size = strlen(FILE_NAME);
    if (size > 0)
    {
        FILE_NAME[size - 1] = '\0';
    }
}

static void Draw_SetName_Backspace(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_BACKSPACE);
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bSetName_Backspace,
    "Backspace", "Backspace",
    "������� ��������� ������",
    "Delete the last character",
    pSetName, FuncActive, OnPress_SetName_Backspace, Draw_SetName_Backspace
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Insert(void)
{
    int size = strlen(FILE_NAME);
    if (size < MAX_SYMBOLS_IN_FILE_NAME - 1)
    {
        FILE_NAME[size] = symbolsAlphaBet[INDEX_SYMBOL][0];
        FILE_NAME[size + 1] = '\0';
    }
}

static void Draw_SetName_Insert(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x26');
    painter.SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON
(
    bSetName_Insert,
    "��������", "Insert",
    "������ ��������� ������",
    "Print the next character",
    pSetName, FuncActive, OnPress_SetName_Insert, Draw_SetName_Insert
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SetName_Save(void)
{
    if (FDRIVE_IS_CONNECTED)
    {
        OnPress_SetName_Exit();
        NEED_SAVE_TO_FLASHDRIVE = 1;
    }
}

static void Draw_SetName_Save(int x, int y)
{
    if (FDRIVE_IS_CONNECTED)
    {
        painter.SetFont(TypeFont_UGO2);
        painter.Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
        painter.SetFont(TypeFont_8);
    }
}

DEF_SMALL_BUTTON
(
    bSetName_Save,
    "���������", "Save",
    "���������� �� ���� ��� �������� ������",
    "Saving to flashdrive with the specified name",
    pSetName, FuncActive, OnPress_SetName_Save, Draw_SetName_Save
);


/*

void DrawSB_MemLastSelect(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 3, y + 2, set.memory.strMemoryLast.isActiveModeSelect ? '\x2a' : '\x28');
    painter.SetFont(TypeFont_8);
}

void PressSB_MemLastSelect(void)
{
    set.memory.strMemoryLast.isActiveModeSelect = !set.memory.strMemoryLast.isActiveModeSelect;
}


void DrawSB_MemExtNewFolder(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 1, y, '\x46');
    painter.SetFont(TypeFont_8);
}

extern const Page pMemory;
extern const Page ppDrive;

*/
