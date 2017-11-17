#pragma once
#include "defines.h"
#include "FPGA/FPGA.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RSHIFT_2_ABS(rShift, range) (-(RShiftZero - (rShift)) * absStepRShift[(uint)(range)])


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const float absStepRShift[];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math
{
public:


    template<class T>
    void CircleIncrease(T &value, int min, int max)
    {
        if (value < max)    { ++value; }
        else                { value = (T)min; }
    }

    template<class T>
    void CircleDecrease(T &value, int min, int max)
    {
        if (value > min)    { --value; }
        else                { value = (T)max; }
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

    int LowSignedBit(uint value);

    template<class T>
    int Sign(T x)
    {
        if (x > (T)(0)) { return 1; }
        if (x < (T)(0)) { return -1; }
        return 0;
    }

    void Smoothing(uint8 *data, int numPoints, int numSmooth);
};


extern class Math math;
