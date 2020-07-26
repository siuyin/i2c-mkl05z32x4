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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

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
	LDD_TDeviceData* i2c = CI2C1_Init(NULL);

	signed char data;
	byte  err;
	byte buf[4];
	buf[0] = 0x2a;
	buf[1] = 1;
	err = CI2C1_MasterSendBlock(i2c, buf, 2, LDD_I2C_SEND_STOP); // turn on the device
	for (int i = 0; i < 1000; i++) {
		CI2C1_Main(i2c);
	}

	while (1) {
		buf[0] = 0x5;
		err = CI2C1_MasterSendBlock(i2c, buf, 1, LDD_I2C_NO_SEND_STOP);
		for (int i = 0; i < 100; i++) {
			CI2C1_Main(i2c);
		}
		err = CI2C1_MasterReceiveBlock(i2c, &data, 1, LDD_I2C_SEND_STOP);
		for (int i = 0; i < 100; i++) {
			CI2C1_Main(i2c);
		}
		for (int i = 0; i < 10000; i++) {
		}
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
