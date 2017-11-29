#include "Menu/Menu.h"
#include "Globals.h"
#include "Menu/MenuFunctions.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Settings/Settings.h"
#include "Hardware/Hardware.h"
#include "Hardware/Sound.h"
#include "Hardware/RTC.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Menu/Pages/PageDisplay.h"
#include "stub.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    NONE,
    INCREASE,
    DECREASE
} DIRECTION;

/// ��������� ������������ ��� �������� ��������� ��� Choice � Governor
typedef struct
{
    void *address;  ///< ����� ��������. ���� 0 - �� ��������
    uint timeStart; ///< ����� ������ �������� � �������������
    DIRECTION dir;  ///< ����������� ��������� ��������
} TimeStruct;

static TimeStruct tsChoice = {0, 0, NONE};
static TimeStruct tsGovernor = {0, 0, NONE};

int8 gCurDigit = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::StartChange(int delta)
{
    if (tsChoice.address != 0)
    {
        return;
    }
    sound.GovernorChangedValue();
    if (HINT_MODE_ENABLE)
    {
        SetItemForHint(this);
    }
    else if (!ItemIsAcitve(this))
    {
        CHOICE_RUN_FUNC_CHANGED(this, false);
    }
    else
    {
        tsChoice.address = this;
        tsChoice.timeStart = gTimeMS;
        tsChoice.dir = delta > 0 ? INCREASE : DECREASE;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Choice::Step()
{
    static const float speed = 0.1f;
    static const int numLines = 12;
    if (tsChoice.address == this)
    {
        float delta = speed * (gTimeMS - tsChoice.timeStart);
        if (delta == 0.0f)
        {
            delta = 0.001f; // ������ � ��������� ������ ������ ����� ��������, ��� ������ 0 ��, �� �� ���������� ������� �����, ������ ��� ���� ����� �������� � ���, ��� �������� ���
        }
        int8 index = *cell;
        if (tsChoice.dir == INCREASE)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            CircleIncrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            CircleDecrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        *cell = index;
        tsChoice.address = 0;
        CHOICE_RUN_FUNC_CHANGED(this, ItemIsAcitve(this));
        NEED_FINISH_DRAW = 1;
        tsChoice.dir = NONE;
        return 0.0f;
    }
    return 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ChangeIndex(int delta)
{
    int index = *cell;
    if (delta < 0)
    {
        ++index;
        if (index == NumSubItems())
        {
            index = 0;
        }
    }
    else
    {
        --index;
        if (index < 0)
        {
            index = NumSubItems() - 1;
        }
    }
    *cell = (int8)index;
    CHOICE_RUN_FUNC_CHANGED(this, ItemIsAcitve(this));
    sound.GovernorChangedValue();
    NEED_FINISH_DRAW = 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::NumSubItems()
{
    return num;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::StartChange(int delta)
{
    sound.GovernorChangedValue();
    if (delta > 0 && tsGovernor.address == this && tsGovernor.dir == INCREASE)
    {
        *cell = NextValue();
    }
    else if (delta < 0 && tsGovernor.address == this && tsGovernor.dir == DECREASE)
    {
        *cell = PrevValue();
    }
    else
    {
        tsGovernor.timeStart = gTimeMS;
        tsGovernor.address = this;
    }
    tsGovernor.dir = delta > 0 ? INCREASE : DECREASE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::NextValue()
{
    return ((*cell) < maxValue) ? ((*cell) + 1) : minValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::PrevValue()
{
    return ((*cell) > minValue) ? ((*cell) - 1) : maxValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Governor::Step()
{
    static const float speed = 0.05f;
    static const int numLines = 10;
    float delta = 0.0f;
    if (tsGovernor.address == this)
    {
        delta = speed * (gTimeMS - tsGovernor.timeStart);
        if (tsGovernor.dir == DECREASE)
        {
            delta *= -1.0f;
            if (delta == 0.0f)
            {
                return -0.001f;
            }
            if (delta < -numLines)
            {
                tsGovernor.dir = NONE;
                *cell = PrevValue();
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0f;
                tsGovernor.address = 0;
            }
        }
        else if (tsGovernor.dir == INCREASE)
        {
            if (delta == 0.0f)
            {
                return 0.001f;
            }
            if (delta > numLines)
            {
                tsGovernor.dir = NONE;
                *cell = NextValue();
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0f;
                tsGovernor.address = 0;
            }
        }
    }
    return delta;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::ChangeValue(int delta)
{
    int16 oldValue = *cell;
    *cell += (int16)(Sign(delta) * Pow10(gCurDigit));
    LIMITATION(*cell, minValue, maxValue);
    if (*cell != oldValue)
    {
        if (funcOfChanged)
        {
            funcOfChanged();
        }
        sound.GovernorChangedValue();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::NextPosition()
{
    if (menu.OpenedItem() == this)
    {
        CircleIncrease<int8>(&gCurDigit, 0, (int8)(NumDigits() - 1));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Governor::NumDigits()
{
    int min = su.NumDigitsInNumber(Abs(minValue));
    int max = su.NumDigitsInNumber(Abs(maxValue));
    if (min > max)
    {
        max = min;
    }
    return max;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
char Governor::GetSymbol(int value)
{
    static const char chars[] =
    {
        SYMBOL_GOVERNOR_SHIFT_0,
        SYMBOL_GOVERNOR_SHIFT_1,
        SYMBOL_GOVERNOR_SHIFT_2,
        SYMBOL_GOVERNOR_SHIFT_3
    };
    while (value < 0)
    {
        value += 4;
    }
    return chars[value % 4];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumSubPages() const
{
    return (NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems() const
{
    return numItems;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumCurrentSubPage()
{
    return MenuCurrentSubPage(name);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
NamePage Page::GetNamePage() const
{
    if (TypeMenuItem((void *)this) != Item_Page)
    {
        return Page_NoPage;
    }
    return name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::SetOpened()
{
    PackedTime time = RTC_GetPackedTime();
    *(seconds) = (int8)time.seconds;
    *(minutes) = (int8)time.minutes;
    *(hours) = (int8)time.hours;
    *(day) = (int8)time.day;
    *(month) = (int8)time.month;
    *(year) = (int8)time.year;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::IncCurrentPosition()
{
    sound.GovernorChangedValue();
    int8 *value[] = { 0, day, month, year, hours, minutes, seconds };
    int8 position = *curField;
    if (position != iSET && position != iEXIT)
    {
        static const int8 max[] = { 0, 31, 12, 99, 23, 59, 59 };
        static const int8 min[] = { 0, 1, 1, 15, 0, 0, 0 };
        *(value[position]) = (*(value[position]))++;
        if (*value[position] > max[position])
        {
            *value[position] = min[position];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::SetNewTime()
{
    RTC_SetTimeAndData(*day, *month, *year, *hours, *minutes, *seconds);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::SelectNextPosition()
{
    CircleIncrease<int8>(curField, 0, 7);
    painter.ResetFlash();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MACaddress::ChangeValue(int delta)
{
    uint8 *value = mac0 + gCurDigit;
    *value += delta > 0 ? 1 : -1;
    sound.GovernorChangedValue();
    display.ShowWarning(NeedRebootDevice);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::DecCurrentPosition()
{
    sound.GovernorChangedValue();
    static const int8 max[] = {0, 31, 12, 99, 23, 59, 59};
    static const int8 min[] = {0, 1, 1, 15, 0, 0, 0};
    int8 *value[] = {0, day, month, year, hours, minutes, seconds};
    int8 position = *curField;
    if (position != iSET && position != iEXIT)
    {
        (*(value[position]))--;
        if (*value[position] < min[position])
        {
            *value[position] = max[position];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetCurrentSB() const
{
    if (ItemIsAcitve(this))
    {
        if (SHOW_STRING_NAVI_TEMP)
        {
            menu.TemporaryEnableStrNavi();
        }
        SetCurrentItem(this, true);
        OpenItem(this, !ItemIsOpened(this));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::ChangeValue(int delta)
{
    if (ct->currentField == 0)
    {
        ct->BrightnessChange(delta);
        sound.GovernorChangedValue();
    }
    else
    {
        ct->ComponentChange(delta);
        sound.GovernorChangedValue();
    }

    PageService_InitGlobalColors();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::NextPosition()
{
    CircleIncrease<int8>(&gCurDigit, 0, port == 0 ? 11 : 16);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::ChangeValue(int delta)
{
    int numByte = 0;
    int numPos = 0;

    GetNumPosIPvalue(&numByte, &numPos);

    int oldValue = 0;

    if (numByte < 4)
    {
        uint8 *bytes = ip0;
        oldValue = bytes[numByte];
    }
    else
    {
        oldValue = *port;
    }

    int newValue = oldValue + Sign(delta) * Pow10(numPos);
    LIMITATION(newValue, 0, numByte == 4 ? 65535 : 255);

    if (oldValue != newValue)
    {
        if (numByte == 4)
        {
            *port = (uint16)newValue;
        }
        else
        {
            ip0[numByte] = (uint8)newValue;
        }
        sound.GovernorChangedValue();
        display.ShowWarning(NeedRebootDevice);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::GetNumPosIPvalue(int *numIP, int *selPos)
{
    if (gCurDigit < 12)
    {
        *numIP = gCurDigit / 3;
        *selPos = 2 - (gCurDigit - (*numIP * 3));
    }
    else
    {
        *numIP = 4;
        *selPos = 4 - (gCurDigit - 12);
    }


}