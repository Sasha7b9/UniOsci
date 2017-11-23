#pragma once 
#include "defines.h"


class Panel
{
public:

    void Init();

    void EnableLEDRegSet(bool enable);
    /// ��������/��������� ��������� ����� 1.
    void EnableLEDChannelA(bool enable);
    /// ��������/��������� ��������� ����� 2.
    void EnableLEDChannelB(bool enable);
    /// ��������/��������� ��������� �����.
    void EnableLEDTrig(bool enable);
    /// �������� ������� � �� ������ ����������.
    void TransmitData(uint16 data);
    /// � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
    void Disable();

    void Enable();
    /// ������� ������� �������.
    PanelButton WaitPressingButton();

    bool ProcessingCommandFromPIC(uint16 command);

    uint16 NextData();
};


extern Panel panel;
