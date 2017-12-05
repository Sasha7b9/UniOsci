#pragma once
#include "Menu/MenuItems.h"
#include "Panel/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @defgroup MenuFunctions Menu Functions
 *  @brief Вспомогательные функции для работы с меню
 *  @{
 */

/// Возвращает true, если текущий элемент страницы с именем namePage открыт
bool CurrentItemIsOpened(NamePage namePage);
/// Сделать/разделать текущим пункт страницы
void SetCurrentItem(const void *item,               ///< адрес элемента, активность которого нужно установить/снять
                    bool active                     ///< true - сделать элемент текущим, false - расделать элемент текущим. В этом случае текущим становится заголовок страницы
                    );
bool ChangeOpenedItem(Control *item, int delta);
/// Возвращает высоту в пикселях открытого элемента Choice или NamePage по адресу item
int HeightOpenedItem(Control *item);
/// Возвращает название элемента по адресу item, как оно выглядит на дисплее прибора
const char *TitleItem(void *item);
/// Возвращает true, если button - функциональная клавиша [1..5]
bool IsFunctionalButton(PanelButton button);
/// Открыть/закрыть элемент меню по адрему item
void OpenItem(const void *item, bool open);
/// Возвращает true, если элемент меню по адрему item открыт
bool ItemIsOpened(const Control *item);

bool IsPageSB(const Control *item);

void CallFuncOnPressButton(void *button);

/** @}
 */
