#include "main.h"
#include "Log.h"
#include "Device.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    device.Init();

    while (1)
    {
        device.Update();
    }
}
