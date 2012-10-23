/******************************************************************************
Filename    : NOFS.c
Author      : pry
Date        : 06/16/2012
Description : The No Filesystem Provided By The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 16/06/2012 Ver1.0.0
  Created The File Only.
2.Modified By pry                                        On 25/06/2012 Ver1.1.0
  Completed All Functions Preliminarily.
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

/*Begin Function:_Sys_NOFS_Low_Level_Format************************************
Description : Low-Level Format The Filesystem At The Assigned Address.For System Use.
Input       : void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem
              To Be Created.
Output      : u8 -Always 0.   
******************************************************************************/
u8 _Sys_NOFS_Low_Level_Format_FS(void* Absolute_Start_Address)
{
	Sys_Memset((u8*)Absolute_Start_Address,'\0',FILE_LENGTH*FILES_IN_FS);
	return(0);
}
/*Finish Function:_Sys_NOFS_Low_Level_Format**********************************/

/*Begin Function:_Sys_NOFS_Fopen***********************************************
Description : The "Fopen" Function Of NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8* Path -The Path Of The File.Like"A\Readme.txt"
              u8 Mode -The Open Mode.Dummy Here.
			  u8 IO_Buffer -The IO Buffer ID,Or The File Identifier.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -If Succeed,"0";else "NOFS_ENOFIL"(0x01).   
******************************************************************************/
u8 _Sys_NOFS_Fopen(u8* Path,u8 Mode,u8 IO_Buffer,void* Absolute_Start_Address)
{
	u8 IOBCB_Count;															   //See If The File Has Been Opened Once.

	for(IOBCB_Count=0;IOBCB_Count<MAX_FILE_OPEN;IOBCB_Count++)
	{																		   //That Means That The File Has Been Opened Once.
		if((IOBCB[IOBCB_Count].Root_Name==*Path)&&(IOBCB[IOBCB_Count].FID_In_FS==(*(Path+2))-0x30))
		return(NOFS_ENOFIL);
	}

	if(IOBCB[IO_Buffer].Root_Name==0)
	{
		IOBCB[IO_Buffer].Root_Name=*(Path);  								   //The Root's Name.
		IOBCB[IO_Buffer].Operation_PID=Current_PID;                            //The Master PID.
		IOBCB[IO_Buffer].FID_In_FS=(*(Path+2))-0x30;                           //The File ID In The Filesystem.
		IOBCB[IO_Buffer].File_Current_Ptr=(void*)((u32)Absolute_Start_Address+((*(Path+2))-0x30)*FILE_LENGTH);//The Pointer Will Be Kept On The Data.The Switch Of Pointers Will Be Handled By The System.
		IOBCB[IO_Buffer].File_Size=FILE_LENGTH;                                //The File Size.
		return(0);                                                             //The File Is Found.
	}																									
	return(NOFS_ENOFIL);													   //The Slot Is Already Filled. Open Failed.
}
/*Finish Function:Sys_NOFS_Fopen**********************************************/

/*Begin Function:_Sys_NOFS_Fread***********************************************
Description : The "Fread" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : void* Buffer -The Buffer To Output The Data.
              u8 Size -The Size Of Each Unit To Read Out.
			  u32 Count -The Number Of Units To Read Out
			  u8 IO_Buffer -The IO Buffer Of The Opened File.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -If Succeed,"0";else "NOFS_ENOFOP"(0x01).   
******************************************************************************/
u8 _Sys_NOFS_Fread(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address)
{
	u8* Buffer_Ptr=Buffer;
	u32 Total_Bytes;
	u8* Start_Address=(u8*)IOBCB[IO_Buffer].File_Current_Ptr;

	for(Total_Bytes=Size*Count;Total_Bytes>0;Total_Bytes--)
	{
		if(_Sys_NOFS_Read_Data_From_Address(Buffer_Ptr++,Start_Address++)==1)
		{
			return(NOFS_ENOFOP);	
		}																	   //Some Error Occurred While Reading.																   
	}

	IOBCB[IO_Buffer].File_Current_Ptr=(void*)((u32)IOBCB[IO_Buffer].File_Current_Ptr+(Size*Count));																  //Update The Pointer.			
	return(0);																   //Reading Completed.
}
/*Finish Function:Sys_NOFS_Fread**********************************************/

/*Begin Function:_Sys_NOFS_Fwrite**********************************************
Description : The "Fwrite" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : void* Buffer -The Buffer To Input The Data.
              u8 Size -The Size Of Each Unit To Write In.
			  u32 Count -The Number Of Units To Write In.
			  u8 IO_Buffer -The IO Buffer Of The Opened File.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -If Succeed,"0";else "NOFS_ENOFOP"(0x01).   
******************************************************************************/
u8 _Sys_NOFS_Fwrite(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address)
{
	u8* Buffer_Ptr=Buffer;
	u32 Total_Bytes;
	u8* Start_Address=(u8*)IOBCB[IO_Buffer].File_Current_Ptr;

	for(Total_Bytes=Size*Count;Total_Bytes>0;Total_Bytes--)
	{
		if(_Sys_NOFS_Write_Data_To_Address(Buffer_Ptr++,Start_Address++)==1)
		{
			return(NOFS_ENOFOP);	
		}																	   //Some Error Occurred While Reading.																   
	}

	IOBCB[IO_Buffer].File_Current_Ptr=(void*)((u32)IOBCB[IO_Buffer].File_Current_Ptr+Size*Count);																  //Update The Pointer.			
	return(0);																   //Reading Completed.
}
/*Finish Function:Sys_NOFS_Fwrite*********************************************/

/*Begin Function:_Sys_NOFS_Fclose**********************************************
Description : The "Fopen" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8 IO_Buffer -The IO Buffer ID,Or The File Identifier Of The File To Close.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output      : u8 -If Succeed,"0";In Fact It Will Always Succeed.   
******************************************************************************/
u8 _Sys_NOFS_Fclose(u8 IO_Buffer,void* Absolute_Start_Address)
{
		IOBCB[IO_Buffer].Root_Name=0;                                          //The Root's Name.
		IOBCB[IO_Buffer].Operation_PID=0;									   //Clear The Master PID.
		IOBCB[IO_Buffer].FID_In_FS=0;                                          //The File ID In The Filesystem.
		IOBCB[IO_Buffer].File_Current_Ptr=0;                                   //Clear Up The Table Completely.
		IOBCB[IO_Buffer].File_Size=0;                                          //The File Size.
		return(0);                                                             //It Always Return 0 Now.(Success.)
}
/*Finish Function:Sys_NOFS_Fclose*********************************************/

/*Begin Function:_Sys_NOFS_Fseek***********************************************
Description : The "Fseek" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8 IO_Buffer -The IO Buffer ID,Or The File Identifier Of The File To Operate.
			  s32 Offset -How Far To Move The Pointer.
			  u8 Position: -Where To Move The Pointer From.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output      : u8 -If Succeed,"0";In Fact It Will Always Succeed.   
******************************************************************************/
u8 _Sys_NOFS_Fseek(u8 IO_Buffer,s32 Offset,u8 Position,void* Absolute_Start_Address)
{
	switch(Position)
	{
		case SEEK_SET:IOBCB[IO_Buffer].File_Current_Ptr=(void*)(((u32)Absolute_Start_Address+IOBCB[IO_Buffer].FID_In_FS*FILE_LENGTH)+Offset);break;                                    //Seek From The Beginning.
		case SEEK_CUR:IOBCB[IO_Buffer].File_Current_Ptr=(void*)((u32)IOBCB[IO_Buffer].File_Current_Ptr+Offset);break;																								   //Seek From The Current Pointer.
		case SEEK_END:IOBCB[IO_Buffer].File_Current_Ptr=(void*)(((u32)Absolute_Start_Address+((IOBCB[IO_Buffer].FID_In_FS+1)*FILE_LENGTH)-1)+Offset);break;//Seek From The End Of The File.
	}
	return(0);																																                           //It Always Succeed Now.
		
}
/*Finish Function:Sys_NOFS_Fseek**********************************************/

/*Begin Function:_Sys_NOFS_Frename*********************************************
Description : The "Frename" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8* Path -The Path Of The File To Renamed.
			  u8* New_Name -The New Name Of The File,The Path Is Not Included.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -It Will Permanently Return "NOFS_ENOFOP"(0x01).The Filesystem Doesn't Support Variable Filename.   
******************************************************************************/
u8 _Sys_NOFS_Frename(u8* Path,u8* New_Name,void* Absolute_Start_Address)
{
	return(NOFS_ENOFOP);													   //The Filesystem Doesn't Support Variable Filename.
}
/*Finish Function:Sys_NOFS_Frename********************************************/

/*Begin Function:_Sys_NOFS_Fremove*********************************************
Description : The "Fremove" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8* Path -The Path Of The File To Removed.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -It Will Permanently Return "NOFS_ENOFOP"(0x01).The Filesystem Doesn't Support File Deletion.   
******************************************************************************/
u8 _Sys_NOFS_Fremove(u8* Path,void* Absolute_Start_Address)
{
	return(NOFS_ENOFOP);													   //The Filesystem Doesn't Support File Deletion.
}
/*Finish Function:Sys_NOFS_Fremove********************************************/

/*Begin Function:_Sys_NOFS_Get_Fproperty***************************************
Description : The "Get_Fproperty" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8* Path -The Path Of The File To Operate.
			  struct File_Property* Property -The Struct To Get File Properties.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -It Will Permanently Return "NOFS_ENOFOP"(0x01).The Filesystem Doesn't Support File Properties.   
******************************************************************************/
u8 _Sys_NOFS_Get_Fproperty(u8* Path,struct NOFS_File_Property* Property,void* Absolute_Start_Address)
{
	return(NOFS_ENOFOP);													   //The Filesystem Doesn't Support File Properties.
}
/*Finish Function:_Sys_NOFS_Get_Fproperty*************************************/

/*Begin Function:_Sys_NOFS_Set_Fproperty***************************************
Description : The "Set_Fproperty" Function For NOFS For System Use.(Don't Call It Directly In The User Application.)
Input       : u8* Path -The Path Of The File To Operate.
			  struct File_Property* Property -The Struct To Set File Properties.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem. 
Output     :  u8 -It Will Permanently Return "NOFS_ENOFOP"(0x01).The Filesystem Doesn't Support File Properties. 
******************************************************************************/
u8 _Sys_NOFS_Set_Fproperty(u8* Path,struct NOFS_File_Property* Property,void* Absolute_Start_Address)
{
	return(NOFS_ENOFOP);													   //The Filesystem Doesn't Support File Properties.
}
/*Finish Function:_Sys_NOFS_Set_Fproperty*************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
