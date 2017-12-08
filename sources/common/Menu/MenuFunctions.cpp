#include "defines.h"
#include "MenuFunctions.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Menu/Pages/Definition.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CurrentItemIsOpened(NamePage namePage)
{
    bool retValue = _GET_BIT(MENU_POS_ACT_ITEM(namePage), 7) == 1;
    return retValue;
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
