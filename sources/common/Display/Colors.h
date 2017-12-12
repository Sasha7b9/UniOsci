#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Color;

#define COLOR_BLACK               (uint8)0
#define COLOR_WHITE               (uint8)1
#define COLOR_GRID                (uint8)2
#define COLOR_DATA_A              (uint8)3
#define COLOR_DATA_B              (uint8)4
#define COLOR_MENU_FIELD          (uint8)5
#define COLOR_MENU_TITLE          (uint8)6
#define COLOR_MENU_TITLE_DARK     (uint8)7
#define COLOR_MENU_TITLE_BRIGHT   (uint8)8
#define COLOR_MENU_ITEM           (uint8)9
#define COLOR_MENU_ITEM_DARK     (uint8)10
#define COLOR_MENU_ITEM_BRIGHT   (uint8)11
#define COLOR_DATA_WHITE_ACCUM_A (uint8)12   ///< Используется как для отрисовки канала на белом фоне, так и для отрисовки накопленных сигналов
#define COLOR_DATA_WHITE_ACCUM_B (uint8)13
#define COLOR_GRID_WHITE         (uint8)14
#define COLOR_EMPTY              (uint8)15
#define COLOR_NUMBER             (uint8)16
#define COLOR_FLASH_10           (uint8)17
#define COLOR_FLASH_01           (uint8)18
#define COLOR_INVERSE            (uint8)19

extern Color gColorChan[4];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Color
{
public:
    static Color FILL;
    static Color BACK;
    static Color GRID;

    Color(uint8 val) : value(val) { };

    static Color Cursors(Channel ch);
    static Color Trig();
    static Color ChanAccum(Channel ch);     ///< Цвет канала в режиме накопления
    static Color MenuItem(bool shade);      ///< Цвет элемента меню.
    static Color MenuTitle(bool shade);     ///< Цвет заголовка страницы. inShade == true, если страница затенена
    static Color BorderMenu(bool shade);    ///< Цвет окантовки меню
    static Color LightShadingText();        ///< Светлый цвет в тени.
    static Color Contrast(Color color);     ///< Возвращает цвет, контрастный к color. Может быть белым или чёрным.
    inline static Color Chan(Channel ch) { return gColorChan[ch]; }
    
    uint8 value;
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4623)
#endif

class ColorType
{
public:
    float   red;
    float   green;
    float   blue;
    float   stepRed;
    float   stepGreen;
    float   stepBlue;
    float   brightness;
    Color   color;
    int8    currentField;
    bool    alreadyUsed;

    /// Если forced == false, то инициализация происходит только при первом вызове функции
    void Init(bool forced);
    void SetBrightness(float brightness = -1.0f);
    void BrightnessChange(int delta);
    void ComponentChange(int delta);
private:
    void CalcSteps();
    void SetColor();
};

#ifdef WIN32
#pragma warning(pop)
#endif

#define MAKE_COLOR(r, g, b) ((uint16)(((b) & 0x1f) + (((g) & 0x3f) << 5) + (((r) & 0x1f) << 11)))
#define R_FROM_COLOR(color) (((uint16)(color) >> 11) & (uint16)0x1f)
#define G_FROM_COLOR(color) (((uint16)(color) >> 5) & (uint16)0x3f)
#define B_FROM_COLOR(color) ((uint16)(color) & 0x1f)
