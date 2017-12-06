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
    static void Init(void);
    /// Звук нажатия на кнопку.
    static void ButtonPress(void);
    /// Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    static void ButtonRelease(void);

    static void GovernorChangedValue(void);
    
    static void RegulatorShiftRotate(void);

    static void RegulatorSwitchRotate(void);

    static void WarnBeepBad(void);

    static void WarnBeepGood(void);
    /// Эту функцию надо вызывать перед записью/стиранием ППЗУ. Звук конфликтует с ППЗУ.
    static void WaitCompletion(void);
};


/** @}  @}
 */
