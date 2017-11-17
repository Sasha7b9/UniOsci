#include "defines.h"
#include "GlobalFunctions.h"
#include "Settings/Settings.h"
#include "Math.h"
#include "Utils/StringUtils.h"
#include <math.h>
#include <string.h>
#include <ctype.h>


//------------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVI(int)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVpV(void *)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
bool EmptyFuncBV(void)
{
    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncpVII(void *, int, int)
{
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVI16(int16)
{
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVB(bool)
{
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* IntToStrCat(char *_buffer, int _value)
{
#define LENGTH_INTTOSTRCAT 10
    char buffer[LENGTH_INTTOSTRCAT];
    for (int i = 0; i < LENGTH_INTTOSTRCAT; i++)
    {
        buffer[i] = 0;
    }
    int pointer = LENGTH_INTTOSTRCAT - 1;

    while (_value > 0)
    {
        buffer[pointer] = (char)(_value % 10);
        _value /= 10;
        pointer--;
    }

    while (*_buffer)
    {
        _buffer++;
    }
    int i = 0;

    for (; i < LENGTH_INTTOSTRCAT; i++)
    {
        if (buffer[i] > 0)
        {
            break;
        }
    }

    for (; i < LENGTH_INTTOSTRCAT; i++)
    {
        *_buffer = 0x30 | buffer[i];
        _buffer++;
    }

    *_buffer = 0;

    return _buffer;
}
