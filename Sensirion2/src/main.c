/*
 * Created on: 2019.7.29
 * Author: byxzwz
 * Version : Dryer1 - 29WDOG
 * 上一版本为 Dryer1 - 24WDOG
 * 更改内容：
 * 1. 当检测到故障 对应的故障计数++ ， 当故障计数> 次 ， LED循环闪烁且判断故障计数是否满足> 次 ， 当检测到故障计数< 次 ，
 * 退出LED循环闪烁
 * 2. 电磁阀开启电流判断和功率器件判断一起来判断是干扰还是电磁阀检测无电流
 * 3. 修改电流检测程序，当检测到无电流，判断是否是由于电磁断开引起的，如果是电磁阀断开，程序执行红色LED闪烁，不继续执行
 * 如果电磁阀连接正常，程序执行红色LED闪烁，重新开启电磁阀等到电磁阀开启电流正常，程序继续执行
 * 当检测到电磁阀开启电流过大，断开对应电磁阀，红色LED闪烁
 *
 */

#include "utility_function.h"
#include "fault_led.h"
#include "can.h"
#include "i2c.h"
#include "check.h"
#include "powercheck.h"
#include "systick.h"
#include "sht3x.h"

/* 切断压力：11±0.2bar
 * 恢复供气压力：8.5±0.1bar
 * 回流时间：20s
 * 若回流间隔>15分，同时气压>7bar，进行强制回流10s
 * 若回流间隔>25分，同时气压>6.5bar，进行强制回流10s
 * 加热器数据：
 * 			额定功率/电压：50W/24V DC
 * 			加热器启动温度：7℃±6℃
 * 			加热器断开温度：29.5℃±3℃
 * 			工作时电流：2A，不工作时0.03A
 */
//#define      VOLTAGE_CONTROL
#define      STANDARD_CONTROL

uint8_t test_data = 0 ;
uint8_t test_data_1 = 0 ;

int main(void)
{
    ft	temperature;        // temperature [°C]
    ft	humidity;	        // relative humidity [%RH]
    etError   error;        // error code

	System_Init();//系统Init
	Calculation() ;
	SHT3X_Init(0x44);
	task_ms(50);
	/*上电自检*/
	for(;;)
	{
		//Fun_AutoControl(2);
		SHT3X_GetTempAndHumi(&temperature, &humidity, REPEATAB_HIGH, MODE_CLKSTRETCH, 100);
	}
	return 0;
}

