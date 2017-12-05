#include "MenuItems.h"
#include "Menu/MenuFunctions.h"
#include "Hardware/Sound.h"
#include "Settings/Settings.h"


/*
template <typename T>
struct Base
{
    const int var0;
    const int var1;
    const T *t;
};

struct A
{
    A(int var0, int var1, int var2, int var3) : b({var0, var1, this}), var2(var2), var3(var3)
    {
    }
    const Base<A> b;
    int var2;
    int var3;
};

struct A a = {1, 2, 3, 4};
*/


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
    return (Control *)items[numElement + (IsPageSB(this) ? 1 : 0)];
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
/*
void ButtonBase::CallFuncOnDraw(int x, int y)
{
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
}
*/

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
    return funcOfActive ? funcOfActive() : true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Control::Keeper()
{
    return (PageBase *)keeper;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
TypeItem Control::Type() const
{
    return type;
}
