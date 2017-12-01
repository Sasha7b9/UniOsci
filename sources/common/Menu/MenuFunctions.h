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

/// Вовзращает true, если элемент меню item является ативным, т.е. может быть нажат
bool ItemIsAcitve(const void *item);
/// Возвращает тип элемента меню по адресу address
TypeItem TypeMenuItem(const void *address);
/// Возвращает true, если текущий элемент страницы с именем namePage открыт
bool CurrentItemIsOpened(Page *page);
/// Сделать/разделать текущим пункт страницы
void SetCurrentItem(const void *item,               ///< адрес элемента, активность которого нужно установить/снять
                    bool active                     ///< true - сделать элемент текущим, false - расделать элемент текущим. В этом случае текущим становится заголовок страницы
                    );
/// Возвращает адрес элемента, которому принадлежит элемент по адресу item
Page *Keeper(const void *item);

bool ChangeOpenedItem(void *item, int delta);
/// Возвращает высоту в пикселях открытого элемента Choice или NamePage по адресу item
int HeightOpenedItem(void *item);
/// Возвращает название элемента по адресу item, как оно выглядит на дисплее прибора
const char *TitleItem(void *item);
/// Возвращает true, если button - функциональная клавиша [1..5]
bool IsFunctionalButton(PanelButton button);
/// Открыть/закрыть элемент меню по адрему item
void OpenItem(const void *item, bool open);
/// Возвращает true, если элемент меню по адрему item открыт
bool ItemIsOpened(const void *item);

bool IsPageSB(const void *item);

void CallFuncOnPressButton(void *button);

/** @}
 */
