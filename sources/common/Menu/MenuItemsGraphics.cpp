#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Hardware/RTC.h"
#include "stub.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawGovernorChoiceColorFormulaHiPart(Control *item, int x, int y, bool pressed, bool shade, bool opened);
static void GovernorIpCommon_DrawOpened(Control *item, int x, int y, int dWidth);
static void DrawValueWithSelectedPosition(int x, int y, int value, int numDigits, int selPos, bool hLine, bool fillNull);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GovernorColor::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawOpened(int x, int y)
{
    static const int delta = 43;
    x -= delta;
    ct->Init(false);
    Painter::DrawRectangle(x - 1, y - 1, MI_WIDTH + delta + 2, MI_HEIGHT + 2, Color::BLACK);
    Painter::DrawRectangle(x, y, MI_WIDTH + delta, MI_HEIGHT, Color::MenuTitle(false));
    Painter::DrawVolumeButton(x + 1, y + 1, MI_WIDTH_VALUE + 2 + delta, MI_HEIGHT_VALUE + 3, 2, Color::MenuItem(false),
                             Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, IsPressed(), IsShade());
    Painter::DrawHLine(y + MI_HEIGHT / 2 + 2, x, x + MI_WIDTH + delta, Color::MenuTitle(false));
    Painter::DrawStringInCenterRect(x + (IsPressed() ? 2 : 1), y + (IsPressed() ? 2 : 1), MI_WIDTH + delta, MI_HEIGHT / 2 + 2, Title(),
                                    Color::WHITE);
    DrawValue(x + 1, y + 19, delta);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawClosed(int x, int y)
{
    ct->Init(false);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, IsPressed(), IsShade() || !IS_ACTIVE(this), true);
    Painter::FillRegion(x + 2, y + 20, MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1, ct->color);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawValue(int x, int y, int delta)
{
    char buffer[20];

    int8 field = ct->currentField;
    char *texts[4] = {"Яр", "Сн", "Зл", "Кр"};

    uint16 color = COLOR(ct->color.value);
    int red = R_FROM_COLOR(color);
    int green = G_FROM_COLOR(color);
    int blue = B_FROM_COLOR(color);
    ct->Init(false);
    int16 vals[4] = {(int16)(ct->brightness * 100.0f), (int16)blue, (int16)green, (int16)red};

    Painter::FillRegion(x, y, MI_WIDTH + delta - 2, MI_HEIGHT / 2 - 3, Color::BLACK);
    x += 92;

    for (int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Painter::FillRegion(x - 1, y + 1, 29, 10, colorBack);
        Painter::DrawText(x, y + 2, texts[i], colorDraw);
        Painter::DrawText(x + 14, y + 2, Int2String(vals[i], false, 1, buffer));
        x -= 30;
    }

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::Draw(int x, int y, bool opened)
{
    if (funcBeforeDraw)
    {
        funcBeforeDraw();
    }
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawOpened(int x, int y)
{
    GovernorIpCommon_DrawOpened(this, x, y, 0);
    DrawValue(x, y + 22);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IS_ACTIVE(this);
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawValue(int x, int y)
{
    char buffer[20];

    int startX = x + 40;
    int16 value = *cell;
    int signGovernor = *cell < 0 ? -1 : 1;
    if (signGovernor == -1)
    {
        value = -value;
    }
    Painter::SetFont(TypeFont_5);
    bool sign = minValue < 0;
    Painter::DrawText(x + 55, y - 5, Int2String(maxValue, sign, 1, buffer), Color::WHITE);
    Painter::DrawText(x + 55, y + 2, Int2String(minValue, sign, 1, buffer));
    Painter::SetFont(TypeFont_8);

    DrawValueWithSelectedPosition(startX, y, value, NumDigits(), gCurDigit, true, true);

    if (sign)
    {
        Painter::DrawChar(startX - 30, y, signGovernor < 0 ? '\x9b' : '\x9a');
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawLowPart(int x, int y, bool, bool shade)
{
    char buffer[20];

    Color colorTextDown = Color::BLACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
                             Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, shade);
    if (shade)
    {
        colorTextDown = Color::MenuItem(false);
    }

    x = Painter::DrawText(x + 4, y + 21, "\x80", colorTextDown);
    if (Menu::OpenedItem() != this)
    {
        int delta = (int)Step();
        if (delta == 0)
        {
            x = Painter::DrawText(x + 1, y + 21, Int2String(*cell, false, 1, buffer));
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y + 19;
            int limWidth = MI_WIDTH_VALUE;
            int limHeight = MI_HEIGHT_VALUE - 1;
            if (delta > 0)
            {
                x = Painter::DrawTextWithLimitationC(drawX, y + 21 - delta, Int2String(*cell, false, 1, buffer),
                                                    Color::BLACK, limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitationC(drawX, y + 21 + 10 - delta, Int2String(NextValue(), false, 1, buffer),
                                                Color::BLACK, limX, limY, limWidth, limHeight);
            }
            if (delta < 0)
            {
                x = Painter::DrawTextWithLimitationC(drawX, y + 21 - delta, Int2String(*cell, false, 1, buffer),
                                                    Color::BLACK, limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitationC(drawX, y + 21 - 10 - delta, Int2String(PrevValue(), false, 1, buffer),
                                                Color::BLACK, limX, limY, limWidth, limHeight);
            }
        }
    }
    else
    {
        x = Painter::DrawText(x + 1, y + 21, Int2String(*cell, false, 1, buffer), Color::WHITE);
    }
    Painter::DrawText(x + 1, y + 21, "\x81", colorTextDown);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x - (port == 0 ? 0 : MOI_WIDTH_D_IP), y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::DrawOpened(int x, int y)
{
    GovernorIpCommon_DrawOpened(this, x, y, port == 0 ? 0 : MOI_WIDTH_D_IP);
    DrawValue(x, y + 22);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IS_ACTIVE(this);
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::DrawValue(int x, int y)
{
    if (gCurDigit > (port == 0 ? 11 : 16))
    {
        gCurDigit = 0;
    }

    uint8 *bytes = ip0;

    x += 15;

    y += 1;

    int numIP = 0;
    int selPos = 0;

    GetNumPosIPvalue(&numIP, &selPos);

    for (int i = 0; i < 4; i++)
    {
        DrawValueWithSelectedPosition(x, y, bytes[i], 3, numIP == i ? selPos : -1, false, true);
        if (i != 3)
        {
            Painter::DrawChar(x + 5, y, '.', Color::WHITE);
        }
        x += 19;
    }

    if (port != 0)
    {
        Painter::DrawChar(x - 13, y, ':', Color::WHITE);
        DrawValueWithSelectedPosition(x + 14, y, *port, 5, numIP == 4 ? selPos : -1, false, true);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::DrawLowPart(int x, int y, bool, bool shade)
{
    const int SIZE = 20;
    char buffer[SIZE];

    Color colorTextDown = Color::BLACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
                             Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, shade);
    if (shade)
    {
        colorTextDown = Color::MenuItem(false);
    }

    snprintf(buffer, SIZE, "%03d.%03d.%03d.%03d", *ip0, *ip1, *ip2, *ip3);

    if (Menu::OpenedItem() != this)
    {
        Painter::DrawText(x + 4, y + 21, buffer, colorTextDown);
    }
    else
    {
        Painter::DrawText(x + 4, y + 21, buffer, Color::WHITE);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MACaddress::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MACaddress::DrawOpened(int x, int y)
{
    GovernorIpCommon_DrawOpened(this, x, y, 0);
    DrawValue(x, y + 22);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MACaddress::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IS_ACTIVE(this);
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MACaddress::DrawValue(int x, int y)
{
    if (gCurDigit > 5)
    {
        gCurDigit = 0;
    }
    uint8 *bytes = mac0;
    x += MOI_WIDTH - 14;
    y++;
    for (int i = 5; i >= 0; --i)
    {
        int value = (int)(*(bytes + i));
        if (gCurDigit == i)
        {
            Painter::FillRegion(x - 1, y, 10, 8, Color::WHITE);
        }
        const int SIZE = 20;
        char buffer[SIZE];
        snprintf(buffer, SIZE, "%02X", value);
        Painter::DrawText(x, y, buffer, gCurDigit == i ? Color::BLACK : Color::WHITE);
        x -= 12;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MACaddress::DrawLowPart(int x, int y, bool, bool shade)
{
    const int SIZE = 20;
    char buffer[SIZE];

    Color colorTextDown = Color::BLACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
                             Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, shade);
    if (shade)
    {
        colorTextDown = Color::MenuItem(false);
    }

    snprintf(buffer, SIZE, "%02X.%02X.%02X.%02X.%02X.%02X", *mac0, *mac1, *mac2, *mac3, *mac4, *mac5);

    if (Menu::OpenedItem() != this)
    {

        Painter::DrawText(x + 4, y + 21, buffer, colorTextDown);
    }
    else
    {
        Painter::DrawText(x + 4, y + 21, buffer, Color::WHITE);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Formula::Draw(int x, int y, bool opened)
{
    if (opened)
    {

    }
    else
    {
        DrawClosed(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Formula::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Formula::DrawLowPart(int x, int y, bool, bool shade)
{
    Color colorTextDown = Color::BLACK;

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
                             Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, shade);
    if (shade)
    {
        colorTextDown = Color::MenuItem(false);
    }
    Painter::SetColor(colorTextDown);
    WriteText(x + 6, y + 21, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Formula::WriteText(int x, int y, bool)
{
    Function func = (Function)(*function);

    if (func != Function_Mul && func != Function_Sum)
    {
        return;
    }

    bool funcIsMul = func == Function_Mul;
    int8 koeff1 = funcIsMul ? *koeff1mul : *koeff1add;
    int8 koeff2 = funcIsMul ? *koeff2mul : *koeff2add;
    if (koeff1 != 0)
    {
        Painter::DrawChar(x, y, koeff1 < 0 ? '-' : '+');
    }
    Painter::DrawChar(x + 5, y, (char)(koeff1 + 0x30));
    Painter::DrawChar(x + 10, y, '*');
    Painter::DrawText(x + 14, y, "K1");
    Painter::DrawChar(x + 27, y, funcIsMul ? '*' : '+');
    if (koeff2 != 0)
    {
        Painter::DrawChar(x + 30, y, koeff2 < 0 ? '-' : '+');
    }
    Painter::DrawChar(x + 39, y, (char)(koeff2 + 0x30));
    Painter::DrawChar(x + 44, y, '*');
    Painter::DrawText(x + 48, y, "K2");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawOpened(int x, int y)
{
    int height = HeightOpened();

    Painter::DrawRectangle(x - 2, y - 1, MP_TITLE_WIDTH + 3, height + 3, gColorBack);

    DrawGovernorChoiceColorFormulaHiPart(this, x - 1, y - 1, IsPressed(), false, true);
    Painter::DrawRectangle(x - 1, y, MP_TITLE_WIDTH + 1, height + 1, Color::MenuTitle(false));

    Painter::DrawHLine(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, Color::MENU_TITLE_BRIGHT,
                             Color::MENU_TITLE_DARK, false, IsShade());
    int8 index = *cell;
    for (int i = 0; i < NumSubItems(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT + 1;
        bool pressed = i == index;
        if (pressed)
        {
            Painter::DrawVolumeButton(x + 1, yItem, MOI_WIDTH - 2, MOSI_HEIGHT - 2, 2, Color::MENU_FIELD, Color::MENU_TITLE_BRIGHT,
                                     Color::MENU_TITLE_DARK, pressed, IsShade());
        }
        Painter::DrawText(x + 4, yItem + 2, NameSubItem(i), pressed ? Color::BLACK : Color::MENU_FIELD);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !funcOfActive();

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 1, Color::MENU_FIELD, Color::MENU_ITEM_BRIGHT,
                             Color::MENU_ITEM_DARK, true, shade);

    int deltaY = (int)Step();
    Color colorText = shade ? Color::MenuItem(true) : Color::BLACK;
    Painter::SetColor(colorText);
    if (deltaY == 0)
    {
        Painter::DrawText(x + 4, y + 21, NameCurrentSubItem());
    }
    else
    {
        Painter::DrawTextWithLimitationC(x + 4, y + 21 - deltaY, NameCurrentSubItem(), colorText, x, y + 19, MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1);
        Painter::DrawHLine(y + (deltaY > 0 ? 31 : 19) - deltaY, x + 3, x + MI_WIDTH_VALUE + 1, Color::BLACK);
        Painter::DrawTextWithLimitationC(x + 4, y + (deltaY > 0 ? 33 : 9) - deltaY, deltaY > 0 ? NameNextSubItem() : NamePrevSubItem(),
                                        colorText, x, y + 19, MI_WIDTH_VALUE, MI_HEIGHT_VALUE - 1);
    }
    Painter::DrawHLine(y + MI_HEIGHT + 1, x, x + MI_WIDTH, Color::BorderMenu(false));

    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !funcOfActive();

    Painter::DrawHLine(y + 1, x, x + MI_WIDTH, Color::MenuTitle(shade));
    Color color = shade ? Color::MenuItem(true) : Color::WHITE;
    Painter::FillRegion(x + 1, y + 2, MI_WIDTH - 2, MI_HEIGHT - 2, Color::MenuItem(false));
    Painter::DrawVolumeButton(x + 3, y + 4, MI_WIDTH - 6, MI_HEIGHT - 6, 2, Color::MenuItem(false), Color::MENU_ITEM_BRIGHT,
                             Color::MENU_ITEM_DARK, pressed, shade);

    int delta = (pressed && (!shade)) ? 2 : 1;

    Painter::DrawStringInCenterRect(x + delta, y + delta, MI_WIDTH, MI_HEIGHT, Title(), color);

    funcForDraw(x, y);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::Draw(int x, int y)
{
    if (funcOfActive())
    {
        if (IsPressed())
        {
            Painter::FillRegion(x, y, WIDTH_SB, WIDTH_SB, gColorFill);
            Painter::SetColor(Color::BLACK);
        }
        else
        {
            Painter::DrawRectangle(x, y, WIDTH_SB, WIDTH_SB, gColorFill);
        }
        funcForDraw(x, y);
    }
    else
    {
        Painter::DrawRectangle(x, y, WIDTH_SB, WIDTH_SB, gColorFill);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::Draw(int x, int y)
{
    bool isShade = IsShade() || !funcOfActive();
    bool isPressed = IsPressed();
    Painter::DrawHLine(y + 1, x, x + MI_WIDTH, Color::BorderMenu(false));

    Painter::DrawVolumeButton(x + 1, y + 2, MI_WIDTH - 2, MI_HEIGHT - 2, 1, Color::MenuItem(false), Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK,
                             isPressed, isShade);

    Color colorText = isShade ? Color::MenuItem(true) : Color::BLACK;
    int delta = 0;
    if (isPressed && (!isShade))
    {
        colorText = Color::WHITE;
        delta = 1;
    }
    Painter::DrawStringInCenterRect(x + delta, y + delta, MI_WIDTH, MI_HEIGHT, Title(), colorText);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::DrawClosed(int x, int y)
{
    char buffer[20];

    bool pressed = IsPressed();
    bool shade = IsShade();
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade, false);

    Painter::DrawVolumeButton(x + 1, y + 17, MI_WIDTH_VALUE + 2, MI_HEIGHT_VALUE + 3, 1, Color::MENU_FIELD, Color::MENU_ITEM_BRIGHT,
                             Color::MENU_ITEM_DARK, true, shade);

    int deltaField = 10;
    int deltaSeparator = 2;
    int startX = 3;
    y += 21;
    PackedTime time = RTClock::GetPackedTime();
    Painter::DrawText(x + startX, y, Int2String((int)time.hours, false, 2, buffer), shade ? Color::MenuItem(true) : Color::BLACK);
    Painter::DrawText(x + startX + deltaField, y, ":");
    Painter::DrawText(x + startX + deltaField + deltaSeparator, y, Int2String((int)time.minutes, false, 2, buffer));
    Painter::DrawText(x + startX + 2 * deltaField + deltaSeparator, y, ":");
    Painter::DrawText(x + startX + 2 * deltaField + 2 * deltaSeparator, y, Int2String((int)time.seconds, false, 2, buffer));

    startX = 44;
    Painter::DrawText(x + startX, y, Int2String((int)time.day, false, 2, buffer));
    Painter::DrawText(x + startX + deltaField, y, ":");
    Painter::DrawText(x + startX + deltaField + deltaSeparator, y, Int2String((int)time.month, false, 2, buffer));
    Painter::DrawText(x + startX + 2 * deltaField + deltaSeparator, y, ":");
    Painter::DrawText(x + startX + 2 * deltaField + 2 * deltaSeparator, y, Int2String((int)time.year, false, 2, buffer));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::DrawOpened(int x, int y)
{
    char buffer[20];

    int width = MI_WIDTH_VALUE + 3;
    int height = 61;
    Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 3, gColorBack);
    DrawGovernorChoiceColorFormulaHiPart(this, x - 1, y - 1, IsPressed(), false, true);

    Painter::DrawRectangle(x - 1, y, width + 1, height + 1, Color::MenuTitle(false));

    Painter::DrawHLine(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, Color::MENU_TITLE_BRIGHT,
                             Color::MENU_TITLE_DARK, false, IsShade());

    int y0 = 21;
    int y1 = 31;

    typedef struct
    {
        int x;
        int y;
        int width;
    } StructPaint;

    int y2 = 41;
    int y3 = 51;
    int dX = 13;
    int wS = 10;
    int x0 = 41;
    StructPaint strPaint[8] =
    {
        {3,             y3, 60},    // Не сохранять
        {x0,            y0, wS},    // день
        {x0 + dX,       y0, wS},    // месяц
        {x0 + 2 * dX,   y0, wS},    // год
        {x0,            y1, wS},    // часы
        {x0 + dX,       y1, wS},    // мин
        {x0 + 2 * dX,   y1, wS},    // сек
        {3,             y2, 46}     // Сохранить
    };

    char strI[8][20];
    strcpy(strI[iEXIT], "Не сохранять");
    strcpy(strI[iDAY], Int2String(*day, false, 2, buffer));
    strcpy(strI[iMONTH], Int2String(*month, false, 2, buffer));
    strcpy(strI[iYEAR], Int2String(*year, false, 2, buffer));
    strcpy(strI[iHOURS], Int2String(*hours, false, 2, buffer));
    strcpy(strI[iMIN], Int2String(*minutes, false, 2, buffer));
    strcpy(strI[iSEC], Int2String(*seconds, false, 2, buffer));
    strcpy(strI[iSET], "Сохранить");

    Painter::DrawText(x + 3, y + y0, "д м г - ", Color::WHITE);
    Painter::DrawText(x + 3, y + y1, "ч м с - ");

    for (int i = 0; i < 8; i++)
    {
        if (*curField == i)
        {
            Painter::FillRegion(x + strPaint[i].x - 1, y + strPaint[i].y, strPaint[i].width, 8, Color::FLASH_10);
        }
        Painter::DrawText(x + strPaint[i].x, y + strPaint[i].y, strI[i], *curField == i ? Color::FLASH_01 : Color::WHITE);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawGovernorChoiceColorFormulaHiPart(Control *item, int x, int y, bool pressed, bool shade, bool opened)
{
    int delta = pressed && !shade ? 1 : 0;
    int width = MI_WIDTH_VALUE;

    if (IS_IP(item) && opened && ((IPaddress*)item)->port != 0)
    {
        width += MOI_WIDTH_D_IP;
    }

    Color color = shade ? Color::MENU_TITLE_DARK : (COLOR_SCHEME_IS_WHITE_LETTERS ? Color::WHITE :Color::BLACK);
    Painter::DrawHLine(y + 1, x, x + width + 3, Color::BorderMenu(false));

    Painter::DrawVolumeButton(x + 1, y + 2, width + 2, MI_HEIGHT_VALUE + 3, 1, Color::MenuItem(false), Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, 
        pressed, shade);

    Painter::DrawText(x + 6 + delta, y + 6 + delta, item->Title(), color);
    
    if(Menu::CurrentItem() == item)
    {
        char symbol = 0;

        if (IS_GOVERNOR(item))
        {
            symbol = Governor::GetSymbol(*((Governor*)item)->cell);
        }
        else if (IS_GOVERNOR(item) || IS_CHOICE_REG(item) ||  (item->IsOpened() && IS_CHOICE(item)))
        {
            symbol = Governor::GetSymbol(*((Choice*)item)->cell);
        }
        else if (IS_TIME(item))
        {
            Time *time = (Time*)item;
            if ((Menu::OpenedItem() == item) && (*time->curField != iEXIT) && (*time->curField != iSET))
            {
                int8 values[7] =
                {
                    0,
                    *time->day,
                    *time->month,
                    *time->year,
                    *time->hours,
                    *time->minutes,
                    *time->seconds
                };
                symbol = Governor::GetSymbol(values[*time->curField]);
            }
        }

        Painter::Draw4SymbolsInRect(x + MI_WIDTH - 13, y + 5 + (item->IsOpened() ? 0 : 15), symbol, shade ? color : Color::BLACK);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
// Если selPos == -1, подсвечивать не нужно
static void DrawValueWithSelectedPosition(int x, int y, int value, int numDigits, int selPos, bool hLine, bool fillNull)
{
    int firstValue = value;
    int height = hLine ? 9 : 8;
    for (int i = 0; i < numDigits; i++)
    {
        int rest = value % 10;
        value /= 10;
        if (selPos == i)
        {
            Painter::FillRegion(x - 1, y, 5, height, gColorFill);
        }
        if (!(rest == 0 && value == 0) || (firstValue == 0 && i == 0))
        {
            Painter::DrawChar(x, y, (char)(rest + 48), selPos == i ? Color::BLACK : Color::WHITE);
        }
        else if (fillNull)
        {
            Painter::DrawChar(x, y, '0', selPos == i ? Color::BLACK : Color::WHITE);
        }
        if (hLine)
        {
            Painter::DrawLine(x, y + 9, x + 3, y + 9, Color::WHITE);
        }
        x -= 6;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void GovernorIpCommon_DrawOpened(Control *item, int x, int y, int dWidth)
{
    int height = 34;
    Control *control = (Control *)item;
    Painter::DrawRectangle(x - 2, y - 1, MP_TITLE_WIDTH + 3 + dWidth, height + 3, gColorBack);
    Painter::DrawRectangle(x - 1, y, MP_TITLE_WIDTH + 1 + dWidth, height + 1, Color::MenuTitle(false));
    Painter::DrawHLine(y + MOI_HEIGHT_TITLE - 1, x, x + MOI_WIDTH + dWidth);
    DrawGovernorChoiceColorFormulaHiPart(item, x - 1, y - 1, control->IsPressed(), false, true);
    Painter::DrawVolumeButton(x, y + MOI_HEIGHT_TITLE, MOI_WIDTH - 1 + dWidth, height - MOI_HEIGHT_TITLE, 1, Color::BLACK, Color::MENU_TITLE_BRIGHT,
                             Color::MENU_TITLE_DARK, false, control->IsShade());
}
