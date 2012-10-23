/******************************************************************************
Filename   : kernel.h
Author     : pry
Date       : 07/04/2012
Description: The Header For The Kernel Of The Operating System.
             The kernel Can Swap The Tasks,Allocate Memory,So They Can
			 Run Properly.
			 It Works As A Processor manager.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 07/04/2012 Ver1.0.0
  Just Created It.
2.Modified By pry                                        On 25/04/2012 Ver1.1.0
  Standardized it.Better Looks.
3.Modified By pry                                        On 02/05/2012 Ver2.0.0
  Added The Preprocessor Control String.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _kernel_h_
#define _kernel_h_							                                   //We Can't Use '.' In The Preprocessor Control Strings.

/*Includes********************************************************************/
#include "defines.h"		                                                   //The Sequence Of Include Is Fixed.
#include "typedefs.h"
#include "globals.h"		                                                   //The "globals.h" is only used here.Use "externs.h" in other cases.
#include "filesystems_in_use.h"
/*End Includes****************************************************************/

/*Private C Function Prototypes***********************************************/
static void _Sys_Memory_Init(u32 Start,u32 End);	                           //Initialize Memory.
static void _Sys_Init(void);							                       //The Init Process.
static void _Sys_Arch(void);						                           //The Arch Process,Which Does Some Chores.
static void _Sys_Kernel_Load(u32 SP);                                          //The Kernel Loading Function.
static void _Sys_Proc_Load(void* Task,u32 SP,u8 PID,u32 Ticks,u8* Name);       //The Process Loading Function.
static void _Sys_Process_Initialization(u8 PID);                               //The Process Stack Initialization Function.
static void _Sys_Systick_Init(u32 Ticks);                                      //Initialize The Systick Timer.
static void _Sys_Start(void);                                                  //The Function To Start The Operating System.
/*End Private C Function Prototypes*******************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
