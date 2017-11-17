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
/// Увелечивает значение по адресу val на delta. Затем, если результат больше max, приравнивает его min
float CircleAddFloat(float *val, float delta, float min, float max);
/// Уменьшает значение по адресу val на delta. Затем, елси результат меньше min, приравнивает его masx
float CircleSubFloat(float *val, float delta, float min, float max);
/// Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить
void AddLimitationFloat(float *val,             ///< по этому адресу хранится значение.
                        float delta,            ///< а это дельта.
                        float min,              ///< нижняя граница диапазона.
                        float max               ///< верхняя граница диапазона.
                        );
/// Возвращает изображение регулятора, соответствующее его текущему положению
char GetSymbolForGovernor(int value);
/// Функция-заглушка. Ничего не делает
void EmptyFuncVV(void);
/// Функция-заглушка. Ничего не делает
void EmptyFuncVpV(void *);
/// Функция-заглушка. Ничего не делает
void EmptyFuncVII(int, int);
void EmptyFuncVI(int);
bool EmptyFuncBV();
void EmptyFuncVB(bool);
/// Функция-заглушка. Ничего не делает
void EmptyFuncpVII(void *, int, int);
/// Заполнить массив одним значением
void FillArrayUINT8(uint8 *data, uint8 value, int numElements);
/// Найти первый элемент массива, не соотвествующий заданному
int FindAnotherElement(uint8 *data, uint8 value, int numElements);

char* IntToStrCat(char *_buffer, int _value);
/// Выводит numElements элементов массива array в одной строке консоли
void LoggingArrayUINT8(char *label, uint8 *array, int numElements);


/** @}  @}
 */
