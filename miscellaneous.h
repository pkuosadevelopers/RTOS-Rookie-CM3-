/******************************************************************************
Filename    : miscellaneous.h
Author      : pry
Date        : 25/04/2012
Description : The Miscellaneous Part For The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 29/04/2012 Ver1.0.0
   Moved The "Sys_Delay" to here from kernel.c.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _miscellaneous_h_
#define _miscellaneous_h_

/*Includes********************************************************************/
#include "defines.h"
#include "typedefs.h"
#include "externs.h"
#include "filesystems_in_use.h"
/*End Includes****************************************************************/

/*Private C Function Prototypes***********************************************/
static u8 _Sys_Query_Positive_Int_Highest(s32 Integer);
static s32 _Sys_Sci(s32 Exp);
/*End Private C Function Prototypes*******************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
