#include "_Math.h"
#include "Settings/Settings.h"
#include "Log.h"
#include "FPGA/FPGAtypes.h"
#include "FPGA/FPGAMath.h"
#include "Display/Grid.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Math_ _math;

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Math_::FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = fabsf(value0) > fabsf(value1) ? fabsf(value0) : fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return fabsf(value0 - value1) < epsilonAbs;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::MinFrom3float(float value1, float value2, float value3)
{
    float retValue = value1;
    if(value2 < retValue)
    {
        retValue = value2;
    }
    if(value3 < retValue)
    {
        retValue = value3;
    }
    return retValue;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::MaxInt(int val1, int val2)
{
    return val1 > val2 ? val1 : val2;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::MinInt(int val1, int val2)
{
    return val1 < val2 ? val1 : val2;
}

/*
    Быстрое преобразование Фурье. Вычисляет модуль спектра для дейсвтительного сигнала.
    Количество отсчётов должно быть 2**N
*/

#ifndef DEBUG
#include "TablesWindow.h"
#include "TablesLog.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------
static float const *Koeff(int numPoints)
{
    float const *tables[3][4] = {
        {koeffsNorm256, koeffsHamming256, koeffsBlack256, koeffsHann256},
        {koeffsNorm512, koeffsHamming512, koeffsBlack512, koeffsHann512},
        {koeffsNorm1024, koeffsHamming1024, koeffsBlack1024, koeffsHann1024},
    };

    int row = 0;
    if (numPoints == 512)
    {
        row = 1;
    }
    else if (numPoints == 1024)
    {
        row = 2;
    }

    return tables[row][PageServiceMath_GetWindowFFT()];
}

#endif


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::CalculateMathFunction(float *data0andResult, float *dataB, int numPoints)
{
    if (MATH_FUNC_IS_SUM)
    {
        int delta = dataB - data0andResult;
        float *end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult += *(data0andResult + delta);
            data0andResult++;
        }
    }
    else if (MATH_FUNC_IS_MUL)
    {
        int delta = dataB - data0andResult;
        float *end = &data0andResult[numPoints];
        while (data0andResult < end)
        {
            *data0andResult *= *(data0andResult + delta);
            data0andResult++;
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((rand() / (float)RAND_MAX) * delta);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Math_::AddInt8WithLimitation(int8 value, int8 delta, int8, int8 max)
{
    int8 retValue = value + delta;
    if (retValue < 0)
    {
        return 0;
    }
    if (retValue > max)
    {
        return max;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::NumDigitsInNumber(int value)
{
    value = FabsInt(value);
    int num = 1;
    while ((value /= 10) > 0)
    {
        num++;
    }
    return num;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math_::FabsInt(int value)
{
    return value >= 0 ? value : -value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define LIMIT                           \
    if (value <= min) { return min; }   \
    if (value >= max) { return max; }   \
    return value;


//----------------------------------------------------------------------------------------------------------------------------------------------------
float Math_::LimitationFloat(float value, float min, float max)
{
    LIMIT
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Math_::CalculateFiltr(const uint8 *data, int x, int numPoints)
{
    if (NUM_SMOOTHING < 2)
    {
        return data[x];
    }

    int count = 1;
    int sum = data[x];
    int startDelta = 1;

    int endDelta = NUM_SMOOTHING / 2;

    for (int delta = startDelta; delta <= endDelta; delta++)
    {
        if (((x - delta) >= 0) && ((x + delta) < (numPoints)))
        {
            sum += data[x - delta];
            sum += data[x + delta];
            count += 2;
        }
    }

    if ((NUM_SMOOTHING % 2) == 1)
    {
        int delta = NUM_SMOOTHING / 2;
        if ((x + delta) < numPoints)
        {
            sum += data[x + delta];
            count++;
        }
    }

    return (uint8)(sum / count);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Math_::CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, bool needSmoothing)
{
    if (NUM_SMOOTHING < 2 || !needSmoothing)
    {
        memcpy(dataOut, dataIn, numPoints);
    }
    else
    {
        bool addCalculation = (NUM_SMOOTHING % 2) == 1;
        int endDelta = NUM_SMOOTHING / 2;
        int startDelta = 1;

        for (int i = 0; i < numPoints; i++)
        {
            int count = 1;
            int sum = dataIn[i];

            for (int delta = startDelta; delta <= endDelta; delta++)
            {
                if (((i - delta) >= 0) && ((i + delta) < (numPoints)))
                {
                    sum += dataIn[i - delta];
                    sum += dataIn[i + delta];
                    count += 2;
                }
            }

            if (addCalculation)
            {
                int delta = NUM_SMOOTHING / 2;
                
                if ((i + delta) < numPoints)
                {
                    sum += dataIn[i + delta];
                    count++;
                }
            }

            dataOut[i] = (uint8)(sum / count);
        }
    }
}
