#pragma once


class VCP
{
public:
    void Init();
    void SendDataAsinch(uint8 *buffer, int size);
    void SendDataSynch(const uint8 *data, int size);
    void SendStringSynch(char *data);                   ///< ��������� ������ ��� ������������ ����.
    void SendFormatStringAsynch(char *format, ...);     ///< ��� ������ ��������� � ������������ ��������� \r\n.
    void SendFormatStringSynch(char *format, ...);      ///< ��� ������ ��������� � ������������ ��������� \r\n.
    void SendByte(uint8 data);
    void Flush();
    void SendStringAsinch(char *data);
};

extern VCP vcp;
