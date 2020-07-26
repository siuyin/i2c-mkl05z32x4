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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

LDD_TDeviceData* i2c;	// i2c logical device pointer.

// ActivateMMA8451QAccelerometer commands the device into active mode.
// Returns 0 if successful.
uint8_t ActivateMMA8451QAccelerometer(void) {
	const uint8_t addr = 0x1d; // only for documentation, this has already been defined in the Processor Expert Logical Device Driver.
	uint8_t cmd[2] = { 0x2a, 1 }; // 0x2a is the control register, 1 is to switch to active mode.

	return CI2C1_MasterSendBlock(i2c, cmd, 2, LDD_I2C_SEND_STOP);
}

int8_t AccelerometerData;
// ReadAccelerometerTask periodically reads the accelerometer and output the value.
void ReadAccelerometerTask(void) {
	static unsigned int nrt;	// next run tick
	enum stateT {
		ready, txBusy, txSent, rxBusy
	};
	// these are with respect to the i2c master.
	static enum stateT state;
	uint8_t reg;

	switch (state) {
	case ready:
		if (tick != nrt) {
			return;
		}
		nrt += 200;	// update every x ticks.

		reg = 0x5;	// z-axis most significant byte
		CI2C1_MasterSendBlock(i2c, &reg, 1, LDD_I2C_NO_SEND_STOP);
		state = txBusy;
		break;
	case txBusy:
		if (CI2C1_MasterGetBlockSentStatus(i2c)) { // All data sent
			state = txSent;
		}
		break;
	case txSent:
		CI2C1_MasterReceiveBlock(i2c, &AccelerometerData, 1, LDD_I2C_SEND_STOP);
		state = rxBusy;
		break;
	case rxBusy:
		if (CI2C1_MasterGetBlockReceivedStatus(i2c)) {	// all data received
			state = ready;
		}
		break;
	}
	return;
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

	ActivateMMA8451QAccelerometer();

	while (1) {
		ReadAccelerometerTask();
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
