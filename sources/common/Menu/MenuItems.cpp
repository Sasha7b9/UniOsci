#include "MenuItems.h"
#include "Menu/MenuFunctions.h"
#include "Hardware/Sound.h"
#include "Settings/Settings.h"


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
void *Page::Item(int numElement) const
{
    return (void *)items[numElement + (IsPageSB(this) ? 1 : 0)];
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
    if (TypeMenuItem(this) == Item_Page)
    {
        CallFuncOnPressButton((void *)items[numItem]);
    }
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
        sound.RegulatorSwitchRotate();
        SetMenuCurrentSubPage(name, MenuCurrentSubPage(name) + 1);
    }
    else if (delta < 0 && MenuCurrentSubPage(name) > 0)
    {
        sound.RegulatorSwitchRotate();
        SetMenuCurrentSubPage(name, MenuCurrentSubPage(name) - 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsActive()
{
    /// \todo Здесь оптимизировать через битовую маску

    if (type == Item_Choice || type == Item_Page || type == Item_Button || type == Item_Governor || type == Item_SmallButton || type == Item_ChoiceReg)
    {
        return funcOfActive ? funcOfActive() : true;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool GovernorColor::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Choice::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Governor::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool IPaddress::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool MACaddress::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Formula::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Button::IsActive()
{
    return funcOfActive ? funcOfActive() : true;
}
