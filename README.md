# udp_rtos_test
Milandr 1986ВЕ3 + RTOS + LWIP + SOCKETS + UDP ECHO SERVER

Внешний Ethernet на микросхеме 5600ВГ1У
драйвер для FreeRTOS в файле library/lwip/port/ethernetif/eth5_if.c
выход ~IRQ Ethernet микросхемы должен быть соединен через транзисторный ключ
на ногу контролера. Настройка внешней шины в файле sys/system_MDR1986VE3.c 
адрес Eth назначается статически и запускается задача UDP эхо сервера
отладка идет через semihosting - включается/отключается в globdefs.h и IAR6 
General Options->LIBRARY CONFIGURATION

-перенес мотание дескрипторов приема и передачи в одно место

-Заменил бинарные семафоры в прерывании счетными. Пропажи пакетов исчезли. буду увеличивать частоту от 40МГц и выше.
