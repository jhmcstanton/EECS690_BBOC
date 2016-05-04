//*****************************************************************************
//
//	Set up ADC_0 Channel_0 to sample voltage and report.
//
//		Author: 		Gary J. Minden
//		Organization:	KU/EECS/EECS 388
//		Date:			2016-02-29 (B60229)
//		Version:		1.0
//
//		Description:	Sample ADC_0_Channel_0 every 0.5 seconds
//
//		Notes:
//
//*****************************************************************************
//

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include "driverlib/sysctl.h"
#include "driverlib/adc.h"


#include "FreeRTOS.h"
#include "task.h"
//#include "queue.h"

#include "stdio.h"

#include "Tasks/Task_ReportData.h"

#define ADC_QUEUE_LENGTH 5

extern double ADC_RESULT;

//
// Define the ReportData Queue
//
//extern QueueHandle_t adc_queue = NULL;

//
//	Gloabal subroutines and variables
//

extern void Task_Simple_ADC0_Ch0( void *pvParameters ) {

	double adc_queue[ADC_QUEUE_LENGTH];
	size_t adc_index = 0;

	//
	//	Measured voltage value
	//
	uint32_t	ADC_Value;

	ReportData_Item		theADCReport;

	//
	//	Enable (power-on) ADC0
	//
	SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0 );

	//
	// Enable the first sample sequencer to capture the value of channel 0 when
	// the processor trigger occurs.
	//
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

	ADCSequenceStepConfigure( ADC0_BASE, 0, 0,
								ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0 );

	ADCSequenceEnable( ADC0_BASE, 0 );

	while ( 1 ) {

		//
		// Trigger the sample sequence.
		//
		ADCProcessorTrigger(ADC0_BASE, 0);

		//
		// Wait until the sample sequence has completed.
		//
		while( !ADCIntStatus( ADC0_BASE, 0, false )) {
		}

		//
		// Read the value from the ADC.
		//
		ADCSequenceDataGet(ADC0_BASE, 0, &ADC_Value);
		ADCIntClear( ADC0_BASE, 0 );

		// sweet conversion
		double vtemp = 91.93067 - 30.455 *(((double) ADC_Value) * 3.3) / 4096;
		adc_queue[adc_index % ADC_QUEUE_LENGTH] = vtemp;
		//UARTprintf("ADC Value: %d\n", ADC_Value);


		int i;
		ADC_RESULT = 0;
		for(i = 0; i < ADC_QUEUE_LENGTH; i++){
			ADC_RESULT += adc_queue[i];
		}
		ADC_RESULT = ADC_RESULT / ADC_QUEUE_LENGTH;
		adc_index++;
		//UARTprintf("ADC Result: %d\n", ADC_RESULT);


		//
		//	Print ADC_Value
		//
//		printf( ">>ADC_Value: %d\n", ADC_Value );

		//
		//	Delay one (1) second.
		//

		theADCReport.TimeStamp = xPortSysTickCount;
		theADCReport.ReportName = 2;
		theADCReport.ReportValue_0 = ADC_RESULT * 10;
		theADCReport.ReportValue_1 = vtemp * 10;

		xQueueSend( ReportData_Queue, &theADCReport, 0 );

		vTaskDelay((1000 * configTICK_RATE_HZ) / 1000 );
	}
}
