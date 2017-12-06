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
    static void Init(void);
    /// ���� ������� �� ������.
    static void ButtonPress(void);
    /// ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    static void ButtonRelease(void);

    static void GovernorChangedValue(void);
    
    static void RegulatorShiftRotate(void);

    static void RegulatorSwitchRotate(void);

    static void WarnBeepBad(void);

    static void WarnBeepGood(void);
    /// ��� ������� ���� �������� ����� �������/��������� ����. ���� ����������� � ����.
    static void WaitCompletion(void);
};


/** @}  @}
 */
