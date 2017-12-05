#include "defines.h"
#include "MenuFunctions.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Menu/Pages/Definition.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeItem TypeMenuItem(const void *address) 
{
    return address ? (*((TypeItem*)address)) : Item_None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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
        Page *page = (Keeper(item));
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
int HeightOpenedItem(void *item) 
{
    TypeItem type = TypeMenuItem(item);
    if(type == Item_Page)
    {
        int numItems = ((const Page *)item)->NumItems() - ((Page *)item)->NumCurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, 0, MENU_ITEMS_ON_DISPLAY);
        return MP_TITLE_HEIGHT + MI_HEIGHT * numItems;
    } 
    else if(type == Item_Choice || type == Item_ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)item)->NumSubItems() * MOSI_HEIGHT - 1;
    }
    return MI_HEIGHT;
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
        Page *page = Keeper(item);
        SetMenuPosActItem(page->GetNamePage(), open ? (page->PosCurrentItem() | 0x80) : (page->PosCurrentItem() & 0x7f));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsOpened(const void *item)
{
    TypeItem type = TypeMenuItem(item);
    Page *page = Keeper(item);
    if(type == Item_Page)
    {
        return CurrentItemIsOpened(Keeper(item)->GetNamePage());
    }
    return (MENU_POS_ACT_ITEM(page->name) & 0x80) != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Page *Keeper(const void *item)
{
    const Page *page = ((Page *)(item))->keeper;
    return (Page *)page;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ChangeOpenedItem(void *item, int delta)
{
    if (delta < 2 && delta > -2)
    {
        return false;
    }

    TypeItem type = TypeMenuItem(item);

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
bool IsPageSB(const void *item)
{
    if (TypeMenuItem(item) == Item_Page)
    {
        return ((Page *)item)->isPageSB;
    }
    return false;
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
