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

void CallFuncOnPressButton(void *button);

/** @}
 */
