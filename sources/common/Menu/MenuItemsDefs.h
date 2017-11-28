#pragma once
#include "Utils/CommonFunctions.h"
#define FuncDraw            EmptyFuncVII
#define EmptyDrawPage       EmptyFuncVV
#define FuncActive          EmptyFuncBV
#define FuncPress           EmptyFuncVV
#define EmptyPressPage      EmptyFuncVV
#define FuncChanged         EmptyFuncVV
#define FuncChangedChoice   EmptyFuncVB
#define FuncBeforeDraw      EmptyFuncVV
#define FuncRegSetPage      EmptyFuncVI


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_PAGE_SB(name, s, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet, titleRU, titleEN, hintRU, hintEN,                   \
    item0, item1, item2, item3, item4, item5)                                                                                                   \
static const void * const  items##name[] = {(void *)item0, (void *)item1, (void *)item2, (void *)item3, (void *)item4, (void *)item5};          \
s const Page name = {Item_Page, true, namePage, 6, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                      \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_2(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN, item1, item2)                            \
static const void * const  items##name[] = {(void *)&item1, (void *)&item2};                                                                    \
s const Page name = {Item_Page, false, namePage, 2, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_3(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN, item1, item2, item3)                     \
static const void * const  items##name[] = {(void *)&item1, (void *)&item2, (void *)&item3};                                                    \
s const Page name = {Item_Page, false, namePage, 3, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_4(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                          \
    item1, item2, item3, item4)                                                                                                                 \
static const void * const  items##name[] = {(void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4};                                    \
s const Page name = {Item_Page, false, namePage, 4, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_5(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                          \
    item1, item2, item3, item4, item5)                                                                                                          \
static const void * const  items##name[] = {(void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5};                    \
s const Page name = {Item_Page, false, namePage, 5, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_6(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                          \
    item1, item2, item3, item4, item5, item6)                                                                                                   \
static const void * const  items##name[] = {(void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6};    \
s const Page name = {Item_Page, false, namePage, 6, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_7(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                          \
    item1, item2, item3, item4, item5, item6, item7)                                                                                            \
static const void * const  items##name[] = {                                                                                                    \
    (void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6, (void *)&item7};                            \
s const Page name = {Item_Page, false, namePage, 7, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_8(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                          \
    item1, item2, item3, item4, item5, item6, item7, item8)                                                                                     \
static const void * const  items##name[] = {                                                                                                    \
    (void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6, (void *)&item7, (void *)&item8};            \
s const Page name = {Item_Page, false, namePage, 8, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_9(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                          \
    item1, item2, item3, item4, item5, item6, item7, item8, item9)                                                                              \
static const void * const  items##name[] = {                                                                                                    \
    (void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6, (void *)&item7,                             \
    (void *)&item8, (void *)&item9};                                                                                                            \
s const Page name = {Item_Page, false, namePage, 9, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                     \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_11(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                         \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11)                                                              \
static const void * const  items##name[] = {                                                                                                    \
    (void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6, (void *)&item7,                             \
    (void *)&item8, (void *)&item9, (void *)&item10, (void *)&item11};                                                                          \
s const Page name = {Item_Page, false, namePage, 11, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                    \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_12(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                         \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12)                                                      \
static const void * const  items##name[] = {                                                                                                    \
    (void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6, (void *)&item7,                             \
    (void *)&item8, (void *)&item9, (void *)&item10, (void *)&item11, (void *)&item12};                                                         \
s const Page name = {Item_Page, false, namePage, 12, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                    \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_13(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                         \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13)                                              \
static const void * const  items##name[] = {                                                                                                    \
    (void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6, (void *)&item7,                             \
    (void *)&item8, (void *)&item9, (void *)&item10, (void *)&item11, (void *)&item12, (void *)&item13};                                        \
s const Page name = {Item_Page, false, namePage, 13, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                    \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};

#define DEF_PAGE_15(name, s, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                         \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10,                                                                      \
    item11, item12, item13, item14, item15)                                                                                                     \
static const void * const  items##name[] = {                                                                                                    \
    (void *)&item1, (void *)&item2, (void *)&item3, (void *)&item4, (void *)&item5, (void *)&item6, (void *)&item7,                             \
    (void *)&item8, (void *)&item9, (void *)&item10, (void *)&item11, (void *)&item12, (void *)&item13, (void *)&item14, (void *)&item15};      \
s const Page name = {Item_Page, false, namePage, 15, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                    \
    items##name, funcPress, EmptyDrawPage, FuncRegSetPage};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_SMALL_BUTTON(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw)                                   \
static const SButton name = { Item_SmallButton, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, funcPress, funcDraw, 0, 0}

#define DEF_SMALL_BUTTON_EXIT(name, keeper, funcActive, funcPress, funcDraw)                                                                \
static const SButton name = { Item_SmallButton, &keeper, funcActive, {"Выход", "Exit", "Кнопка для выхода в предыдущее меню",               \
    "Button to return to the previous menu"}, funcPress, funcDraw, 0, 0}

#define DEF_SMALL_BUTTON_HINTS_2(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2)                                                                       \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 }};              \
static const SButton name = { Item_SmallButton, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, funcPress, funcDraw, hints##name, 2}

#define DEF_SMALL_BUTTON_HINTS_3(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2, FuncDrawHint3, hintRu3, hintEn3)                                      \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 },               \
    { FuncDrawHint3, hintRu3, hintEn3 } };                                                                                                  \
static const SButton name = { Item_SmallButton, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, funcPress, funcDraw, hints##name, 3}

#define DEF_SMALL_BUTTON_HINTS_5(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2, FuncDrawHint3, hintRu3, hintEn3,                                      \
    FuncDrawHint4, hintRu4, hintEn4, FuncDrawHint5, hintRu5, hintEn5)                                                                       \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 },               \
    { FuncDrawHint3, hintRu3, hintEn3 }, { FuncDrawHint4, hintRu4, hintEn4 }, { FuncDrawHint5, hintRu5, hintEn5 } };                        \
static const SButton name = { Item_SmallButton, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, funcPress, funcDraw, hints##name, 5}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR(name, titleRU, titleEN, hintRU, hintEN, keeper, cell, min, max, funcActive, funcChanged, funcBeforeDraw)                       \
static const Governor name = {Item_Governor, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, min, max, (int16 *)(&(cell)), funcChanged,    \
    funcBeforeDraw};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_2(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2)                                                                             \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2};                                     \
static const Choice name = {Item_Choice, 2, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_3(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3)                                                           \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3};                   \
static const Choice name = {Item_Choice, 3, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_4(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4)                                         \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4}; \
static const Choice name = {Item_Choice, 4, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_5(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5)                       \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,  \
    nameRu5, nameEn5};                                                                                              \
static const Choice name = {Item_Choice, 5, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_6(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5, nameRu6, nameEn6)     \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,  \
    nameRu5, nameEn5, nameRu6, nameEn6};                                                                            \
static const Choice name = {Item_Choice, 6, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_7(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                       \
    nameRu6, nameEn6, nameRu7, nameEn7)                                                                             \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4,           \
    nameEn4, nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7};                                                 \
static const Choice name = {Item_Choice, 7, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_8(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                       \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8)                                                           \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,  \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8};                                        \
static const Choice name = {Item_Choice, 8, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_9(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                           \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9)                                             \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,      \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9};                          \
static const Choice name = {Item_ChoiceReg, 9, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell,  \
    hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_10(name, keeper, cell, funcActive, funcChanged, funcDraw, titleRU, titleEN, hintRU, hintEN,      \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                           \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10)                         \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,      \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10};      \
static const Choice name = {Item_ChoiceReg, 10, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, (int8 *)&cell, \
    hints##name, funcChanged, funcDraw};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_TIME(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, cur, h, mi, s, mo, d, y)   \
static const Time name = { Item_Time, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN}, &cur, &h, &mi, &s, &mo, &d, &y };
