#pragma once
#include "Display/Colors.h"
#include "Display/Display.h"
#include "defines.h"
#include "MenuItemsDefs.h"
#include "Menu/MenuPagesNames.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup MenuItems Menu Items
 *  @{
 */

extern int8 gCurDigit;

#define MENU_ITEMS_ON_DISPLAY       5   ///< ������� ������� ���� ���������� �� ������ �� ���������.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ���� ������� ����
typedef enum
{
    Item_None,           
    Item_Choice,        ///< ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
    Item_Button,        ///< ������.
    Item_Page,          ///< ��������.
    Item_Governor,      ///< ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������.
    Item_Time,          ///< ��������� ������ �����.
    Item_IP,            ///< ��������� ������ IP-�����.
    Item_GovernorColor, ///< ��������� ������� ����.
    Item_Formula,       ///< ��������� ������� ���� � ������������ ��� �������������� ������� (��������� � ��������)
    Item_MAC,           ///< MAC-�����
    Item_ChoiceReg,     ///< ������� ������, � ������� ����� �������������� �� �������, � ������
    Item_SmallButton,   ///< ������ ��� ������ ����� ������
    Item_NumberItems
} TypeItem;


/// ����� ����� ��� ���� ����� ��������� ����
#define COMMON_PART_MENU_ITEM                                                                       \
    TypeItem     type;           /* ��� ����� */                                                    \
    const Page  *keeper;         /* ����� ��������, ������� �����������. ��� Page_Main = 0 */       \
    pFuncBV      funcOfActive;   /* ������� �� ������ ������� */                                    \
    const char  *titleHint[4];   /* �������� �������� �� ������� � ���������� ������. ����� ��������� ��� ������ ������ */

#define COMMON_PART_MENU_ITEM_CHOICE                                                                \
    TypeItem    type;           /* ��� ����� */                                                     \
    int8        num;            /* ����� ��������� */                                               \
    const Page *keeper;         /* ����� ��������, ������� �����������. ��� Page_Main = 0 */        \
    pFuncBV     funcOfActive;   /* ������� �� ������ ������� */                                     \
    const char *titleHint[4];   /* �������� �������� �� ������� � ���������� ������. ����� ��������� ��� ������ ������ */

#define COMMON_PART_MENU_ITEM_PAGE                                                                  \
    TypeItem    type;           /* ��� ����� */                                                     \
    bool        isPageSB;       /* ���� true, �� ��� �������� ����� ������ */                       \
    NamePage    name;           /* ��� �� ������������ NamePage */                                  \
    int8        numItems;                                                                           \
    const Page *keeper;         /* ����� ��������, ������� �����������. ��� Page_Main = 0 */        \
    pFuncBV     funcOfActive;   /* ������� �� ������ ������� */                                     \
    const char *titleHint[4];   /* �������� �������� �� ������� � ���������� ������. ����� ��������� ��� ������ ������ */

class SButton;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///
/// ��������� �������� ����.
class Page
{
public:
    COMMON_PART_MENU_ITEM_PAGE
    const void   * const *items;            ///< ����� ��������� �� ������ ���� �������� (� ������� ��������)
                                            ///< ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 0 - B_Menu, 1...5 - B_F1...B_F5
    pFuncVV  funcOnPress;                   ///< ����� ���������� ��� ������� �� �������� ��������
    pFuncVV  funcOnDraw;                    ///< ����� ���������� ����� ��������� ������
    pFuncVI  funcRegSetSB;                  ///< � �������� ����� ������ ���������� ��� �������� ����� ���������
    int NumSubPages() const;                ///< D��������� ����� ���������� � �������� �� ������ page
    int NumItems() const;                   ///< ���������� ���������� ��������� � �������� �� ������ page
    int NumCurrentSubPage();                ///< ���������� ����� ������� ���������� �������� �� ������ page
    NamePage GetNamePage() const;           ///< ���������� ��� �������� page
    void SetCurrentSB() const;              ///< ���������� ������� ������ �������� � ������ ��������.
    
    void *Item(int numElement) const;       ///< ���������� ����� �������� ���� �������� ��������
    SButton* SmallButonFromPage(int numButton);
    /// \todo ���������� ������� ������� �������� �������� �� ������ page �� ������. ���� ������� ����������� 0, ��� ����� 0, ���� ������� 
    /// ����������� 1, ��� ����� 5 � �.�.
    int PosItemOnTop();
    /// �������� ������� ��������� ������� ������ ��� ������ numItem �������� page
    void ShortPressOnItem(int numItem);
    /// ���������� ������� �������� �������� ������� page
    int8 PosCurrentItem() const;
    void ChangeSubPage(int delta) const;
    void Draw(int x, int y);
};

//typedef class Page Page;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
/// ��������� ������.
class Button
{
public:
    COMMON_PART_MENU_ITEM
    pFuncVV     funcOnPress;        ///< �������, ������� ���������� ��� ������� �� ������.
    pFuncVII    funcForDraw;        ///< ������� ����� ���������� �� ����� ��������� ������.
    void CallFuncOnDraw(int x, int y);
    void Draw(int x, int y);
};

#define DEF_BUTTON(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw) \
static const Button name = { Item_Button, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, funcPress, funcDraw };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///
typedef struct
{
    pFuncVII    funcDrawUGO;    ///< ��������� �� ������� ��������� ����������� �������� ������
    pString     helpUGO[2];     ///< ������� � ������� �����������.
} StructHelpSmallButton;


/// ��������� ������ ��� ��������������� ������ ����.
class SButton
{
public:
    COMMON_PART_MENU_ITEM
    pFuncVV                         funcOnPress;    ///< ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                        funcForDraw;    ///< ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpSmallButton    *hintUGO; 
    int                             numHints;
    void Draw(int x, int y);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
/// ��������� ���������.
class Governor
{
public:
    COMMON_PART_MENU_ITEM
    int16   minValue;       ///< ���������� ��������, ������� ����� ��������� ���������.
    int16   maxValue;       ///< ������������ ��������.
    int16  *cell;
    pFuncVV funcOfChanged;  ///< �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcBeforeDraw; ///< �������, ������� ���������� ����� ����������
    /// ��������� �������� �������� ���������� ��� ���������� �������� ���� ���� Governor (� ����������� �� ����� delta).
    void StartChange(int detla);
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 NextValue();
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue();
    /// ������������ ��������� ���� ��������.
    float Step();
    /// �������� �������� � ������� ������� ��� ��������� ��������.
    void ChangeValue(int delta);
    /// ��� �������� �������� ������������ ������ �� ��������� �������.
    void NextPosition();
    /// ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits();
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
    /// ���������� ����������� ����������, ��������������� ��� �������� ���������
    static char GetSymbol(int value);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class Choice
{
public:
    COMMON_PART_MENU_ITEM_CHOICE
    int8                *cell;
    const char  * const *names;             ///< �������� ������ �� ������� � ���������� ������.
    pFuncVB			    funcOnChanged;      ///< ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII            funcForDraw;        ///< ������� ���������� ����� ��������� ��������. 
    void StartChange(int delta);
    float Step();                           ///< ������������ ��������� ���� ��������.
    void ChangeIndex(int delta);            ///< �������� �������� choice � ����������� �� �������� � ����� delta.
    int NumSubItems();                      ///< ���������� ���������� ��������� ������ � �������� �� ������ choice
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    /// ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const char *NameCurrentSubItem();
    /// ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const char *NameNextSubItem();
    const char *NamePrevSubItem();
    /// ���������� ��� �������� ������ �������� choice � ������� i ��� ��� �������� � �������� ���� ���������
    const char *NameSubItem(int i);
};

typedef struct
{
    COMMON_PART_MENU_ITEM
    uint8 *ip0;
    uint8 *ip1;
    uint8 *ip2;
    uint8 *ip3;
    pFuncVB funcOfChanged;
    uint16 *port;
    void NextPosition();                            ///< ��� �������� �������� ������������ ������ �� ��������� �������.
    void ChangeValue(int delta);                    ///< �������� �������� � ������� ������� ��� �������� ��������.
    void GetNumPosIPvalue(int *numIP, int *selPos); ///< ���������� ����� �������� ����� (4 - ����� �����) � ����� ������� ������� � �����.
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
} IPaddress;

typedef struct
{
    COMMON_PART_MENU_ITEM
    uint8 *mac0;
    uint8 *mac1;
    uint8 *mac2;
    uint8 *mac3;
    uint8 *mac4;
    uint8 *mac5;
    pFuncVB funcOfChanged;
    void ChangeValue(int delta);
    void Draw(int x, int y, bool opened);
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
} MACaddress;

/// ��������� ������� ���� ��� ��������� ������������� � ������ �������������� �������
#define FIELD_SIGN_MEMBER_1_ADD 0
#define FIELD_SIGN_MEMBER_1_MUL 1
#define FIELD_SIGN_MEMBER_2_ADD 2
#define FIELD_SIGN_MEMBER_2_MUL 3
#define POS_SIGN_MEMBER_1       0
#define POS_KOEFF_MEMBER_1      1
#define POS_SIGN_MEMBER_2       2
#define POS_KOEFF_MEMBER_2      3
typedef struct
{
    COMMON_PART_MENU_ITEM
    int8   *function;       ///< ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8   *koeff1add;      ///< ����� ������������ ��� ������ ����� ��� ��������
    int8   *koeff2add;      ///< ����� ������������ ��� ������ ����� ��� ��������
    int8   *koeff1mul;      ///< ����� ������������ ��� ������ ����� ��� ���������
    int8   *koeff2mul;      ///< ����� ������������ ��� ������ ����� ��� ���������
    int8   *curDigit;       ///< ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����, 2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV funcOfChanged;  ///< ��� ������� ���������� ����� ��������� ��������� �������� ����������.
    void Draw(int x, int y, bool opened);
    void DrawClosed(int x, int y);
    void DrawLowPart(int x, int y, bool pressed, bool shade);
    void WriteText(int x, int y, bool opened);
} Formula;

typedef struct
{
    COMMON_PART_MENU_ITEM
    ColorType  *ct;                 ///< ��������� ��� �������� �����.
    pFuncVV     funcOnChanged;      ///< ��� ������� ����� �������� ����� ��������� �������� ��������.
    void ChangeValue(int delta);    ///< �������� ������� ����� � governor.
    void Draw(int x, int y, bool opened);
private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y, int delta);
} GovernorColor;

#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7

/// ������������� � ���������� �����.
class Time
{
public:
    COMMON_PART_MENU_ITEM
    int8 *curField;   ///< ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����, 6 - ���, 7 - ����������.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
    void SetOpened();
    void IncCurrentPosition();
    void SetNewTime();
    void SelectNextPosition();
    void DecCurrentPosition();
    void Draw(int x, int y, bool opened);
    void DrawClosed(int x, int y);
    void DrawOpened(int x, int y);
};

/// ��������� ��� �������� ����, ����������� �� ������� �����
typedef struct
{
    int16 i;
} StructFuncReg;

#define CHOICE_RUN_FUNC_CHANGED(c, val)     \
    if(c->funcOnChanged)                    \
    {                                       \
        c->funcOnChanged(val);              \
    }


typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];

/** @}  @}
 */
