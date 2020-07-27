/* ###################################################################
 **     Filename    : main.c
 **     Project     : gpiojunk
 **     Processor   : MKL05Z32VLF4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2020-07-25, 22:00, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "CI2C1.h"
#include "Bit1.h"
#include "SysTick.h"
#include "AS1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdio.h>
#include <string.h>

LDD_TDeviceData* uart0;

// ActivateMMA8451QAccelerometer commands the device into active mode.
// Returns 0 if successful.
uint8_t ActivateMMA8451QAccelerometer(void) {
	const uint8_t addr = 0x1d; // only for documentation, this has already been defined in the Processor Expert Logical Device Driver.
	uint8_t cmd[2] = { 0x2a, 0x3 }; // 0x2a is the control register, 3 is to switch to active mode and enable fast-read byte only transfers for x,y and z registers.

	return CI2C1_MasterSendBlock(i2c, cmd, 2, LDD_I2C_SEND_STOP);
}

// ReadAccelerometerTask periodically reads the accelerometer and output the value.
void ReadAccelerometerTask(void) {
	static unsigned int nrt;	// next run tick
	if (tick != nrt) {
		return;
	}
	nrt += 100;	// update every x ticks.

	uint8_t reg = 0x1;	// x-axis most significant byte, then fast-read 0x3 (y) and 0x5 (z) registers
	CI2C1_MasterSendBlock(i2c, &reg, 1, LDD_I2C_NO_SEND_STOP);
	while (!CI2C1_MasterGetBlockSentStatus(i2c)) { // wait for data to be sent
	}
	CI2C1_MasterReceiveBlock(i2c, &(AccelerometerData), 3, LDD_I2C_SEND_STOP);
}

void OutputSerialTask(void) {
	static unsigned int nrt;
	if (tick != nrt) {
		return;
	}
	nrt += 500;

	static char buf[40];
	snprintf(buf, 39, "(%+4d,%+4d,%+4d)\r\n", AccelerometerData.X,
			AccelerometerData.Y, AccelerometerData.Z);
	AS1_SendBlock(uart0, buf, strlen(buf));
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */

	redLED = Bit1_Init(NULL);
	i2c = CI2C1_Init(NULL);
	uart0 = AS1_Init(NULL);

	ActivateMMA8451QAccelerometer();

	while (1) {

		ReadAccelerometerTask();
		OutputSerialTask();

	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
