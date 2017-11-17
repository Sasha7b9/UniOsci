#include "GlobalFunctions.h"
#include "Settings/Settings.h"
#include "Log.h"
#include "Display/Symbols.h"
#include "Math.h"
#include "Utils/StringUtils.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

bool String2Int(char *str, int *value)  
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    int length = strlen(str);
    if (length == 0)
    {
        return false;
    }
    *value = 0;
    int pow = 1;
    for(int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if(val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    }
    if(sign == -1)
    {
        *value *= -1;
    }
    return true;
}

char*    Bin2String(uint8 value, char buffer[9])
{
    for(int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }
    return buffer;
}

char* Float2Db(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    strcat(bufferOut, strUtils.Float2String(value, false, numDigits, buffer));
    strcat(bufferOut, "Да");
    return bufferOut;
}

bool IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}

float MaxFloat(float val1, float val2, float val3)
{
    float retValue = val1;
    if(val2 > retValue)
    {
        retValue = val2;
    }
    if(val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}

float CircleAddFloat(float *val, float delta, float min, float max)
{
    *val += delta;
    if(*val > max)
    {
        *val = min;
    }
    return *val;
}

float CircleSubFloat(float *val, float delta, float min, float max)
{
    *val -= delta;
    if(*val < min)
    {
        *val = max;
    }
    return *val;
}

void AddLimitationFloat(float *val, float delta, float min, float max)
{
    float sum = *val + delta;
    if(sum < min)
    {
        *val = min;
    }
    else if(sum > max)
    {
        *val = max;
    }
    else
    {
        *val = sum;
    }
}

void SwapInt(int *value0, int *value1)
{
    int temp = *value0;
    *value0 = *value1;
    *value1 = temp;
}

void SortInt(int *value0, int *value1)
{
    if(*value1 < *value0)
    {
        SwapInt(value0,  value1);
    }
}

char GetSymbolForGovernor(int value)
{
    static const char chars[] =
    {
        SYMBOL_GOVERNOR_SHIFT_0,
        SYMBOL_GOVERNOR_SHIFT_1,
        SYMBOL_GOVERNOR_SHIFT_2,
        SYMBOL_GOVERNOR_SHIFT_3
    };
    while(value < 0)
    {
        value += 4;
    }
    return chars[value % 4];
}

void EmptyFuncVV() { }

void EmptyFuncVI(int i) { }

void EmptyFuncVpV(void *empty) { }

void EmptyFuncpVII(void *v, int i, int ii) { }

void EmptyFuncVI16(int16 i) {}

void EmptyFuncVB(bool b) {}

void IntToStrCat(char *_buffer, int _value)
{
    static const int LENGHT = 10;
    char buffer[LENGHT];
    for(int i = 0; i < LENGHT; i++)
    {
        buffer[i] = 0;
    }
    int pointer = LENGHT -1;

    while(_value > 0)
    {
        buffer[pointer] = (unsigned char)(_value % 10);
        _value /= 10;
        pointer--;
    }

    while(*_buffer)
    {
        _buffer++;
    }
    int i = 0;

    for(; i < LENGHT; i++)
    {
        if(buffer[i] > 0)
        {
            break;
        }
    }

    for(; i < LENGHT; i++)
    {
        *_buffer = 0x30 | buffer[i];
        _buffer++;
    }

    *_buffer = 0;
}
