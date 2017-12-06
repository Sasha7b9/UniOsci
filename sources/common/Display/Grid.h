#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Display
 *  @{
 *  @defgroup Grid
 *  @brief Параметры сетки
 *  @{
 */

class Grid
{
public:
    static int Left(void);

    static int Right(void);

    static int Width(void);

    static float DeltaY(void);

    static float DeltaX(void);

    static int WidthInCells(void);

    static int FullBottom(void);

    static int FullHeight(void);

    static int FullCenterHeight(void);

    static int ChannelBottom(void);

    static int ChannelHeight(void);

    static int ChannelCenterHeight(void);

    static int MathHeight(void);

    static int MathBottom(void);

    static int MathTop(void);

    static int BottomMessages(void);

    static void CoordTrigLevel(int *left, int *top, int width);
};

/** @}  @}
 */
