#pragma once
#include "defines.h"
#include "FPGATypes.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RShiftMin       20
#define RShiftZero      500
#define RShiftMax       980
#define TrigLevZero     500
#define TrigLevMax      980
#define TrigLevMin      20
#define TShiftZero      500
#define TShiftMin       0
#define TShiftMax       60000

/// Ќа столько единиц нужно изменить значение смещени€, чтобы маркер смещени€ по напр€жению передвинулс€ на одну точку.
#define STEP_RSHIFT     (((RShiftMax - RShiftMin) / 24) / 20)
#define STEP_TRIGLEV    STEP_RSHIFT

enum Pin
{
    SPI3_SCK,
    SPI3_DAT,
    SPI3_CS1,
    SPI3_CS2,
    A1,
    A2,
    A3,
    A4,
    LF1,
    LF2,
    A1S,
    A0S,
    LFS,
    Num_Pins
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HardwareFPGA
{
    friend class FPGA;
private:
};

class FPGA
{
public:

    void Init();

    void Update();

    void Start();

    void OnPressStart();

    void GiveStart();

    void LoadSettings();

    void IncreaseRange(Channel ch);

    void DecreaseRange(Channel ch);

    void IncreaseTBase();

    void DecreaseTBase();

    void RShiftChange(Channel ch, int delta);

    void TrigLevChange(int delta);

    void TShiftChange(int delta);

    void SetRShift(Channel ch, uint16 rShift);

    void LoadTrigPolarity();

    void LoadTrigMode();

    void LoadTrigSource();

    void LoadTrigInput();

    void Stop();

    void LoadRanges();
    /// «апустить цикл чтени€ дл€ тестер-компонента. ¬ течение time секунд должно быть считано numPoints точек
    void StartForTester(int numPoints);

    void ReadForTester(uint8 *dataA, uint8 *dataB);

private:

    void GPIO_Init();

    void LoadTBase();

    void LoadTrigLev();

    void LoadRShift(Channel ch);

    void LoadTShift();

    void WritePin(Pin pin, int enable);

    void SetPin(Pin pin);

    void ResetPin(Pin pin);

    uint GetPin(Pin pin);

    GPIO_TypeDef *GetPort(Pin pin);

    void WriteRegisters(Pin cs, uint16 value);

    void ReadData();

    uint8 ReadFlag();

    uint16 ReadLastRecord();

    void ReadDataChanenl(Channel ch, uint8 data[FPGA_MAX_NUM_POINTS]);
    /// ”становить в соотвествующие положени€ выводы, отвечающие за источник и вход синхронизации
    void LoadTrigSourceInput();
    /// „итать канал в рандомизаторе с адреса address
    void ReadDataChanenlRand(Channel ch, uint8 *address, uint8 *data);

    bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);

    int CalculateShift();

    HardwareFPGA hardware;

    bool isRunning;
    /// True, если дан запуск
    bool givingStart;
};


extern FPGA fpga;
