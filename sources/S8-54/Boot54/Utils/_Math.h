#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


#define LIMIT_ABOVE(x, max)         if(x > (max)) { (x) = (max); }

#define SET_MIN_IF_LESS(x, min)     if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)   if((x) > (max)) { (max) = (x); }

#define LIMITATION_ABOVE(var, value, max)   var = (value); if((var) > (max)) { (var) = (max); }

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };

#define MAX_VOLTAGE_ON_SCREEN(range) (tableScalesRange[(range)] * 5)

#define POINT_2_VOLTAGE(value, range, rShift) (((value) - MIN_VALUE) * voltsInPixel[(range)] - MAX_VOLTAGE_ON_SCREEN((range)) - RSHIFT_2_ABS((rShift), (range)))

#define TSHIFT_2_ABS(tShift, tBase)  (absStepTShift[(tBase)] * (tShift) * 2.0f)

#define TSHIFT_2_REL(tShiftAbs, tBase) ((tShiftAbs) / absStepTShift[(tBase)] / 2.0f)

#define ROUND(x) ((x) += 0.5f, (x))

#define PI 3.141592653589793f

extern const float tableScalesRange[RangeSize];
extern const float absStepRShift[];
extern const float voltsInPixel[];
extern const float absStepTShift[];

int     Math_RShift2Rel(float rShiftAbs, Range range);
float   Math_VoltageCursor(float shiftCurU, Range range, uint16 rShift);
float   Math_TimeCursor(float shiftCurT, TBase tBase);
void    Math_DataExtrapolation(uint8 *data, uint8 *there, int size);
void    Math_PointsRelToVoltage(const uint8 *points, int numPoints, Range range, uint16 rShift, float *voltage);
uint8   Math_VoltageToPoint(float voltage, Range range, uint16 rShift);
void	Math_PointsVoltageToRel(const float *voltage, int numPoints, Range range, uint16 rShift, uint8 *points);
uint8   Math_GetMaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
uint8   Math_GetMinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
uint8   Math_GetMaxFromArray(const uint8 *data, int firstPoint, int lastPoint);
uint8   Math_GetMaxFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);
uint8   Math_GetMinFromArray(const uint8 *data, int firstPoint, int lastPoint);
uint8   Math_GetMinFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);    // Работает с внешним ОЗУ - считывать можно 16-битные значения.

float   Math_GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);   // Возвращает координату x пересечения линии, проходящей через (x0, y0), (x1, y1), с горизонтальной линией, проходящей через точку с ординатой yHorLine.
bool    Math_FloatsIsEquals(float value0, float value1, float epsilonPart);                     // Сравнивает два числа. Возвращает true, если числа отличаются друг от друга не более, чем на epsilonPart. При этом для расчёта epsilonPart используется большее в смысле модуля число.
void    Math_CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);
void    Math_CalculateMathFunction(float *data0andResult, float *dataB, int numPoints);
float   Math_RandFloat(float min, float max);
int     Math_NumDigitsInNumber(int value);                                                      // Вычисляет число разрядов в целом типа int.

float   LimitationFloat(float value, float min, float max);

uint8   Math_CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);
