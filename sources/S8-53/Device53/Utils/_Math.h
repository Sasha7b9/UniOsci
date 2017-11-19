#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


#define POINT_2_VOLTAGE(value, range, rShift)   \
                (((float)(value) - (float)MIN_VALUE) * voltsInPixel[(range)] - MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)))

#define TSHIFT_2_ABS(tShift, tBase)  (absStepTShift[(tBase)] * (tShift) * 2.0f)

#define TSHIFT_2_REL(tShiftAbs, tBase) ((int)((tShiftAbs) / absStepTShift[(tBase)] / 2.0f))

extern const float tableScalesRange[RangeSize];
extern const float absStepRShift[];
extern const float voltsInPixel[];
extern const float absStepTShift[];

int     Math_RShift2Rel(float rShiftAbs, Range range);
float   Math_VoltageCursor(float shiftCurU, Range range, int16 rShift);
float   Math_TimeCursor(float shiftCurT, TBase tBase);
void    Math_DataExtrapolation(uint8 *data, uint8 *there, int size);
void    Math_PointsRelToVoltage(const uint8 *points, int numPoints, Range range, int16 rShift, float *voltage);
uint8   Math_VoltageToPoint(float voltage, Range range, int16 rShift);
void	Math_PointsVoltageToRel(const float *voltage, int numPoints, Range range, int16 rShift, uint8 *points);
uint8   Math_GetMaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
uint8   Math_GetMinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
/// \brief Возвращает координату x пересечения линии, проходящей через (x0, y0), (x1, y1), с горизонтальной линией, проходящей через точку с 
/// ординатой yHorLine.
float   Math_GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
void    Math_CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);
void    Math_CalculateMathFunction(float *data0andResult, float *data1, int numPoints);
int8    Math_AddInt8WithLimitation(int8 value, int8 delta, int8 min, int8 max);
uint8   Math_CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);
void    Math_CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
