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
PanelButton GetFuncButtonFromY(int _y)
{
    int y = GRID_TOP + GRID_HEIGHT / 12;
    int step = GRID_HEIGHT / 6;
    PanelButton button = B_Menu;
    for(int i = 0; i < 6; i++)
    {
        if(_y < y)
        {
            return button;
        }
        button = (PanelButton)((int)button + 1);    // button++;
        y += step;
    }
    return  B_F5;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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
void Menu::DrawTitlePage(Page *page, int layer, int yTop)
{
    int x = CalculateX(layer);
    if (IS_PAGE_SB(page))
    {
        SMALL_BUTTON_FROM_PAGE(page, 0)->Draw(LEFT_SB, yTop + 3);
        return;
    }
    int height = page->HeightOpened();
    bool shade = page->CurrentItemIsOpened();
    Painter::FillRegion(x - 1, yTop, MP_TITLE_WIDTH + 2, height + 2, Color::BACK);
    Painter::DrawRectangle(x, yTop, MP_TITLE_WIDTH + 1, height + 1, Color::BorderMenu(shade));

    if (shade)
    {
        Painter::FillRegion(x + 1, yTop + 1, MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1, COLOR_MENU_TITLE_DARK);
        Painter::FillRegion(x + 4, yTop + 4, MP_TITLE_WIDTH - 7, MP_TITLE_HEIGHT - 7, COLOR_MENU_TITLE_DARK);
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, yTop + 1, MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1, 2, Color::MenuTitle(false), 
            COLOR_MENU_TITLE_BRIGHT, COLOR_MENU_TITLE_DARK, shade, shade);
    }
    
    Painter::DrawVLine(x, yTop, yTop + page->HeightOpened(), Color::BorderMenu(false));
    bool condDrawRSet = page->NumSubPages() > 1 && NOT_CHOICE_REG(Menu::CurrentItem()) && 
        NOT_GOVERNOR(CurrentItem()) && IS_PAGE(OpenedItem());
    int delta = condDrawRSet ? -10 : 0;
    Color colorText = shade ? Color::LightShadingText() : COLOR_BLACK;
    x = Painter::DrawStringInCenterRect(x, yTop, MP_TITLE_WIDTH + 2 + delta, MP_TITLE_HEIGHT, page->Title(), colorText);
    if(condDrawRSet)
    {
        Painter::Draw4SymbolsInRect(x + 4, yTop + 11, Governor::GetSymbol(page->NumCurrentSubPage()), colorText);
    }

    itemUnderButton[GetFuncButtonFromY(yTop)] = page;

    delta = 0;
    
    Painter::SetColor(colorText);
    DrawPagesUGO(page, CalculateX(layer) + MP_TITLE_WIDTH - 3 + delta, yTop + MP_TITLE_HEIGHT - 2 + delta);
    DrawNestingPage(page, CalculateX(layer) + 5, yTop + MP_TITLE_HEIGHT - 8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawPagesUGO(Page *page, int right, int bottom)
{
    int size = 4;
    int delta = 2;
    
    int allPages = (page->NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
    int currentPage = page->NumCurrentSubPage();

    int left = right - (size + 1) * allPages - delta + (3 - allPages);
    int top = bottom - size - delta;

    for(int p = 0; p < allPages; p++)
    {
        int x = left + p * (size + 2);
        if(p == currentPage)
        {
            Painter::FillRegion(x, top, size, size);
        }
        else
        {
            Painter::DrawRectangle(x, top, size, size);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawNestingPage(Page *page, int left, int bottom)
{
    if (page != (Page *)&mainPage)
    {
        int nesting = 0;

        PageBase *parent = KEEPER(page);

        while (parent != &mainPage)
        {
            page = (Page *)parent;
            parent = KEEPER(page);
            nesting++;
        }

        int size = 4;
        int delta = 2;

        for (int i = 0; i <= nesting; i++)
        {
            int x = left + i * (size + delta);
            Painter::DrawRectangle(x, bottom, size, size);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGovernor(void *item, int x, int y)
{
    ((Governor *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawIPaddress(void *item, int x, int y)
{
    ((IPaddress *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMACaddress(void *item, int x, int y)
{
    ((MACaddress *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawFormula(void *item, int x, int y)
{
    ((Formula *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChoice(void *item, int x, int y)
{
    ((Choice *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSmallButton(void *item, int, int y)
{
    ((SButton *)item)->Draw(LEFT_SB, y + 7);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTime(void *item, int x, int y)
{
    ((Time *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGovernorColor(void *item, int x, int y)
{
    ((GovernorColor *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawButton(void *item, int x, int y)
{
    ((Button *)item)->Draw(x, y);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPage(void *item, int x, int y)
{
    ((Page *)item)->Draw(x, y, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawItemsPage(Page *page, int layer, int yTop)
{
    void (*funcOfDraw[Item_NumberItems])(void *, int, int) = 
    {  
        EmptyFuncpVII,      // Item_None
        DrawChoice,         // Item_Choice
        DrawButton,         // Item_Button
        DrawPage,           // Item_Page
        DrawGovernor,       // Item_Governor
        DrawTime,           // Item_Time
        DrawIPaddress,      // Item_IP
        DrawGovernorColor,  // Item_GovernorColor
        DrawFormula,        // Item_Formula
        DrawMACaddress,     // TypeItem_Mac
        DrawChoice,         // Item_ChoiceReg
        DrawSmallButton     // Item_SmallButton
    };
    int posFirstItem = page->PosItemOnTop();
    int posLastItem = posFirstItem + MENU_ITEMS_ON_DISPLAY - 1;
    LIMITATION(posLastItem, 0, page->NumItems() - 1);
    for(int posItem = posFirstItem; posItem <= posLastItem; posItem++)
    {
        Control *item = page->Item(posItem);
        if(item)
        {
            int top = yTop + MI_HEIGHT * (posItem - posFirstItem);
            funcOfDraw[item->type](item, CalculateX(layer), top);
            itemUnderButton[GetFuncButtonFromY(top)] = item;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawOpenedPage(Page *page, int layer, int yTop)
{
    DrawTitlePage(page, layer, yTop);
    DrawItemsPage(page, layer, yTop + MP_TITLE_HEIGHT);
    if (page->CurrentItemIsOpened())
    {
        int8 posCurItem = page->PosCurrentItem();
        Control *item = page->Item(posCurItem);
        for (int i = 0; i < 5; i++)
        {
            if (itemUnderButton[i + B_F1] != item)
            {
                itemUnderButton[i + B_F1] = 0;
            }
        }
        if (IS_CHOICE(item) || IS_CHOICE_REG(item))
        {
            ((Choice *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (IS_GOVERNOR(item))
        {
            ((Governor *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (IS_GOVERNOR_COLOR(item))
        {
            ((GovernorColor *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (IS_TIME(item))
        {
            ((Time *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (IS_IP(item))
        {
            ((IPaddress *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (IS_MAC(item))
        {
            ((MACaddress *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
    }

    if (page->funcOnDraw)
    {
        page->funcOnDraw();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::CalculateX(int layer)
{
    return MP_X - layer * GRID_DELTA / 4;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::ItemOpenedPosY(Control *item)
{
    Page *page = (Page *)KEEPER(item);
    int8 posCurItem = page->PosCurrentItem();
    int y = GRID_TOP + (posCurItem % MENU_ITEMS_ON_DISPLAY) * MI_HEIGHT + MP_TITLE_HEIGHT;
    if(y + ((Control *)item)->HeightOpened() > GRID_BOTTOM)
    {
        y = GRID_BOTTOM - ((Control *)item)->HeightOpened() - 2;
    }
    return y + 1;
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
            DrawOpenedPage(IS_PAGE(item) ? (Page *)item : (Page *)KEEPER(item), 0, GRID_TOP);
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
