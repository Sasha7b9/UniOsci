#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringUtils
{
public:
    /// @brief ����������� voltage ����� � ��������� ������
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ������������ ����
    char *Voltage2String(float voltage,     ///< �������� ���������� � �������
                         bool alwaysSign,   ///< ���� ����������� � true, ���� ��������� ������
                         char buffer[20]    ///< ���� ������������ ������������ ��������
    );
    /// \brief ��������� ����� � ��������� ������ � ��������� ���. numDigits - ����� �������� ���������
    /// \attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ������������ ����.
    /// \retval ��������� �� ������ � ������.
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);
    /// @brief ����������� time ������ � ��������� ������
    char* Time2String(float time,           ///< ����� � ��������. ����� ���� �������������
                      bool alwaysSign,      ///< ���� true, ���� ������������ ���� ���� time > 0
                      char buffer[20]       ///< ���� ������������ ������������ ��������
    );
    /// ����������� freq ���� � ��������� ������.
    char* Freq2String(float freq, bool, char bufferOut[20]);

    char* FloatFract2String(float value, bool alwaysSign, char bufferOut[20]);
    /// ����������� ������� � ������.
    char* Phase2String(float phase, bool, char bufferOut[20]);
    /// ����������� freq ���� � ��������� ������. ��� ���� ����� ���������� ���� � ������ ����� numDigits
    char* Freq2StringAccuracy(float freq, char bufferOut[20], int numDigits);
    /// @brief ����������� value � ��������� ������. ��� ���� ��������� ��� 8 ��������
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    char* Bin2String(uint8 value, char bufferOut[9]);
    /// @brief ����������� value � ��������� ������
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    char* Bin2String16(uint16 value, char bufferOut[19]);
private:
    static int NumDigitsInIntPart(float value);
};

extern StringUtils strUtils;

typedef char *(StringUtils::*pFuncPCFBPC)(float, bool, char*);
