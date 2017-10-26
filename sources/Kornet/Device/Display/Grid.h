#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Grid
{
public:

    // ��������� ������������ ��� ������� ��������, ������� �������� ����������� ��� ����� �������� � ���������� ����� - ��������, ��������� ����
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;
    static const int LEFT = 19;

    // ��� �������� ����� �������� � ����������� �� ������������� ������
    int Left();
    int Top();
    int Width();
    int Height();
    int Bottom();
    int Right();
    int NumRows();
    int NumCols();

private:
};


extern Grid grid;
