/*
 * systick.c
 *
 *  Created on: 2019年11月13日
 *      Author: Cenx
 */

#include "systick.h"

uint16_t fac_us = 40 ;
uint16_t fac_ms = 40 * 1000 ;
uint32_t Timesysitck = 0;


uint32_t SysTick_Config1(uint32_t ticks)
{
  if ((ticks - 1) > SysTick_LOAD_RELOAD_Msk)  return (1);      /* Reload value impossible */

  SysTick->LOAD  = ticks - 1;                                  /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk ;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}

void Calculation()
{
//	uint32_t frequency;
//	frequency = 20000000 ;
	SysTick->CTRL |= 1 << 2 ;//选择主CLK
//    fac_us = frequency/1000000 ;
//    fac_ms = fac_us * 1000 ;
}

void task_us(uint16_t count)
{
	uint32_t temp = 0 ;
    SysTick->LOAD = (count * 40) ;
    SysTick->VAL = 0 ;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
    do
    {
    	temp = SysTick->CTRL ;
    }while((temp&0x01)&&(!(temp&(1<<16)))) ;
//    while((SysTick->CTRL & (1 << 16)) == 0) ;
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk ;
    SysTick->VAL = 0 ;
}

/*因为LOAD是24位 所以count最大值=2^24/20000=838 20000=20*1000*/
void task_ms(uint16_t count)
{
	uint32_t temp = 0 ;
    SysTick->LOAD = 40 * count * 1000 ;
    SysTick->VAL = 0 ;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
    do
    {
    	temp = SysTick->CTRL ;
    }while((temp&0x01)&&(!(temp&(1<<16)))) ;
//    while((SysTick->CTRL & (1 << 16)) == 0) ;
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk ;
    SysTick->VAL = 0 ;
}

void task_ms1(uint16_t count)
{
	uint8_t i = 0 ;
	for(i = 0 ; i < count ; i ++)
	{
		task_ms(1) ;
	}
}

void task_s(int8_t count)
{
	int8_t i , j ;
	for(i = 0 ; i < count ; i ++)
	{
		for(j = 0 ; j < 10 ; j ++)
		{
			task_ms(100);
		}
	}
}

void sdelay_us(uint32_t time)
{
	uint32_t temp ;
    SysTick->LOAD = (time * 40) ;
    SysTick->VAL = 0 ;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
    do
    {
    	temp = SysTick->CTRL ;
    }while((temp&0x01)&&(!(temp&(1<<16)))) ;
//    while((SysTick->CTRL & (1 << 16)) == 0) ;
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk ;
    SysTick->VAL = 0 ;
    Timesysitck ++ ;
}

void sdelay_ms(uint32_t time)
{
	uint32_t i = 0 ;
	for(i = 0 ; i < time ; i ++)
	{
		sdelay_us(1) ;
	}
}


//SysTick定时器中断服务例程（10ms中断一次）
void SysTick_Handler(void)
{
//	Timesysitck --;
	if(Timesysitck)
		Timesysitck -- ;
	else
		return ;
}

