#pragma once
#include "defines.h"
#include "FPGA_Types.h"
#include "Panel/Controls.h"
#include "DataStorage.h"
#include "Settings/SettingsTrig.h"
#include "Settings/SettingsService.h"


class FPGA
{
public:

    void Init();
    /// ���������� ���������� ����������� �������� � �������.
    void SetNumSignalsInSec(int numSigInSec);

    void Update();
    /// ������ � ������� ���� ������ ��������.
    void WriteToHardware				        
                            (uint8 *address,    ///< ����� ��������.
                                uint8 value,    ///< ������������ ��������.
                                bool restart    ///< true ��������, ��� ����� ������ ����� �������� ����� ���������, ���� �� ����� ������ �� ��������� � ������ �����.
                            );

    StateWorkFPGA FPGA_CurrentStateWork();
    /// ��������� ���� ����� ����������.
    void OnPressStartStop();
    /// ������ �������� ����� ����������.
    void Start();
    /// ��������� ������� ����� ����������.
    void Stop(bool pause);
    /// ���������� true, ���� ������ ��������� �� � �������� ����� ����������.
    bool IsRunning();
    /// ��������� ������� ���������. ����� �� ����� ������������ �������� FPGA_RestoreState().
    void SaveState();
    /// ��������������� ���������, ����� ���������� �������� FPGA_SaveState().
    void RestoreState();
    /// �������� �������� �������.
    float GetFreq();
    /// ������� ������. ����� ��� ������ �������������, ��� ���������� ������� ����� �� �������� ���������������.
    void ClearData();
    /// ���������� �������������� ��������. ����� ��� ���������� ����������� ����� � ������ �������������.
    void SetAdditionShift(int shift);
    /// ���������� true,���� ��� ����� �������� � ������ �������������.
    bool AllPointsRandomizer();
    /// ���������� ���������� ���������, �� ������� ����� �������������� ������ � ������ �������������.
    void SetNumberMeasuresForGates(int number);
    /// ������������� ��������� �������������.
    void SwitchingTrig();
    /// ��������� ������� ������ �������.
    void StartAutoFind();
    /// ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
    void TemporaryPause();

    void FillDataPointer(DataSettings *dp);
    /// ����� � ���������� ������� ������������� �� ���������� ���������� �������
    void FindAndSetTrigLevel();
    /// ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
    void LoadSettings();
    /// ���������� ����� ������ �� �����.
    void SetModeCouple(Channel chan, ModeCouple modeCoupe);
    /// ��������/��������� ������ �� ����� ������.
    void EnableChannelFiltr(Channel chan, bool enable);
    /// ���������� ������� �� ����������.
    void SetRange(Channel chan, Range range);
    /// ��������� ������� �� ����������.
    bool RangeIncrease(Channel chan);
    /// ��������� ������� �� ����������.
    bool RangeDecrease(Channel chan);
    /// ���������� ������� �� �������.
    void SetTBase(TBase tBase);
    /// ��������� ������� �� �������.
    void TBaseDecrease();
    /// ��������� ������� �� �������.
    void TBaseIncrease();
    /// ���������� ������������� �������� �� ����������.
    void SetRShift(Channel chan, int16 rShift);
    /// ���������� ������������� �������� �� �������.
    void SetTShift(int tShift);
    /// ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ���� ������.
    void SetDeltaTShift(int16 shift);
    /// ��������/��������� ����� �������� ���������.
    void SetPeackDetMode(PeackDetMode peackDetMode);
    /// ��������/��������� ����������.
    void SetCalibratorMode(CalibratorMode calibratorMode);
    /// ��������� � ������� ������������ ���������� �������.
    void LoadKoeffCalibration(Channel chan);
    /// ���������� ������������� ������� �������������.
    void SetTrigLev(TrigSource chan, int16 trigLev);
    /// ���������� �������� �������������.
    void SetTrigSource(TrigSource trigSource);
    /// ���������� ���������� �������������.
    void SetTrigPolarity(TrigPolarity polarity);
    /// ���������� ����� ����� �������������.
    void SetTrigInput(TrigInput trigInput);
    /// ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����.
    const char* GetTShiftString(int16 tShiftRel, char buffer[20]);
    /// ������ ������� ����������.
    void ProcedureCalibration();

    StateWorkFPGA CurrentStateWork();

private:
    /// �������� ����������� �������� � ���������� �����.
    void LoadTBase();
    /// �������� �������� �� ������� � ���������� �����.
    void LoadTShift();
    /// �������� �������� �� ���������� � ���������� �����.
    void LoadRange(Channel chan);
    /// �������� �������� �� ���������� � ���������� �����.
    void LoadRShift(Channel chan);
    /// �������� ������ ������������� � ���������� �����.
    void LoadTrigLev();                                  
    /// ������� ���������� ������������� � ���������� �����.
    void LoadTrigPolarity();
    /// ��������� ��� ��������� ���������� ������� � ������������� � ���������� �����.
    void SetAttribChannelsAndTrig(TypeWriteAnalog type);
    /// ��������� ������� WR_UPR (������� �������� � ����������).
    void LoadRegUPR();

    void WriteToAnalog(TypeWriteAnalog type, uint data);

    void WriteToDAC(TypeWriteDAC type, uint16 data);
    /// ��������� ������.
    void DataRead(
                        bool necessaryShift,    ///< ������� ����, ��� ������ ����� �������.
                        bool saveToStorage      ///< ����� � ������ ������������� ��� ��������, ��� ���� ��������� ���������
                        );

    bool CalculateGate(uint16 rand, uint16 *min, uint16 *max);

    int CalculateShift();
    /// ����������� ������.
    void InverseDataIsNecessary(Channel chan, uint8 *data);

    void AutoFind();

    uint8 CalculateMin(uint8 buffer[100]);

    uint8 CalculateMax(uint8 buffer[100]);

    bool ProcessingData();

    void ReadRandomizeMode();

    void ReadRealMode(bool necessaryShift);

    Range FindRange(Channel chan);

    Range AccurateFindRange(Channel chan);

    bool FindParams(Channel chan, TBase *tBase);

    bool AccurateFindParams(Channel chan);

    bool FindWave(Channel chan);
};


extern FPGA fpga;
