/*--Task_HeaterOn.c
 *
 *  Author:			Gary J. Minden
 *	Organization:	KU/EECS/EECS 388
 *  Date:			March 3, 2016 (B60303)
 *
 *  Description:	Turns heater on for 0.5 seconds of every second.
 *  				The HeaterOn_H signal is PortG_0. The alternative
 *  				function is M0PWM4.
 *
 *  				We will also toggle Tiva Evaluation Board D2
 *
 */

#include	"inc/hw_ints.h"
#include	"inc/hw_memmap.h"
#include	"inc/hw_types.h"
#include	"inc/hw_uart.h"

#include	<stddef.h>
#include	<stdbool.h>
#include	<stdint.h>
#include	<stdarg.h>
#include    <math.h>

#include	"driverlib/sysctl.h"
#include	"driverlib/pin_map.h"
#include	"driverlib/gpio.h"
#include    "driverlib/pwm.h"

#include "Tasks/Task_ReportData.h"

#include	"FreeRTOS.h"
#include	"task.h"

#include	"stdio.h"

#define		TimeBase_mS		1000
#define     Toggle_Period   100
#define     DUTY            0.75
#define		OnTime_mS		500
#define		OffTime_mS		( TimeBase_mS - OnTime_mS )
#define     TestTime        500
#define     PULSE_AMT       300 // 25%

extern double ADC_RESULT;

#define     TARGET_TEMP    35
#define     STOP_THRESH    1
#define     START_THRESH   5

extern void Task_HeaterOn( void *pvParameters ) {

	ReportData_Item		theADCReport;
	int toggle = 0x00;
	int counter = 0;
	//
	//	Enable (power-on) PortG
	//
	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOG );

	//
	// Enable the GPIO Port N.
	//
	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPION );

	//
    // Configure GPIO_G to drive the HeaterOn_H.
    //
    GPIOPinTypeGPIOOutput( GPIO_PORTG_BASE, GPIO_PIN_0 );
    GPIOPadConfigSet( GPIO_PORTG_BASE,
    					GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );

	//
	// Configure GPIO_N to drive the Status LED.
	//
	GPIOPinTypeGPIOOutput( GPIO_PORTN_BASE, GPIO_PIN_0 );
	GPIOPadConfigSet( GPIO_PORTN_BASE,
						GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );

	bool temp_rising = false;
	while ( 1 ) {

		if(temp_rising){
			if(ADC_RESULT >= TARGET_TEMP - STOP_THRESH){
				GPIOPinWrite (GPIO_PORTG_BASE, GPIO_PIN_0, 0x00 );
				//PWMGenDisable(GPIO_PORTG_BASE, GPIO_PIN_0 );
				GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_0, 0x00 );
				temp_rising = false;
				toggle = 0x00;
				counter = 0;
			}else if((!toggle && counter == floor(Toggle_Period * DUTY)) || (toggle && counter == floor(Toggle_Period * (1 - DUTY)))){
				GPIOPinWrite (GPIO_PORTG_BASE, GPIO_PIN_0, toggle );
				GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_0, toggle );
				toggle = !toggle;
				counter = 0;
			}
			counter++;
		} else if(!temp_rising && ADC_RESULT < TARGET_TEMP - START_THRESH ){
			temp_rising = true;
			GPIOPinWrite (GPIO_PORTG_BASE, GPIO_PIN_0, 0x01 );
			GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_0, 0x01 );
		}
		theADCReport.TimeStamp = xPortSysTickCount;
		theADCReport.ReportName = 3;
		theADCReport.ReportValue_1 = counter;
		theADCReport.ReportValue_0 = temp_rising;

		xQueueSend( ReportData_Queue, &theADCReport, 0 );

		vTaskDelay( ( TestTime * configTICK_RATE_HZ ) / TimeBase_mS );
	}
}
