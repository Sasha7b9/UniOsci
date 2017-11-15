#pragma once
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Settings/SettingsTypes.h"
#include "Hardware/RTC.h"


class DataStorage
{
public:
    /// �������� ���� ���������� ���������
    void Clear();
    /// �������� ��������� ������. ��� ���� ����������� ��������� ������� ��������� �������.
    void AddData(uint8 *data0, uint8 *data1, DataSettings dss);
    /// ���������� ����� ����������� ���������, ������� � ����������, � ������ �� �����������, ��� � ����������.
    int NumElementsWithSameSettings();
    /// ���������� ����� ����������� ���������, ������� � ����������, � �������� ����������� �������
    int NumElementsWithCurrentSettings();
    /// ���������� ���������� ���������� ���������
    int NumElementsInStorage();
    /// �������� ��������� �� ������
    bool GetDataFromEnd(int fromEnd, DataSettings **ds, uint8 **data0, uint8 **data1);

    uint8* GetData(Channel chan, int fromEnd);
    /// �������� ���������� ������ �� ���������� ����������.
    uint8* GetAverageData(Channel chan);
    /// ������� ����� ��������� ��������� � ������.
    int AllDatas();
    /// �������� �������������� ����� ������� 0 - �����, 1 - ������.
    uint8* GetLimitation(Channel chan, int direction);                                      

    int NumberAvailableEntries();
private:

    void CalculateSums(void);
    /// ���������� ���������� ��������� ������ � ������
    int MemoryFree();
    /// ���������, ������� ������ ���������, ����� ��������� ��������� � ����������� dp
    int SizeElem(DataSettings *dp);
    /// ������� ������ (����� ������) ���������
    void RemoveFirstElement();
    /// ��������� ������
    void PushData(DataSettings *dp, uint8 *data0, uint8 *data1);
    /// ���������� ��������� �� ���������, ��������� �� elem
    DataSettings* NextElem(DataSettings *elem);
    /// ���������� ��������� �� ������, ��������� �� indexFromEnd o� ���������� �����������
    DataSettings* FromEnd(int indexFromEnd);
    /// ���������� true, ���� ��������� ��������� � ��������� elemFromEnd0 � elemFromEnd1 ���������, � false � ���� ������.
    bool SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1);
    /// ��������� true, ���� ��������� � ����� ���������� ���������
    bool SettingsIsEquals(DataSettings *dp0, DataSettings *dp1);
    /// ������� �������� ���, ���� � ����
    void ClearLimitsAndSums();

    void CalculateLimits(uint8 *data0, uint8 *data1, DataSettings *dss);

    DataSettings* GetSettingsDataFromEnd(int fromEnd);
    /// �������� ������ ������ chan ��, ������������ ds, � ���� �� ���� ����� ������� dataImportRel. ������������ �������� false ��������, ��� ������ ����� ��������.
    bool CopyData(DataSettings *ds, Channel chan, uint8 datatImportRel[NumChannels][FPGA_MAX_POINTS]);

    void PrintElement(DataSettings *dp);
};

extern DataStorage dataStorage;
