#pragma once
#include "defines.h"

                                                
                   
char*   Bin2String                              //  Преобразует value в текстовую строку. При этом выводятся все 8 разрядов.
                                                //  @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе.
                    (uint8 value,
                    char buffer[9]);
char*   Bin2String16                            // Преобразует value в текстовую строку
                                                // @attention Строка будте храниться до следующего вызова функции. Если рузультат нужен большее количество времени, то его нужно скопировать себе.
                    (uint16 value,
                    char buffer[19]);
char*   Hex8toString                            // Преобразует value в текстовую строку в шестнадцатиричном виде.
                                                // @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе.
                    (uint8 value,
                    char buffer[3]);
char*   Int2String                              // Преобразует value в текстовую строку.
                                                // @attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопировать себе.
                    (int value,                 // значение.
                    bool alwaysSign,            // если установлен в true, знак будет выводиться всегда.
                    int numMinFields,           // минимальное число занимаемых знакомест. Если для вывода числа столько не требуется, лишние заполняются нулями.
                    char buffer[20]
                    );
bool    String2Int(char *str, int *value);
char*   Float2Db(float value, int numDigits, char bufferOut[20]);   // Преобразует децибелы в текстовую строку.
bool    IntInRange                              // Возвращает true, если value входит в диапазон [min; max].
                   (int value,                  // проверяемое значение.
                   int min,                     // нижняя граница диапазона.
                   int max                      // верхняя граница диапазона.
                   );
float   MaxFloat(float val1, float val2, float val3);                   // Возвращает максимальное значение из трёх.
float   CircleAddFloat(float *val, float delta, float min, float max);  // Увелечивает значение по адресу val на delta. Затем, если результат больше max, приравнивает его min.
float   CircleSubFloat(float *val, float delta, float min, float max);  // Уменьшает значение по адресу val на delta. Затем, елси результат меньше min, приравнивает его masx.
void    AddLimitationFloat                                              // Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить.
                            (float *val,                                // по этому адресу хранится значение.
                            float delta,                                // а это дельта.
                            float min,                                  // нижняя граница диапазона.
                            float max                                   // верхняя граница диапазона.
                            );
void    SwapInt(int *value0, int *value1);                      // Обменивает местами содержимое памяти по адресам value0 и value1.
void    SortInt(int *value0, int *value1);                      // Размещает переменные value0 и value1 в порядке возрастания.
char    GetSymbolForGovernor(int value);                        // Возвращает изображение регулятора, соответствующее его текущему положению.
void    EmptyFuncVV();                                      // Функция-заглушка. Ничего не делает.
void    EmptyFuncVI(int);
void    EmptyFuncVpV(void *);                                   // Функция-заглушка. Ничего не делает.
void    EmptyFuncpVII(void *, int, int);                        // Функция-заглушка. Ничего не делает.
void    EmptyFuncVI16(int16);                                   // Функция-заглушка. Ничего не делает.
void    EmptyFuncVB(bool);                                      // Функция-заглушка. Ничего не делает.
void    IntToStrCat(char *_buffer, int _value);
