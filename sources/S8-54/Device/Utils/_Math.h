#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


class Math_
{
public:
    /// Сравнивает два числа. Возвращает true, если числа отличаются друг от друга не более, чем на epsilonPart. При этом для расчёта epsilonPart используется большее в смысле модуля число
    bool FloatsIsEquals(float value0, float value1, float epsilonPart);

    float MinFrom3float(float value1, float value2, float value3);

    int MinInt(int val1, int val2);

    int MaxInt(int val1, int val2);

    void CalculateMathFunction(float *data0andResult, float *dataB, int numPoints);

    float RandFloat(float min, float max);

    int8 AddInt8WithLimitation(int8 value, int8 delta, int8 min, int8 max);
    /// Вычисляет число разрядов в целом типа int
    int NumDigitsInNumber(int value);
    /// Возвращает модуль value
    int FabsInt(int value);

    float LimitationFloat(float value, float min, float max);

    uint8 CalculateFiltr(const uint8 *data, int x, int numPoints);

    void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, bool needSmoothing);
};


extern Math_ _math;


/** @}  @}
 */
