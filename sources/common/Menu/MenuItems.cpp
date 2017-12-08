#include "MenuItems.h"
#include "Menu.h"
#include "Menu/MenuFunctions.h"
#include "Hardware/Sound.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


#define NAME_FROM_INDEX(index) (names[index * 2 + LANG])
//#define ITEM_FROM_INDEX(index) (items[(index + LANG) * 2])

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *Choice::NameCurrentSubItem()
{
    return ((int8 *)cell == 0) ? "" : NAME_FROM_INDEX(*cell); // names[*((int8 *)cell)][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameNextSubItem()
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) + 1;

    if (index == NumSubItems())
    {
        index = 0;
    }
    return NAME_FROM_INDEX(index);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NamePrevSubItem()
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) - 1;

    if (index < 0)
    {
        index = NumSubItems() - 1;
    }
    return NAME_FROM_INDEX(index);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Page::Item(int numElement) const
{
    return (Control *)items[numElement + (isPageSB ? 1 : 0)];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
SButton* Page::SmallButonFromPage(int numButton)
{
    return (SButton *)items[numButton];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Button::CallFuncOnDraw(int x, int y)
{
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::PosItemOnTop()
{
    return NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameSubItem(int i)
{
    return NAME_FROM_INDEX(i);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ShortPressOnItem(int numItem)
{
    CallFuncOnPressButton((void *)items[numItem]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(name) & 0x7f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ChangeSubPage(int delta) const
{
    if (delta > 0 && MenuCurrentSubPage(name) < NumSubPages() - 1)
    {
        Sound::RegulatorSwitchRotate();
        SetMenuCurrentSubPage(name, MenuCurrentSubPage(name) + 1);
    }
    else if (delta < 0 && MenuCurrentSubPage(name) > 0)
    {
        Sound::RegulatorSwitchRotate();
        SetMenuCurrentSubPage(name, MenuCurrentSubPage(name) - 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsActive() const
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Control::Keeper() const
{
    return (PageBase *)keeper;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
TypeItem Control::Type() const
{
    return type;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPageSB() const
{
    return isPageSB;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Control::HeightOpened() const
{
    if (type == Item_Page)
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, 0, MENU_ITEMS_ON_DISPLAY);
        return MP_TITLE_HEIGHT + MI_HEIGHT * numItems;
    }
    else if (type == Item_Choice || type == Item_ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * MOSI_HEIGHT - 1;
    }
    return MI_HEIGHT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsShade() const
{
    return CurrentItemIsOpened(((Page *)keeper)->GetNamePage()) && (this != Menu::OpenedItem());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPressed() const
{
    return this == Menu::ItemUnderKey();
}
