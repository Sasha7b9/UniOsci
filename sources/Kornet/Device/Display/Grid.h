#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Grid
{
public:

    // Константы используются для расчёта размеров, которые остаются одинаковыми при любых размерах и пропорциях сетки - например, элементов меню
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;
    static const int LEFT = 19;

    // Эти значения могут меняться в зависимости от выставленного режима
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
