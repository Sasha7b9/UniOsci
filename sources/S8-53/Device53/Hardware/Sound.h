

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

#define Sound_RegulatorSwitchRotate() sound.RegulatorSwitchRotate()

class Sound
{
public:

    void Init();

    void ButtonPress();
    /// Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    void ButtonRelease();

    void GovernorChangedValue();

    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();
};


extern Sound sound;
