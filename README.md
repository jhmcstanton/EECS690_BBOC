# EECS 690 Black Box Oven Controller


## Summary

This attempts to regulate temperature inside a black box oven by
reading temperature from a thermistor and powering a power resistor
(to generate heat) using a MOSFET as a switch. 

## Technology

The code is run on a [TI Tiva TM4C1294
Board](http://www.ittc.ku.edu/~gminden/Advanced_Embedded_Systems/PDFs/TI_TM4C1294NCPDT.pdf)
and utilizes [Free RTOS](http://www.freertos.org/) for task
management.

The controller in the oven utilizes a [Vishay
Thermistor](http://www.ittc.ku.edu/~gminden/Advanced_Embedded_Systems/PDFs/Project_01_Datasheets/Vishay_Thermistor_ntcle100.pdf)
and a [Vishay Power
MOSFET](http://www.ittc.ku.edu/~gminden/Advanced_Embedded_Systems/PDFs/Project_01_Datasheets/Vishay_PowerMOSFET_sihf510.pdf).

