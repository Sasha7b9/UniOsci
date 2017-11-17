#include "defines.h"
#include "StringUtils.h"
#include "Settings/Settings.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


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
