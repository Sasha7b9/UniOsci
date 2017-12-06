#pragma once 
#include "defines.h"
#include "Panel/Controls.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Panel 
 *  @brief ������ � ������� ����������
 *  @{
 */

class Panel
{
public:
    /// ������������� ����������� �����������
    static void Init(void);
    /// ������� ����� �������� ������ ���� ��� ��������� ���������� � ������ ����� ������� ����������
    static void Update(void);
    /// ��������/��������� �������� ������ �� �������
    static void EnableLEDChannel(Channel ch, bool enable);
    /// ��������/��������� �������� ���������
    static void EnableLEDRegSet(bool enable);
    /// ��������/��������� �������� �����
    static void EnableLEDTrig(bool enable);
    /// �������� ������ � �� ����������
    static void TransmitData(uint16 data);
    /// �������� ������ ����������
    /** ����� ������ ���������, �� ���������� ���� ���������� ��������� ���������� releasedButton */
    static void Disable(void);
    /// �������� ������ ����������
    static void Enable(void);
    /// ���������� ���������� �������
    static bool ProcessingCommandFromPIC(uint16 command);
    /// ���������� ��������� ������, �������������� ��� �������� � ������, �� ������
    static uint16 NextData(void);
    /// �������� ������� �������
    static PanelButton WaitPressingButton(void);
    /// ��������� ������, ���� ���� ������ ������ �������
    static void DisableIfNessessary(void);
};


/**  @}
 */
