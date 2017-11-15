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
        if (value < max)
        {
            ++value;
        }
        else
        {
            value = (uint8)min;
        }
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
            if (max - term >= *value)
            {
                *value += (uint16)term;
            }
            else
            {
                *value = max;
            }
        }
        else
        {
            if (min - term <= *value)
            {
                *value += (uint16)term;
            }
            else
            {
                *value = min;
            }
        }
    }

    /// Переводит значение voltage в тествоый вид в готовом для вывода на экран виде
    char *Voltage2String(float voltage, bool alwaysSign, char buffer[20]);
    /// Переводит число с плавающей точкой в текстовый вид. numDigits - число цифровых знакомест
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

    template<class T>
    static void Limitation(T *value, T min, T max)
    {
        if (*value < min)
        {
            *value = min;
        }
        else if (*value > max)
        {
            *value = max;
        }
    }

    int LowSignedBit(uint value);

    char *Bin2String16(uint16 value, char valBuffer[19]);

    template<class T>
    int Sign(T x)
    {
        if (x > (T)(0)) { return 1; }
        if (x < (T)(0)) { return -1; }
        return 0;
    }

    void Smoothing(uint8 *data, int numPoints, int numSmooth);

private:
    static int NumDigitsInIntPart(float value);

    char *Bin2String(uint8 value, char buffer[9]);
};


extern Math math;
