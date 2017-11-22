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
