#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


float   Math_TimeCursor(float shiftCurT, TBase tBase);
void    Math_PointsRelToVoltage(const uint8 *points, int numPoints, Range range, int16 rShift, float *voltage);
uint8   Math_VoltageToPoint(float voltage, Range range, int16 rShift);
void	Math_PointsVoltageToRel(const float *voltage, int numPoints, Range range, int16 rShift, uint8 *points);
/// \brief Возвращает координату x пересечения линии, проходящей через (x0, y0), (x1, y1), с горизонтальной линией, проходящей через точку с 
/// ординатой yHorLine.
float   Math_GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
void    Math_CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);
void    Math_CalculateMathFunction(float *data0andResult, float *data1, int numPoints);
int8    Math_AddInt8WithLimitation(int8 value, int8 delta, int8 min, int8 max);
uint8   Math_CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);
void    Math_CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
