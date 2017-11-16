#pragma once

#pragma warning(push)
#pragma warning(disable : 4464 4820)

#if defined(STM32F429xx) || defined(STM32F437xx) || defined(STM32F407xx)

#include "../STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff.h"

#else

#include "../STM32Cube_F2/Middlewares/Third_Party/FatFs/src/ff.h"

#endif

#pragma warning(pop)
