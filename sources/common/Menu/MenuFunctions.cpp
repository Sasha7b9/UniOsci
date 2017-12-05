#include "defines.h"
#include "MenuFunctions.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Menu/Pages/Definition.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool    CurrentItemIsOpened(NamePage namePage)
{
    bool retValue = _GET_BIT(MENU_POS_ACT_ITEM(namePage), 7) == 1;
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void    SetCurrentItem(const void *item, bool active)
{
    if(item != 0)
    {
        Page *page = (Page *)((Control *)item)->Keeper();
        if(!active)
        {
            SetMenuPosActItem(page->name, 0x7f);
        }
        else
        {
            for(int i = 0; i < page->NumItems(); i++)
            {
                if(page->Item(i) == item)
                {
                    SetMenuPosActItem(page->name, (int8)i);
                    return;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *TitleItem(void *item) 
{
    return ((Page *)item)->titleHint[LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool IsFunctionalButton(PanelButton button)
{
    return button >= B_F1 && button <= B_F5;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OpenItem(const void *item, bool open)
{
    if(item)
    {
        Page *page = (Page *)((Control *)item)->Keeper();
        SetMenuPosActItem(page->GetNamePage(), open ? (page->PosCurrentItem() | 0x80) : (page->PosCurrentItem() & 0x7f));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsOpened(const Control *item)
{
    TypeItem type = item->Type();
    Page *page = (Page *)((Control *)item)->Keeper();
    if(type == Item_Page)
    {
        return CurrentItemIsOpened(((Page *)(((Control *)item)->Keeper()))->GetNamePage());
    }
    return (MENU_POS_ACT_ITEM(page->name) & 0x80) != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ChangeOpenedItem(Control *item, int delta)
{
    if (delta < 2 && delta > -2)
    {
        return false;
    }

    TypeItem type = item->Type();

    if (type == Item_Page)
    {
        ((const Page *)item)->ChangeSubPage(delta);
    }
    else if (type == Item_IP)
    {
        ((IPaddress *)item)->ChangeValue(delta);
    }
    else if (type == Item_MAC)
    {
        ((MACaddress *)item)->ChangeValue(delta);
    }
    else if (type == Item_ChoiceReg || type == Item_Choice)
    {
        ((Choice *)item)->ChangeIndex(MENU_IS_SHOWN ? delta : -delta);
    }
    else if (type == Item_Governor)
    {
        ((Governor *)item)->ChangeValue(delta);
    }
    
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CallFuncOnPressButton(void *button)
{
    if (button)
    {
        Button *btn = (Button*)button;
        if (btn->funcOnPress)
        {
            btn->funcOnPress();
        }
    }
}
