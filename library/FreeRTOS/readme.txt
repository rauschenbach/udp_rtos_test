Each real time kernel port consists of three files that contain the core kernel
components and are common to every port, and one or more files that are 
specific to a particular microcontroller and or compiler.

+ The FreeRTOS/Source directory contains the three files that are common to 
every port - list.c, queue.c and tasks.c.  The kernel is contained within these 
three files.  croutine.c implements the optional co-routine functionality - which
is normally only used on very memory limited systems.

+ The FreeRTOS/Source/Portable directory contains the files that are specific to 
a particular microcontroller and or compiler.

+ The FreeRTOS/Source/include directory contains the real time kernel header 
files.

See the readme file in the FreeRTOS/Source/Portable directory for more 
information.

Порт FreeRTOS для контроллера 1986ВЕ1Т фирмы Миландр.

Сделан на основе порта для Cortex-M0.
В качестве системного таймера используется Таймер4, 
т.к. SysTick неправильно работает на частотах >25 МГц (см. Errata - ошибка 11)

!!! В файле FreeRTOSConfig.h необходимо заменить вектор прерывания от системного таймера !!!
строку:
	#define xPortSysTickHandler SysTick_Handler
заменить на:
	#define xPortSysTickHandler Timer4_IRQHandler


В файл port.c внесены следующие изменения:

1.) Переписана ф-я prvSetupTimerInterrupt() - запускает Таймер4 со счетом вниз и прерыванием по достижению 0

2.) Изменена ф-я xPortStartScheduler() - задается приоритет и включается прерывание Таймера4

3.) Изменена ф-я xPortSysTickHandler() - добавлен сброс события в прерывание от системного таймера
