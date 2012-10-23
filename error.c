/******************************************************************************
Filename   : error.c
Author     : pry
Date       : 25/04/2012
Description: The Error Reporting Part For The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/04/2012 Ver1.0.0
  Created The Basic Functions."HardFault_Handler" Is Not Mature Now.
2.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "error.h"
/*End Includes****************************************************************/

/*Begin Function:Sys_Restart***************************************************
Description : Restarts The OS When A Hard Fault Occurs.
Input       : Void.
Output      : Void.
******************************************************************************/
void Sys_Restart(void)
{
	DISABLE_SYSTICK();	                 									   //Stop The Counter First,Prepare For A Restart.
	DISABLE_ALL_INTS();
	
	NVIC_AIRCR=RESET_VALUE;													//Perform A Reset Of The System.It Will Not Reset The Peripherals
	//NVIC_AIRCR=RESTART_VALUE;												   //Perform A Total Restart.
	
	while(1);
}
/*Finish Function:Sys_Restart*************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
