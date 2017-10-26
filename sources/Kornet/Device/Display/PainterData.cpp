#include "PainterData.h"
#include "defines.h"
#include "Grid.h"
#include "Log.h"
#include "Painter.h"
#include "FPGA/Storage.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PainterData painterData;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PainterData::DrawData()
{
    uint8 *dataA = 0;
    uint8 *dataB = 0;

    storage.GetData(&dataA, &dataB);

    DrawChannel(A, dataA);
    DrawChannel(B, dataB);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawChannel(Channel ch, uint8 data[FPGA_MAX_NUM_POINTS])
{
    int bottom = grid.Bottom();
    int left = grid.Left();

    if (!SET_ENABLED(ch))
    {
        return;
    }
    
    float scale = (float)grid.Height() / 255.0f;

    if (VIEW_MODE_IS_LINES)
    {
        if (THICKNESS_SIGNAL_IS_3)
        {
            painter.SetColor(Color::ChanHalf(ch));

            float valuePrev = bottom - data[0] * scale;

            for (int i = 1; i < 281; i++)
            {
                float value = bottom - data[i] * scale;

                painter.DrawVLine(left + i - 2, valuePrev, value);
                painter.DrawVLine(left + i, valuePrev, value);

                valuePrev = value;
            }
        }

        painter.SetColor(Color::Chan(ch));

        int x = left;

        for (int i = 1; i < 281; i++)
        {
            float value = bottom - data[i] * scale;
            float valuePrev = bottom - data[i - 1] * scale;
            value += math.Sign(valuePrev - value);          // ��� ������� ����� �� �������� ����� ����� ������� �� ����������� � ����� ������������-
                                                            // ��� ���� ������������ �����
            painter.DrawVLine(x++, valuePrev, value);
        }
    }
    else
    {
        if (THICKNESS_SIGNAL_IS_3)
        {
            painter.SetColor(Color::ChanHalf(ch));

            for (int i = 0; i < 280; i++)
            {
                float value = bottom - data[i] * scale;
                painter.DrawHLine(value, left + i - 1, left + i + 1);
                painter.DrawVLine(left + i, value - 1, value + 1);
            }
        }

        painter.SetColor(Color::Chan(ch));

        for (int i = 0; i < 280; i++)
        {
            float value = bottom - data[i] * scale;
            painter.SetPoint(left + i, value);
        }
    }
}
