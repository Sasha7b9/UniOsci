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
