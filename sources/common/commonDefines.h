#pragma once

#if (!(defined S8_53) && !(defined S8_54) && !(defined S8_55) && !(defined KORNET))
#error ("You must selelect device")
#endif

#if ((defined S8_54) || (defined S8_55))
#define S8_54_55
#endif

#if defined S8_53
#define MODEL_RU "С8-53"
#define MODEL_EN "S8-53"
#elif defined S8_54
#define MODEL_RU "С8-54"
#define MODEL_EN "S8-54"
#elif defined S8_55
#define MODEL_RU "С8-55"
#define MODEL_EN "S8-55"
#endif


typedef const    char *const    pString;
typedef unsigned char           uint8;
typedef unsigned int            uint;
typedef signed   char           int8;
typedef unsigned short int      uint16;
typedef signed   short int      int16;
typedef unsigned char           uchar;
typedef unsigned long  long int uint64;
typedef unsigned char           BYTE;

#ifdef WIN32
typedef unsigned int uint32_t;
#endif


typedef void    (*pFuncVV)(void);
typedef void    (*pFuncVpV)(void*);
typedef bool    (*pFuncBV)(void);
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef void    (*pFuncVI16)(int16);
typedef bool    (*pFuncBU8)(uint8);
typedef void    (*pFuncVI16pI16pI16)(int16, int16 *, int16 *);
typedef float   (*pFuncFU8)(uint8);
typedef char*   (*pFuncCFB)(float, bool);
typedef char*   (*pFuncCFBC)(float, bool, char*);
typedef void    (*pFuncVU8)(uint8 *);
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

#ifdef WIN32
#define __attribute__(x)
#else
#pragma anon_unions
#endif

typedef union
{
    uint16 halfWord;
    uint8  byte[2];
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
} BitSet16;

typedef union
{
    uint    word;
    uint16  halfWord[2];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
    };
    uint8   byte[4];
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
    };
} BitSet32;

typedef union
{
    uint64 dword;
    uint   word[2];
    uint16 halfWord[4];
    uint8  byte[8];
    struct
    {
        int sword0;
        int sword1;
    };
    struct
    {
        uint word0;
        uint word1;
    };
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
        uint16 halfWord2;
        uint16 halfWord3;
    };
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        uint8 byte4;
        uint8 byte5;
        uint8 byte6;
        uint8 byte7;
    };
} BitSet64;

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)

#define ENABLE_RU "Вкл"
#define ENABLE_EN "On"
#define DISABLE_RU "Откл"
#define DISABLE_EN "Off"

#define SAFE_FREE(x) if(x) free(x); (x) = 0;

#define ERROR_HANDLER() _Error_Handler(__FILE__, __LINE__);
void _Error_Handler(char *, int);
