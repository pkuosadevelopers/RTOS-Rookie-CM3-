/***************************************************
Filename    : RAMFS_typedefs.h
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
#ifndef _RAMFS_typedefs_h_
#define _RAMFS_typedefs_h_

/*********************Structs********************/
struct RAMFS_FILE_BLOCK									//One Of The Data Blocks Of RAMFS.
{	
	u8 File_ID;
	struct RAMFS_FILE_BLOCK* Prev_File_Block;			//The Pointer Pointing To The Previous Block.The Exact Point Of Pointing Is At The End Of Its Data Segment.											//The File ID(The Place Of The File In The Filesystem MFT.)				  
	u8 Data[BLOCK_SIZE-4-4-1];     						//The Data.
	struct RAMFS_FILE_BLOCK* Next_File_Block;			//The Pointer Pointing To The Next Block.The Exact Point Of Pointing Is At The Start Of Its Data Segment.	
	u8 Padding;                                         //The Padding Byte,Should Always Be 0x00.The Block Start Identifier.	
};

struct _RAMFS_MFT					   //The Master File table For RAMFS.
{
	s8 File_Name[MAX_NAME_LENGTH];
	u32 File_Size;                     //The Size Of The File.
	s8 Category[8];					   //Allows 8 Characters Only.Can Be Customized.use char type(s8).
	u8 Read_Write;					   //The R/W Mode. See "defines.h" for more details.
	s8 Author[8];					   //The Author Of The File.
	s8 Create_Time[6];                 //The Creation Time.
	s8 Last_Change_Time[6];			   //The Last Time That It Is Changed.
	struct RAMFS_FILE_BLOCK* First_File_Block;
};

struct RAMFS_File_Property{									   //Take Note That The "File_Size" Could Not Be Altered At will.
	s8 Category[8];					   //Allows 8 Characters Only.Can Be Customized.use char type(s8).
	u8 Read_Write;					   //The R/W Mode. See "defines.h" for more details.
	s8 Author[8];					   //The Author Of The File.
	s8 Create_Time[6];                 //The Creation Time.
	s8 Last_Change_Time[6];			   //The Last Time That It Is Changed.
};
/*****************END Structs*********************/

#endif
/************Preprocessor Control*********/

/******************End Of File************************/
