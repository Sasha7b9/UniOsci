#pragma once
#include "defines.h"
#include "Panel/Controls.h"
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
    void Init(void);

    void ChangeStateFlashDrive();
    /// Функция должна вызываться в главном цикле.
    void UpdateInput(void);
    /// Функция обработки короткого нажатия кнопки (менее 0.5 сек.).
    void ShortPressureButton(PanelButton button);
    /// Функция обработки длинного нажатия кнопки (более 0.5 сек.).
    void LongPressureButton(PanelButton button);
    /// Функция вызывается, когда кнопка переходит из отжатого в нажатое положение.
    void PressButton(PanelButton button);
    /// Функция вызывается, когда кнопка переходит из нажатого в отжатое положение.
    void ReleaseButton(PanelButton button);
    /// Функция вызывается в момент нажатия гувернёра.
    void PressReg(PanelRegulator reg);
    /// Функция обработки поворота ручки УСТАНОВКА вправо.
    void RotateRegSetRight(void);
    /// Функция обработки поворота ручки УСТАНОВКА влево.
    void RotateRegSetLeft(void);
    /// Установить время автоматического сокрытия меню в соответствии с установками.
    void SetAutoHide(bool active);
    /// Возвращает адрес пункта меню, находящегося под нажатой в данный момент кнопкой.
    void *ItemUnderKey(void);
    /// Возвращает путь к текущему пункту меню в текстовом виде, готовом к выводу на экран.
    char *StringNavigation(char buffer[100]);

    void Show(bool show);

    void OpenItemTime(void);
    /// Функция временно включает отображение строки навигации меню, если задано настройками.
    void TemporaryEnableStrNavi(void);
    /// Ввыполнить эту функцию после Menu_UpdateInput().
    void RunAfterUpdate(pFuncVV func);
    /// Нарисовать меню
    void Draw();
    /// Возвращает адрес открытого элемента меню
    void *OpenedItem(void);
    /// Возвращает тип открытого элемента меню
    TypeItem TypeOpenedItem(void);

    Page *GetOpenedPage(void);

    /// Возвращает адрес текущего элемента меню (текущим, как правило, является элемент, кнопка которого была нажата последней
    void *CurrentItem(void);
    /// Закрыть открытый элемент меню
    void CloseOpenedItem(void);
    /// Уменьшает или увеличивает значение Governor, GovernorColor или Choice по адресу item в зависимости от знака delta
    void ChangeItem(void *item, int delta);
    /// Возвращает номер страницы по её адресу
    int CalculateNumPage(Page *page);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \todo STUB
    void OpenPageAndSetItCurrent(Page *page);

    void ShortPressOnPageItem(Page *, int);

private:
    void *RetLastOpened(Page *_page, TypeItem *_type);
};


extern Menu menu;


/** @}
 */
