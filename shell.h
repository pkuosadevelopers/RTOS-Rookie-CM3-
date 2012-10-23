 /*****************************************************************************
Filename    : shell.h
Author      : pry 
Date        : 17/07/2012
Description : The Shell For The Operating System.This Is Its Header.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 17/07/2012 Ver1.0.0
  Created The File Only.The File Is Still Very Messy.
******************************************************************************/

/*Includes********************************************************************/
#include "defines.h"
#include "typedefs.h"
#include "externs.h"														   //The Sequence Of Include Is Fixed.
#include "filesystems_in_use.h"
/*End Includes****************************************************************/

/*Defines*********************************************************************/
#define STR(x)               (u8*)(&(*(x)))
#define COMMAND_QUIT         -1
#define COMMAND_NOT_KNOWN	 -2	  
#define COMMAND_NONE         0x00
#define COMMAND_LS           0x01
#define COMMAND_PWD          0x02
#define COMMAND_DU           0x03
#define COMMAND_MKDIR        0x04
#define COMMAND_FIND         0x05
#define COMMAND_CP           0x06
#define COMMAND_MV           0x07
#define COMMAND_RM           0x08
#define COMMAND_EDIT         0x09
#define COMMAND_TOP          0x0A
#define COMMAND_SIGNAL       0x0B
#define COMMAND_RESTART      0x0C
/*End Defines*****************************************************************/

/*Private Global Variables****************************************************/
u8 Arguments[COMMAND_SPACE];
u8 Command[COMMAND_SPACE];
/*End Private Global Variables************************************************/

/*Private C Function Prototypes***********************************************/ 
static void _Sys_SH_Get_Command(u8* Command);
static s32 _Sys_SH_Parse_Command(u8* Command,void* Arguments);
static void _Sys_SH_Execute_Command(s32 Function_ID,void* Arguments);

static void _Sys_SH_Unknown_Command(void);
static void _Sys_SH_ls(void* Arguments);
static void _Sys_SH_pwd(void* Arguments);
static void _Sys_SH_du(void* Arguments);
static void _Sys_SH_mkdir(void* Arguments);
static void _Sys_SH_find(void* Arguments);
static void _Sys_SH_cp(void* Arguments);
static void _Sys_SH_mv(void* Arguments);
static void _Sys_SH_rm(void* Arguments);
static void _Sys_SH_edit(void* Arguments);
static void _Sys_SH_top(void* Arguments);
static void _Sys_SH_signal(void* Arguments);
static void _Sys_SH_restart(void* Arguments);
/*End Private C Function Prototypes*******************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
