#pragma once


/// Возвращает адрес элемента меню, соответствующего данной button
void *ItemUnderButton(PanelButton button);
/// Возвращает true, если кнопка, соответствующая элементу меню item, находится в нажатом положении
bool IsPressed(void *item                   ///< Указатель на элемент меню
                );

