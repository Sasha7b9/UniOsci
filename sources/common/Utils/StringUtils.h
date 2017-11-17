#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringUtils
{
public:
    /// @brief ѕреобразует voltage вольт в текстовую строку
    /// @attention —трака будет хранитьс€ до следующего вызова функции. ≈сли результат нужен большее количество времени, то его нужно скопитровать себе
    char *Voltage2String(float voltage,     ///< значение напр€жени€ в вольтах
                         bool alwaysSign,   ///< если установлено в true, знак выводитс€ всегда
                         char buffer[20]    ///< сюда записываетс€ возвращаемое значение
    );
    /// ѕереводит число с плавающей точкой в текстовый вид. numDigits - число цифровых знакомест
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);
    /// @brief ѕреобразует time секунд в текстовую строку
    char* Time2String(float time,           ///< врем€ в секундах. ћожет быть отрицательным
                      bool alwaysSign,      ///< если true, знак показываетс€ даже если time > 0
                      char buffer[20]       ///< сюда записываетс€ возвращаемое значение
    );
    /// ѕреобразует freq герц в текстовую строку.
    char* Freq2String(float freq, bool, char bufferOut[20]);

    char* FloatFract2String(float value, bool alwaysSign, char bufferOut[20]);
    /// ѕреобразует градусы в строку.
    char* Phase2String(float phase, bool, char bufferOut[20]);
private:
    static int NumDigitsInIntPart(float value);
};

extern StringUtils strUtils;

typedef char *(StringUtils::*pFuncPCFBPC)(float, bool, char*);
