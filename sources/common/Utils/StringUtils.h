#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringUtils
{
public:
    /// Переводит значение voltage в тествоый вид в готовом для вывода на экран виде
    char *Voltage2String(float voltage, bool alwaysSign, char buffer[20]);
    /// Переводит число с плавающей точкой в текстовый вид. numDigits - число цифровых знакомест
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

private:
    static int NumDigitsInIntPart(float value);
};

extern StringUtils strUtils;
