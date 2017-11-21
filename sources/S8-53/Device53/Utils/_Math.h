#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


void    Math_CalculateMathFunction(float *data0andResult, float *data1, int numPoints);
int8    Math_AddInt8WithLimitation(int8 value, int8 delta, int8 min, int8 max);
uint8   Math_CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);
void    Math_CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
