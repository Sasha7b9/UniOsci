#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RSHIFT_2_ABS(rShift, range) (-(RShiftZero - ((int)(rShift))) * absStepRShift[(uint)(range)])

#define LIMIT_BELOW(x, min)         if((x) < (min)) { x = (min); }

#define LIMIT_ABOVE(x, max)         if((x) > (max)) { x = (max); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const float absStepRShift[];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math
{
public:
    /// Возвращает true, если value входит в диапазон [min; max]
    bool IntInRange(int value, int min, int max );

    int LowSignedBit(uint value);

    void Smoothing(uint8 *data, int numPoints, int numSmooth);
    /// Возвращает максимальное значение из трёх
    float MaxFloat(float val1, float val2, float val3);
    /// Обменивает местами содержимое памяти по адресам value0 и value1
    void SwapInt(int *value0, int *value1);
    /// Размещает переменные value0 и value1 в порядке возрастания
    void SortInt(int *value0, int *value1);
    
    int MinFrom2Int(int val0, int val1);

    template<class T>
    void CircleIncrease(T *value, int min, int max)
    {
        if (*value < max)   { ++(*value); }
        else                { *value = (T)min; }
    }

    template<class T>
    void CircleDecrease(T *value, int min, int max)
    {
        if (*value > min)   { --(*value); }
        else                { *value = (T)max; }
    }

    template<class T>
    static void LimitationIncrease(T *value, T max)
    {
        if ((*value) < max) { ++(*value); }
    }

    /// Инкрементировать *value, но не больше, чем max
    template<class T>
    static void LimitationDecrease(T *value, T min)
    {
        if (*value > min)  { --(*value); }
    }

    /// Прибавить к *value term и ограничить, если результат выходит за границы [min, max]
    template<class T>
    static void AddtionThisLimitation(T *value, int term, T min, T max)
    {
        if (term > 0)
        {
            if (max - term >= *value)   { *value += (T)term; }
            else                        { *value = max;      }
        }
        else
        {
            if (min - term <= *value)   { *value += (T)term; }
            else                        { *value = min;      }
        }
    }

    template<class T>
    static void Limitation(T *value, T min, T max)
    {
        if (*value < min)       { *value = min; }
        else if (*value > max)  { *value = max; }
    }

    template<class T>
    static T LimitationRet(T value, T min, T max)
    {
        if(value < min) { return min; }
        if(value > max) { return max; }
        return value;
    }
};


extern class Math math;

/// Вычисление 10**pow.
int Pow10(int pow);

template<class T> int Sign(T x)
{
    if (x > (T)(0)) { return 1; }
    if (x < (T)(0)) { return -1; }
    return 0;
}

template<class T> T Abs(T x)
{
    if (x < (T)0)   { return -x; }
    return x;
}

typedef void (Math::*pFuncMathPI8II)(int8 *, int, int);
typedef void (Math::*pFuncMathPIII)(int *, int, int);
