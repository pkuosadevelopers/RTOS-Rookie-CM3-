/******************************************************************************
Filename    : shell_arch.c
Author      : pry 
Date        : 24/07/2012
Description : The Shell Functions Related To The Hardware.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 24/07/2012 Ver1.0.0
  Created The File Only.
******************************************************************************/

/*Includes********************************************************************/
#include "defines.h"
#include "typedefs.h"
#include "externs.h"														   //The Sequence Of Include Is Fixed.
#include "filesystems_in_use.h"
/*End Includes****************************************************************/

/*Private Defines*************************************************************/
//Datamask
#define USART_DATA_MASK                       0xFF
//End Datamask

/*End Private Defines*********************************************************/

/*Private Global Variables****************************************************/

/*End Private Global Variables************************************************/

/*Private C Function Prototypes***********************************************/
static void _Sys_USART_Configuration(void);
/*End Private C Function Prototypes*******************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
