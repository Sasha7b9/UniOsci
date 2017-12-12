#include "defines.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Globals.h"
#include "Menu/Pages/Definition.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawHintItem(int x, int y, int width)
{
    if (!gItemHint)
    {
        return;
    }

    pString names[Item_NumberItems][2] =
    {
        {"",            ""},        // Item_None
        {"",            ""},        // Item_Choice
        {"Кнопка",      "Button"},  // Item_Button
        {"Страница",    "Page"},    // Item_Page
        {"",            ""},        // Item_Governor
        {"",            ""},        // Item_Time
        {"",            ""},        // Item_IP
        {"",            ""},        // Item_GovernorColor
        {"",            ""},        // Item_Formula
        {"",            ""},        // Item_MAC
        {"",            ""},        // Item_ChoiceReg
        {"Кнопка",      "Button"}   // Item_SmallButton
    };
    Language lang = LANG;
    Page *item = (Page *)gItemHint;

    const int SIZE = 100;
    char title[SIZE];
    snprintf(title, SIZE, "%s \"%s\"", names[gItemHint->type][lang], item->titleHint[lang]);

    if (item->type == Item_SmallButton)
    {
        y -= 9;
    }
    Painter::DrawStringInCenterRectAndBoundItC(x, y, width, 15, title, Color::BACK, Color::FILL);
    y = Painter::DrawTextInBoundedRectWithTransfers(x, y + 15, width, item->titleHint[2 + lang], Color::BACK, Color::FILL);
    if (item->type == Item_SmallButton)
    {
        Painter::DrawHintsForSmallButton(x, y, width, (SButton*)item);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::CalculateX(int layer)
{
    return MP_X - layer * GRID_DELTA / 4;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Draw()
{
    if (MENU_IS_SHOWN || NOT_PAGE(OpenedItem()))
    {
        ResetItemsUnderButton();
        Control *item = OpenedItem();
        if (MENU_IS_SHOWN)
        {
            if(IS_PAGE(item))
            {
                ((Page *)item)->Draw(CalculateX(0), GRID_TOP, true);
            }
            else
            {
                ((Page *)KEEPER(item))->Draw(CalculateX(0), GRID_TOP, true);
            }
        }
        else
        {
            if (IS_CHOICE(item) || IS_CHOICE_REG(item))
            {
                ((Choice *)item)->Draw(CalculateX(0), GRID_TOP, false);
                Painter::DrawVLine(CalculateX(0), GRID_TOP + 1, GRID_TOP + 34, Color::BorderMenu(false));
                Painter::DrawVLine(CalculateX(0) + 1, GRID_TOP + 1, GRID_TOP + 34);
                Painter::DrawVLine(GRID_RIGHT, GRID_TOP + 30, GRID_TOP + 40, Color::FILL);
                Painter::DrawVLine(CalculateX(0) - 1, GRID_TOP + 1, GRID_TOP + 35, Color::BACK);
                Painter::DrawHLine(GRID_TOP + 35, CalculateX(0) - 1, GRID_RIGHT - 1);
            }
            else if (IS_GOVERNOR(item))
            {
                ((Governor *)item)->Draw(CalculateX(0), GRID_TOP, true);
                Painter::DrawHLine(GRID_TOP, CalculateX(0) - 2, GRID_RIGHT, Color::FILL);
                Painter::DrawVLine(GRID_RIGHT, GRID_TOP, GRID_TOP + 40);
            }
        }
    }

    if (HINT_MODE_ENABLE)
    {
        int x = 1;
        int y = 0;
        int width = 318;
        if (MENU_IS_SHOWN)
        {
            width = MenuIsMinimize() ? 289 : 220;
        }
        Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
                                                   LANG_RU ? "Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о её назначении. "
                                                   "Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с." :
                                                   "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
                                                   "To disable this mode, press the button HELP and hold it for 0.5s.",
                                                   Color::BACK, Color::FILL);
        y += LANG_RU ? 49 : 40;
        if (gStringForHint)
        {
            Painter::DrawTextInBoundedRectWithTransfers(x, y, width, gStringForHint, Color::BACK, COLOR_WHITE);
        }
        else if (gItemHint)
        {
            DrawHintItem(x, y, width);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ResetItemsUnderButton()
{
    for (int i = 0; i < B_NumButtons; i++)
    {
        itemUnderButton[i] = 0;
    }
}
