/******************************************************************************
Filename    : error.h
Author      : pry
Date        : 25/06/2012
Description : The Header Of The Error Reporting Part For The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/06/2012 Ver1.0.0
  Created The File Only.
2.Modified By pry                                        On 23/07/2012 Ver2.0.0
  The Lines Were Aligned And A Severe Mistake On The Preprocessor Control 
  Was Fixed.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _error_h_
#define _error_h_

/*Includes********************************************************************/
#include "defines.h"
#include "typedefs.h"
#include "externs.h"
#include "filesystems_in_use.h"
/*End Includes****************************************************************/

/*Private Defines*************************************************************/
#define NVIC_AIRCR		*((vu32*)0xE000ED0C)
#define RESET_VALUE     0x05FA0001
#define RESTART_VALUE   0x05FA0004
/*End Private Defines*********************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
