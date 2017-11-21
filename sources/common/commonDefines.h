#pragma once

#if (!(defined S8_53) && !(defined S8_54) && !(defined S8_55) && !(defined KORNET))
#error ("You must selelect device")
#endif

#if ((defined S8_54) || (defined S8_55))
#define S8_54_55
#endif


typedef const    char *const    pString;
typedef unsigned char           uint8;
typedef unsigned int            uint;
typedef signed   char           int8;
typedef unsigned short int      uint16;
typedef signed   short int      int16;
typedef unsigned char           uchar;
typedef unsigned long  long int uint64;


typedef void    (*pFuncVV)(void);
typedef void    (*pFuncVpV)(void*);
typedef bool    (*pFuncBV)(void);
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef void    (*pFuncVI16)(int16);
typedef bool    (*pFuncBU8)(uint8);
typedef void    (*pFuncVI16pI16pI16)(int16, int16*, int16*);
typedef float   (*pFuncFU8)(uint8);
typedef char*   (*pFuncCFB)(float, bool);
typedef char*   (*pFuncCFBC)(float, bool, char*);
typedef void    (*pFuncpU8)(uint8*);
typedef void    (*pFuncVpVIIB)(void*, int, int, bool);

#define _GET_BIT(value, bit)    (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)    ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)  ((value) &= (~(1 << bit)))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]


#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_INT16   SHRT_MAX
#define ERROR_VALUE_UINT8   255
#define ERROR_VALUE_INT     INT_MAX
#define ERROR_STRING_VALUE  "--.--"
