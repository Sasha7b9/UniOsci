#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_VOLTAGE_ON_SCREEN(range) (tableScalesRange[(range)] * 5.0f)

#define POINT_2_VOLTAGE(value, range, rShift)   \
                (((float)(value) - (float)MIN_VALUE) * voltsInPixel[(range)] - MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)))

#define RSHIFT_2_REL(rShiftAbs, range) mathFPGA.RShift2Rel(rShiftAbs, range)

#define RSHIFT_2_ABS(rShift, range) (-(RShiftZero - ((int)(rShift))) * absStepRShift[(uint)(range)])

#define TSHIFT_2_REL(tShiftAbs, tBase) ((int)((tShiftAbs) / absStepTShift[(tBase)] / 2.0f))

#define TSHIFT_2_ABS(tShift, tBase)  (absStepTShift[(tBase)] * (tShift) * 2.0f)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const float tableScalesRange[];
extern const float absStepRShift[];
extern const float voltsInPixel[];
extern const float absStepTShift[];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MathFPGA
{
public:
    float VoltageCursor(float shiftCurU, Range range, int16 rShift);
    int RShift2Rel(float rShiftAbs, Range range);
};


extern MathFPGA mathFPGA;
