/*
 * systeminit.c
 *
 *  Created on: 2020年5月15日
 *      Author: Cenx
 */
#include "systeminit.h"

void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);//606666 0.1s 252777 0.1s
}

void delay_s(uint8_t y)
{
	uint8_t i = 0 ;uint8_t j = 0 ;
	for(i = 0 ; i < y ; i++)
		for(j = 0 ; j < 10 ; j++)
		{
			#ifdef INTERCLK
					delay(252777);
			#endif

			#ifdef EXTERNCLK
					delay(252777);
					delay(252777);
			#endif
		}
}

void delay_s1(uint16_t y)
{
	uint16_t i = 0 ;
	for(i = 0 ; i < y ; i++)
	{
		#ifdef INTERCLK
				delay(2527);
		#endif

		#ifdef EXTERNCLK
				delay(5070);
		#endif
	}
}

void Clk_Internal()
{
 ICS_C1|=ICS_C1_IRCLKEN_MASK;       /* Enable the internal reference clock*/
 ICS_C3= 0x90;                      /* Reference clock frequency = 31.25 kHz*/
 while(!(ICS_S & ICS_S_LOCK_MASK)); /* Wait for PLL, running at 40 MHz (1280 * 31.25 kHz) */
 ICS_C2|=ICS_C2_BDIV(1);	        /* BDIV=2, Bus clock = 20 MHz*/
 ICS_S |= ICS_S_LOLS_MASK;   		/* Clear Loss of lock sticky bit */
}

/*
 * 外部晶振
 * core 40 busclk 20
 * */
void Clk_External()
{
    OSC_CR = 0x96;   //选择外部时钟，高功耗高增益
    while ((OSC_CR & OSC_CR_OSCINIT_MASK) == 0);//等待OSC时钟设置完成
    //FEE模式
    ICS_C2 = 0x20;      //8M/256 = 31.25K  ICSOUT = 31.25*1280=40M
    ICS_C1 = 0x18;      //BDIV=1
    while ((ICS_S & ICS_S_IREFST_MASK) == 1);
    while ((ICS_S & ICS_S_LOCK_MASK) == 0);

    SIM_CLKDIV = 0x01100000;  // 40M BUS Clock = ICSOUT/2 20M

    ICS_C2 = 0x00;//BDIV=1
}

