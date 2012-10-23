/******************************************************************************
Filename    : NOFS_arch.c
Author      : pry
Date        : 06/16/2012
Description : The Functions Directly Operating The Hardwares Of The No Filesystem.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 24/06/2012 Ver1.0.0
  Created The File Only.
2.Modified By pry                                        On 24/06/2012 Ver1.1.0
  Completed The Main Functions.
3.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines,And Made The Module Preliminarily Configurable.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"	   
#include "NOFS_config.h"
#include "NOFS_globals.h"				//The sequence of include is fixed.
#include "..\..\filesystem.h"
/*End Includes****************************************************************/

/*Begin Function:_Sys_NOFS_Read_Data_From_Address******************************
Description : The Fucntion Directly Operating The Hardware.
			  Reads Data From A Certain Address.
Input       : u8* Data -The Data Obtained From A Certain Address.Receives Data.
	        : u8* Address -The Address To Read Data From.
Output      : u8 --If Successful,0;In Fact It Will Always Succeed.
******************************************************************************/
u8 _Sys_NOFS_Read_Data_From_Address(u8* Data, u8* Address)	  
{
	*Data=*Address;
	return(0);                                                    			   //Now It Will Only Succeed.
}
/*End Function:_Sys_NOFS_Read_Data_From_Address*******************************/

/*Begin Function:_Sys_NOFS_Write_Data_To_Address*******************************
Description : The Fucntion Directly Operating The Hardware.
			  Writes Data To A Certain Address.
Input       : u8* Data -The Data To Write To A Certain Address.
	        : u8* Address -The Address To Write Data To.
Output      : u8 -If Successful,0;In Fact It Will Always Succeed.
******************************************************************************/
u8 _Sys_NOFS_Write_Data_To_Address(u8* Data, u8* Address)	  
{
	*Address=*Data;
	return(0);                                                    			   //Now It Will Only Succeed.
}
/*End Function:_Sys_NOFS_Write_Data_To_Address********************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
