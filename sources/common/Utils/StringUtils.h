#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringUtils
{
public:
    /// ��������� �������� voltage � �������� ��� � ������� ��� ������ �� ����� ����
    char *Voltage2String(float voltage, bool alwaysSign, char buffer[20]);
    /// ��������� ����� � ��������� ������ � ��������� ���. numDigits - ����� �������� ���������
    char *Float2String(float value, bool alwaysSign, int numDigits, char bufferOut[20]);

private:
    static int NumDigitsInIntPart(float value);
};

extern StringUtils strUtils;
