#include "defines.h"
#include "defines.h"
#include "Sound.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Settings/Settings.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void TIM7_Config(uint16 prescaler, uint16 period);
static uint16 CalculatePeriodForTIM();
static void SetWave();
static void CalculateSine();
static void CalculateMeandr();
static void CalculateTriangle();


static const int POINTS_IN_PERIOD = 10;
static uint8 points[POINTS_IN_PERIOD] = {0};
static float frequency = 0.0f;
static float amplitude = 0.0f;
static TypeWave typeWave = TypeWave_Sine;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sound::Init()
{
    return;
    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T7_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handleDAC);

    HAL_DAC_Init(&handleDAC);

    HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Stop()
{
    return;
    HAL_DAC_Stop_DMA(&handleDAC, DAC_CHANNEL_1);
    gBF.soundIsBeep = 0;
    gBF.soundWarnIsBeep = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Beep(TypeWave typeWave_, float frequency_, float amplitude_, int duration)
{
    return;
    if (gBF.soundWarnIsBeep == 1)
    {
        return;
    }
    if (!SOUND_ENABLED)
    {
        return;
    }
    if (frequency != frequency_ || amplitude != amplitude_ || typeWave != typeWave_)
    {
        frequency = frequency_;
        amplitude = amplitude_;
        typeWave = typeWave_;
        
        Stop();
        SetWave();
    }

    Stop();
    
    gBF.soundIsBeep = 1;
    HAL_DAC_Start_DMA(&handleDAC, DAC_CHANNEL_1, (uint32_t*)points, POINTS_IN_PERIOD, DAC_ALIGN_8B_R);

    Timer::SetAndStartOnce(kStopSound, Stop, duration);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::ButtonPress()
{
    return;
    Beep(TypeWave_Sine, 2000.0f, 0.5f, 50);
    gBF.buttonIsPressed = 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::ButtonRelease()
{
    return;
    if (gBF.buttonIsPressed == 1)
    {
        Beep(TypeWave_Sine, 1000.0f, 0.25f, 50);
        gBF.buttonIsPressed = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::GovernorChangedValue()
{
    return;
    Beep(TypeWave_Sine, 1000.0f, 0.5f, 50);
    gBF.buttonIsPressed = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::RegulatorShiftRotate()
{
    return;
    Beep(TypeWave_Sine, 1.0f, 0.35f, 3);
    gBF.buttonIsPressed = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::RegulatorSwitchRotate()
{
    return;
    Beep(TypeWave_Triangle, 2500.0f, 0.5f, 25);
    gBF.buttonIsPressed = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WarnBeepBad()
{
    return;
    Beep(TypeWave_Meandr, 250.0f, 1.0f, 500);
    gBF.soundWarnIsBeep = 1;
    gBF.buttonIsPressed = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WarnBeepGood()
{
    return;
    Beep(TypeWave_Triangle, 1000.0f, 0.5f, 250);
    gBF.buttonIsPressed = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SetWave()
{
    TIM7_Config(0, CalculatePeriodForTIM());

    if(typeWave == TypeWave_Sine)
    {
        CalculateSine();
    }
    else if(typeWave == TypeWave_Meandr)
    {
        CalculateMeandr();
    }
    else if(typeWave == TypeWave_Triangle)
    {
        CalculateTriangle();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TIM7_Config(uint16 prescaler, uint16 period)
{
    static TIM_HandleTypeDef htim =
    {
        TIM7,
        {
            0,
            TIM_COUNTERMODE_UP,
        }
    };

    htim.Init.Prescaler = prescaler;
    htim.Init.Period = period;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&htim);

    TIM_MasterConfigTypeDef masterConfig =
    {
        TIM_TRGO_UPDATE,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);

    HAL_TIM_Base_Stop(&htim);
    HAL_TIM_Base_Start(&htim);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 CalculatePeriodForTIM()
{
    return (uint16)(120e6f / frequency / POINTS_IN_PERIOD);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateSine()
{
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        float step = 2.0f * 3.1415926f / (POINTS_IN_PERIOD - 1);
        float value = (sinf(i * step) + 1.0f) / 2.0f;
        points[i] = (uint8)(value * amplitude * 255);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateMeandr()
{
    for (int i = 0; i < POINTS_IN_PERIOD / 2; i++)
    {
        points[i] = (uint8)(255 * amplitude);
    }
    for (int i = POINTS_IN_PERIOD / 2; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateTriangle()
{
    float k = 255.0 / POINTS_IN_PERIOD;
    for (int i = 0; i < POINTS_IN_PERIOD; i++)
    {
        points[i] = (uint8)(k * i * amplitude);
    }
}
