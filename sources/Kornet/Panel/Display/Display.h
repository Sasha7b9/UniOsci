#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    void Init();

    void Update();

    uint8 *GetBuffer();

    void ToggleBuffers();
};


extern Display display;
extern LTDC_HandleTypeDef hltdc;
