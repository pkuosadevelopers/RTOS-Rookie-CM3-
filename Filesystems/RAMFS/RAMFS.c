/***************************************************
Filename    : RAMFS.c
Author      : pry
Date        : 07/04/2012
Description : The RAM Filesystem Provided By The OS.
**************************************************/

/****************Version Information************
1.Created By pry                On 01/05/2012 Ver1.0.0
  Created The File Only.
2.Modofied By pry               On 02/05/2012 Ver1.1.0
  Created The Functions Necessary For The Filesystem.
  Updated The Relationship Tree.
*************************************************/

/***************Includes*************************/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "RAMFS_config.h"
#include "RAMFS_globals.h"				//The sequence of include is fixed.
#include "..\..\filesystem.h"
/****************END Includes*********************/

/*********Begin Function:_Sys_RAMFS_Low_Level_Format********
Description : Low-Level Format The Filesystem At The Assigned Address.For System Use.
Input       : u32 Absolute_Start_Address -The Absolute Start Address Of The Filesystem
              To Be Created.
Output      : Void.   
***********************************************************/
void _Sys_RAMFS_Low_Level_Format_FS(u32* Absolute_Start_Address)
{
	Sys_Memset(Absolute_Start_Address,0,sizeof(struct _RAMFS_MFT)*MAX_FILE_IN_FS+sizeof(struct RAMFS_FILE_BLOCK)*MAX_BLOCK_NUMBER);
}
/*********Finish Function:_Sys_RAMFS_Low_Level_Format*******/

/*************Begin Function:_Sys_RAMFS_Fopen*******
Description : The "Fopen" Function For RAMFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8* Path -The Path Of The File.Like"A\Readme.txt"
              u8 Mode -The Open Mode.Must Be The Following Or Their Combination.
			  See "defines.h" for more details.
#define       READ       0x01
#define       WRITE      0x02
#define       ADD        0x04
#define       CREATE     0x08		//(Other Bits Are Reserved)

			  u8 IO_Buffer -The IO Buffer ID,Or The File Identifier.
			  u32 Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -If Succeed,"0";else "1".   
**************************************************/
u8 _Sys_RAMFS_Fopen(u8* Path,u8 Mode,u8 IO_Buffer,u32 Absolute_Start_Address)
{
	u8 MFT_Count=0,Create_MFT_Count=0,Name_Count=0,File_Block_Count=0;
	s32 Name=0;
	struct _RAMFS_MFT (*MFT_Ptr)[MAX_FILE_IN_FS]=(struct _RAMFS_MFT (*)[MAX_FILE_IN_FS])Absolute_Start_Address;		//Assign The Address.
	//struct _RAMFS_MFT (*MFT_Ptr)=(struct _RAMFS_MFT (*))Absolute_Start_Address;		//Assign The Address.
	struct RAMFS_FILE_BLOCK (*Block_Ptr)[MAX_BLOCK_NUMBER]=(struct RAMFS_FILE_BLOCK (*)[MAX_BLOCK_NUMBER])(Absolute_Start_Address+MAX_FILE_IN_FS*sizeof(struct _RAMFS_MFT)); 
	for(MFT_Count=0;MFT_Count<MAX_FILE_IN_FS;MFT_Count++)
	{
		if(Sys_Strcmp(((u8*)(*MFT_Ptr)[MFT_Count].File_Name),Path+2,LONGEST_PATH-2)==0)	                                                     //Compare The File Name And Find It Out.
		{	
			//Now The MFT_Count Contains The File ID(Or The Place Of File In The MFT) In The Filesystem.
			IOBCB[IO_Buffer].Root_Name=*(Path);                                                                       //The Root's Name.
			IOBCB[IO_Buffer].FID_In_FS=MFT_Count;                                                                     //The File ID In The Filesystem.
			IOBCB[IO_Buffer].File_Current_Ptr=(u32*)((*MFT_Ptr)[MFT_Count].First_File_Block+sizeof(struct RAMFS_FILE_BLOCK*)+sizeof(u8)); //The Pointer Will Be Kept On The Data.The Switch Of Pointers Will Be Handled By The System.
			IOBCB[IO_Buffer].File_Size=(*MFT_Ptr)[MFT_Count].File_Size;                                                  //The File Size.
			IOBCB[IO_Buffer].Buffer=(u32*)_Sys_Malloc(FILE_BUFFER_LEN);                                               //The Buffer Pointer.	//if(IOCB[IO_Buffer].Buffer=(u32*))..Now We Suggest That The memory Allocation Is Successful.
			IOBCB[IO_Buffer].Buffer_Start=IOBCB[IO_Buffer].Buffer;	                                                 //The Buffer Start Address.
			IOBCB[IO_Buffer].Buffer_End=IOBCB[IO_Buffer].Buffer+FILE_BUFFER_LEN;                                       //The Buffer End Address. 
			return(0);                                                                                               //The File Is Found.
		}

		if((MFT_Count==MAX_FILE_IN_FS)&&((Mode&CREATE_FILE)!=0))                                                          //The File Does Not Exist,And There Is "CREATE".
		{
			for(MFT_Count=0;MFT_Count<MAX_FILE_IN_FS;MFT_Count++)			 //Search For An Empty Slot To Put The File In.
			{
				if((*MFT_Ptr)[MFT_Count].File_Size==0)							 //There Must Be No File In It.
				{	
					for(Name_Count=0;Name_Count<MAX_FILE_IN_FS;Name_Count++)	  //The New File Name Will Be A Integer.Find One That Is Unused.
					{	
						for(Create_MFT_Count=0;Create_MFT_Count<MAX_FILE_IN_FS;Create_MFT_Count++)
						{
							Sys_Sscanf((*MFT_Ptr)[MFT_Count].File_Name,"%d",&Name,0);
						    if(Name==Name_Count)
								break;											  //That Integer Can't Be Used As a Name.
						}
						if(Create_MFT_Count==MAX_FILE_IN_FS)					  //This Indicates That The Whole Table Has Been Gone Through,And The name Will Be Unique.
							break;
					}

					if(Name_Count==MAX_FILE_IN_FS)								 //There is No Vacancy,The Creation Failed.
						return(1); 
					for(File_Block_Count=0;File_Block_Count<MAX_BLOCK_NUMBER;File_Block_Count++)
					{
						if((*Block_Ptr)[File_Block_Count].File_ID==0)				 //There Is No File In This Slot.
						{
							(*Block_Ptr)[File_Block_Count].File_ID=MFT_Count;       //Fill The File ID.
							(*Block_Ptr)[File_Block_Count].Prev_File_Block=EOF;
							(*Block_Ptr)[File_Block_Count].Next_File_Block=EOF;	 //Now The File Is merely Created,It Has Only One Block.
							break;
						}
							 	
					} 
					if(File_Block_Count==MAX_BLOCK_NUMBER) 
						return(1);                              				//There Is No Empty File Block Available.Abort.
					Sys_Sprintf((s8*)(*MFT_Ptr)[MFT_Count].File_Name,"%d",&Name,0,0);       //The Temporary File Name.An Unused Number. 
					(*MFT_Ptr)[MFT_Count].File_Size=0;
					(*MFT_Ptr)[MFT_Count].Read_Write=READ|WRITE;
					(*MFT_Ptr)[MFT_Count].First_File_Block=&((*Block_Ptr)[File_Block_Count])+1+4;//Point To The Data Segment.

					IOBCB[IO_Buffer].Root_Name=*(Path);                                                                       //The Root's Name.
					IOBCB[IO_Buffer].FID_In_FS=MFT_Count;                                                                     //The File ID In The Filesystem.
					IOBCB[IO_Buffer].File_Current_Ptr=(u32*)((*MFT_Ptr)[MFT_Count].First_File_Block+sizeof(struct RAMFS_FILE_BLOCK*)+sizeof(s8));//The Pointer Will Be Kept On The Data.The Switch Of Pointers Will Be Handled By The System.
					IOBCB[IO_Buffer].File_Size=(*MFT_Ptr)[MFT_Count].File_Size;                                                  //The File Size.
					IOBCB[IO_Buffer].Buffer=(u32*)_Sys_Malloc(FILE_BUFFER_LEN);                                               //The Buffer Pointer.	//if(IOCB[IO_Buffer].Buffer=(u32*))..Now We Suggest That The memory Allocation Is Successful.
					IOBCB[IO_Buffer].Buffer_Start=IOBCB[IO_Buffer].Buffer;	                                                 //The Buffer Start Address.
					IOBCB[IO_Buffer].Buffer_End=IOBCB[IO_Buffer].Buffer+FILE_BUFFER_LEN;										 //The Buffer End Address. 
					return(0);
				}
													 
				if(MFT_Count==MAX_FILE_IN_FS-1)      //There is No Empty Slots.
				{
					return(1);						 //The Operation Failed.
				}                                 
			}
				                                                          
		}
	}
	return(1);                      						 //Now The File Is Not Found,And It's Not Allowed To Create Anyone.Failure.
}
/********Finish Function:Sys_RAMFS_Fopen**********/

/******************End Of File************************/
