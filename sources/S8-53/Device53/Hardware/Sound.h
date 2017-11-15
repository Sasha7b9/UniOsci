

#pragma once
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    TypeWave_Sine,
    TypeWave_Meandr,
    TypeWave_Triangle
} TypeWave;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Sound
{
public:

    void Init();

    void ButtonPress();
    /// ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    void ButtonRelease();

    void GovernorChangedValue();

    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();
};


extern Sound sound;
