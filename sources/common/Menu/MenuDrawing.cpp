#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/MenuFunctions.h"
#include "Utils/CommonFunctions.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "Menu/MenuFunctions.h"
#include "Settings/Settings.h"
#include "Globals.h"
#include "Menu/Pages/Definition.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ResetItemsUnderButton();
static void DrawOpenedPage(Page *page, int layer, int yTop);
static void DrawTitlePage(Page *page, int layer, int yTop);
static void DrawItemsPage(Page *page, int layer, int yTop);
static void DrawPagesUGO(Page *page, int right, int bottom);
static void DrawNestingPage(Page *page, int left, int bottom);
static int CalculateX(int layer);
static int ItemOpenedPosY(void *item);

static void *itemUnderButton[B_NumButtons] = {0};

//extern const PageBase mainPage;

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
    TypeItem type = gItemHint->Type();
    Language lang = LANG;
    Page *item = (Page *)gItemHint;

    const int SIZE = 100;
    char title[SIZE];
    snprintf(title, SIZE, "%s \"%s\"", names[type][lang], item->titleHint[lang]);

    if (item->type == Item_SmallButton)
    {
        y -= 9;
    }
    Painter::DrawStringInCenterRectAndBoundItC(x, y, width, 15, title, gColorBack, gColorFill);
    y = Painter::DrawTextInBoundedRectWithTransfers(x, y + 15, width, item->titleHint[2 + lang], gColorBack, gColorFill);
    if (item->type == Item_SmallButton)
    {
        Painter::DrawHintsForSmallButton(x, y, width, (SButton*)item);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawTitlePage(Page *page, int layer, int yTop)
{
    int x = CalculateX(layer);
    if (page->IsPageSB())
    {
        page->SmallButonFromPage(0)->Draw(LEFT_SB, yTop + 3);
        return;
    }
    int height = page->HeightOpened();
    bool shade = CurrentItemIsOpened(page->GetNamePage());
    Painter::FillRegion(x - 1, yTop, MP_TITLE_WIDTH + 2, height + 2, gColorBack);
    Painter::DrawRectangle(x, yTop, MP_TITLE_WIDTH + 1, height + 1, Color::BorderMenu(shade));

    if (shade)
    {
        Painter::FillRegion(x + 1, yTop + 1, MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1, Color::MENU_TITLE_DARK);
        Painter::FillRegion(x + 4, yTop + 4, MP_TITLE_WIDTH - 7, MP_TITLE_HEIGHT - 7, Color::MENU_TITLE_DARK);
    }
    else
    {
        Painter::DrawVolumeButton(x + 1, yTop + 1, MP_TITLE_WIDTH - 1, MP_TITLE_HEIGHT - 1, 2, Color::MenuTitle(false), Color::MENU_TITLE_BRIGHT, Color::MENU_TITLE_DARK, shade, shade);
    }
    
    Painter::DrawVLine(x, yTop, yTop + page->HeightOpened(), Color::BorderMenu(false));
    bool condDrawRSet = page->NumSubPages() > 1 && Menu::CurrentItem()->Type() != Item_ChoiceReg && 
        Menu::CurrentItem()->Type() != Item_Governor && Menu::TypeOpenedItem() == Item_Page;
    int delta = condDrawRSet ? -10 : 0;
    Color colorText = shade ? Color::LightShadingText() : Color::BLACK;
    x = Painter::DrawStringInCenterRect(x, yTop, MP_TITLE_WIDTH + 2 + delta, MP_TITLE_HEIGHT, TitleItem(page), colorText);
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
static void DrawPagesUGO(Page *page, int right, int bottom)
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
static void DrawNestingPage(Page *page, int left, int bottom)
{
    if (page != (Page *)&mainPage)
    {
        int nesting = 0;

        PageBase *parent = page->Keeper();

        while (parent != &mainPage)
        {
            page = (Page *)parent;
            parent = page->Keeper();
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
    ((Page *)item)->Draw(x, y);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawItemsPage(Page *page, int layer, int yTop)
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
    int count = 0;
    for(int posItem = posFirstItem; posItem <= posLastItem; posItem++)
    {
        Control *item = page->Item(posItem);
        if(item)
        {
            TypeItem type = item->Type();
            int top = yTop + MI_HEIGHT * count;
            funcOfDraw[type](item, CalculateX(layer), top);
            itemUnderButton[GetFuncButtonFromY(top)] = item;
        }
        count++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawOpenedPage(Page *page, int layer, int yTop)
{
    DrawTitlePage(page, layer, yTop);
    DrawItemsPage(page, layer, yTop + MP_TITLE_HEIGHT);
    if (CurrentItemIsOpened(page->GetNamePage()))
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
        TypeItem type = item->Type();
        if (type == Item_Choice || type == Item_ChoiceReg)
        {
            ((Choice *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == Item_Governor)
        {
            ((Governor *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == Item_GovernorColor)
        {
            ((GovernorColor *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == Item_Time)
        {
            ((Time *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == Item_IP)
        {
            ((IPaddress *)item)->Draw(CalculateX(1), ItemOpenedPosY(item), true);
        }
        else if (type == Item_MAC)
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
int CalculateX(int layer)
{
    return MP_X - layer * GRID_DELTA / 4;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool IsShade(void *item)
{
    Control *control = (Control *)item;
    Page *keeper = (Page *)control->Keeper();

    return CurrentItemIsOpened(keeper->GetNamePage()) && (item != Menu::OpenedItem());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool IsPressed(void *item)
{
    return item == Menu::ItemUnderKey();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void *ItemUnderButton(PanelButton button)
{
    return itemUnderButton[button];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int ItemOpenedPosY(void *item)
{
    Page *page = (Page *)((Control *)item)->Keeper();
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
    if (MENU_IS_SHOWN || TypeOpenedItem() != Item_Page)
    {
        ResetItemsUnderButton();
        Control *item = OpenedItem();
        if (MENU_IS_SHOWN)
        {
            DrawOpenedPage(item->Type() == Item_Page ? (Page *)item : (Page *)((Control *)item)->Keeper(), 0, GRID_TOP);
        }
        else
        {
            if (item->Type() == Item_Choice || item->Type() == Item_ChoiceReg)
            {
                ((Choice *)item)->Draw(CalculateX(0), GRID_TOP, false);
                Painter::DrawVLine(CalculateX(0), GRID_TOP + 1, GRID_TOP + 34, Color::BorderMenu(false));
                Painter::DrawVLine(CalculateX(0) + 1, GRID_TOP + 1, GRID_TOP + 34);
                Painter::DrawVLine(GRID_RIGHT, GRID_TOP + 30, GRID_TOP + 40, gColorFill);
                Painter::DrawVLine(CalculateX(0) - 1, GRID_TOP + 1, GRID_TOP + 35, gColorBack);
                Painter::DrawHLine(GRID_TOP + 35, CalculateX(0) - 1, GRID_RIGHT - 1);
            }
            else if (item->Type() == Item_Governor)
            {
                ((Governor *)item)->Draw(CalculateX(0), GRID_TOP, true);
                Painter::DrawHLine(GRID_TOP, CalculateX(0) - 2, GRID_RIGHT, gColorFill);
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
                                                   gColorBack, gColorFill);
        y += LANG_RU ? 49 : 40;
        if (gStringForHint)
        {
            Painter::DrawTextInBoundedRectWithTransfers(x, y, width, gStringForHint, gColorBack, Color::WHITE);
        }
        else if (gItemHint)
        {
            DrawHintItem(x, y, width);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ResetItemsUnderButton()
{
    for (int i = 0; i < B_NumButtons; i++)
    {
        itemUnderButton[i] = 0;
    }
}
