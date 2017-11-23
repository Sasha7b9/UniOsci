#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/CommonFunctions.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Page pChanA;
extern const Page pChanB;
extern const Page pTrig;
extern const Page pTime;
extern const Page pDisplay;
extern const Page pCursors;
extern const Page pMemory;
extern const Page pMeasures;
extern const Page pService;
extern const Page pHelp;
extern const Page pDebug;
extern const Page mainPage;


/// Ã≈Õﬁ
DEF_PAGE_11(mainPage,,
    Page_Main, 0, FuncActive, EmptyPressPage,
    "Ã≈Õﬁ", "MENU",
    "", "",
    pDisplay,   // ƒ»—œÀ≈…
    pChanA,     //  ¿Õ¿À 1
    pChanB,     //  ¿Õ¿À 2
    pTrig,      // —»Õ’–
    pTime,      // –¿«¬≈–“ ¿
    pCursors,   //  ”–—Œ–€
    pMemory,    // œ¿Ãﬂ“‹
    pMeasures,  // »«Ã≈–≈Õ»ﬂ
    pService,   // —≈–¬»—
    pHelp,      // œŒÃŒŸ‹
    pDebug      // Œ“À¿ƒ ¿
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
const void *PageForButton(PanelButton button)
{
    static const void *pages[] =
    {  
        0,                  // B_Empty
        (void *)&pChanA,    // B_ChannelA
        (void *)&pService,  // B_Service
        (void *)&pChanB,    // B_ChannelB
        (void *)&pDisplay,  // B_Display
        (void *)&pTime,     // B_Time
        (void *)&pMemory,   // B_Memory
        (void *)&pTrig,     // B_Trig
        0,                  // B_Start
        (void *)&pCursors,  // B_Cursors
        (void *)&pMeasures, // B_Measures
        0,                  // B_Power
        (void *)&pHelp,     // B_Help
        0,                  // B_Menu
        0,                  // B_F1
        0,                  // B_F2
        0,                  // B_F3
        0,                  // B_F4
        0,                  // B_F5
    };

    return pages[button];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool IsMainPage(const void *item)
{
    return item == &mainPage;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawSB_Exit(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x2e');
    painter.SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressSB_Exit(void)
{
    display.RemoveAddDrawFunction();
}
