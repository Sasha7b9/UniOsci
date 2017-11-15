#pragma once
#include "DisplayTypes.h"
#include "Settings/SettingsTypes.h"
#include "Painter.h"
#include "Tables.h"


class Display
{
public:
    void Init();

    void Update();

    void RotateRShift(Channel channel);

    void RotateTrigLev();

    void Redraw();

    void EnableTrigLabel(bool enable);
    /// ������� ������ ���������� ��� ������� ���������� ������. ���������� ��� ��������� �������� �� ����������.
    void ResetP2Ppoints(bool empty);
    /// ��������� ����� � ������ ����������� ������ � ��������� data00, data01 - ��� ������� ������; data10, data11 - ��� ������� ������
    void AddPoints(uint8 data00, uint8 data01, uint8 data10, uint8 data11);

    void ShowWarningBad(Warning warning);

    void ShowWarningGood(Warning warning);

    void ClearFromWarnings();

    void SetDrawMode(DrawMode mode, pFuncVV func);

    void SetAddDrawFunction(pFuncVV func);

    void RemoveAddDrawFunction();

    void Clear();

    void ShiftScreen(int delta);

    void ChangedRShiftMarkers();

    void AddStringToIndicating(const char *string);

    void OneStringUp();

    void OneStringDown();

    void SetPauseForConsole(bool pause);
    /// ����� ��������� ���������� ������ ��� ������� ����� ������� ���� ���.
    void RunAfterDraw(pFuncVV func);
    /// ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    void DisableShowLevelRShiftA();
    /// ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    void DisableShowLevelRShiftB();
    
    void DisableShowLevelTrigLev();

    void OnRShiftMarkersAutoHide();

private:

    void ShowWarn(const char *message);
    /// ���������� �����.
    void DrawGrid(int left, int top, int width, int height);
    /// ���������� ������ �����.
    void DrawFullGrid();

    void DrawCursorsWindow();

    void DrawCursorsRShift();
    /// ���������� ������� �������� �� ����������
    void DrawCursorRShift(Channel chan);
    /// ���������� ������ ������ �������������.
    void DrawCursorTrigLevel();
    /// ���������� ������ �������� �� �������.
    void DrawCursorTShift();
    /// �������� ���������� ��� ������ - � ������ ����� �������.
    void DrawLowPart();

    void DrawHiPart();

    void DrawHiRightPart();

    void DrawMath();

    void DrawSpectrum();
    /// ������� ������� �����.
    void DrawTime(int x, int y);
    /// ���������� ������� ��������� ���������.
    void DrawCursors();
    /// ������� �������� �������� ��������� ���������.
    void WriteCursors();
    /// ������� �������� �������������� ���������.
    void DrawMeasures();
    /// �������� ��������� ���������� �������.
    void DrawConsole();
    /// �������� ��������������.
    void DrawWarnings();

    void DrawTimeForFrame(uint timeMS);
    /// ���������� �������������� ������ ��������� ���������.
    void DrawHorizontalCursor
                                (int y,             ///< �������� �������� �������.
                                int xTearing        ///< ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������.
                                );
    /// ���������� ������������ ������ ��������� ���������.
    void DrawVerticalCursor
                                (int x,             ///< �������� �������� �������.
                                int yTearing        ///< ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������.
                                );
    /// ������� �������� ������ �������������. 
    void WriteValueTrigLevel();

};


extern Display display;
