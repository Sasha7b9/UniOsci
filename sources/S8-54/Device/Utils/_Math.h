#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup Math
 *  @{
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//extern const float tableScalesRange[RangeSize];
//extern const float absStepRShift[];
//extern const float voltsInPixel[];
//extern const float voltsInPoint[];
//extern const float absStepTShift[];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math_
{
public:
    void DataExtrapolation(uint8 *data, uint8 *there, int size);

    uint8 GetMaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);

    uint8 GetMinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);

    uint8 GetMaxFromArray(const uint8 *data, int firstPoint, int lastPoint);

    uint8 GetMaxFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);

    uint8 GetMinFromArray(const uint8 *data, int firstPoint, int lastPoint);
    /// Работает с внешним ОЗУ - считывать можно 16-битные значения
    uint8 GetMinFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);
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
