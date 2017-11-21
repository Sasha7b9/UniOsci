#include "FPGAMath.h"
#include "FPGA/FPGATypes.h"
#include "Display/DisplayTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MathFPGA mathFPGA;

const float tableScalesRange[] = { 2e-3f, 5e-3f, 10e-3f, 20e-3f, 50e-3f, 100e-3f, 200e-3f, 500e-3f, 1.0f, 2.0f, 5.0f, 10.0f, 20.0f };

const float absStepRShift[] =
{
    2e-3f   / 20 / STEP_RSHIFT,
    5e-3f   / 20 / STEP_RSHIFT,
    10e-3f  / 20 / STEP_RSHIFT,
    20e-3f  / 20 / STEP_RSHIFT,
    50e-3f  / 20 / STEP_RSHIFT,
    100e-3f / 20 / STEP_RSHIFT,
    200e-3f / 20 / STEP_RSHIFT,
    500e-3f / 20 / STEP_RSHIFT,
    1.0f    / 20 / STEP_RSHIFT,
    2.0f    / 20 / STEP_RSHIFT,
    5.0f    / 20 / STEP_RSHIFT,
    10.0f   / 20 / STEP_RSHIFT,
    20.0f   / 20 / STEP_RSHIFT
};

const float absStepTShift[] =
{
#if defined(S8_54_55)
    1e-9f   / 20.0f,
#endif
    2e-9f   / 20, 
    5e-9f   / 20, 
    10e-9f  / 20, 
    20e-9f  / 20,
    50e-9f  / 20,   // 1.0  Это коэффициенты для реализации алгоритма прореживания отсчётов
    100e-9f / 20,   // 2.0
    200e-9f / 20,   // 4.0
    500e-9f / 20,   // 10.0
    1e-6f   / 20,   // 20.0
    2e-6f   / 20,   // 40.0
    5e-6f   / 20,   // 100.0
    10e-6f  / 20,   // 200.0
    20e-6f  / 20,   // 400.0
    50e-6f  / 20,   // 1e3
    100e-6f / 20,   // 2e3
    200e-6f / 20,   // 4e3
    500e-6f / 20,   // 10e3
    1e-3f   / 20,   // 20e3
    2e-3f   / 20,   // 40e3
    5e-3f   / 20,   // 100e3
    10e-3f  / 20,   // 200e3
    20e-3f  / 20,   // 400e3
    50e-3f  / 20,   // 1e4
    100e-3f / 20,   // 2e4
    200e-3f / 20,   // 4e4
    500e-3f / 20,   // 10e4
    1.0f    / 20,   // 20e4
    2.0f    / 20,   // 40e4
    5.0f    / 20,   // 100e4
    10.0f   / 20    // 200e4
};

const float voltsInPixel[] =
{
    2e-3f   / GRID_DELTA,   // 2mV
    5e-3f   / GRID_DELTA,   // 5mV
    10e-3f  / GRID_DELTA,   // 10mV
    20e-3f  / GRID_DELTA,   // 20mV
    50e-3f  / GRID_DELTA,   // 50mV
    100e-3f / GRID_DELTA,   // 100mV
    200e-3f / GRID_DELTA,   // 200mV
    500e-3f / GRID_DELTA,   // 500mV
    1.0f    / GRID_DELTA,   // 1V
    2.0f    / GRID_DELTA,   // 2V
    5.0f    / GRID_DELTA,   // 5V
    10.0f   / GRID_DELTA,   // 10V
    20.0f   / GRID_DELTA    // 20V
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float MathFPGA::VoltageCursor(float shiftCurU, Range range, int16 rShift)
{
    return MAX_VOLTAGE_ON_SCREEN(range) - shiftCurU * voltsInPixel[range] - RSHIFT_2_ABS(rShift, range);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int MathFPGA::RShift2Rel(float rShiftAbs, Range range)
{
    int retValue = RShiftZero + (int)(rShiftAbs / absStepRShift[range]);
    if (retValue < RShiftMin)
    {
        retValue = RShiftMin;
    }
    else if (retValue > RShiftMax)
    {
        retValue = RShiftMax;
    }
    return retValue;
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
float MathFPGA::TimeCursor(float shiftCurT, TBase tBase)
{
    return shiftCurT * absStepTShift[tBase];
}
