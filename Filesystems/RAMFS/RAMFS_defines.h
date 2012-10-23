/***************************************************
Filename    : RAMFS_externs.h
Author      : pry
Date        : 01/05/2012
Description : The Header Of The RAM Filesystem Provided By The OS.
                          For Declarations Only.
**************************************************/

/****************Version Information************
1.Created By pry                On 01/05/2012 Ver1.0.0
  Created The File Only.
*************************************************/
/************Preprocessor Control*********/
#ifndef _RAMFS_defines_h_
#define _RAMFS_defines_h_
/******************Defines******************/
#define MAX_FILE_IN_FS    128        //The Maximum number Of Files In The Filesystem.
#define MAX_NAME_LENGTH   8			 //The maximum Filename length.In Bytes.
#define BLOCK_SIZE        256        //The maximum Block Size,In Bytes.
#define MAX_BLOCK_NUMBER  1000       //The Maximum Block Number In The Filesystem.
#define EOF               0x00000000 //The "End Of File" Identifier.
/*****************END Defines***************/

#endif
/*******End Preprocessor Control*******/

/******************End Of File************************/
