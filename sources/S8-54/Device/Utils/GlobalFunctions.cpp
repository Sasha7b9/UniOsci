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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Translate trans;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Translate::Time2StringAccuracy(float time, bool alwaysSign, char buffer[20], int numDigits)
{
    buffer[0] = 0;
    const char *suffix = DICT(Ds);

    float fabsTime = fabsf(time);

    if (time == ERROR_VALUE_FLOAT)
    {
        strcat(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    else if (fabsTime + 0.5e-10f < 1e-6f)
    {
        suffix = DICT(Dns);
        time *= 1e9f;
    }
    else if (fabsTime + 0.5e-7f < 1e-3f)
    {
        suffix = DICT(Dus);
        time *= 1e6f;
    }
    else if (fabsTime + 0.5e-3f < 1.0f)
    {
        suffix = DICT(Dms);
        time *= 1e3f;
    }

    char bufferOut[20];
    strcat(buffer, strUtils.Float2String(time, alwaysSign, numDigits, bufferOut));
    strcat(buffer, suffix);

    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Translate::Float2Db(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    strcat(bufferOut, strUtils.Float2String(value, false, numDigits, buffer));
    strcat(bufferOut, "��");
    return bufferOut;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Translate::BCD2Int(uint bcd)
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
bool IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
float CircleAddFloat(float *val, float delta, float min, float max)
{
    *val += delta;
    if(*val > max)
    {
        *val = min;
    }
    return *val;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float CircleSubFloat(float *val, float delta, float min, float max)
{
    *val -= delta;
    if(*val < min)
    {
        *val = max;
    }
    return *val;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SwapInt(int *value0, int *value1)
{
    int temp = *value0;
    *value0 = *value1;
    *value1 = temp;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SortInt(int *value0, int *value1)
{
    if(*value1 < *value0)
    {
        SwapInt(value0,  value1);
    }
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
void BubbleSortINT(int array[], int num)
{
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                int time = array[j];
                array[j] = array[j + 1];
                array[j + 1] = time;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LoggingArrayUINT8(char *label, uint8 *array, int numElements)
{                                   // (1 + 3) -> 1 ������� �� ������ � �������� 3 �� 8-������ �����
                                    // 1 -> ����������� ����
                                    // strlen(label) -> �� ������ �����
                                    // 3 -> �� " - "
    char *buffer = (char *)malloc(numElements * (1 + 3) + 1 + strlen(label) + 3);   // 1 + 3 - 

    buffer[0] = 0;

    strcat(buffer, label);
    strcat(buffer, " - ");

    char bufferNumber[5];                               // ����� ��� ���������� ������������� 8-�������� ��������

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
