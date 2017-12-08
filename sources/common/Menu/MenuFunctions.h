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

void CallFuncOnPressButton(void *button);

/** @}
 */
