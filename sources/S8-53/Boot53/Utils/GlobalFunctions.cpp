#include "defines.h"
#include "GlobalFunctions.h"
#include "Settings/Settings.h"
#include "Math.h"
#include "Utils/StringUtils.h"
#include <math.h>
#include <string.h>
#include <ctype.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------------------------------
char* FloatFract2String(float value, bool alwaysSign, char bufferOut[20])
{
    return strUtils.Float2String(value, alwaysSign, 4, bufferOut);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Int2String(int value, bool alwaysSign, int numMinFields, char buffer[20])
{
    char format[20] = "%";
    sprintf(&(format[1]), "0%d", numMinFields);
    strcat(format, "d");
    if (alwaysSign && value >= 0)
    {
        buffer[0] = '+';
        sprintf(buffer + 1, format, value);
    }
    else
    {
        sprintf(buffer, format, value);
    }
    return buffer;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
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
    for (int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    }
    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Hex8toString(uint8 value, char buffer[3], bool upper)
{
    sprintf(value < 16 ? (buffer[0] = '0', buffer + 1) : (buffer), upper ? "%X" : "%x", value);
    return buffer;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Hex16toString(uint16 value, char buffer[5], bool)
{
    sprintf(buffer, "%04X", value);
    return buffer;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Time2String(float time, bool alwaysSign, char buffer[20])
{
    return Time2StringAccuracy(time, alwaysSign, buffer, 4);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Time2StringAccuracy(float time, bool alwaysSign, char buffer[20], int numDigits)
{
    buffer[0] = 0;
    char *suffix = 0;

    float fabsTime = fabsf(time);
    bool russian = set.common.lang == Russian;

    if (time == ERROR_VALUE_FLOAT)
    {
        strcat(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    else if (fabsTime + 0.5e-10f < 1e-6f)
    {
        suffix = russian ? "íñ" : "ns";
        time *= 1e9f;
    }
    else if (fabsTime + 0.5e-7f < 1e-3f)
    {
        suffix = russian ? "ìêñ" : "us";
        time *= 1e6f;
    }
    else if (fabsTime + 0.5e-3f < 1.0f)
    {
        suffix = russian ? "ìñ" : "ms";
        time *= 1e3f;
    }
    else
    {
        suffix = russian ? "ñ" : "s";
    }

    char bufferOut[20];
    strcat(buffer, strUtils.Float2String(time, alwaysSign, numDigits, bufferOut));
    strcat(buffer, suffix);

    return buffer;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Phase2String(float phase, bool, char bufferOut[20])
{
    char buffer[20];
    sprintf(bufferOut, "%s\xa8", strUtils.Float2String(phase, false, 4, buffer));
    return bufferOut;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char*  Freq2String(float freq, bool, char bufferOut[20])
{
    return Freq2StringAccuracy(freq, bufferOut, 4);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Freq2StringAccuracy(float freq, char bufferOut[20], int numDigits)
{
    bufferOut[0] = 0;
    char *suffix = 0;
    if (freq == ERROR_VALUE_FLOAT)
    {
        strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6f)
    {
        suffix = set.common.lang == Russian ? "ÌÃö" : "MHz";
        freq /= 1e6f;
    }
    else if (freq >= 1e3f)
    {
        suffix = set.common.lang == Russian ? "êÃö" : "kHz";
        freq /= 1e3f;
    }
    else
    {
        suffix = set.common.lang == Russian ? "Ãö" : "Hz";
    }
    char buffer[20];
    strcat(bufferOut, strUtils.Float2String(freq, false, numDigits, buffer));
    strcat(bufferOut, suffix);
    return bufferOut;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
char* Float2Db(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    strcat(bufferOut, strUtils.Float2String(value, false, numDigits, buffer));
    strcat(bufferOut, "äÁ");
    return bufferOut;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
bool IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
float MaxFloat(float val1, float val2, float val3)
{
    float retValue = val1;
    if (val2 > retValue)
    {
        retValue = val2;
    }
    if (val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
float CircleAddFloat(float *val, float delta, float min, float max)
{
    *val += delta;
    if (*val > max)
    {
        *val = min;
    }
    return *val;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
float CircleSubFloat(float *val, float delta, float min, float max)
{
    *val -= delta;
    if (*val < min)
    {
        *val = max;
    }
    return *val;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void AddLimitationFloat(float *val, float delta, float min, float max)
{
    float sum = *val + delta;
    if (sum < min)
    {
        *val = min;
    }
    else if (sum > max)
    {
        *val = max;
    }
    else
    {
        *val = sum;
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void SwapInt(int *value0, int *value1)
{
    int temp = *value0;
    *value0 = *value1;
    *value1 = temp;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void SortInt(int *value0, int *value1)
{
    if (*value1 < *value0)
    {
        SwapInt(value0, value1);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVV(void)
{
}


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


//------------------------------------------------------------------------------------------------------------------------------------------------------
int BCD2Int(uint bcd)
{
    int pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}
