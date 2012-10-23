/******************************************************************************
Filename    : NOFS_defines.h
Author      : pry
Date        : 01/05/2012
Description : The Header Of The NO Filesystem Provided By The OS.For Declarations Only.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 23/06/2012 Ver1.0.0
  Created The File Only.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _NOFS_defines_h_
#define _NOFS_defines_h_

/*Defines*********************************************************************/
#define FILES_IN_FS       10        										   //The Maximum number Of Files In The Filesystem.
#define FILE_LENGTH       128	   								 			   //The maximum Filename length.In Bytes.

//Errors
#define NOFS_ENOFIL       0x01												   //The "Error_No_File" Error For "_Sys_NOFS_Fopen" In <NOFS.c>.
#define NOFS_ENOFOP		  0x01												   //The "Error_No_File_Operations" Error For "_Sys_NOFS_Fread","_Sys_NOFS_Fwrite","_Sys_NOFS_Frename","_Sys_NOFS_Fremove","_Sys_NOFS_Get_Fproperty" And "_Sys_NOFS_Set_Fproperty" In <NOFS.c>.
//End Errors
/*End Defines*****************************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
