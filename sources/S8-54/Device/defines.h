#pragma once

#define S8_54

#include "commonDefines.h"

#ifdef WIN32
#define __DSB()
#define __NOP()
#define __ALIGN_BEGIN
#define __ALIGN_END
#define __weak
#endif

#include <stm32f4xx_hal.h>
#include <limits.h>

#define NUM_VER "1.3"

#ifdef S8_54
#define MODEL_RU "С8-54"
#define MODEL_EN "S8-54"
#endif

#ifdef S8_55
#define MODEL_RU "С8-55"
#define MODEL_EN "S8-55"
#endif



#define INTERRUPT_P2P   // Если определено, то для чтения точек используется прерывание
 
#include <stm32f4xx_hal.h>


#define OLD_RECORDER

typedef signed      char        int8;
typedef signed      short   int int16;
typedef unsigned    char        uint8;
typedef unsigned    char        uint8_t;
typedef unsigned    short   int uint16;
typedef unsigned    short   int uint16_t;
typedef unsigned    int         uint;
typedef unsigned    int         uint32;
typedef unsigned    int         uint32_t;
typedef unsigned    char        uchar;
typedef uint8                   BYTE;


#ifndef WIN32
typedef signed      char        int8_t;
#else
#define __attribute__(x)
#endif


#define DEBUG


#define __IO volatile
    

typedef void    (*pFuncVV)(void);               ///< Указатель на функцию, ничего не принимающую и ничего не возвращающую.
typedef void    (*pFuncVpV)(void *);            ///< Указатель на функцию, принимающую указатель на void и ничего не возвращающую.
typedef bool    (*pFuncBV)(void);
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef void    (*pFuncVI16)(int16);
typedef bool    (*pFuncBU8)(uint8);
typedef void    (*pFuncVI16pI16pI16)(int16, int16 *, int16 *);
typedef float   (*pFuncFU8)(uint8);
typedef char   *(*pFuncCFB)(float, bool);
typedef char   *(*pFuncCFBC)(float, bool, char *);
typedef void    (*pFuncVU8)(uint8 *);
typedef void    (*pFuncVpVIIB)(void *, int, int, bool);
typedef int     (*pFuncIU8I)(uint8 *, int);

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4201 4310)
#else
#pragma anon_unions
#endif


// Объединение размером 16 бит
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

// Объединение размером 32 бита
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

// Объединение размером 64 бита
typedef union
{
    long long unsigned int  dword;
    uint                    word[2];
    struct
    {
        uint uword0;
        uint uword1;
    };
    struct
    {
        int word0;
        int word1;
    };
    uint16  halfWord[4];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
        uint16 halfWord2;
        uint16 halfWord3;
    };
    uint8   byte[8];
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

typedef struct
{
    int16  rel;
    float  abs;
} StructRelAbs;

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

#define BINARY_U8( bits ) _bitset(0##bits)

#define DISABLE_RU  "Откл"
#define DISABLE_EN  "Disable"
#define ENABLE_RU   "Вкл"
#define ENABLE_EN   "Enable"

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_INT16   SHRT_MAX
#define ERROR_VALUE_UINT8   255
#define ERROR_VALUE_INT     INT_MAX
#define ERROR_STRING_VALUE  "--.--"
#define M_PI                3.14159265358979323846f

#define SET_FLAG(flag)      ((flag) = 1)
#define CLEAR_FLAG(flag)    ((flag) = 0)

#define HARDWARE_ERROR HardwareErrorHandler(__FILE__, __FUNCTION__, __LINE__);
void HardwareErrorHandler(const char *file, const char *function, int line);

#define SAFE_FREE(x) if(x) free(x); (x) = 0;

#ifdef WIN32
#pragma warning(pop)
#endif
