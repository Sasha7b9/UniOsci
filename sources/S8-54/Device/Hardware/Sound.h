#pragma once
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Sound
 *  @brief Работа со звуковым динамиком
 *  @{
 */


typedef enum
{
    TypeWave_Sine,
    TypeWave_Meandr,
    TypeWave_Triangle
} TypeWave;


class Sound
{
public:
    void Init(void);
    /// Звук нажатия на кнопку.
    void ButtonPress(void);
    /// Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    void ButtonRelease(void);

    void GovernorChangedValue(void);
    
    void RegulatorShiftRotate(void);

    void RegulatorSwitchRotate(void);

    void WarnBeepBad(void);

    void WarnBeepGood(void);
    /// Эту функцию надо вызывать перед записью/стиранием ППЗУ. Звук конфликтует с ППЗУ.
    void WaitCompletion(void);
};


extern Sound sound;

/** @}  @}
 */
