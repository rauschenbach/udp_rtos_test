#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "main.h"


//void systick_set_timeout(s32, void*);
void systick_set_timeout(s32 t0, void (*foo)(int));
void systick_clear_timeout(void);
void systick_func(void);
void delay(uint32_t);
u32_t sys_now(void);

#endif /* __SYSTICK_H */
