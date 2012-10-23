/******************************************************************************
Filename    : NOFS_globals.h
Author      : pry
Date        : 16/06/2012
Description : The Header Of The No Filesystem Provided By The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 16/06/2012 Ver1.0.0
  Created The File Only.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _NOFS_globals_h_
#define _NOFS_globals_h_

/*Includes********************************************************************/
#include "NOFS_defines.h"
#include "NOFS_typedefs.h"
/*End Includes****************************************************************/

/*Global Variables************************************************************/

/*End Global Variables********************************************************/

/*C Function Prototypes*******************************************************/
//From NOFS.c
u8 _Sys_NOFS_Low_Level_Format_FS(void* Absolute_Start_Address);
u8 _Sys_NOFS_Fopen(u8* Path,u8 Mode,u8 IO_Buffer,void* Absolute_Start_Address);
u8 _Sys_NOFS_Fread(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);
u8 _Sys_NOFS_Fwrite(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);
u8 _Sys_NOFS_Fclose(u8 IO_Buffer,void* Absolute_Start_Address);
u8 _Sys_NOFS_Fseek(u8 IO_Buffer,s32 Offset,u8 Position,void* Absolute_Start_Address);
//u8 _Sys_NOFS_Frename(u8* Path,u8* New_Name,void* Absolute_Start_Address);
//u8 _Sys_NOFS_Fremove(u8* Path,void* Absolute_Start_Address);
//u8 _Sys_NOFS_Get_Fproperty(u8* Path,struct File_Property* Property,void* Absolute_Start_Address);
//u8 _Sys_NOFS_Set_Fproperty(u8* Path,struct File_Property* Property,void* Absolute_Start_Address);
//End From NOFS.c

//From NOFS_Arch.c
u8 _Sys_NOFS_Read_Data_From_Address(u8* Data, u8* Address);
u8 _Sys_NOFS_Write_Data_To_Address(u8* Data, u8* Address);	
//End From NOFS_Arch.c
/*End C Function Prototypes***************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
