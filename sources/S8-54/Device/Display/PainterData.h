#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** @addtogroup Painter
 *  @{
 *  @defgroup PainterData
 *  @brief Функции отрисовки данных
 *  @{
 */


class PainterData
{
public:
    /// Отрисовка сигнала на основном экране.
    static void DrawData(void);
    /// Прервать процесс рисования
    static void InterruptDrawing(void);
    /// Нарисовать результат математической функции
    static void DrawMath(void);
};


/** @}  @}
 */
