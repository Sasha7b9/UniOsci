#include "FPGA.h" 
#include "FPGA_Types.h"
#include "Display/Display.h"
#include "Display/DisplayPrimitives.h"
#include "Display/Colors.h"
#include "Display/Painter.h"
#include "Panel/Panel.h"
#include "Hardware/FSMC.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include <stdio.h>
#include <limits.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int16    CalculateAdditionRShift(Channel chan, Range range);	///< �������� ���������� �������� ������ �� ����������.
static float    CalculateKoeffCalibration(Channel chan);			///< �������� ����������� ���������� ������ �� ����������.
static void     AlignmentADC();
static void     FuncAttScreen();								///< ������� ���������� ������ � ������ ����������.
static float    CalculateDeltaADC(Channel chan, float *avgADC1, float *avgADC2, float *delta);
static void     DrawParametersChannel(Channel chan, int x, int y, bool inProgress);

static float deltaADC[2] = {0.0f, 0.0f};
static float deltaADCPercents[2] = {0.0f, 0.0f};
static float avrADC1[2] = {0.0f, 0.0f};
static float avrADC2[2] = {0.0f, 0.0f};

static float deltaADCold[2] = {0.0f, 0.0f};
static float deltaADCPercentsOld[2] = {0.0f, 0.0f};
static float avrADC1old[2] = {0.0f, 0.0f};
static float avrADC2old[2] = {0.0f, 0.0f};

static int8 shiftADC0 = 0;
static int8 shiftADC1 = 0;

static float koeffCal0 = -1.0f;
static float koeffCal1 = -1.0f;

static ProgressBar bar0;                            // ��������-��� ��� ���������� ������� ������.
static ProgressBar bar1;                            // ��������-��� ��� ���������� ������� ������.

static uint startTimeChan0 = 0;                     // ����� ������ ���������� ������� ������.
static uint startTimeChan1 = 0;                     // ����� ������ ���������� ������� ������.

static float koeffCalibrationOld[2];

extern void LoadStretchADC(Channel chan);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnTimerDraw(void)
{
    display.Update();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ProcedureCalibration(void)
{
    bool chanAenable = SET_ENABLED_A;
    bool chanBenable = SET_ENABLED_B;

    SET_ENABLED_A = SET_ENABLED_B = true;

    display.SetDrawMode(DrawMode_Hand, FuncAttScreen);
    Timer_Enable(kTimerDrawHandFunction, 100, OnTimerDraw);

    koeffCalibrationOld[A] = STRETCH_ADC_A;
    koeffCalibrationOld[B] = STRETCH_ADC_B;

    bar0.fullTime = bar0.passedTime = bar1.fullTime = bar1.passedTime = 0;

    fpga.SaveState();                               // ��������� ������� ���������.
    panel.Disable();                                // ��������� ������ ����������.

    while(1)
    {
        gStateFPGA.stateCalibration = StateCalibration_ADCinProgress;                  // ��������� ��������� ������������ ���.

        fpga.SetTBase(TBase_500us);
        fpga.SetTShift(0);
        STRETCH_ADC_A = 1.0f;
        STRETCH_ADC_B = 1.0f;
        fpga.LoadKoeffCalibration(A);
        fpga.LoadKoeffCalibration(B);
        fpga.SetRange(A, Range_500mV);
        fpga.SetRange(B, Range_500mV);
        fpga.SetRShift(A, RShiftZero);
        fpga.SetRShift(B, RShiftZero);
        fpga.SetModeCouple(A, ModeCouple_GND);
        fpga.SetModeCouple(B, ModeCouple_GND);
        FSMC_Write(WR_ADD_RSHIFT_DAC1, 0);
        FSMC_Write(WR_ADD_RSHIFT_DAC2, 0);

        deltaADCPercentsOld[0] = CalculateDeltaADC(A, &avrADC1old[A], &avrADC2old[A], &deltaADCold[A]);
        deltaADCPercentsOld[1] = CalculateDeltaADC(B, &avrADC1old[B], &avrADC2old[B], &deltaADCold[B]);

        AlignmentADC();

        deltaADCPercents[A] = CalculateDeltaADC(A, &avrADC1[A], &avrADC2[A], &deltaADC[A]);
        deltaADCPercents[B] = CalculateDeltaADC(B, &avrADC1[B], &avrADC2[B], &deltaADC[B]);

        gStateFPGA.stateCalibration = StateCalibration_RShift0start;                 

        koeffCal0 = koeffCal1 = ERROR_VALUE_FLOAT;

		if(panel.WaitPressingButton() == B_Start)             // ������� ������������� ��� ������ ��������� ���������� ������� ������.
        {
			gStateFPGA.stateCalibration = StateCalibration_RShift0inProgress;

			koeffCal0 = CalculateKoeffCalibration(A);
			if(koeffCal0 == ERROR_VALUE_FLOAT)
            {
				gStateFPGA.stateCalibration = StateCalibration_ErrorCalibration0;
				panel.WaitPressingButton();
                DEBUG_STRETCH_ADC_TYPE = StretchADC_Hand;
                LoadStretchADC(A);
            }
            else
            {
                STRETCH_ADC_A = koeffCal0;
                fpga.LoadKoeffCalibration(A);
            }
			
            for (int range = 0; range <= RangeSize; range++)
            {
                for (int mode = 0; mode < 2; mode++)
                {
                    if (!(mode == 0 && (range == Range_2mV || range == Range_5mV || range == Range_10mV)))
                    {
                        fpga.SetModeCouple(A, (ModeCouple)mode);
                        set.chan[A].rShiftAdd[range][mode] = 0;
                        int16 rShiftAdd = CalculateAdditionRShift(A, (Range)range);
                        set.chan[A].rShiftAdd[range][mode] = rShiftAdd;
                    }
                }
            }
		}

        gStateFPGA.stateCalibration = StateCalibration_RShift1start;

        HAL_Delay(500);

		if(panel.WaitPressingButton() == B_Start)                 // ������� ������������� ��� ������ ��������� ���������� ������� ������.
        {
			gStateFPGA.stateCalibration = StateCalibration_RShift1inProgress;

            koeffCal1 = CalculateKoeffCalibration(B);
			if(koeffCal1 == ERROR_VALUE_FLOAT)
            {
				gStateFPGA.stateCalibration = StateCalibration_ErrorCalibration1;
				panel.WaitPressingButton();
                DEBUG_STRETCH_ADC_TYPE = StretchADC_Hand;
                LoadStretchADC(B);
			}
            else
            {
                STRETCH_ADC_B = koeffCal1;
                fpga.LoadKoeffCalibration(B);
            }

            for (int range = 0; range < RangeSize; range++)
            {
                for (int mode = 0; mode < 2; mode++)
                {
                    if (!(mode == 0 && (range == Range_2mV || range == Range_5mV || range == Range_10mV)))
                    {
                        fpga.SetModeCouple(B, (ModeCouple)mode);
                        set.chan[B].rShiftAdd[range][mode] = 0;
                        set.chan[B].rShiftAdd[range][mode] = CalculateAdditionRShift(B, (Range)range);
                    }
                }
            }

            /*
            for (int range = 2; range < RangeSize; range++)
            {
                fpga.SetModeCouple(B, ModeCouple_AC);
                set.chan[B].rShiftAdd[range][ModeCouple_AC] = 0;
                set.chan[B].rShiftAdd[range][ModeCouple_AC] = CalculateAdditionRShift(B, (Range)range);
                set.chan[B].rShiftAdd[range][ModeCouple_DC] = set.chan[B].rShiftAdd[range][ModeCouple_AC];
            }
            */
		}

        break;
    }

    fpga.RestoreState();

    SET_BALANCE_ADC_A = shiftADC0;
    SET_BALANCE_ADC_B = shiftADC1;
    FSMC_Write(WR_ADD_RSHIFT_DAC1, SET_BALANCE_ADC_A);
    FSMC_Write(WR_ADD_RSHIFT_DAC2, SET_BALANCE_ADC_B);

    fpga.SetRShift(A, SET_RSHIFT_A);
    fpga.SetRShift(B, SET_RSHIFT_B);

    STRETCH_ADC_A = (koeffCal0 == ERROR_VALUE_FLOAT) ? koeffCalibrationOld[0] : koeffCal0;

    fpga.LoadKoeffCalibration(A);

    STRETCH_ADC_B = (koeffCal1 == ERROR_VALUE_FLOAT) ? koeffCalibrationOld[1] : koeffCal1;
    fpga.LoadKoeffCalibration(B);

    gStateFPGA.stateCalibration = StateCalibration_None;
    panel.WaitPressingButton();
    panel.Enable();
    Timer_Disable(kTimerDrawHandFunction);
    display.SetDrawMode(DrawMode_Auto, 0);
    gStateFPGA.stateCalibration = StateCalibration_None;

    SET_ENABLED_A = chanAenable;
    SET_ENABLED_B = chanBenable;

    fpga.OnPressStartStop();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FuncAttScreen(void)
{
    painter.BeginScene(COLOR_BLACK);

    static bool first = true;
    static uint startTime = 0;
    if(first)
    {
        first = false;
        startTime = gTimerMS;
    }
    int16 y = 10;
    display.Clear();
    
    painter.SetColor(COLOR_FILL);
    
#define dX 20
#define dY -15
    
    switch(gStateFPGA.stateCalibration)
    {
        case StateCalibration_None:
        {
                painter.DrawTextInRect(40 + dX, y + 25 + dY, SCREEN_WIDTH - 100, 200, "���������� ���������. ������� ����� ������, ����� ����� �� ������ ����������.");

                painter.DrawText(10 + dX, 55 + dY, "�������� ���� 1� :");
                painter.DrawText(10 + dX, 80 + dY, "�������� ���� 2� :");
                for (int i = 0; i < RangeSize; i++)
                {
                    painter.DrawFormatText(95 + i * 16 + dX, 55 + dY, COLOR_FILL, "%d", set.chan[A].rShiftAdd[i][0]);
                    painter.DrawFormatText(95 + i * 16 + dX, 65 + dY, COLOR_FILL, "%d", set.chan[A].rShiftAdd[i][1]);
                    painter.DrawFormatText(95 + i * 16 + dX, 80 + dY, COLOR_FILL, "%d", set.chan[B].rShiftAdd[i][0]);
                    painter.DrawFormatText(95 + i * 16 + dX, 90 + dY, COLOR_FILL, "%d", set.chan[B].rShiftAdd[i][1]);
                }
                
                painter.DrawFormatText(10 + dX, 110 + dY, COLOR_FILL, "����������� ���������� 1� : %f, %d", STRETCH_ADC_A, (int)(STRETCH_ADC_A * 0x80));
                painter.DrawFormatText(10 + dX, 130 + dY, COLOR_FILL, "������������ ���������� 2� : %f, %d", STRETCH_ADC_B, (int)(STRETCH_ADC_B * 0x80));

                DrawParametersChannel(A, 10 + dX, 150 + dY, false);
                DrawParametersChannel(B, 10 + dX, 200 + dY, false);
        }
            break;

        case StateCalibration_ADCinProgress:
            DrawParametersChannel(A, 5, 25, true);
            DrawParametersChannel(B, 5, 75, true);
            break;

        case StateCalibration_RShift0start:
            painter.DrawTextInRect(50, y + 25, SCREEN_WIDTH - 100, 200, "���������� �� ����� ������ 1 ����� ����������� � ������� ������ ����/����. \
���� �� �� ������ ����������� ������ ������, ������� ����� ������ ������.");
            break;

        case StateCalibration_RShift0inProgress:
            break;

        case StateCalibration_RShift1start:
            painter.DrawTextInRect(50, y + 25, SCREEN_WIDTH - 100, 200, "���������� �� ����� ������ 2 ����� ����������� � ������� ������ ����/����. \
���� �� �� ������ ����������� ������ �����, ������� ����� ������ ������.");
            break;

        case StateCalibration_RShift1inProgress:
            break;

        case StateCalibration_ErrorCalibration0:
            painter.DrawTextInRect(50, y + 25, SCREEN_WIDTH - 100, 200, "�������� !!! ����� 1 �� �����������.");
            break;

        case StateCalibration_ErrorCalibration1:
            painter.DrawTextInRect(50, y + 25, SCREEN_WIDTH - 100, 200, "�������� !!! ����� 2 �� �����������.");
            break;
    }

    /*
    if(stateFPGA.stateCalibration == kNone || stateFPGA.stateCalibration == kRShift0start || stateFPGA.stateCalibration == kRShift1start) {
        x = 230;
        y = 187;
        int delta = 32;
        width = 80;
        height = 25;
        DrawRectangle(x, y, width, height, COLOR_BLACK);
        DrawStringInCenterRect(x, y, width, height, "����������", COLOR_BLACK, false);
        DrawRectangle(x, y - delta, width, height, COLOR_BLACK);
        DrawStringInCenterRect(x, y - delta, width, height, "��������", COLOR_BLACK, false);
    }
    */
    char buffer[100];
    sprintf(buffer, "%.1f", (gTimerMS - startTime) / 1000.0f);
    painter.DrawTextC(0, 0, buffer, COLOR_BLACK);

    painter.EndScene();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawParametersChannel(Channel chan, int eX, int eY, bool inProgress)
{
    painter.SetColor(COLOR_FILL);
    if(inProgress)
    {
        painter.DrawText(eX, eY + 4, chan == 0 ? "����� 1" : "����� 2");
        ProgressBar *bar = (chan == A) ? &bar0 : &bar1;
        bar->width = 240;
        bar->height = 15;
        bar->y = eY;
        bar->x = 60;
        ProgressBar_Draw(bar);
    }

    if(Settings_DebugModeEnable())
    {
        int x = inProgress ? 5 : eX;
        int y = eY + (inProgress ? 110 : 0);
        painter.DrawText(x, y, "���������� �� ����:");
        char buffer[100] = {0};
        sprintf(buffer, "���1 = %.2f/%.2f, ���2 = %.2f/%.2f, d = %.2f/%.2f", avrADC1old[chan] - AVE_VALUE, avrADC1[chan] - AVE_VALUE, 
                                                                             avrADC2old[chan] - AVE_VALUE, avrADC2[chan] - AVE_VALUE,
                                                                             deltaADCold[chan], deltaADC[chan]);
        y += 10;
        painter.DrawText(x, y, buffer);
        buffer[0] = 0;
        sprintf(buffer, "����������� A�� = %.2f/%.2f %%", deltaADCPercentsOld[chan], deltaADCPercents[chan]);
        painter.DrawText(x, y + 11, buffer);
        buffer[0] = 0;
        sprintf(buffer, "�������� %d", SET_BALANCE_ADC(chan));
        painter.DrawText(x, y + 19, buffer);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDeltaADC(Channel chan, float *avgADC1, float *avgADC2, float *delta)
{
    uint *startTime = (chan == A) ? &startTimeChan0 : &startTimeChan1;
    *startTime = gTimerMS;
    
    ProgressBar *bar = (chan == A) ? &bar0 : &bar1;
    bar->passedTime = 0;
    bar->fullTime = 0;

    fpga.SetTrigSource((TrigSource)chan);
    fpga.SetTrigLev((TrigSource)chan, TrigLevZero);

    uint8 *address1 = chan == A ? RD_ADC_A1 : RD_ADC_B1;
    uint8 *address2 = chan == A ? RD_ADC_A2 : RD_ADC_B2;

    static const int numCicles = 10;
    for(int cicle = 0; cicle < numCicles; cicle++)
    {
        FSMC_Write(WR_START, 1);
        while(_GET_BIT(FSMC_Read(RD_FL), 2) == 0) {};
        fpga.SwitchingTrig();
        while(_GET_BIT(FSMC_Read(RD_FL), 0) == 0) {};
        FSMC_Write(WR_STOP, 1);

        for(int i = 0; i < FPGA_MAX_POINTS; i++)
        {
            if(chan == A)
            {
                *avgADC1 += FSMC_Read(address1);
                *avgADC2 += FSMC_Read(address2);
                FSMC_Read(RD_ADC_B1);
                FSMC_Read(RD_ADC_B2);
            }
            else
            {
                FSMC_Read(RD_ADC_A1);
                FSMC_Read(RD_ADC_A2);
                *avgADC1 += FSMC_Read(address1);
                *avgADC2 += FSMC_Read(address2);
            }
        }
        
        bar->passedTime = gTimerMS - *startTime;
        bar->fullTime = bar->passedTime * (float)numCicles / (cicle + 1);
    }

    *avgADC1 /= (FPGA_MAX_POINTS * numCicles);
    *avgADC2 /= (FPGA_MAX_POINTS * numCicles);

    *delta = *avgADC1 - *avgADC2;

    return ((*avgADC1) - (*avgADC2)) / 255.0f * 100;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AlignmentADC(void)
{
    shiftADC0 = (deltaADCold[0] > 0) ? (deltaADCold[0] + 0.5) : (deltaADCold[0] - 0.5);
    SET_BALANCE_ADC_A = shiftADC0;
    shiftADC1 = (deltaADCold[1] > 0) ? (deltaADCold[1] + 0.5) : (deltaADCold[1] - 0.5);
    SET_BALANCE_ADC_B = shiftADC1;
    FSMC_Write(WR_ADD_RSHIFT_DAC1, SET_BALANCE_ADC_A);
    FSMC_Write(WR_ADD_RSHIFT_DAC2, SET_BALANCE_ADC_B);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int16 CalculateAdditionRShift(Channel chan, Range range)
{
    fpga.SetRange(chan, range);
    fpga.SetRShift(chan, RShiftZero);
    fpga.SetTBase(TBase_200us);
    fpga.SetTrigSource(chan == A ? TrigSource_ChannelA : TrigSource_ChannelB);
    fpga.SetTrigPolarity(TrigPolarity_Front);
    fpga.SetTrigLev((TrigSource)chan, TrigLevZero);

    fpga.WriteToHardware(WR_UPR, BINARY_U8(00000000), false);   // ������������� ����� ����������� � ����

    int numMeasures = 8;
    int sum = 0;
    int numPoints = 0;

    int time = gTimerMS;

    while(gTimerMS - time < 50) {};

    for(int i = 0; i < numMeasures; i++)
    {
        uint startTime = gTimerMS;
        const uint timeWait = 100;

        FSMC_Write(WR_START, 1);
        while(_GET_BIT(FSMC_Read(RD_FL), 2) == 0 && (gTimerMS - startTime < timeWait)) {}; 
        if(gTimerMS - startTime > timeWait)                 // ���� ������ ������� ����� ������� - 
        {
            return ERROR_VALUE_INT16;                       // ����� � �������.
        }

        fpga.SwitchingTrig();

        startTime = gTimerMS;

        while(_GET_BIT(FSMC_Read(RD_FL), 0) == 0 && (gTimerMS - startTime < timeWait)) {};
        if(gTimerMS - startTime > timeWait)                 // ���� ������ ������� ����� ������� - 
        {
            return ERROR_VALUE_INT16;                       // ����� � �������.
        }

        FSMC_Write(WR_STOP, 1);

        uint8 *addressRead1 = chan == A ? RD_ADC_A1 : RD_ADC_B1;
        uint8 *addressRead2 = chan == A ? RD_ADC_A2 : RD_ADC_B2;

        for(int i = 0; i < FPGA_MAX_POINTS; i += 2)
        {
            sum += FSMC_Read(addressRead1);
            sum += FSMC_Read(addressRead2);
            numPoints += 2;
        }
    }

    float aveValue = (float)sum / numPoints;
    int16 retValue = -(aveValue - AVE_VALUE) * 2;

    if(retValue < - 100 || retValue > 100)
    {
        return ERROR_VALUE_INT16;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateKoeffCalibration(Channel chan)
{
    fpga.WriteToHardware(WR_UPR, BINARY_U8(00000100), false);

    fpga.SetRShift(chan, RShiftZero - 40 * 4);
    fpga.SetModeCouple(chan, ModeCouple_DC);
    fpga.SetTrigSource((TrigSource)chan);
    fpga.SetTrigLev((TrigSource)chan, TrigLevZero + 40 * 4);
    
    int numMeasures = 16;
    int sumMIN = 0;
    int numMIN = 0;
    int sumMAX = 0;
    int numMAX = 0;

    for(int i = 0; i < numMeasures; i++)
    {
        uint startTime = gTimerMS;
        const uint timeWait = 1000;

        while(gTimerMS - startTime < 20) {}
        startTime = gTimerMS;

        FSMC_Write(WR_START, 1);
        while(_GET_BIT(FSMC_Read(RD_FL), 2) == 0 && (gTimerMS - startTime > timeWait)) {};
        if(gTimerMS - startTime > timeWait)
        {
            return ERROR_VALUE_FLOAT;
        }

        fpga.SwitchingTrig();
        startTime = gTimerMS;

        while(_GET_BIT(FSMC_Read(RD_FL), 0) == 0 && (gTimerMS - startTime > timeWait)) {};
        if(gTimerMS - startTime > timeWait)
        {
            return ERROR_VALUE_FLOAT;
        }

        FSMC_Write(WR_STOP, 1);

        uint8 *addressRead1 = chan == A ? RD_ADC_A1 : RD_ADC_B1;
        uint8 *addressRead2 = chan == A ? RD_ADC_A2 : RD_ADC_B2;

        for(int i = 0; i < FPGA_MAX_POINTS; i += 2)
        {
            uint8 val0 = FSMC_Read(addressRead1);
            if(val0 > AVE_VALUE + 60)
            {
                numMAX++;
                sumMAX += val0;
            }
            else if(val0 < AVE_VALUE - 60)
            {
                numMIN++;
                sumMIN += val0;
            }

            uint8 val1 = FSMC_Read(addressRead2);
            if(val1 > AVE_VALUE + 60)
            {
                numMAX++;
                sumMAX += val1;
            }
            else if(val1 < AVE_VALUE - 60)
            {
                numMIN++;
                sumMIN += val1;
            }
        }
    }

    float aveMin = (float)sumMIN / (float)numMIN;
    float aveMax = (float)sumMAX / (float)numMAX;

    float retValue = 160.0f / (aveMax - aveMin);

    if(retValue < 0.5f || retValue > 1.5f)
    {
        return ERROR_VALUE_FLOAT;
    }
    return retValue;
}
