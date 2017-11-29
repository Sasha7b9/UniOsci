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
extern const      Page pDebug;
extern const     Page ppConsole;                            ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹
extern const  Governor gConsole_NumStrings;                 ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –‡ÁÏÂ ¯ËÙÚ‡
extern const    Choice cConsole_SizeFont;                   ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –‡ÁÏÂ ¯ËÙÚ‡
extern const    Choice cConsole_ModeStop;                   ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –ÂÊ. ÓÒÚ‡ÌÓ‚‡
extern const    Page pppConsole_Registers;                  ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€
extern const    Choice cConsole_Registers_ShowAll;          ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œÓÍ‡Á˚‚‡Ú¸ ‚ÒÂ
extern const    Choice cConsole_Registers_RD_FL;            ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - RD_FL
static bool    IsActive_Console_Registers(void);
extern const    Choice cConsole_Registers_RShiftA;          ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒÏ. 1Í
extern const    Choice cConsole_Registers_RShiftB;          ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒÏ. 2Í
extern const    Choice cConsole_Registers_TrigLev;          ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒËÌı.
extern const    Choice cConsole_Registers_RangeA;           ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬ŒÀ‹“/ƒ≈À 1
extern const    Choice cConsole_Registers_RangeB;           ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬ŒÀ‹“/ƒ≈À 2
extern const    Choice cConsole_Registers_TrigParam;        ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. ÒËÌı.
extern const    Choice cConsole_Registers_ChanParamA;       ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. Í‡Ì. 2
extern const    Choice cConsole_Registers_ChanParamB;       ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. Í‡Ì. 2
extern const    Choice cConsole_Registers_TBase;            ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬–≈Ãﬂ/ƒ≈À
extern const    Choice cConsole_Registers_TShift;           ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - “ ÒÏ.
extern const    Button bConsole_SizeSettings;               ///< Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –‡ÁÏÂ Ì‡ÒÚÓÂÍ
static void        Draw_Console_SizeSettings(int x, int y); 
extern const     Page ppADC;                                ///< Œ“À¿ƒ ¿ - ¿÷œ
extern const    Page pppADC_Balance;                        ///< Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ—
extern const    Choice cADC_Balance_Mode;                   ///< Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - –ÂÊËÏ
static void   OnChanged_ADC_Balance_Mode(bool active);
static void        Draw_ADC_Balance_Mode(int x, int y);
extern const  Governor gADC_Balance_ShiftA;                 ///< Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - —ÏÂ˘ÂÌËÂ 1
static bool    IsActive_ADC_Balance_ShiftAB(void);
static void   OnChanged_ADC_Balance_ShiftA(void);
extern const  Governor gADC_Balance_ShiftB;                 ///< Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - —ÏÂ˘ÂÌËÂ 2 
static void   OnChanged_ADC_Balance_ShiftB(void);
extern const    Page pppADC_Stretch;                        ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿
extern const    Choice cADC_Stretch_Mode;                   ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –ÂÊËÏ
       void   OnChanged_ADC_Stretch_Mode(bool active);
extern const  Governor gADC_Stretch_A;                      ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –‡ÒÚˇÊÍ‡ 1Í
static bool    IsActive_ADC_StretchAB(void);
static void   OnChanged_ADC_Stretch_A(void);
extern const  Governor gADC_Stretch_B;                      ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –‡ÒÚˇÊÍ‡ 2Í
static void   OnChanged_ADC_Stretch_B(void);
extern const  Governor gADC_Stretch_Ak20mV;                 ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 20Ï¬/1¬ 1Í
extern const  Governor gADC_Stretch_Ak50mV;                 ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 50Ï¬ 1Í 
extern const  Governor gADC_Stretch_Ak100mV;                ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 100Ï¬/5¬ 1Í
extern const  Governor gADC_Stretch_Ak2V;                   ///< Œ“À¿ƒ ¿ - A÷œ - –¿—“ﬂ∆ ¿ - 2¬ 1Í
extern const  Governor gADC_Stretch_Bk20mV;                 ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 20Ï¬/1¬ 2Í
extern const  Governor gADC_Stretch_Bk50mV;                 ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 50Ï¬ 2Í 
extern const  Governor gADC_Stretch_Bk100mV;                ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 100Ï¬/5¬ 2Í
extern const  Governor gADC_Stretch_Bk2V;                   ///< Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 2¬ 2Í
extern const    Page pppADC_Shift;                          ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ
extern const    Button bADC_Shift_Reset;                    ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —·ÓÒ
static void     OnPress_ADC_Shift_Reset(void);
extern const  Governor gADC_Shift_A2mV;                     ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 2Ï¬ ÔÓÒÚ
static void   OnChanged_ADC_Shift_A(void);
extern const  Governor gADC_Shift_B2mV;                     ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 2Ï¬ ÔÓÒÚ
static void   OnChanged_ADC_Shift_B(void);
extern const  Governor gADC_Shift_A5mV;                     ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 5Ï¬ ÔÓÒÚ
extern const  Governor gADC_Shift_B5mV;                     ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 5Ï¬ ÔÓÒÚ
extern const  Governor gADC_Shift_A10mV;                    ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 10Ï¬ ÔÓÒÚ
extern const  Governor gADC_Shift_B10mV;                    ///< Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 10Ï¬ ÔÓÒÚ
extern const     Page ppRand;                               ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ–
extern const  Governor gRand_NumMeasures;                   ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - ¬˚·-Í/‚ÓÓÚ‡
static void   OnChanged_Rand_NumMeasures(void);
extern const  Governor gRand_NumAverage;                    ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - ”ÒÂ‰Ì.
extern const  Governor gRand_NumSmooth;                     ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —„Î‡ÊË‚‡ÌËÂ
extern const    Choice cRand_ShowInfo;                      ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - »ÌÙÓÏ‡ˆËˇ
extern const    Choice gRand_ShowStat;                      ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —Ú‡ÚËÒÚËÍ‡
extern const  Governor gRand_TimeCompensation;              ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– -  ÓÏÔÂÌÒ‡ˆËˇ Á‡‰ÂÊÍË
static void   OnChanged_Rand_TimeCompensation(void);
extern const  Governor gRand_AddTimeShift;                  ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —ÏÂ˘ÂÌËÂ
static void   OnChanged_Rand_AddTimeShift(void);
extern const  Governor gRand_Pretriggered;                  ///< Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - œÂ‰Á‡ÔÛÒÍ
static void   OnChanged_Rand_Pretriggered(void);
extern const     Page ppChannels;                           ///< Œ“À¿ƒ ¿ -  ¿Õ¿À€
extern const    Choice cChannels_BandwidthA;                ///< Œ“À¿ƒ ¿ -  ¿Õ¿À€ - œÓÎÓÒ‡ 1
static void   OnChanged_Channels_BandwidthA(bool active);
extern const    Choice cChannels_BandwidthB;                ///< Œ“À¿ƒ ¿ -  ¿Õ¿À€ - œÓÎÓÒ‡ 2
static void   OnChanged_Channels_BandwidthB(bool active);
extern const    Choice cStats;                              ///< Œ“À¿ƒ ¿ - —Ú‡ÚËÒÚËÍ‡
extern const    Choice cDisplayOrientation;                 ///< Œ“À¿ƒ ¿ - ŒËÂÌÚ‡ˆËˇ
       void   OnChanged_DisplayOrientation(bool);
extern const    Choice cEMS;                                ///< Œ“À¿ƒ ¿ - –ÂÊËÏ ›Ã—
static void   OnChanged_EMS(bool);
extern const Governor mgPred;                               ///< Œ“À¿ƒ ¿ - œÂ‰Á‡ÔÛÒÍ
static void   OnChanged_Pred(void);
extern const Governor mgPost;                               ///< Œ“À¿ƒ ¿ - œÓÒÎÂÁ‡ÔÛÒÍ
static void   OnChanged_Post(void);
extern const     Page ppSettings;                           ///< Œ“À¿ƒ ¿ - Õ¿—“–Œ… »
static void     OnPress_Settings(void);
extern const   SButton bSettings_Exit;                      ///< Œ“À¿ƒ ¿ - Õ¿—“–Œ… » - ¬˚ıÓ‰
static void     OnPress_Settings_Exit(void);
extern const    Button bSaveFirmware;                       ///< Œ“À¿ƒ ¿ - —Óı. ÔÓ¯Ë‚ÍÛ
static bool    IsActive_SaveFirmware(void);
static void     OnPress_SaveFirmware(void);
extern const     Page ppSerialNumber;                       ///< Œ“À¿ƒ ¿ - —/Õ
static void     OnPress_SerialNumber(void);
static void        Draw_EnterSerialNumber(void);
static void    OnRegSet_SerialNumber(int);
extern const   SButton bSerialNumber_Exit;                  ///< Œ“À¿ƒ ¿ - —/Õ - ¬˚ıÓ‰
static void     OnPress_SerialNumber_Exit(void);
extern const   SButton bSerialNumber_Change;                ///< Œ“À¿ƒ ¿ - —/Õ - œÂÂÈÚË
static void     OnPress_SerialNumber_Change(void);
static void        Draw_SerialNumber_Change(int, int);
extern const   SButton bSerialNumber_Save;                  ///< Œ“À¿ƒ ¿ - —/Õ - —Óı‡ÌËÚ¸
static void     OnPress_SerialNumber_Save(void);
static void        Draw_SerialNumber_Save(int, int);
extern const    Button bEraseData;                          ///< Œ“À¿ƒ ¿ - —ÚÂÂÚ¸ ‰‡ÌÌ˚Â
static void     OnPress_EraseData(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ¬ ˝ÚÓÈ ÒÚÛÍÚÛÂ ·Û‰ÛÚ ı‡ÌËÚ¸Òˇ ‰‡ÌÌ˚Â ÒÂËÈÌÓ„Ó ÌÓÏÂ‡ ÔË ÓÚÍ˚ÚÓÈ ÒÚ‡ÌËˆÂ ppSerialNumer
typedef struct
{
    int number;     ///< —ÓÓÚ‚ÂÚÒÚ‚ÂÌÌÓ, ÔÓˇ‰ÍÓ‚˚È ÌÓÏÂ.
    int year;       ///< —ÓÓÚ‚ÂÚÒÚ‚ÂÌÌÓ, „Ó‰.
    int curDigt;    ///< —ÓÓÚ‚ÂÚÒÚ‚ÂÌÌÓ, ÌÓÏÂÓÏ (0) ËÎË „Ó‰ÓÏ (1) ÛÔ‡‚ÎˇÂÚ Û˜Í‡ ”—“¿ÕŒ¬ ¿.
} StructForSN;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ ///
DEF_PAGE_13
(
    pDebug, ,
    Page_Debug, &mainPage, FuncActive, EmptyPressPage,
    "Œ“À¿ƒ ¿", "DEBUG",
    "",
    "",
    ppConsole,              // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹
    ppADC,                  // Œ“À¿ƒ ¿ - ¿÷œ
    ppRand,			        // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ–
    ppChannels,		        // Œ“À¿ƒ ¿ -  ¿Õ¿À€
    cStats,			        // Œ“À¿ƒ ¿ - —Ú‡ÚËÒÚËÍ‡
    cDisplayOrientation,    // Œ“À¿ƒ ¿ - ŒËÂÌÚ‡ˆËˇ
    cEMS,                   // Œ“À¿ƒ ¿ - –ÂÊËÏ ›Ã—
    mgPred,			        // Œ“À¿ƒ ¿ - œÂ‰Á‡ÔÛÒÍ
    mgPost,			        // Œ“À¿ƒ ¿ - œÓÒÎÂÁ‡ÔÛÒÍ
    ppSettings,		        // Œ“À¿ƒ ¿ - Õ¿—“–Œ… »
    bSaveFirmware,          // Œ“À¿ƒ ¿ - —Óı. ÔÓ¯Ë‚ÍÛ
    ppSerialNumber,         // Œ“À¿ƒ ¿ - —/Õ
    bEraseData              // Œ“À¿ƒ ¿ - —ÚÂÂÚ¸ ‰‡ÌÌ˚Â
);

const Page * pointerPageDebug = &pDebug;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ ///
DEF_PAGE_5(         ppConsole, 
    Page_Debug_Console, &pDebug, FuncActive, EmptyPressPage,
    " ŒÕ—ŒÀ‹", "CONSOLE",
    "",
    "",
    gConsole_NumStrings,    // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - ◊ËÒÎÓ ÒÚÓÍ
    cConsole_SizeFont,      // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –‡ÁÏÂ ¯ËÙÚ‡
    cConsole_ModeStop,      // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –ÂÊ. ÓÒÚ‡ÌÓ‚‡
    pppConsole_Registers,   // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€
    bConsole_SizeSettings   // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –‡ÁÏÂ Ì‡ÒÚÓÂÍ
);

//---------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - ◊ËÒÎÓ ÒÚÓÍ ---
DEF_GOVERNOR
(
    gConsole_NumStrings,
    "◊ËÒÎÓ ÒÚÓÍ", "Number strings",
    "",
    "",
    ppConsole, CONSOLE_NUM_STRINGS, 0, 33, FuncActive, FuncChanged, FuncBeforeDraw
);

//-------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –‡ÁÏÂ ¯ËÙÚ‡ ---
DEF_CHOICE_2
(
    cConsole_SizeFont, ppConsole,
    set.dbg_SizeFont, FuncActive, FuncChangedChoice, FuncDraw,
    "–‡ÁÏÂ ¯ËÙÚ‡", "Size font",
    "",
    "",
    "5", "5",
    "8", "8"    
);

//-------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –ÂÊ. ÓÒÚ‡ÌÓ‚‡ ---
DEF_CHOICE_2
(
    cConsole_ModeStop, ppConsole,
    MODE_PAUSE_CONSOLE, FuncActive, FuncChangedChoice, FuncDraw,
    "–ÂÊ. ÓÒÚ‡ÌÓ‚‡", "Mode stop",
    "œÂ‰ÓÒÚ‡‚ÎˇÂÚ ‚ÓÁÏÓÊÌÓÒÚ¸ ÔËÓÒÚ‡ÌÓ‚ÍË ‚˚‚Ó‰‡ ‚ ÍÓÌÒÓÎ¸ ÔÛÚ∏Ï Ì‡Ê‡ÚËˇ Ì‡ ÍÌÓÔÍÛ œ”— /—“Œœ",
    "It provides the ability to pause the output to the console by pressing the œ”— /—“Œœ button",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ ///
DEF_PAGE_12(    pppConsole_Registers, static,
    Page_Debug_Console_Registers, &ppConsole, FuncActive, EmptyPressPage,
    "–≈√»—“–€", "REGISTERS",
    "",
    "",
    cConsole_Registers_ShowAll,     // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œÓÍ‡Á˚‚‡Ú¸ ‚ÒÂ
    cConsole_Registers_RD_FL,       // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - RD_FL
    cConsole_Registers_RShiftA,     // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒÏ. 1Í
    cConsole_Registers_RShiftB,     // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒÏ. 2Í
    cConsole_Registers_TrigLev,     // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒËÌı
    cConsole_Registers_RangeA,      // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬ŒÀ‹“/ƒ≈À 1
    cConsole_Registers_RangeB,      // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬ŒÀ‹“/ƒ≈À 2
    cConsole_Registers_TrigParam,   // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. ÒËÌı.
    cConsole_Registers_ChanParamA,  // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. Í‡Ì. 1
    cConsole_Registers_ChanParamB,  // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. Í‡Ì. 2
    cConsole_Registers_TBase,       // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬–≈Ãﬂ/ƒ≈À
    cConsole_Registers_TShift       // Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - “ ÒÏ.
);

//-------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œÓÍ‡Á˚‚‡Ú¸ ‚ÒÂ ---
DEF_CHOICE_2
(
    cConsole_Registers_ShowAll, pppConsole_Registers,
    DBG_SHOW_ALL, FuncActive, FuncChangedChoice, FuncDraw,
    "œÓÍ‡Á˚‚‡Ú¸ ‚ÒÂ", "Show all",
    "œÓÍ‡Á˚‚‡Ú¸ ‚ÒÂ ÁÌ‡˜ÂÌËˇ, Á‡Ò˚Î‡ÂÏ˚Â ‚ Â„ËÒÚ˚",
    "To show all values transferred in registers",
    "ÕÂÚ", "No",
    "ƒ‡",  "Yes"
);

//----------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - RD_FL ---
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

//-------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒÏ. 1Í ---
DEF_CHOICE_2
(
    cConsole_Registers_RShiftA, pppConsole_Registers,
    set.dbg_ShowRShift[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U ÒÏ. 1Í", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒÏ. 2Í ---
DEF_CHOICE_2
(
    cConsole_Registers_RShiftB, pppConsole_Registers,
    set.dbg_ShowRShift[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U ÒÏ. 2Í", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//-------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - U ÒËÌı. ---
DEF_CHOICE_2
(
    cConsole_Registers_TrigLev, pppConsole_Registers,
    set.dbg_ShowTrigLev, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "U ÒËÌı.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬ŒÀ‹“/ƒ≈À 1 ---
DEF_CHOICE_2
(
    cConsole_Registers_RangeA, pppConsole_Registers,
    set.dbg_ShowRange[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "¬ŒÀ‹“/ƒ≈À 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//----------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬ŒÀ‹“/ƒ≈À 2 ---
DEF_CHOICE_2
(
    cConsole_Registers_RangeB, pppConsole_Registers,
    set.dbg_ShowRange[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "¬ŒÀ‹“/ƒ≈À 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. ÒËÌı. ---
DEF_CHOICE_2
(
    cConsole_Registers_TrigParam, pppConsole_Registers,
    set.dbg_ShowTrigParam, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "œ‡‡Ï. ÒËÌı.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. Í‡Ì. 2 ---
DEF_CHOICE_2
(
    cConsole_Registers_ChanParamA, pppConsole_Registers,
    set.dbg_ShowChanParam[A], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "œ‡‡Ï. Í‡Ì. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - œ‡‡Ï. Í‡Ì. 2 ---
DEF_CHOICE_2
(
    cConsole_Registers_ChanParamB, pppConsole_Registers,
    set.dbg_ShowChanParam[B], IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "œ‡‡Ï. Í‡Ì. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - ¬–≈Ãﬂ/ƒ≈À ---
DEF_CHOICE_2
(
    cConsole_Registers_TBase, pppConsole_Registers,
    set.dbg_ShowTBase, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "¬–≈Ãﬂ/ƒ≈À", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//----------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –≈√»—“–€ - “ ÒÏ. ---
DEF_CHOICE_2
(
    cConsole_Registers_TShift, pppConsole_Registers,
    set.dbg_ShowTShift, IsActive_Console_Registers, FuncChangedChoice, FuncDraw,
    "“ ÒÏ.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN
);

//------------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ -  ŒÕ—ŒÀ‹ - –‡ÁÏÂ Ì‡ÒÚÓÂÍ ---
static void Draw_Console_SizeSettings(int x, int y)
{
    char buffer[30];
    sprintf(buffer, "–‡ÁÏ.Ì‡ÒÚ. %d", sizeof(Settings));
    painter.DrawText(x + 6, y + 13, buffer, gColorBack);
}

DEF_BUTTON
(
    bConsole_SizeSettings,
    "", "",
    "œÓÍ‡Á˚‚‡ÂÚ ÚÂÍÛ˘ËÈ ‡ÁÏÂ ÒÚÛÍÚÛ˚ ‰Îˇ ÒÓı‡ÌÂÌËˇ Ì‡ÒÚÓÂÍ",
    "Displays the current size of the structure to save settings",
    ppConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ - ¿÷œ ///
DEF_PAGE_3
(
    ppADC, static,
    Page_Debug_ADC, &pDebug, FuncActive, EmptyPressPage,
    "¿÷œ", "ADC",
    "",
    "",
    pppADC_Balance, // Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ—
    pppADC_Stretch, // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿
    pppADC_Shift    // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ
);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— ///
DEF_PAGE_3
(
    pppADC_Balance, static,
    Page_Debug_ADC_Balance, &ppADC, FuncActive, EmptyPressPage,
    "¡¿À¿Õ—", "BALANCE",
    "",
    "",
    cADC_Balance_Mode,      // Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - –ÂÊËÏ
    gADC_Balance_ShiftA,    // Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - —ÏÂ˘ÂÌËÂ 1
    gADC_Balance_ShiftB     // Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - —ÏÂ˘ÂÌËÂ 2
);

//----------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - –ÂÊËÏ ---
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
    "–ÂÊËÏ", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "–Â‡Î¸Ì˚È", "Real",
    "–Û˜ÌÓÈ",   "Manual"
);

//------------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - —ÏÂ˘ÂÌËÂ 1 ---
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
    "—ÏÂ˘ÂÌËÂ 1", "Offset 1",
    "",
    "",
    pppADC_Balance, shiftADCA, -125, 125, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftA, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - ¿÷œ - ¡¿À¿Õ— - —ÏÂ˘ÂÌËÂ 2 ---
static void OnChanged_ADC_Balance_ShiftB(void)
{
    NRST_BALANCE_ADC_B = shiftADCB;
}

DEF_GOVERNOR
(
    gADC_Balance_ShiftB,
    "—ÏÂ˘ÂÌËÂ 2", "Offset 2",
    "",
    "",
    pppADC_Balance, shiftADCB, -125, 125, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftB, FuncBeforeDraw
);

static const Choice emptyChoice = {Item_Choice};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ ///
DEF_PAGE_15
(
    pppADC_Stretch, static,
    Page_Debug_ADC_Stretch, &ppADC, FuncActive, EmptyPressPage,
    "–¿—“ﬂ∆ ¿", "STRETCH",
    "”ÒÚ‡Ì‡‚ÎË‚‡ÂÚ ÂÊËÏ Ë ‚ÂÎË˜ËÌÛ ‡ÒÚˇÊÍË (‰Îˇ Û˜ÌÓ„Ó ÂÊËÏ‡)",
    "Sets mode and the value of stretching (manual mode)",
    cADC_Stretch_Mode,      // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –ÂÊËÏ
    gADC_Stretch_A,         // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –‡ÒÚˇÊÍ‡ 1Í
    gADC_Stretch_B,         // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –‡ÒÚˇÊÍ‡ 2Í
    emptyChoice,
    emptyChoice,
    gADC_Stretch_Ak20mV,    // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 20Ï¬/1¬ 1Í
    gADC_Stretch_Ak50mV,    // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 50Ï¬ 1Í 
    gADC_Stretch_Ak100mV,   // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 100Ï¬/5¬ 1Í
    gADC_Stretch_Ak2V,      // Œ“À¿ƒ ¿ - A÷œ - –¿—“ﬂ∆ ¿ - 2¬ 1Í
    emptyChoice,
    gADC_Stretch_Bk20mV,    // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 20Ï¬/1¬ 2Í
    gADC_Stretch_Bk50mV,    // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 50Ï¬ 2Í 
    gADC_Stretch_Bk100mV,   // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 100Ï¬/5¬ 2Í
    gADC_Stretch_Bk2V,      // Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 2¬ 2Í
    emptyChoice
);

//--------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –ÂÊËÏ ---
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
    "–ÂÊËÏ", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "–Â‡Î¸Ì˚È", "Real",
    "–Û˜ÌÓÈ",   "Manual"
);

//--------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –‡ÒÚˇÊÍ‡ 1Í ---
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
    "–‡ÒÚˇÊÍ‡ 1Í", "Stretch 1ch",
    "«‡‰‡∏Ú Û˜ÌÛ˛ ‡ÒÚˇÊÍÛ ÔÂ‚Ó„Ó Í‡Ì‡Î‡.\n1 Â‰ËÌËˆ‡ = 0.0001",
    "Sets the manual stretching of the first channel.\n1 = 0.0001",
    pppADC_Stretch, stretchA, -10000, 10000, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_A, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - –‡ÒÚˇÊÍ‡ 2Í ---
static void OnChanged_ADC_Stretch_B(void)
{
    NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE) = stretchB;
}

DEF_GOVERNOR
(
    gADC_Stretch_B,
    "–‡ÒÚˇÊÍ‡ 2Í", "Stretch 2ch",
    "«‡‰‡∏Ú Û˜ÌÛ˛ ‡ÒÚˇÊÍÛ ‚ÚÓÓ„Ó Í‡Ì‡Î‡.\n1 Â‰ËÌËˆ‡ = 0.0001",
    "Sets the manual stretching of the second channel.\n1 = 0.0001",
    pppADC_Stretch, stretchB, -10000, 10000, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_B, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 20Ï¬/1¬ 1Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak20mV,
    "20Ï¬/1¬ 1Í", "20mV/1V 1k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_20mV_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 50Ï¬ 1Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak50mV,
    "50Ï¬ 1Í", "50mV 1k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_50mV_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 100Ï¬/5¬ 1Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak100mV,
    "100Ï¬/5¬ 1Í", "100mV/5V 1ch",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_100mV_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 2¬ 1Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Ak2V,
    "2¬ 1Í", "2V 1ch",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_2V_A, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 20Ï¬/1¬ 2Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk20mV,
    "20Ï¬/1¬ 2Í", "20mV/1V 2k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_20mV_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - ƒÓÔ ÒÏÂ˘ 50Ï¬ 2Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk50mV,
    "50Ï¬ 2Í", "50mV 2k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_50mV_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 100Ï¬/5¬ 2Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk100mV,
    "100Ï¬/5¬ 2Í", "100mV/5V 2k",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_100mV_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - –¿—“ﬂ∆ ¿ - 2¬ 2Í ---
DEF_GOVERNOR
(
    gADC_Stretch_Bk2V,
    "2¬ 2Í", "2V 2ch",
    "",
    "",
    pppADC_Stretch, NRST_ADD_STRETCH_2V_B, -10000, 10000, FuncActive, FuncChanged, FuncBeforeDraw
);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ ///
DEF_PAGE_7(         pppADC_Shift,
    static,
    Page_Debug_ADC_Shift, &ppADC, FuncActive, EmptyPressPage,
    "ƒŒœ —Ã≈Ÿ", "ADD RSHFIT",
    "",
    "",
    bADC_Shift_Reset,   // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —·ÓÒ
    gADC_Shift_A2mV,    // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 2Ï¬ ÔÓÒÚ
    gADC_Shift_B2mV,    // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 2Ï¬ ÔÓÒÚ
    gADC_Shift_A5mV,    // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 5Ï¬ ÔÓÒÚ
    gADC_Shift_B5mV,    // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 5Ï¬ ÔÓÒÚ
    gADC_Shift_A10mV,   // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 10Ï¬ ÔÓÒÚ
    gADC_Shift_B10mV    // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 10Ï¬ ÔÓÒÚ
);

// Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —·ÓÒ ------------------------------------------------------------------------------------------------------------------
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

DEF_BUTTON
(
    bADC_Shift_Reset,
    "—·ÓÒ", "Reset",
    "",
    "",    
    pppADC_Shift, FuncActive, OnPress_ADC_Shift_Reset, FuncDraw);

//------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 2Ï¬ ÔÓÒÚ ---
static void OnChanged_ADC_Shift_A(void)
{
    FPGA_SetRShift(A, SET_RSHIFT_A);
}

DEF_GOVERNOR
(
    gADC_Shift_A2mV,
    "—Ï 1Í 2Ï¬ ÔÓÒÚ", "Shift 1ch 2mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_A(Range_2mV), -100, 100, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 2Ï¬ ÔÓÒÚ ---
static void OnChanged_ADC_Shift_B(void)
{
    FPGA_SetRShift(B, SET_RSHIFT_B);
}

DEF_GOVERNOR
(
    gADC_Shift_B2mV,
    "—Ï 2Í 2Ï¬ ÔÓÒÚ", "Shift 2ch 2mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_B(Range_2mV), -100, 100, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 5Ï¬ ÔÓÒÚ ---
DEF_GOVERNOR
(
    gADC_Shift_A5mV,
    "—Ï 1Í 5Ï¬ ÔÓÒÚ", "Shift 1ch 5mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_A(Range_5mV), -100, 100, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 5Ï¬ ÔÓÒÚ ---
DEF_GOVERNOR
(
    gADC_Shift_B5mV,
    "—Ï 2Í 5Ï¬ ÔÓÒÚ", "Shift 2ch 5mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_B(Range_5mV), -100, 100, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 1Í 10Ï¬ ÔÓÒÚ ---
DEF_GOVERNOR
(
    gADC_Shift_A10mV,
    "—Ï 1Í 10Ï¬ ÔÓÒÚ", "Shift 1ch 10mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_A(Range_10mV), -100, 100, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
);

//----------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ - —Ï 2Í 10Ï¬ ÔÓÒÚ ---
DEF_GOVERNOR
(
    gADC_Shift_B10mV,
    "—Ï 2Í 10Ï¬ ÔÓÒÚ", "Shift 2ch 10mV DC",
    "",
    "",
    pppADC_Shift, RSHIFT_ADD_STABLE_B(Range_10mV), -100, 100, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– ///
DEF_PAGE_8(     ppRand, static,
    Page_Debug_Rand, &pDebug, FuncActive, EmptyPressPage,
    "–¿Õƒ-“Œ–", "RANDOMIZER",
    "",
    "",
    gRand_NumAverage,       // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - ”ÒÂ‰Ì.
    gRand_NumSmooth,        // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —„Î‡ÊË‚‡ÌËÂ
    gRand_NumMeasures,      // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - ¬˚·-Í/‚ÓÓÚ‡
    cRand_ShowInfo,         // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - »ÌÙÓÏ‡ˆËˇ
    gRand_ShowStat,         // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —Ú‡ÚËÒÚËÍ‡
    gRand_TimeCompensation, // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– -  ÓÏÔÂÌÒ‡ˆËˇ Á‡‰ÂÊÍË
    gRand_AddTimeShift,     // Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —ÏÂ˘ÂÌËÂ
    gRand_Pretriggered      // Œ“À¿ƒ ¿ - –AÕƒ-“Œ– - œÂ‰Á‡ÔÛÒÍ
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(     ppChannels,                                                                                                    // Œ“À¿ƒ ¿ -  ¿Õ¿À€ ///
    Page_Debug_Channels, &pDebug, FuncActive, EmptyPressPage,
    " ¿Õ¿À€", "CHANNELS",
    "",
    "",
    cChannels_BandwidthA,   // Œ“À¿ƒ ¿ -  ¿Õ¿À€ - œÓÎÓÒ‡ 1
    cChannels_BandwidthB    // Œ“À¿ƒ ¿ -  ¿Õ¿À€ - œÓÎÓÒ‡ 2
);

//-------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ¿ÕÀ¿€ - œÓÎÓÒ‡ 1 ---
static void OnChanged_Channels_BandwidthA(bool)
{
    FPGA_SetBandwidth(A);
}

DEF_CHOICE_7
(
    cChannels_BandwidthA, ppChannels,
    BANDWIDTH_DEBUG(A), FuncActive, OnChanged_Channels_BandwidthA, FuncDraw,
    "œÓÎÓÒ‡ 1", "Bandwidth 1",
    "«‰ÂÒ¸ ÏÓÊÌÓ ‚˚·‡Ú¸ ÔÓÎÓÒÛ, ÍÓÚÓ‡ˇ ·Û‰ÂÚ ‰ÂÈÒÚ‚Ó‚‡Ú¸ ‚  ¿Õ¿À1-œÓÎÓÒ‡ ÔË ‚˚·ÓÂ ÁÌ‡˜ÂÌËˇ œÓÎÌ‡ˇ",
    "Here you can select the bandwidth, which will operate in CHANNEL1-Bandwidth when set to Full",
    "œÓÎÌ‡ˇ", "Full",
    "20Ã√ˆ", "20MHz",
    "100Ã√ˆ", "100MHz",
    "200Ã√ˆ", "200MHz",
    "350Ã√ˆ", "350MHz",
    "650Ã√ˆ", "650MHz",
    "750Ã√ˆ", "750MHz"
);

//-------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ -  ¿ÕÀ¿€ - œÓÎÓÒ‡ 2 ---
static void OnChanged_Channels_BandwidthB(bool)
{
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_7
(
    cChannels_BandwidthB, ppChannels,
    BANDWIDTH_DEBUG(B), FuncActive, OnChanged_Channels_BandwidthB, FuncDraw,
    "œÓÎÓÒ‡ 2", "Bandwidth 2",
    "«‰ÂÒ¸ ÏÓÊÌÓ ‚˚·‡Ú¸ ÔÓÎÓÒÛ, ÍÓÚÓ‡ˇ ·Û‰ÂÚ ‰ÂÈÒÚ‚Ó‚‡Ú¸ ‚  ¿Õ¿À2-œÓÎÓÒ‡ ÔË ‚˚·ÓÂ ÁÌ‡˜ÂÌËˇ œÓÎÌ‡ˇ",
    "Here you can select the bandwidth, which will operate in CHANNEL2-Bandwidth when set to Full",
    "œÓÎÌ‡ˇ", "Full",
    "20Ã√ˆ", "20MHz",
    "100Ã√ˆ", "100MHz",
    "200Ã√ˆ", "200MHz",
    "350Ã√ˆ", "350MHz",
    "650Ã√ˆ", "650MHz",
    "750Ã√ˆ", "750MHz"
);

//-------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - ¬˚·-Í/‚ÓÓÚ‡ ---
static void OnChanged_Rand_NumMeasures(void)
{
    FPGA_SetNumberMeasuresForGates(NUM_MEASURES_FOR_GATES);
}

DEF_GOVERNOR
(
    gRand_NumMeasures,
    "¬˚·-Í/‚ÓÓÚ‡", "Samples/gates",
    "",
    "",
    ppRand, NUM_MEASURES_FOR_GATES, 1, 2500,
    FuncActive, OnChanged_Rand_NumMeasures, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– -  ÓÏÔÂÌÒ‡ˆËˇ Á‡‰ÂÊÍË ---
static void OnChanged_Rand_TimeCompensation(void)
{
    FPGA_SetDeltaTShift(TIME_COMPENSATION);
}

DEF_GOVERNOR
(
    gRand_TimeCompensation,
    " ÓÏÔÂÌÒ‡ˆËˇ Á‡‰ÂÊÍË", "Compenstaion time",
    "œÓ‰ÒÚÓÈÍ‡ ÍÓÏÔÂÌÒ‡ˆËË Á‡‰ÂÊÍË ¿÷œ 40 ÌÒ",
    "",
    ppRand, TIME_COMPENSATION, 0, 510, FuncActive, OnChanged_Rand_TimeCompensation, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------------ Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —ÏÂ˘ÂÌËÂ ---
static void OnChanged_Rand_AddTimeShift(void)
{
    FPGA_SetTShift(SET_TSHIFT);
}

int16 addShift = 0;

DEF_GOVERNOR
(
    gRand_AddTimeShift,
    "ƒÓÔ ÒÏÂ˘ÂÌËÂ", "Add shift",
    "ƒÓ·‡‚Ó˜ÌÓÂ ÒÏ˘ÂÌËÂ ÔË ‚‡˘ÂÌËË tShift",
    "",
    ppRand, addShift, -100, 100, FuncActive, OnChanged_Rand_AddTimeShift, FuncBeforeDraw
);

//---------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - œÂ‰Á‡ÔÛÒÍ ---
static void OnChanged_Rand_Pretriggered(void)
{
    LoadTShift();
}

DEF_GOVERNOR
(
    gRand_Pretriggered,
    "œÂ‰Á‡ÔÛÒÍ", "Pretiggered",
    "¬ÂÎË˜ËÌ‡ ÔÂ‰Á‡ÔÛÒÍ‡, ÍÓÚÓ‡ˇ ÔË¯ÂÚÒˇ ‚ ‡Ì‰ÓÏËÁ‡ÚÓ",
    "",
    ppRand, PRETRIGGERED, 0, 30000, FuncActive, OnChanged_Rand_Pretriggered, FuncBeforeDraw
);

//------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - ”ÒÂ‰Ì. ---
DEF_GOVERNOR
(
    gRand_NumAverage,
    "”ÒÂ‰Ì.", "Average",
    "",
    "",
    ppRand, NRST_NUM_AVE_FOR_RAND, 1, 32, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —„Î‡ÊË‚‡ÌËÂ ---
DEF_GOVERNOR
(
    gRand_NumSmooth,
    "—„Î‡ÊË‚‡ÌËÂ", "Smoothing",
    "",
    "",
    ppRand, NRST_NUM_SMOOTH_FOR_RAND, 1, 10, FuncActive, FuncChanged, FuncBeforeDraw
);

//--------------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - œÂ‰Á‡ÔÛÒÍ ---
static int16 pred;

DEF_GOVERNOR
(
    mgPred,
    "œÂ‰Á‡ÔÛÒÍ", "",
    "", "",
    pDebug, pred, 0, 15000, FuncActive, OnChanged_Pred, FuncBeforeDraw
);

// Œ“À¿ƒ ¿ - œÓÒÎÂÁ‡ÔÛÒÍ -----------------------------------------------------------------------------------------------------------------------------
static int16 post;

DEF_GOVERNOR
(
    mgPost,
    "œÓÒÎÂÁ‡ÔÛÒÍ", "",
    "", "",
    pDebug, post, 0, 15000, FuncActive, OnChanged_Post, FuncBeforeDraw
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_SB(        ppSettings,                                                                                             // Œ“À¿ƒ ¿ - Õ¿—“–Œ… » ///
    PageSB_Debug_Settings, &pDebug, FuncActive,
    OnPress_Settings, FuncDrawPage, FuncRegSetPage,
    "Õ¿—“–Œ… »", "SETTINGS",
    "œÓÍ‡Á‡Ú¸ ËÌÙÓÏ‡ˆË˛ Ó Ì‡ÒÚÓÈÍ‡ı",
    "Show settings information",
    &bSettings_Exit,    // Œ“À¿ƒ ¿ - Õ¿—“–Œ… » - ¬˚ıÓ‰
    0,
    0,
    0,
    0,
    0
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Settings_Exit(void)
{
    display.SetDrawMode(DrawMode_Auto, 0);
}

DEF_SMALL_BUTTON_EXIT(  bSettings_Exit,                                                                          //--- Œ“À¿ƒ ¿ - Õ¿—“–Œ… » - ¬˚ıÓ‰ ---
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

    //Painter_DrawFormatText(x0, Y_AND_INCREASE, "–‡ÁÏÂ ÓÒÌÓ‚ÌÓÈ ÒÚÛÍÚÛ˚ %d", sizeof(set));
    DRAW_FORMAT("–‡ÁÏÂ ÓÒÌÓ‚ÌÓÈ ÒÚÛÍÚÛ˚ : %d", sizeof(set));
    painter.DrawText(x0, Y_AND_INCREASE, "ÕÂÒ·‡Ò˚‚‡ÂÏ‡ˇ ÒÚÛÍÚÛ‡:");
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

    pString s[3] = {"‚˚ÍÎ˛˜ÂÌÓ", "Ì‡ÒÚÓÂÌÓ ‡‚ÚÓÏ‡ÚË˜ÂÒÍË", "Á‡‰‡ÌÓ ‚Û˜ÌÛ˛"};
    DRAW_FORMAT("balanceADCtype : %s", (NRST_BALANCE_ADC_TYPE < 3 ? s[NRST_BALANCE_ADC_TYPE] : "!!! ÌÂÔ‡‚ËÎ¸ÌÓÂ ÁÌ‡˜ÂÌËÂ !!!"));
    DRAW_FORMAT("stretchADCtype : %s", (NRST_STRETCH_ADC_TYPE < 3 ? s[NRST_STRETCH_ADC_TYPE] : "!!! ÌÂÔ‡‚ËÎ¸ÌÓÂ ÁÌ‡˜ÂÌËÂ !!!"));
  
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

static void OnChanged_Pred(void)
{
    gPred = ~pred;
    static char buffer[30];
    LOG_WRITE("pred %d %s", pred, su.Hex16toString((uint16)gPred, buffer));
}

static void OnChanged_Post(void)
{
    gPost = (uint16)~post;
    static char buffer[30];
    LOG_WRITE("post %d %s", post, su.Hex16toString(gPost, buffer));
}

//---------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - »ÌÙÓÏ‡ˆËˇ ---
DEF_CHOICE_2
(
    cRand_ShowInfo, ppRand,
    SHOW_RAND_INFO, FuncActive, FuncChangedChoice, FuncDraw,
    "»ÌÙÓÏ‡ˆËˇ", "Information",
    "œÓÍ‡Á˚‚‡Ú¸ ËÌÙÓÏ‡ˆË˛ Ó ‚ÓÓÚ‡ı ‡Ì‰ÓÏËÁ‡ÚÓ‡",
    "To show information on randomizer gate",
    "ÕÂ ÔÓÍ‡Á˚‚‡Ú¸", "Hide",
    "œÓÍ‡Á˚‚‡Ú¸",    "Show"
);

//---------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - –¿Õƒ-“Œ– - —Ú‡ÚËÒÚËÍ‡ ---
DEF_CHOICE_2
(
    gRand_ShowStat, ppRand,
    SHOW_RAND_STAT, FuncActive, FuncChangedChoice, FuncDraw,
    "—Ú‡ÚËÒÚËÍ‡", "Statistics",
    "œÓÍ‡Á˚‚‡Ú¸ „‡ÙËÍ ÒÚ‡ÚËÒÚËÍË",
    "Statistics show schedule",
    "ÕÂ ÔÓÍ‡Á˚‚‡Ú¸", "Hide",
    "œÓÍ‡Á˚‚‡Ú¸",    "Show"
);

//---------------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - –ÂÊËÏ ›Ã— ---
static void OnChanged_EMS(bool)
{
    FPGA_SetBandwidth(A);
    FPGA_SetBandwidth(B);
}

DEF_CHOICE_2
(
    cEMS, pDebug,
    MODE_EMS, FuncActive, OnChanged_EMS, FuncDraw,
    "–ÂÊËÏ ›Ã—", "EMS mode",
    "œËÌÛ‰ËÚÂÎ¸ÌÓ ‚ÍÎ˛˜‡ÂÚ ÙËÎ¸Ú 20Ã√ˆ, Ò„Î‡ÊË‚‡ÌËÂ ÔÓ 4-Ï ÚÓ˜Í‡Ï, ÛÒÂ‰ÌÂÌËÂ ÔÓ 8-ÏË ÚÓ˜Í‡Ï",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN
);

//--------------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - ŒËÂÌÚ‡ˆËˇ ---
void OnChanged_DisplayOrientation(bool)
{
    display.SetOrientation(DISPLAY_ORIENTATION);
}

DEF_CHOICE_2
(
    cDisplayOrientation, pDebug,
    DISPLAY_ORIENTATION, FuncActive, OnChanged_DisplayOrientation, FuncDraw,
    "ŒËÂÌÚ‡ˆËˇ", "DisplayOrientation",
    "”ÒÚ‡Ì‡‚ÎË‚‡ÂÚ ÓËÂÌÚ‡ˆË˛ ‰ËÒÔÎÂˇ",
    "Sets display orientation",
    "œˇÏ‡ˇ",   "Direct",
    "Œ·‡ÚÌ‡ˇ", "Back"
);

//--------------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - —Ú‡ÚËÒÚËÍ‡ ---
DEF_CHOICE_2
(
    cStats, pDebug,
    SHOW_STAT, FuncActive, FuncChangedChoice, FuncDraw,
    "—Ú‡ÚËÒÚËÍ‡", "Statistics",
    "œÓÍ‡Á˚‚‡Ú¸/ÌÂ ÔÓÍ‡Á˚‚‡Ú¸ ‚ÂÏˇ/Í‡‰, Í‡‰Ó‚ ‚ ÒÂÍÛÌ‰Û, ÍÓÎË˜ÂÒÚ‚Ó ÒË„Ì‡ÎÓ‚ Ò ÔÓÒÎÂ‰ÌËÏË Ì‡ÒÚÓÈÍ‡ÏË ‚ Ô‡ÏˇÚË/ÍÓÎË˜ÂÒÚ‚Ó ÒÓı‡ÌˇÂÏ˚ı ‚ Ô‡ÏˇÚË ÒË„Ì‡ÎÓ‚",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "ÕÂ ÔÓÍ‡Á˚‚‡Ú¸", "Hide",
    "œÓÍ‡Á˚‚‡Ú¸",    "Show"
);

// Œ“À¿ƒ ¿ - —Óı. ÔÓ¯Ë‚ÍÛ --------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(
    bSaveFirmware,
    "—Óı. ÔÓ¯Ë‚ÍÛ", "Save firmware",
    "—Óı‡ÌÂÌËÂ ÔÓ¯Ë‚ÍË - ÒÂÍÚÓÓ‚ 5, 6, 7 Ó·˘ËÏ Ó·˙∏ÏÓÏ 3 ı 128 Í¡, „‰Â ı‡ÌËÚÒˇ ÔÓ„‡ÏÏ‡",
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

// Œ“À¿ƒ ¿ - —ÚÂÂÚ¸ ‰‡ÌÌ˚Â --------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(
    bEraseData,
    "—ÚÂÂÚ¸ ‰‡ÌÌÂ", "Erase data",
    "—ÚË‡ÂÚ ÒÓı‡Ì∏ÌÌ˚Â ‰‡ÌÌ˚Â ËÁ œœ«”",
    "Erase all saved datas from EEPROM",
    pDebug, FuncActive, OnPress_EraseData, FuncDraw);

static void OnPress_EraseData(void)
{
    display.FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    FLASH_DeleteAllData();
    display.FuncOnWaitStop();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_SB(        ppSerialNumber,                                                                                               // Œ“À¿ƒ ¿ - —/Õ ///
    PageSB_Debug_SerialNumber, &pDebug,
    FuncActive, OnPress_SerialNumber, FuncDrawPage, OnRegSet_SerialNumber,
    "—/Õ", "S/N",
    "«‡ÔËÒ¸ ÒÂËÈÌÓ„Ó ÌÓÏÂ‡ ‚ OTP-Ô‡ÏˇÚ¸. ¬Õ»Ã¿Õ»≈!!! Œ“P-Ô‡ÏˇÚ¸ - Ô‡ÏˇÚ¸ Ò Ó‰ÌÓÍ‡ÚÌÓÈ Á‡ÔËÒ¸˛.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    &bSerialNumber_Exit,    // Œ“À¿ƒ ¿ - —/Õ - ¬˚ıÓ‰
    &bSerialNumber_Change,  // Œ“À¿ƒ ¿ - —/Õ - œÂÂÈÚË
    0,
    0,
    0,
    &bSerialNumber_Save     // Œ“À¿ƒ ¿ - —/Õ - —Óı‡ÌËÚ¸
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

    // “ÂÔÂ¸ ‚˚‚Â‰ÂÏ ËÌÙÓÏ‡ˆË˛ Ó· ÓÒÚ‡‚¯ÂÏÒˇ ÏÂÒÚÂ ‚ OTP-Ô‡ÏˇÚË ‰Îˇ Á‡ÔËÒË

    int allShots = OTP_GetSerialNumber(buffer);

    painter.DrawFormText(x0 + deltaX, y0 + 130, gColorFill, "“ÂÍÛ˘ËÈ ÒÓı‡Ì∏ÌÌ˚È ÌÓÏÂ %s", buffer[0] == 0 ? "-- ----" : buffer);

    painter.DrawFormText(x0 + deltaX, y0 + 100, gColorFill, "ŒÒÚ‡ÎÓÒ¸ ÏÂÒÚ‡ ‰Îˇ %d ÔÓÔ˚ÚÓÍ", allShots);
}

static void OnRegSet_SerialNumber(int angle)
{
    typedef int (*pFunc)(int *, int, int);

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

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Exit(void)
{
    OnPressSB_Exit();
    FREE_EXTRAMEM();
}

DEF_SMALL_BUTTON_EXIT(  bSerialNumber_Exit,                                                                            //--- Œ“À¿ƒ ¿ - —/Õ - ¬˚ıÓ‰ ---
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Exit, DrawSB_Exit
);


//----------------------------------------------------------------------------------------------------------------------- Œ“À¿ƒ ¿ - —/Õ - ¬ÒÚ‡‚ËÚ¸ ---
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
    "¬ÒÚ‡‚ËÚ¸", "Insert",
    "¬ÒÚ‡‚ÎˇÂÚ ‚˚·‡Ì˚È ÒËÏ‚ÓÎ",
    "Inserts the chosen symbol",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
);

// Œ“À¿ƒ ¿ - —/Õ - —Óı‡ÌËÚ¸ -------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON
(
    bSerialNumber_Save,
    "—Óı‡ÌËÚ¸", "Save",
    "«‡ÔËÒ˚‚‡ÂÚ ÒÂËÈÌ˚È ÌÓÏÂ ‚ OTP",
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

        /// Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ. —Ã≈Ÿ. œ¿Ã. - ¬ÂÎË˜ËÌ‡
        const Governor mgDebugADCaltShift =
        {
            Item_Governor, &mspDebugADCaltShift,
            {
                "¬ÂÎË˜ËÌ‡", "Value"
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


const Page mspDebugADCaltShift =    // Œ“À¿ƒ ¿ - ¿÷œ - ƒŒœ —Ã≈Ÿ œ¿Ã
{
    Item_Page, &ppADC,
    {
        "ƒŒœ —Ã≈Ÿ œ¿Ã", "ALT SHIFT MEM"
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
