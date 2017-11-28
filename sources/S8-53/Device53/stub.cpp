#include "stub.h"
#include "Log.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer_SetAndStartOnce(TypeTimer type, uint dTms, pFuncVV func)
{
    LOG_ERROR("заглушка %s", __FUNCTION__);
}

void Painter::DrawTextInRect(int x, int y, int width, int height, char *text)
{
    DrawTextInRect(x, y, width, text);
}

void Menu::OpenPageAndSetItCurrent(NamePage namePage)
{

}

void Measure_RotateRegSet(int)
{

}

Page* Menu::PagePointerFromName(NamePage namePage)
{
    return 0;
}

void Menu::ShortPressOnPageItem(Page *, int)
{

}
