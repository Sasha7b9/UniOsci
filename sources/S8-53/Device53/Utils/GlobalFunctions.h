#pragma once
#include "defines.h"

                                                
                   
char*   Bin2String                              //  Преобразует value в текстовую строку. При этом выводятся все 8 разрядов.
                                                //  @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе.
                    (uint8 value,
                    char buffer[9]);
float   CircleAddFloat(float *val, float delta, float min, float max);  // Увелечивает значение по адресу val на delta. Затем, если результат больше max, приравнивает его min.
float   CircleSubFloat(float *val, float delta, float min, float max);  // Уменьшает значение по адресу val на delta. Затем, елси результат меньше min, приравнивает его masx.
void    AddLimitationFloat                                              // Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить.
                            (float *val,                                // по этому адресу хранится значение.
                            float delta,                                // а это дельта.
                            float min,                                  // нижняя граница диапазона.
                            float max                                   // верхняя граница диапазона.
                            );
char    GetSymbolForGovernor(int value);                        // Возвращает изображение регулятора, соответствующее его текущему положению.
void    EmptyFuncVV();                                      // Функция-заглушка. Ничего не делает.
void    EmptyFuncVI(int);
void    EmptyFuncVpV(void *);                                   // Функция-заглушка. Ничего не делает.
void    EmptyFuncpVII(void *, int, int);                        // Функция-заглушка. Ничего не делает.
void    EmptyFuncVI16(int16);                                   // Функция-заглушка. Ничего не делает.
void    EmptyFuncVB(bool);                                      // Функция-заглушка. Ничего не делает.
void    IntToStrCat(char *_buffer, int _value);
