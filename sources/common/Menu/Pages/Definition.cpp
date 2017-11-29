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

//---------------------------------------------------------------------------------------------------------------------------------------------------
int CalculateYforCurs(int y, bool top)
{
    return top ? y + MI_HEIGHT / 2 + 4 : y + MI_HEIGHT - 2;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
int CalculateXforCurs(int x, bool left)
{
    return left ? x + MI_WIDTH - 20 : x + MI_WIDTH - 5;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateXY(int *x0, int *x1, int *y0, int *y1)
{
    *x0 = CalculateXforCurs(*x0, true);
    *x1 = CalculateXforCurs(*x1, false);
    *y0 = CalculateYforCurs(*y0, true);
    *y1 = CalculateYforCurs(*y1, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursVoltage(int x, int y, bool top, bool bottom)
{
    x -= 65;
    y -= 21;
    int x0 = x, x1 = x, y0 = y, y1 = y;
    CalculateXY(&x0, &x1, &y0, &y1);
    for (int i = 0; i < (top ? 3 : 1); i++)
    {
        painter.DrawHLine(y0 + i, x0, x1);
    }
    for (int i = 0; i < (bottom ? 3 : 1); i++)
    {
        painter.DrawHLine(y1 - i, x0, x1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateConditions(int16 pos0, int16 pos1, CursCntrl cursCntrl, bool *cond0, bool *cond1)
{
    bool zeroLessFirst = pos0 < pos1;
    *cond0 = cursCntrl == CursCntrl_1_2 || (cursCntrl == CursCntrl_1 && zeroLessFirst) || (cursCntrl == CursCntrl_2 && !zeroLessFirst);
    *cond1 = cursCntrl == CursCntrl_1_2 || (cursCntrl == CursCntrl_1 && !zeroLessFirst) || (cursCntrl == CursCntrl_2 && zeroLessFirst);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursTime(int x, int y, bool left, bool right)
{
    x -= 58;
    y -= 16;
    int x0 = x, x1 = x, y0 = y, y1 = y;
    CalculateXY(&x0, &x1, &y0, &y1);
    for (int i = 0; i < (left ? 3 : 1); i++)
    {
        painter.DrawVLine(x0 + i, y0, y1);
    }
    for (int i = 0; i < (right ? 3 : 1); i++)
    {
        painter.DrawVLine(x1 - i, y0, y1);
    }
}
