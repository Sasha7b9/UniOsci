#include "defines.h"
#include "GlobalFunctions.h"
#include "Log.h"
#include "Display/Symbols.h"
#include "FPGA/fpgaTypes.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include "Utils/Dictionary.h"
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
