#pragma once

/// Здесь ID всех представленных в меню страниц
typedef enum
{
    Page_Display,                   ///< ДИСПЕЙ
    Page_DisplayAccumulation,       ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ
    Page_DisplayAverage,            ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ
    Page_DisplayGrid,               ///< ДИСПЛЕЙ - СЕТКА
    Page_DisplayRefresh,            ///< ДИСПЛЕЙ - ОБНОВЛЕНИЕ
    Page_Main,                      ///< Главная страница меню. Из неё растут все остальные
    Page_Debug,                     ///< ОТЛАДКА
    Page_DebugConsole,              ///< ОТЛАДКА - КОНСОЛЬ
    Page_DebugADC,                  ///< ОТЛАДКА - АЦП
    Page_DebugADCbalance,           ///< ОТЛАДКА - АЦП - БАЛАНС
    Page_DebugADCstretch,           ///< ОТЛАДКА - АЦП - РАСТЯЖКА
    Page_DebugADCrShift,            ///< ОТЛАДКА - АЦП - ДОП. СМЕЩЕНИЕ
    Page_DebugADCaltShift,          ///< ОТЛАДКА - АЦП - ДОП. СМЕЩ. ПАМ.
    Page_DebugShowRegisters,        ///< ОТЛАДКА - РЕГИСТРЫ
    Page_DebugRandomizer,           ///< ОТЛАДКА - РАНД-ТОР
    Page_DebugInformation,          ///< ОТЛАДКА - ИНФОРМАЦИЯ
    Page_DebugSound,                ///< ОТЛАДКА - ЗВУК
    Page_DebugAltera,               ///< ОТЛАДКА - АЛЬТЕРА
    Page_ChannelA,                  ///< КАНАЛ 1
    Page_ChannelB,                  ///< КАНАЛ 2
    Page_Cursors,                   ///< КУРСОРЫ
    Page_Trig,                      ///< СИНХР.
    Page_Memory,                    ///< ПАМЯТЬ
    Page_MemoryExt,                 ///< ПАМЯТЬ - ВНЕШН. ЗУ
    Page_Measures,                  ///< ИЗМЕРЕНИЯ
    Page_MeasuresField,             ///< ИЗМЕРЕНИЯ - ЗОНА
    Page_Service,                   ///< СЕРВИС
    Page_ServiceDisplay,            ///< СЕРВИС - ДИСПЛЕЙ
    Page_ServiceDisplayColors,      ///< СЕРВИС - ДИСПЛЕЙ - ЦВЕТА
    Page_ServiceCalibrator,         ///< СЕРВИС - КАЛИБРАТОР
    Page_ServiceEthernet,           ///< СЕРВИС - ETHERNET
    Page_Help,                      ///< ПОМОЩЬ
    Page_Time,                      ///< РАЗВЕРТКА
    Page_Math,                      ///< СЕРВИС - МАТЕМАТИКА
    Page_MathFFT,
    Page_TrigAuto,                  ///< СИНХР - АВТ ПОИСК
    Page_SB_Curs,                   ///< Открыта страница КУРСОРЫ - Измерения
    PageSB_Measures_Tune,           ///< Открыта страница ИЗМЕРЕНИЯ - Настроить
    Page_SB_MeasTuneField,          ///< Открыта страница ИЗМЕРЕНИЯ - ЗОНА - Задать
    PageSB_Memory_Last,             ///< Открыта страница ПАМЯТЬ - Последние
    PageSB_Memory_Internal,         ///< Открыта страница ПАМЯТЬ - Внутр ЗУ
    Page_SB_MemExtSetMask,          ///< Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - МАСКА
    Page_SB_MemExtSetName,          ///< Появляется перед сохранением файла на флешку для задания имени файла при соответствующей опции ВНЕШН ЗУ - Имя файла
    PageSB_Memory_Drive_Manager,    ///< Открыта страница ПАМЯТЬ - ВНЕШН ЗУ - КАTAЛОГ
    Page_SB_Help,                   ///< Открыта страница HELP
    PageSB_Service_FFT_Cursors,     ///< Открыта страница СЕРВИС - Математика - Курсоры
    PageSB_Service_Function,        ///< СЕРВИС - МАТЕМАТИКА - ФУНКЦИЯ
    Page_SB_Information,            ///< СЕРВИС - ИНФОРМАЦИЯ
    PageSB_Debug_SerialNumber,      ///< ОТАДКА - С/Н
    Page_NumPages,
    Page_NoPage
} NamePage;             // При добавлении страниц нужно изменять инициализацию SettingsMenu в SSettings.c
