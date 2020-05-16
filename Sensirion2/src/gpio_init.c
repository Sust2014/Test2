/*
 * gpio_init.c
 *
 *  Created on: 2019年6月17日
 *      Author: Cenx
 */

#include "gpio_init.h"
#include "adc_init.h"
#include "can.h"

void Gpio_Init()
{
	GPIOA_PDDR |= 1 << LED_G ;//LED_G设为输出
	GPIOB_PDDR |= 1 << LED_R ;//LED_R设为输出
	GPIOA_PDDR &= ~(1 << Connect_1) ;//检测恢复供气电磁阀和排气电磁阀是否连接正常
	GPIOA_PIDR &= ~(1 << Connect_1) ;
	GPIOA_PDDR &= ~(1 << Connect_2) ;//检测回流电磁阀和控制输出电磁阀是否连接正常
	GPIOA_PIDR &= ~(1 << Connect_2) ;
	GPIOA_PDDR |= 1 << Recover_air_supply_solenoid_valve ;//恢复供气电磁阀
	GPIOA_PDDR |= 1 << Venting_solenoid_valve ;//排气电磁阀
	GPIOA_PDDR |= 1 << Backflow_solenoid_valve ;//回流电磁阀
	GPIOB_PDDR |= 1 << Control_solenoid_valve ;//控制输出电磁阀
	GPIOA_PDDR &= ~(1 << K1_OUT) ;//油压开关
	GPIOA_PIDR &= ~(1 << K1_OUT) ;
	GPIOA_PDDR &= ~(1 << Input_voltage) ;//输入电压
	GPIOA_PIDR &= ~(1 << Input_voltage) ;
//	GPIOA_PDOR |= 1 << Recover_air_supply_solenoid_valve ;
//	GPIOA_PDOR |= 1 << Venting_solenoid_valve ;
//	GPIOA_PDOR |= 1 << Backflow_solenoid_valve ;
//	GPIOA_PDOR |= 1 << Control_solenoid_valve ;
}

void Gpio_unused()
{
	GPIOA_PDDR |= (1 << 2 || 1 << 3 || 1 << 10 || 1 << 11 || 1 << 12 || 1 << 16 || 1 << 17 || 1 << 18
			|| 1 << 19 || 1 << 21 || 1 << 24 || 1 << 25 || 1 << 29 || 1 << 30 || 1 << 31);
	GPIOB_PDDR |= (1 << 2 || 1 << 3 || 1 << 4 || 1 << 6 || 1 << 7 || 1 << 9 || 1 << 10 || 1 << 11
			|| 1 << 12 || 1 << 13 || 1 << 16 || 1 << 17 || 1 << 18 || 1 << 19 || 1 << 24 || 1 << 25
			|| 1 << 26 || 1 << 30 || 1 << 31);
}


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
			delay(252777);
}

void delay_s1(uint16_t y)
{
	uint16_t i = 0 ;
	for(i = 0 ; i < y ; i++)
		delay(2527);
}

void delay_us(uint16_t y)
{
	uint16_t i = 0 ;
	for(i = 0 ; i < y ; i++)
		delay(252);
}

void Venting_On()
{
	GPIOA_PDOR |= 1 << Venting_solenoid_valve ;//打开排气电磁阀
	Venting_flag = true ;
}

void Venting_Off()
{
	GPIOA_PDOR &= ~(1 << Venting_solenoid_valve) ;//打开排气电磁阀
	Venting_flag = false ;
}

void Recover_On()
{
	GPIOA_PDOR |= 1 << Recover_air_supply_solenoid_valve ;
	Recover_flag = true ;
}

void ForeceRecover()
{
	GPIOA_PDOR |= 1 << Recover_air_supply_solenoid_valve ;
	delay_s(2);
	GPIOA_PDOR &= ~(1 << Recover_air_supply_solenoid_valve) ;
}
void Recover_Off()
{
	GPIOA_PDOR &= ~(1 << Recover_air_supply_solenoid_valve) ;
	Recover_flag = false ;
}

void Backflow_On()
{
	GPIOA_PDOR |= 1 << Backflow_solenoid_valve ;
	Backflow_flag = true ;
}

void Backflow_Off()
{
	GPIOA_PDOR &= ~(1 << Backflow_solenoid_valve) ;
	Backflow_flag = false ;
}

void Control_On()
{
	GPIOB_PDOR |= 1 << Control_solenoid_valve ;
	Control_flag = true ;
}

void Control_Off()
{
	GPIOB_PDOR &= ~(1 << Control_solenoid_valve) ;
	Control_flag = false ;
}

void Clk_Init()
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
void Clk_Init1()
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

void WDOG_structure_init()
{
    WDOG_ConfigType WDOGConfig = {{0}};     /* Watchdog configuration structure */

    WDOGConfig.sBits.bWaitEnable   = 1;
    WDOGConfig.sBits.bStopEnable   = 1;
    WDOGConfig.sBits.bDbgEnable    = 1;
    WDOGConfig.sBits.bUpdateEnable = 0;
    WDOGConfig.sBits.bDisable      = 0; /* Enable WDOG */
    WDOGConfig.sBits.bClkSrc       = WDOG_CLK_INTERNAL_1KHZ;
    WDOGConfig.u16TimeOut          = 1000;  /* 1s */
    WDOGConfig.u16WinTime          = 0;

    WDOG_Init(&WDOGConfig);
}

void System_Init()
{
	count_9 = 1 ;
	uint8_t test ;
	No_high_flag = false ;
//    Gpio_unused();
	Gpio_Init();
	Clk_Init();
	FTM2_Init_Output_Cmp();
	CAN_Init(MSCAN_BUS_CLK,MSCAN_SJW,MSCAN_BRP,MSCAN_TSEG1,MSCAN_TSEG2);
//	(void)CAN_Fill_STD_Msg(&sendMsg1, 0x02, sendData1, 8);

	//WDOG_structure_init();
	Enable_Interrupt(FTM2_IRQn);
//	Enable_Interrupt(SysTick_IRQn);
}

void BTS724G_detection()
{
	if(Input_voltage_detaction1 || Input_voltage_detaction2)
	{
		if(Input_voltage_detaction2 == 0)
		{
			ForeceRecover();
		}

		if(Input_voltage_detaction2 == 1)
		{
			Recover_On();
			delay_s1(150);

			if(Recover_error == 1 || Recover_error == 0)
			{
				delay_s(2);
			}

			Recover_Off();

			Recover_error = 0 ;
		}

		while(Input_voltage_detaction1 || Input_voltage_detaction2)
		{
			LED_G_OFF ;
			LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
			LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(500);
		}
	}
}
void Self_inspection()
{
	Input_voltage_Detection();
	sensor_Voltage_Detection();
	T_voltage_Detection();
	BTS724G_detection();//电磁阀连接检测
	Solenoid_valve_Post();
	VentingPipelinePressure();
}

void Valve_detection()
{
	if(Venting_error == 1)//打开检测到电磁阀电流不对 重新开启继续检测
	{
		Venting_error = 0 ;
		Venting_Off();
		delay_s1(11);
		Venting_On();
		delay_s1(11);
		delay_s1(90);
		if(Venting_error == 1)
		{
			Venting_error = 0 ;
			Venting_Off();
			delay_s1(11);
			Venting_On();
			delay_s1(11);
			delay_s1(90);
			solenoid_Voltage_Detection();
		}
		else
			solenoid_Voltage_Detection();
	}

	if(Venting_error == 2)
	{
		solenoid_Voltage_Detection();
	}

	if(Recover_error == 1)//打开检测到电磁阀电流不对 重新开启继续检测
	{
		Recover_error = 0 ;
		Recover_Off();
		delay_s1(11);
		Recover_On();
		delay_s1(11);
		delay_s1(90);
		if(Recover_error == 1)
		{
			Recover_error = 0 ;
			Recover_Off();
			delay_s1(11);
			Recover_On();
			delay_s1(11);
			delay_s1(90);
			solenoid_Voltage_Detection();
		}
		else
			solenoid_Voltage_Detection();
	}

	if(Recover_error == 2)
	{
		solenoid_Voltage_Detection();
	}

	if(Backflow_error == 1)//打开检测到电磁阀电流不对 重新开启继续检测
	{
		Backflow_error = 0 ;
		Backflow_Off();
		delay_s1(11);
		Backflow_On();
		delay_s1(11);
		delay_s1(90);
		if(Backflow_error == 1)
		{
			Backflow_error = 0 ;
			Backflow_Off();
			delay_s1(11);
			Backflow_On();
			delay_s1(11);
			delay_s1(90);
			solenoid_Voltage_Detection();
		}
		else
			solenoid_Voltage_Detection();
	}

	if(Backflow_error == 2)
	{
		solenoid_Voltage_Detection();
	}
}
void Solenoid_valve_Post()
{
	Venting_On();
	delay_s1(11);
//	solenoid_Voltage_Detection();
	delay_s1(90);
	Valve_detection();
	delay_s1(100);
	Venting_Off();

	delay_s1(15);

	Backflow_On();
	delay_s1(11);
	delay_s1(90);
	Valve_detection();
	delay_s1(100);
	Backflow_Off();

	delay_s1(15);

	Recover_On();
	delay_s1(11);
	delay_s1(90);
	Valve_detection();
	delay_s1(100);
	Recover_Off();

	delay_s1(15);

}

void VentingPipelinePressure()
{
	Venting_On();
	delay_s1(11);
//	solenoid_Voltage_Detection();
	delay_s1(90);
	Valve_detection();
	delay_s(2);
	Venting_Off();

	delay_s1(15);

	Recover_On();
	delay_s1(11);
	delay_s1(90);
	Valve_detection();
	delay_s(2);
	Recover_Off();

	delay_s1(15);
}

uint8_t Gpio_Input_Detection(GPIO_Type * gpio , uint8_t gpio_pin)
{
	if((gpio->PDIR & (1 << gpio_pin)) == 0)
	{
		delay_s1(1);
		if((gpio->PDIR & (1 << gpio_pin)) == 0)
		{
			delay_s1(1);
			if((gpio->PDIR & (1 << gpio_pin)) == 0)
			{
				delay_s1(1);
				if((gpio->PDIR & (1 << gpio_pin)) == 0)
				{
					delay_s1(1);
					if((gpio->PDIR & (1 << gpio_pin)) == 0)
					{
						delay_s1(1);
						if((gpio->PDIR & (1 << gpio_pin)) == 0)
						{
							return 1 ;
						}
						else
							return 0 ;
					}
					else
						return 0 ;
				}
				else
					return 0 ;
			}
			else
				return 0 ;
		}
		else
			return 0;
	}
	else
		return 0;
}

uint8_t Gpio_Inputl(GPIO_Type * gpio , uint8_t gpio_pin)
{
	if((gpio->PDIR & (1 << gpio_pin)))
	{
		delay_s1(1);
		if((gpio->PDIR & (1 << gpio_pin)))
		{
			delay_s1(1);
			if((gpio->PDIR & (1 << gpio_pin)))
			{
				delay_s1(1);
				if((gpio->PDIR & (1 << gpio_pin)))
				{
					delay_s1(1);
					if((gpio->PDIR & (1 << gpio_pin)))
					{
						delay_s1(1);
						if((gpio->PDIR & (1 << gpio_pin)))
						{
							return 1 ;
						}
						else
							return 0 ;
					}
					else
						return 0 ;
				}
				else
					return 0 ;
			}
			else
				return 0 ;
		}
		else
			return 0;
	}
	else
		return 0;
}

