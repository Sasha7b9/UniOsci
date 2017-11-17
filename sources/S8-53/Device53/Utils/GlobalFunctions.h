#pragma once
#include "defines.h"

                                                
                   
char*   Bin2String                              //  Преобразует value в текстовую строку. При этом выводятся все 8 разрядов.
                                                //  @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе.
                    (uint8 value,
                    char buffer[9]);
char    GetSymbolForGovernor(int value);                        // Возвращает изображение регулятора, соответствующее его текущему положению.
void    EmptyFuncVI16(int16);                                   // Функция-заглушка. Ничего не делает.
void    EmptyFuncVB(bool);                                      // Функция-заглушка. Ничего не делает.
void    IntToStrCat(char *_buffer, int _value);
