#include "Globals.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "HelpContent.h"
#include "Utils/CommonFunctions.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageHelp
 *  @{
 */

extern const Page mpHelp;

void DrawSB_Help_ParagraphEnter(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    painter.SetFont(TypeFont_8);
}

void DrawSB_Help_ParagraphLeave(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    painter.SetFont(TypeFont_8);
}

void DrawSB_Help_ParagraphPrev(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 5, '\x4c');
    painter.SetFont(TypeFont_8);
}

void DrawSB_Help_ParagraphNext(int x, int y)
{
    painter.SetFont(TypeFont_UGO2);
    painter.Draw4SymbolsInRect(x + 2, y + 5, '\x4e');
    painter.SetFont(TypeFont_8);
}

void OnHelpRegSet(int angle) 
{

}

DEF_SMALL_BUTTON(   sbHelpParagraphEnter,                                                                                      // ������ - ������� ///
    "�������", "Open",
    "��������� ������ �������",
    "Opens the section of the reference",
    mpHelp, HelpContent_EnterParagraphIsActive, HelpContent_EnterParagraph, DrawSB_Help_ParagraphEnter
);

DEF_SMALL_BUTTON(   sbHelpParagraphLeave,                                                                                      // ������ - ������� ///
    "�������", "Close",
    "��������� ������ �������",
    "Closes the section of the reference",
    mpHelp, HelpContent_LeaveParagraphIsActive, HelpContent_LeaveParagraph, DrawSB_Help_ParagraphLeave
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphPrev,                                                                             // ������ - ���������� ������ ///
    "���������� ������", "Previous section",
    "������� ���������� ������ �������",
    "To choose the previous section of the reference",
    mpHelp, FuncActive, HelpContent_PrevParagraph,  DrawSB_Help_ParagraphPrev
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphNext,                                                                              // ������ - ��������� ������ ///
    "��������� ������", "Next section",
    "������� ��������� ������ �������",
    "To choose the next section of the reference",
    mpHelp, FuncActive, HelpContent_NextParagraph, DrawSB_Help_ParagraphNext
);

static void PressSB_Help_Exit()
{
    display.RemoveAddDrawFunction();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  sbExitHelp,                                                                                              // ������ - ����� ///
    mpHelp, FuncActive, PressSB_Help_Exit, DrawSB_Exit
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page * pointerPageHelp = &mpHelp;

DEF_PAGE_SB(        mpHelp,                                                                                                             // ������� ///
    "������", "HELP",
    "������� ������� ������",
    "To open sections of the help",
    &sbExitHelp,
    &sbHelpParagraphEnter,
    &sbHelpParagraphLeave,
    0,
    &sbHelpParagraphPrev,
    &sbHelpParagraphNext,
    PageSB_Help, &mainPage, FuncActive, FuncPress, HelpContent_Draw, OnHelpRegSet
);


/** @}  @}
 */
