/* Circular buffer example, keeps one slot open */
#include <stdlib.h>
#include <stdio.h>
#include "circbuf.h"
#include "main.h"

#define MAX_SEM_NUMS	5

static SemaphoreHandle_t sema[MAX_SEM_NUMS];
static int num = 0;
static SemaphoreHandle_t *pSema;	// = &sema[num];


/* Инициализация буфера, если его нет!  */
int cb_init(CircularBuffer * cb, int size)
{
    int res = -1;


    cb->size = size + 1;	/* include empty elem */
    cb->start = 0;
    cb->end = 0;

    /* Все семафоры израсходованы */
    if (num > (MAX_SEM_NUMS - 1)) {
	return res;
    }

    if (cb->elems == NULL) {
	cb->elems = (ElemType *) MALLOC(cb->size * sizeof(ElemType));

	if (cb->elems == NULL) {	/* Не смог выделить память */
	    return res;
	}
    }

    pSema = &sema[num];
    if (*pSema == NULL) {
	/* Создание мьютексного семафора, который мы будем использовать
	 * для обслуживания доступа к порту Serial:*/
	*pSema = xSemaphoreCreateMutex();
	if (*pSema != NULL) {
	    /* Если семафор создан, то делаем его доступным (Give)
	     * для использования при доступе к порту Serial */
	    xSemaphoreGive(*pSema);
	    num++;	/* Увеличим номер доступного семафора */
	    res = 0;
	}
    }
    return res;
}


/* Удалить буфер, если он существует! */
void cb_free(CircularBuffer * cb)
{
    if (cb->elems != NULL) {
	FREE(cb->elems);	/* OK if null */
	cb->elems = 0;
    }
    vSemaphoreDelete(*pSema);
    num--;
}

/**
 * Буфер полный?
 */
int cb_is_full(CircularBuffer * cb)
{
    return (cb->end + 1) % cb->size == cb->start;
}

/**
 * Буфер пустой?
 */
int cb_is_empty(CircularBuffer * cb)
{
    return cb->end == cb->start;
}

/**
 * Очистить буфер
 */
void cb_clear(CircularBuffer * cb)
{
    cb->start = 0;
    cb->end = 0;
}


/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cb_write(CircularBuffer * cb, ElemType * elem)
{
    /* Если семафор недоступен, ждем 5 тиков планировщика,
     * после чего снова проверим семафор. */
    while (xSemaphoreTake(*pSema, (TickType_t) 5) == pdFALSE) {
    }
    cb->elems[cb->end] = *elem;
    cb->end = (cb->end + 1) % cb->size;
    if (cb->end == cb->start) {
	cb->start = (cb->start + 1) % cb->size;	/* full, overwrite */
    }
    /* Освободить семафор */
    xSemaphoreGive(*pSema);
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cb_read(CircularBuffer * cb, ElemType * elem)
{
    while (xSemaphoreTake(*pSema, (TickType_t) 5) == pdFALSE) {
    }
    *elem = cb->elems[cb->start];
    cb->start = (cb->start + 1) % cb->size;

    /* Освободить семафор */
    xSemaphoreGive(*pSema);
}
