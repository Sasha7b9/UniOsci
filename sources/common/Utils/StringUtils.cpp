#include "defines.h"
#include "StringUtils.h"
#include "Settings/Settings.h"
#include "Dictionary.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StringUtils strUtils;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *StringUtils::Voltage2String(float voltage, bool alwaysSign, char buffer[20])
{
    buffer[0] = 0;
    char *suffix;
    if (voltage == ERROR_VALUE_FLOAT)
    {
        strcat(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    else if (fabsf(voltage) + 0.5e-4f < 1e-3f)
    {
        suffix = LANG_RU ? "\x10ìêÂ" : "\x10uV";
        voltage *= 1e6f;
    }
    else if (fabsf(voltage) + 0.5e-4f < 1.0f)
    {
        suffix = LANG_RU ? "\x10ìÂ" : "\x10mV";
        voltage *= 1e3f;
    }
    else if (fabsf(voltage) + 0.5e-4f < 1000.0f)
    {
        suffix = LANG_RU ? "\x10Â" : "\x10V";
    }
    else
    {
        suffix = LANG_RU ? "\x10êÂ" : "\x10kV";
        voltage *= 1e-3f;
    }

    CHAR_BUF(bufferOut, 20);

    Float2String(voltage, alwaysSign, 4, bufferOut);
    strcat(buffer, bufferOut);
    strcat(buffer, suffix);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *StringUtils::Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char *pBuffer = bufferOut;

    if (value == ERROR_VALUE_FLOAT)
    {
        strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }

    if (!alwaysSign)
    {
        if (value < 0)
        {
            *pBuffer = '-';
            pBuffer++;
        }
    }
    else
    {
        *pBuffer = value < 0 ? '-' : '+';
        pBuffer++;
    }

    char format[] = "%4.2f\0\0";

    format[1] = (char)numDigits + 0x30;

    int numDigitsInInt = NumDigitsInIntPart(value);

    format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
    if (numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }

    snprintf(pBuffer, 19, format, fabsf(value));

    float val = atof(pBuffer);

    if (NumDigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = NumDigitsInIntPart(val);
        format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
        sprintf(pBuffer, format, value);
    }

    bool signExist = alwaysSign || value < 0;
    while (strlen(bufferOut) < numDigits + (signExist ? 2 : 1))
    {
        strcat(bufferOut, "0");
    }

    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int StringUtils::NumDigitsInIntPart(float value)
{
    float fabsValue = fabsf(value);

    int numDigitsInInt = 0;
    if (fabsValue >= 10000)
    {
        numDigitsInInt = 5;
    }
    else if (fabsValue >= 1000)
    {
        numDigitsInInt = 4;
    }
    else if (fabsValue >= 100)
    {
        numDigitsInInt = 3;
    }
    else if (fabsValue >= 10)
    {
        numDigitsInInt = 2;
    }
    else
    {
        numDigitsInInt = 1;
    }

    return numDigitsInInt;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* StringUtils::Time2String(float time, bool alwaysSign, char buffer[20])
{
    buffer[0] = 0;
    char *suffix = 0;
    if (time == ERROR_VALUE_FLOAT)
    {
        strcat(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    else if (fabsf(time) + 0.5e-10f < 1e-6f)
    {
        suffix = LANG_RU ? "íñ" : "ns";
        time *= 1e9f;
    }
    else if (fabsf(time) + 0.5e-7f < 1e-3f)
    {
        suffix = LANG_RU ? "ìêñ" : "us";
        time *= 1e6f;
    }
    else if (fabsf(time) + 0.5e-3f < 1.0f)
    {
        suffix = LANG_RU ? "ìñ" : "ms";
        time *= 1e3f;
    }
    else
    {
        suffix = LANG_RU ? "ñ" : "s";
    }

    char bufferOut[20];
    strcat(buffer, Float2String(time, alwaysSign, 4, bufferOut));
    strcat(buffer, suffix);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *StringUtils::Freq2String(float freq, bool, char bufferOut[20])
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
        suffix = LANG_RU ? "ÌÃö" : "MHz";
        freq /= 1e6f;
    }
    else if (freq >= 1e3f)
    {
        suffix = LANG_RU ? "êÃö" : "kHz";
        freq /= 1e3f;
    }
    else
    {
        suffix = LANG_RU ? "Ãö" : "Hz";
    }
    char buffer[20];
    strcat(bufferOut, Float2String(freq, false, 4, buffer));
    strcat(bufferOut, suffix);
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* StringUtils::FloatFract2String(float value, bool alwaysSign, char bufferOut[20])
{
    return Float2String(value, alwaysSign, 4, bufferOut);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* StringUtils::Phase2String(float phase, bool, char bufferOut[20])
{
    char buffer[20];
    sprintf(bufferOut, "%s\xa8", Float2String(phase, false, 4, buffer));
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *StringUtils::Freq2StringAccuracy(float freq, char bufferOut[20], int numDigits)
{
    bufferOut[0] = 0;
    const char *suffix = DICT(DHz);
    if (freq == ERROR_VALUE_FLOAT)
    {
        strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6f)
    {
        suffix = DICT(DMHz);
        freq /= 1e6f;
    }
    else if (freq >= 1e3f)
    {
        suffix = DICT(DkHz);
        freq /= 1e3f;
    }
    char buffer[20];
    strcat(bufferOut, Float2String(freq, false, numDigits, buffer));
    strcat(bufferOut, suffix);
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *StringUtils::Bin2String(uint8 value, char buffer[9])
{
    for (int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }
    buffer[8] = '\0';
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *StringUtils::Bin2String16(uint16 value, char valBuffer[19])
{
    char buffer[9];
    strcpy(valBuffer, Bin2String((uint8)(value >> 8), buffer));
    strcpy((valBuffer[8] = ' ', valBuffer + 9), Bin2String((uint8)value, buffer));
    valBuffer[18] = '\0';
    return valBuffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *StringUtils::Hex8toString(uint8 value, char buffer[3])
{
    sprintf(buffer, "%02X", value);
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char* StringUtils::Hex16toString(uint16 value, char buffer[5])
{
    sprintf(buffer, "%04X", value);
    return buffer;
}
