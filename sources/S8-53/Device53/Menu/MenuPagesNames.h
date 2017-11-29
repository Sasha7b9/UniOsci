#pragma once

/// ����� ID ���� �������������� � ���� �������
typedef enum
{
    Page_Display,                   ///< ������
    Page_DisplayAccumulation,       ///< ������� - ����������
    Page_DisplayAverage,            ///< ������� - ����������
    Page_DisplayGrid,               ///< ������� - �����
    Page_DisplayRefresh,            ///< ������� - ����������
    Page_Main,                      ///< ������� �������� ����. �� �� ������ ��� ���������
    Page_Debug,                     ///< �������
    Page_DebugConsole,              ///< ������� - �������
    Page_DebugADC,                  ///< ������� - ���
    Page_DebugADCbalance,           ///< ������� - ��� - ������
    Page_DebugADCstretch,           ///< ������� - ��� - ��������
    Page_DebugADCrShift,            ///< ������� - ��� - ���. ��������
    Page_DebugADCaltShift,          ///< ������� - ��� - ���. ����. ���.
    Page_DebugShowRegisters,        ///< ������� - ��������
    Page_DebugRandomizer,           ///< ������� - ����-���
    Page_DebugInformation,          ///< ������� - ����������
    Page_DebugSound,                ///< ������� - ����
    Page_DebugAltera,               ///< ������� - �������
    Page_ChannelA,                  ///< ����� 1
    Page_ChannelB,                  ///< ����� 2
    Page_Cursors,                   ///< �������
    Page_Trig,                      ///< �����.
    Page_Memory,                    ///< ������
    Page_MemoryExt,                 ///< ������ - �����. ��
    Page_Measures,                  ///< ���������
    Page_MeasuresField,             ///< ��������� - ����
    Page_Service,                   ///< ������
    Page_ServiceDisplay,            ///< ������ - �������
    Page_ServiceDisplayColors,      ///< ������ - ������� - �����
    Page_ServiceCalibrator,         ///< ������ - ����������
    Page_ServiceEthernet,           ///< ������ - ETHERNET
    Page_Help,                      ///< ������
    Page_Time,                      ///< ���������
    Page_Math,                      ///< ������ - ����������
    Page_MathFFT,
    Page_TrigAuto,                  ///< ����� - ��� �����
    Page_SB_Curs,                   ///< ������� �������� ������� - ���������
    PageSB_Measures_Tune,           ///< ������� �������� ��������� - ���������
    Page_SB_MeasTuneField,          ///< ������� �������� ��������� - ���� - ������
    PageSB_Memory_Last,             ///< ������� �������� ������ - ���������
    PageSB_Memory_Internal,         ///< ������� �������� ������ - ����� ��
    Page_SB_MemExtSetMask,          ///< ������� �������� ������ - ����� �� - �����
    Page_SB_MemExtSetName,          ///< ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ��������������� ����� ����� �� - ��� �����
    PageSB_Memory_Drive_Manager,    ///< ������� �������� ������ - ����� �� - ��TA���
    Page_SB_Help,                   ///< ������� �������� HELP
    PageSB_Service_FFT_Cursors,     ///< ������� �������� ������ - ���������� - �������
    PageSB_Service_Function,        ///< ������ - ���������� - �������
    Page_SB_Information,            ///< ������ - ����������
    PageSB_Debug_SerialNumber,      ///< ������ - �/�
    Page_NumPages,
    Page_NoPage
} NamePage;             // ��� ���������� ������� ����� �������� ������������� SettingsMenu � SSettings.c
