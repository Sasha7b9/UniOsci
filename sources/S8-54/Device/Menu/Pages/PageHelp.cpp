#include "defines.h"
#include "Globals.h"
#include "Menu/MenuItems.h"
#include "Definition.h"
#include "HelpContent.h"
#include "Utils/CommonFunctions.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Page pHelp;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawSB_Help_ParagraphEnter(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    painter.SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawSB_Help_ParagraphLeave(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    painter.SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawSB_Help_ParagraphPrev(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 5, '\x4c');
    painter.SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawSB_Help_ParagraphNext(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 5, '\x4e');
    painter.SetFont(TypeFont_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnHelpRegSet(int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON
(
    sbHelpParagraphEnter,
    "Открыть",  "Open",
    "Открывает раздел справки",
    "Opens the section of the reference",
    pHelp, HelpContent_EnterParagraphIsActive, HelpContent_EnterParagraph, DrawSB_Help_ParagraphEnter
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON
(
    sbHelpParagraphLeave,
     "Закрыть", "Close",
     "Закрывает раздел справки",
     "Closes the section of the reference",
    pHelp, HelpContent_LeaveParagraphIsActive, HelpContent_LeaveParagraph, DrawSB_Help_ParagraphLeave
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON
(  
    sbHelpParagraphPrev,
    "Предыдущий раздел", "Previous section",
    "Выбрать предыдущий раздел справки",
    "To choose the previous section of the reference",
    pHelp, FuncActive, HelpContent_PrevParagraph, DrawSB_Help_ParagraphPrev
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON
(
    sbHelpParagraphNext,
    "", 
    "",
    "", "",
    pHelp, FuncActive, HelpContent_NextParagraph, DrawSB_Help_ParagraphNext
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON
(
    sbExitHelp,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button to return to the previous menu",
    pHelp, FuncActive, HelpContent_NextParagraph, DrawSB_Help_ParagraphNext
);

//----------------------------------------------------------------------------------------------------------------------------------------------------

const Page * pointerPageHelp = &pHelp;

DEF_PAGE_SB(pHelp, ,
    PageSB_Help, &mainPage, FuncActive,
    FuncPress, HelpContent_Draw, OnHelpRegSet,
    "ПОМОЩЬ", "HELP",
    "Открыть разделы помощи",
    "To open sections of the help",
    &sbExitHelp,
    &sbHelpParagraphEnter,
    &sbHelpParagraphLeave,
    0,
    &sbHelpParagraphPrev,
    &sbHelpParagraphNext
);
