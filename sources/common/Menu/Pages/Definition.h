#pragma once
#include "defines.h"
#include "Panel/Controls.h"
#include "Settings/SettingsTypes.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает страницу меню, которая должна открываться по нажатию кнопки button.
const void *PageForButton(PanelButton button);
/// Возвращает true, если item - адрес главной страницы меню.
bool IsMainPage(const void *item);
/// Функция отрисовки кнопки выхода из режима малого меню.
void DrawSB_Exit(int x, int y);
/// Универсальная функция отработки нажатия кнопки выхода из режима малого меню.
void OnPressSB_Exit(void);

void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);

void DrawMenuCursTime(int x, int y, bool left, bool right);
/// Рассчитывает условия отрисовки УГО малых кнопок управления выбором курсорами.
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl cursCntrl, bool *cond0, bool *cond1);

extern const Page mainPage;

extern const Page pDisplay;
extern const Page ppDisplay_Accum;
extern const Page ppDisplay_Average;
extern const Page ppDisplay_Grid;
extern const Page ppDisplay_Settings;
extern const Page pppDisplay_Settings_Colors;

extern const Page pChanA;

extern const Page pChanB;

extern const Page pTrig;
extern const Page ppTrig_Search;

extern const Page pTime;

extern const Page pCursors;
extern const Page ppCursors_Set;

extern const Page pMemory;
extern const Page ppMemory_Last;                // ПАМЯТЬ - ПОСЛЕДНИЕ
extern const Page ppMemory_Internal;            // ПАМЯТЬ - ВНУТР ЗУ
extern const Page ppMemory_Drive;
extern const Page pppMemory_Drive_FileManager;
extern const Page pppMemory_Drive_Mask;
extern const Page pMemory_SetName;

extern const Page pMeasures;
extern const Page ppMeasures_FreqMeter;
extern const Page ppMeasures_Tune;

extern const Page pService;
extern const Page ppService_Calibrator;
extern const Page ppService_FFT;
extern const Page pppService_FFT_Cursors;
extern const Page ppService_Function;
extern const Page ppService_Ethernet;
extern const Page ppService_Sound;
extern const Page ppService_RTC;
extern const Page ppService_Information;

extern const Page pHelp;

extern const Page pDebug;
extern const Page ppDebug_Console;
extern const Page pppDebug_Console_Registers;
extern const Page ppDebug_ADC;
extern const Page pppDebug_ADC_Balance;
extern const Page pppDebug_ADC_Stretch;
extern const Page pppDebug_ADC_Shift;
extern const Page ppDebug_Rand;
extern const Page ppDebug_Channels;
extern const Page ppDebug_Settings;
extern const Page ppDebug_SerialNumber;
