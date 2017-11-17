#pragma once
#include "Data/DataSettings.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Utils
 *  @{
 *  @defgroup GlobalFunctions Global Functions
 *  @{
 */

/// Выделяет память из кучи для канала. Настройки длины памяти берёт из ds или set (если ds == 0)
void *AllocMemForChannelFromHeap(Channel ch, DataSettings *ds);
/// Возвращает количество памяти, требуемой для сохранения данных одного канала
int RequestBytesForChannel(Channel ch, DataSettings *ds);
/// Возвращает изображение регулятора, соответствующее его текущему положению
char GetSymbolForGovernor(int value);
/// Заполнить массив одним значением
void FillArrayUINT8(uint8 *data, uint8 value, int numElements);
/// Найти первый элемент массива, не соотвествующий заданному
int FindAnotherElement(uint8 *data, uint8 value, int numElements);

/** @}  @}
 */
