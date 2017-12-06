#pragma once
#include "Display/DisplayTypes.h"
#include "Settings/SettingsTypes.h"
#include "Tables.h"
#include "Display/Painter.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// C��������, ��������� �� �����
typedef enum
{
    LimitChan1_Volts,               ///< ������ �������� �� ����������� ������� ������.
    LimitChan2_Volts,               ///< ������ �������� �� ���������� ������� ������.
    LimitSweep_Time,                ///< ������ �������� �� �������.
    EnabledPeakDet,                 ///< ������� ������� ��������.
    LimitChan1_RShift,              ///< ������ �������� �� ���������� ������� ������.
    LimitChan2_RShift,              ///< ������ �������� �� ���������� ������� ������.
    LimitSweep_Level,               ///< ������ ������ �������������.
    LimitSweep_TShift,              ///< ������ �������� �� �������.
    TooSmallSweepForPeakDet,        ///< ������� ������� �������� ��� �������� ���������.
    TooFastScanForRecorder,         ///< ������� ������� �������� ��� ����������� ������.
    FileIsSaved,                    ///< ���� ������� �� ������.
    SignalIsSaved,                  ///< ������ ������� � ���.
    SignalIsDeleted,                ///< �� ���� ����� ������.
    MenuDebugEnabled,               ///< �������� ���� �������.
    TimeNotSet,                     ///< ����� �� �����������.
    SignalNotFound,                 ///< ������ �� ������ (���������).
    SetTPosToLeft,                  ///< ������������.
    NeedRebootDevice,               ///< ��������� ������������.
    ImpossibleEnableMathFunction,   ///< ���������� �������� �������������� �������.
    ImpossibleEnableFFT,            ///< ���������� �������� ���������� ���.
    WrongFileSystem,                ///< ���������� ���������� ������.
    WrongModePeackDet,              ///< ���������� �������� ������� ��������.
    DisableChannelB,                ///< ��������� ��������� ����� 2.
    TooLongMemory,                  ///< ��������� � ������������� ��������� ������ 32�.
    NoPeakDet32k,                   ///< ������� �������� �� �������� ��� ����� ������ 32�.
    NoPeakDet16k,                   ///< ������� �������� �� �������� ��� ����� ������ 16�.
    Warn50Ohms,                     ///< �������������� �� ����������� �������� ������ �� 50 ����.
    WarnNeedForFlashDrive,          ///< ��������� ������������ ������.
    FirmwareSaved,                  ///< �������� ��������� �� ������.
    FullyCompletedOTP,              ///< 
    Warning_Count
} Warning;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Display
 *  @brief ������ � ��������
 *  @details ��� ���������� ������ ������ ������� ����� ������� �� DS_GetData(0), � ��������� ������ ������ �� DS_GetData(1).
 *  @{
 */

class Display
{
public:
    /// �������������
    static void Init(void);
    /// ��� ������� ����� �������� ������ ���� - ���������
    static void Update(void);
    /// ��� ������� ����� �������� ��� �������� ����� �������� �� ��������� ��� ��������� ��������� ��������������� ��������
    static void RotateRShift(Channel ch);
    /// ��� ������� ����� �������� ��� �������� ����� ������ ������������� ��� ��������� ��������� ��������������� ��������
    static void RotateTrigLev(void);

    static void EnableTrigLabel(bool enable);

    static void SetAddDrawFunction(pFuncVV func);

    static pFuncVV GetAddDrawFunction(void);

    static void RemoveAddDrawFunction(void);

    static void Clear(void);

    static void ShiftScreen(int delta);

    static void ChangedRShiftMarkers(bool active);

    static void AddStringToIndicating(const char *string);

    static void OneStringUp(void);

    static void OneStringDown(void);

    static void SetPauseForConsole(bool pause);

    static void SetOrientation(DisplayOrientation orientation);
    /// ������� ��������� ���������� �������
    static void DrawConsole(void);
    /// @brief ���������� ������� � ����� ��������� ������.
    /// @details �������� ��� ��������.
    /// 1. DrawMode_Hand - � ���� ������ ����� ���������� ������� func(), ������������ �������������, � �������� 25 ��� � �������.
    /// ������������ � �������, ����� �� ����������� ������� ����.
    /// 2. DrawMode_Auto � func == 0 - � ���� ������ ����� ����������� ������� Update() � ������� �����.
    /// 3. DrawMode_Auto � func != 0 - � ���� ������ ����� ����������� ������� func(), ������������ �������������, �� � ����
    /// �������� �����, ������ ���������� �� Update() ������ Update().
    static void SetDrawMode(DrawMode mode, pFuncVV func);

    static void ClearFromWarnings(void);

    static void ShowWarning(Warning warning);

    // ������ ������� ��� ������ ��������� � ������� �������
    static void FuncOnWaitStart(const char *text, bool eraseBackground);

    static void FuncOnWaitStop(void);
};


/** @}
 */
