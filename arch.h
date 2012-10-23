/******************************************************************************
Filename    : arch.h
Author      : pry 
Date        : 22/07/2012
Description : The Include Files Concerned With The Architecture.It Also Contains 
              The Basic Types,Which Should Not Be Included In The "typedefs.h".
			  The File Shall Be DIRECTLY Included In The C Files Separately.
******************************************************************************/

/*Version Information**********************************************************
1.Created By  pry                                        On 22/07/2012 Ver1.0.0
  Created The File To Standardize The Operating System.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _arch_h_
#define _arch_h_


/*Basic Types*****************************************************************/
#if(DEFINE_BASIC_TYPES==TRUE)
typedef int  s32;
typedef short s16;
typedef char  s8;

typedef const int sc32; 
typedef const short sc16; 
typedef const char sc8;   

typedef volatile int32_t  vs32;
typedef volatile int16_t  vs16;
typedef volatile int8_t   vs8;

typedef volatile const int32_t vsc32; 
typedef volatile const int16_t vsc16; 
typedef volatile const int8_t vsc8;  

typedef unsigned int  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef const unsigned int uc32;  
typedef const unsigned short uc16;  
typedef const unsigned char uc8;   

typedef volatile unsigned int  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile const unsigned int vuc32;
typedef volatile const unsigned short vuc16;  
typedef volatile const unsigned char vuc8;  
#endif
/*End Basic Types*************************************************************/

/*Includes********************************************************************/
#include "stm32f10x_conf.h"
/*End Includes****************************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
