#pragma once


typedef const char * const      pString;

#define _GET_BIT(value, bit)    (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)    ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)  ((value) &= (~(1 << bit)))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))
