#include "defines.h"
#include "MenuFunctions.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Menu/Pages/Definition.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
