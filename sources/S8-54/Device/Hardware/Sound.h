#pragma once
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Sound
 *  @brief ������ �� �������� ���������
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
    /// ���� ������� �� ������.
    void ButtonPress(void);
    /// ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    void ButtonRelease(void);

    void GovernorChangedValue(void);
    
    void RegulatorShiftRotate(void);

    void RegulatorSwitchRotate(void);

    void WarnBeepBad(void);

    void WarnBeepGood(void);
    /// ��� ������� ���� �������� ����� �������/��������� ����. ���� ����������� � ����.
    void WaitCompletion(void);
};


extern Sound sound;

/** @}  @}
 */
