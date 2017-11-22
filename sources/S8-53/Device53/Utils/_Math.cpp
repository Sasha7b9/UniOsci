#include "_Math.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Log.h"
#include "FPGA/FPGATypes.h"
#include "FPGA/FPGAMath.h"
#include "Display/Grid.h"
#include "Hardware/Timer.h"
#include <math.h>


/*
    Быстрое преобразование Фурье. Вычисляет модуль спектра для дейсвтительного сигнала.
    Количество отсчётов должно быть 2**N
*/

#ifndef DEBUG
#include "TablesWindow.h"
#include "TablesLog.h"

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
