#pragma once
#include "Generator.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "FPGA/FPGAMath.h"
#include <stdlib.h>
#include <math.h>


static void SetParametersWave(Channel channel, TypeWave typeWave, float frequency, float startAngle, float amplWave, float amplNoise);
static void StartNewWave(Channel channel);
static uint8 GetSampleWave(Channel channel);


const SGenerator Generator = 
{
    SetParametersWave,
    StartNewWave,
    GetSampleWave,
};


static float NewNoiseValue(Channel channel);
static uint8 GetSampleSinusWave(Channel channel, int numSample);
static uint8 GetSampleMeanderWave(Channel channel, int numSample);


static TypeWave type[2] = {Wave_Sinus, Wave_Meander};
static float freq[2] = {1000.0f, 500.0f};
static float angle[2] = {0.05f, 0.1f};
static float ampl[2] = {1.0f, 0.5f};
static float amplNoise[2] = {0.1f, 0.1f};
static int numSample[2] = {0, 0};

void SetParametersWave(Channel channel, TypeWave typeWave, float frequency, float startAngle, float amplWave, float amplNoise_)
{
    type[channel] = typeWave;
    freq[channel] = frequency;
    angle[channel] = startAngle;
    ampl[channel] = amplWave;
    amplNoise[channel] = amplNoise_;
}

void StartNewWave(Channel channel)
{
    numSample[0] = numSample[1] = 0;
}

uint8 GetSampleWave(Channel channel)
{
    return (type[channel] == Wave_Sinus) ? GetSampleSinusWave(channel, (numSample[channel])++) : GetSampleMeanderWave(channel, (numSample[channel])++);
}

uint8 GetSampleSinusWave(Channel channel, int numSample)
{
    float dT = numSample * TSHIFT_2_ABS(1, SET_TBASE);
    float voltage = ampl[channel] * sinf(2 * PI * freq[channel] * dT + angle[channel]) + NewNoiseValue(channel);
    return mathFPGA.Voltage2Point(voltage, SET_RANGE(channel), SET_RSHIFT(channel));
}

uint8 GetSampleMeanderWave(Channel channel, int numSample)
{
    return 0;
}

float NewNoiseValue(Channel channel)
{
    static float prevNoise[2] = {0.0f, 0.0f};            // ����� �������� �������� ���� �� ���������� �����, ����������� ��� ������� ���� � ������� �����.

    float noise = prevNoise[channel];

    float halfAmplNoiseAbs = ampl[channel] * amplNoise[channel] / 2.0f;

    float deltaRand = halfAmplNoiseAbs;

    noise += math.RandFloat(-deltaRand, deltaRand);

    while (noise < -halfAmplNoiseAbs)
    {
        noise += math.RandFloat(0, deltaRand * 2);
    }

    while (noise > halfAmplNoiseAbs)
    {
        noise -= math.RandFloat(0, deltaRand * 2);
    }

    prevNoise[channel] = noise;

    return noise;
}
