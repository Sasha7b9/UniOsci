#include "defines.h"
#include "GlobalFunctions.h"
#include "Log.h"
#include "Display/Symbols.h"
#include "FPGA/fpgaTypes.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include "Utils/Dictionary.h"
#include "Utils/_Math.h"
#include "Utils/StringUtils.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


//----------------------------------------------------------------------------------------------------------------------------------------------------
void *AllocMemForChannelFromHeap(Channel ch, DataSettings *ds)
{
    int numBytes = RequestBytesForChannel(ch, ds);
    if(numBytes)
    {
        return malloc(numBytes);
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
int RequestBytesForChannel(Channel ch, DataSettings *ds)
{
    ENumPointsFPGA numBytes;
    PeakDetMode peakDet;

    if(ds)
    {
        numBytes = (ENumPointsFPGA)ENUM_BYTES(ds);
        peakDet = PEAKDET(ds);
    }
    else
    {
        numBytes = NumPoints_2_ENumPoints(NUM_BYTES(ds));
        peakDet = SET_PEAKDET;
    }

    if((numBytes == FNP_32k) || (numBytes == FNP_16k && peakDet == PeakDet_Enable))
    {
        if(ch == A)
        {
            return FPGA_MAX_POINTS * 2;
        }
        return 0;
    }

    return FPGA_MAX_POINTS;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVV(void) { }

//----------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVpV(void *) { }

//----------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncVII(int, int) { }

void EmptyFuncVI(int) {};

bool EmptyFuncBV() { return true; };

void EmptyFuncVB(bool) {};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void EmptyFuncpVII(void *, int, int) { }

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FillArrayUINT8(uint8 *data, uint8 value, int numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        data[i] = value;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int FindAnotherElement(uint8 *data, uint8 value, int numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        if (data[i] != value)
        {
            return i;
        }
    }

    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *IntToStrCat(char *_buffer, int _value)
{
#define LENGTH_INTTOSTRCAT 10
    char buffer[LENGTH_INTTOSTRCAT];
    for(int i = 0; i < LENGTH_INTTOSTRCAT; i++)
    {
        buffer[i] = 0;
    }
    int pointer = LENGTH_INTTOSTRCAT -1;

    while(_value > 0)
    {
        buffer[pointer] = (char)(_value % 10);
        _value /= 10;
        pointer--;
    }

    while(*_buffer)
    {
        _buffer++;
    }
    int i = 0;

    for(; i < LENGTH_INTTOSTRCAT; i++)
    {
        if(buffer[i] > 0)
        {
            break;
        }
    }

    for(; i < LENGTH_INTTOSTRCAT; i++)
    {
        *_buffer = 0x30 | buffer[i];
        _buffer++;
    }

    *_buffer = 0;

    return _buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LoggingArrayUINT8(char *label, uint8 *array, int numElements)
{                                   // (1 + 3) -> 1 позиция на пробел и максимум 3 на 8-битное число
                                    // 1 -> завершающий ноль
                                    // strlen(label) -> на запись метки
                                    // 3 -> на " - "
    char *buffer = (char *)malloc(numElements * (1 + 3) + 1 + strlen(label) + 3);   // 1 + 3 - 

    buffer[0] = 0;

    strcat(buffer, label);
    strcat(buffer, " - ");

    char bufferNumber[5];                               // Буфер для текстового представления 8-битового значения

    for (int i = 0; i < numElements; i++)
    {
        sprintf(bufferNumber, "%d ", array[i]);
        strcat(buffer, bufferNumber);
        for (int num = 0; num < 4 - strlen(bufferNumber); num++)
        {
            strcat(buffer, " ");
        }
    }

    LOG_WRITE(buffer);

    free(buffer);
}
