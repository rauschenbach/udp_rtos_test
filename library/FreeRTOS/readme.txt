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

���� FreeRTOS ��� ����������� 1986��1� ����� �������.

������ �� ������ ����� ��� Cortex-M0.
� �������� ���������� ������� ������������ ������4, 
�.�. SysTick ����������� �������� �� �������� >25 ��� (��. Errata - ������ 11)

!!! � ����� FreeRTOSConfig.h ���������� �������� ������ ���������� �� ���������� ������� !!!
������:
	#define xPortSysTickHandler SysTick_Handler
�������� ��:
	#define xPortSysTickHandler Timer4_IRQHandler


� ���� port.c ������� ��������� ���������:

1.) ���������� �-� prvSetupTimerInterrupt() - ��������� ������4 �� ������ ���� � ����������� �� ���������� 0

2.) �������� �-� xPortStartScheduler() - �������� ��������� � ���������� ���������� �������4

3.) �������� �-� xPortSysTickHandler() - �������� ����� ������� � ���������� �� ���������� �������
