#pragma once


typedef const char * const      pString;

#define _GET_BIT(value, bit)    (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)    ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)  ((value) &= (~(1 << bit)))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]
