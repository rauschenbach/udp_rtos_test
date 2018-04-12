#include "systick.h"
#include "uart1.h"

static u32 LocalTime = 0;

/* Здесь указатель на функцыю, которую будем вызывать при таймауте */
static struct {
    int timer;
    void (*call_back_func) (int);
} systick_timeout;

/**
 * Миллисекунды
 */
u32_t sys_now(void)
{
    return LocalTime;		/* ms */
}

/**
  * Установка таймаута
  */
void systick_set_timeout(s32 t0, void (*foo)(int))
{
	systick_timeout.timer = t0;
        systick_timeout.call_back_func = foo;
}

void systick_clear_timeout(void)
{
	systick_timeout.timer = 0;
	systick_timeout.call_back_func = NULL;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void systick_func(void)
{
    LocalTime++;

   /* как достиг нуля - вызываем функцию */
    if (systick_timeout.timer > 0) {
	systick_timeout.timer--;
	if (systick_timeout.timer == 0 && systick_timeout.call_back_func != NULL) {
	    systick_timeout.call_back_func(RX_TIMEOUT);
	}
    }
}
