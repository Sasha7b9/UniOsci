#include "Math.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math math;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float absStepRShift[] =
{
    2e-3f / 20 / STEP_RSHIFT,
    5e-3f / 20 / STEP_RSHIFT,
    10e-3f / 20 / STEP_RSHIFT,
    20e-3f / 20 / STEP_RSHIFT,
    50e-3f / 20 / STEP_RSHIFT,
    100e-3f / 20 / STEP_RSHIFT,
    200e-3f / 20 / STEP_RSHIFT,
    500e-3f / 20 / STEP_RSHIFT,
    1.0f / 20 / STEP_RSHIFT,
    2.0f / 20 / STEP_RSHIFT,
    5.0f / 20 / STEP_RSHIFT,
    10.0f / 20 / STEP_RSHIFT,
    20.0f / 20 / STEP_RSHIFT
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Math::LowSignedBit(uint value)
{
    int verValue = 1;

    for (int i = 0; i < 32; i++)
    {
        if (verValue & value)
        {
            return i;
        }

        verValue <<= 1;
    }

    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Math::Bin2String16(uint16 value, char valBuffer[19])
{
    char buffer[9];
    strcpy(valBuffer, Bin2String((uint8)(value >> 8), buffer));
    strcpy((valBuffer[8] = ' ', valBuffer + 9), Bin2String((uint8)value, buffer));
    valBuffer[18] = '\0';
    return valBuffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Math::Bin2String(uint8 value, char buffer[9])
{
    for (int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(value, bit) ? '1' : '0';
    }
    buffer[8] = '\0';
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math::Smoothing(uint8 *data, int numPoints, int numSmooth)
{
    if (numSmooth == 0 || numSmooth == 1)
    {
        return;
    }

    float *buffer = (float *)malloc(numPoints * sizeof(float));
    int  *num = (int *)malloc(numPoints * sizeof(int));

    for (int i = 1; i < numPoints; i++)
    {
        buffer[i] = 0.0f;

        num[i] = 0;
        for (int j = -numSmooth / 2; j < numSmooth / 2; j++)
        {
            int index = i + j;
            if (index >= 1 && index < numPoints)
            {
                buffer[i] += data[index];
                ++num[i];
            }
        }
    }
    
    for (int i = 1; i < numPoints; i++)
    {
        data[i] = (uint8)(buffer[i] / num[i] + 0.5f);
    }

    free(buffer);
    free(num);
}
