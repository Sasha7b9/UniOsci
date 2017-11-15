#include "DisplayPrimitives.h"
#include "Colors.h"
#include "Painter.h"
#include <stdio.h> 


void ProgressBar_Draw(ProgressBar *bar)
{
    int x = bar->x;
    int y = bar->y;
    char buffer[100] = {0};
    float passedPercents = bar->fullTime == 0 ? 0 : bar->passedTime / bar->fullTime * 100;
    sprintf(buffer, "��������� %.1f %%", passedPercents);
    painter.DrawStringInCenterRectC(x, y - 15, bar->width, bar->height, buffer, COLOR_FILL);
    painter.DrawRectangle(bar->x, bar->y, bar->width, bar->height);
    painter.FillRegion(bar->x, bar->y, bar->width * passedPercents / 100.0f, bar->height);
    buffer[0] = 0;
    sprintf(buffer, "�������� %.1f �", (int)(bar->fullTime - bar->passedTime) / 1000.0f);
    painter.DrawStringInCenterRect(x, y + bar->height, bar->width, bar->height, buffer);
}
