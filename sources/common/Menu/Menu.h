#pragma once
#include "defines.h"
#include "Panel/Controls.h"
#include "Menu/MenuPagesNames.h"
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Menu
 *  @{
 *  @defgroup Pages
 *  @{
 *  @}
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
public:
    /// Инициализация
    static void Init(void);

    static void ChangeStateFlashDrive();
    /// Функция должна вызываться в главном цикле.
    static void UpdateInput(void);
    /// Функция обработки короткого нажатия кнопки (менее 0.5 сек.).
    static void ShortPressureButton(PanelButton button);
    /// Функция обработки длинного нажатия кнопки (более 0.5 сек.).
    static void LongPressureButton(PanelButton button);
    /// Функция вызывается, когда кнопка переходит из отжатого в нажатое положение.
    static void PressButton(PanelButton button);
    /// Функция вызывается, когда кнопка переходит из нажатого в отжатое положение.
    static void ReleaseButton(PanelButton button);
    /// Функция вызывается в момент нажатия гувернёра.
    static void PressReg(PanelRegulator reg);
    /// Функция обработки поворота ручки УСТАНОВКА вправо.
    static void RotateRegSetRight(void);
    /// Функция обработки поворота ручки УСТАНОВКА влево.
    static void RotateRegSetLeft(void);
    /// Установить время автоматического сокрытия меню в соответствии с установками.
    static void SetAutoHide(bool active);
    /// Возвращает адрес пункта меню, находящегося под нажатой в данный момент кнопкой.
    static void *ItemUnderKey(void);
    /// Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    static char *StringNavigation(char buffer[100]);

    static void Show(bool show);

    static void OpenItemTime(void);
    /// Функция временно включает отображение строки навигации меню, если задано настройками.
    static void TemporaryEnableStrNavi(void);
    /// Ввыполнить эту функцию после Menu_UpdateInput().
    static void RunAfterUpdate(pFuncVV func);
    /// Нарисовать меню
    static void Draw();
    /// Возвращает адрес открытого элемента меню
    static Control *OpenedItem(void);
    /// Возвращает тип открытого элемента меню
    static TypeItem TypeOpenedItem(void);

    static NamePage GetNameOpenedPage(void);

    static Page *PagePointerFromName(NamePage namePage);
    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    static Control *CurrentItem(void);
    /// Закрыть открытый элемент меню
    static void CloseOpenedItem(void);
    /// Уменьшает или увеличивает значение Governor, GovernorColor или Choice по адресу item в зависимости от знака delta
    static void ChangeItem(void *item, int delta);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \todo STUB
    static void OpenPageAndSetItCurrent(NamePage namePage);

    static void ShortPressOnPageItem(Page *, int);

    static NamePage GetNamePage(const Page *);

private:
    static void *RetLastOpened(Page *_page, TypeItem *_type);
};


/** @}
 */
