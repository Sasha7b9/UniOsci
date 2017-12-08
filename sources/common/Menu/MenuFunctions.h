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

bool ChangeOpenedItem(Control *item, int delta);
/// Возвращает название элемента по адресу item, как оно выглядит на дисплее прибора
const char *TitleItem(void *item);
/// Возвращает true, если button - функциональная клавиша [1..5]
bool IsFunctionalButton(PanelButton button);
/// Открыть/закрыть элемент меню по адрему item
void OpenItem(const void *item, bool open);
/// Возвращает true, если элемент меню по адрему item открыт
bool ItemIsOpened(const Control *item);

void CallFuncOnPressButton(void *button);

/** @}
 */
