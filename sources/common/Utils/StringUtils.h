#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringUtils
{
public:
    /// @brief Преобразует voltage вольт в текстовую строку
    /// @attention Страка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопитровать себе
    char *Voltage2String(float voltage,     ///< значение напряжения в вольтах
                         bool alwaysSign,   ///< если установлено в true, знак выводится всегда
                         char buffer[20]    ///< сюда записывается возвращаемое значение
    );
    /// \brief Переводит число с плавающей точкой в текстовый вид. numDigits - число цифровых знакомест
    /// \attention Строка будет храниться до следующего вызова функции. Если результат нужен большее количество времени, то его нужно скопироавать себе.
    /// \retval указатель на строку с числом.
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);
    /// @brief Преобразует time секунд в текстовую строку
    char* Time2String(float time,           ///< время в секундах. Может быть отрицательным
                      bool alwaysSign,      ///< если true, знак показывается даже если time > 0
                      char buffer[20]       ///< сюда записывается возвращаемое значение
    );
    /// Преобразует freq герц в текстовую строку.
    char* Freq2String(float freq, bool, char bufferOut[20]);

    char* FloatFract2String(float value, bool alwaysSign, char bufferOut[20]);
    /// Преобразует градусы в строку.
    char* Phase2String(float phase, bool, char bufferOut[20]);
    /// Преобразует freq герц в текстовую строку. При этом число количество цифр в строке равно numDigits
    char* Freq2StringAccuracy(float freq, char bufferOut[20], int numDigits);
private:
    static int NumDigitsInIntPart(float value);
};

extern StringUtils strUtils;

typedef char *(StringUtils::*pFuncPCFBPC)(float, bool, char*);
