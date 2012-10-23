/******************************************************************************
Filename    : NOFS_typedefs.h
Author      : pry
Date        : 01/05/2012
Description : The Header Of The RAM Filesystem Provided By The OS.
                          For Declarations Only.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 01/05/2012 Ver1.0.0
  Created The File Only.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _NOFS_typedefs_h_
#define _NOFS_typedefs_h_

/*Structs*********************************************************************/
//struct NOFS_FILE														       //One Of The Files Of NOFS.
//{	
//	u8 Data[FILE_LENGTH];
//};
struct NOFS_File_Property
{									  
	u8 Null;																   //The Filesystem Don't Support File Property At All.
};
/*End Structs*****************************************************************/

#endif
/*Preprocessor Control********************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
