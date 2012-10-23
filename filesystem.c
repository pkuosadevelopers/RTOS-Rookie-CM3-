 /*****************************************************************************
Filename    : filesystem.c
Author      : pry
Date        : 01/05/2012
Description : The Filesystem Managing Module For The OS.The Filesystem Is A 
              Virtual One.Thus,It Can Support Many Other FS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 01/05/2012 Ver1.0.0
  Only Added 2 Basic Functions.
2.Modified By pry                                        On 24/06/2012 Ver1.1.0
  Modified The "Sys_Fopen" Function.
  Finished The "Sys_Fread","Sys_Fwrite","Sys_Fclose"&"Sys_Fseek" Function.
3.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Added The Config Reporter.
4.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines,And Made The Module Configurable.
5.Modified By pry                                        On 26/07/2012 Ver2.1.0
  Fixed A Severe Error In "_Sys_Close_All_Files".
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "filesystem.h"
/*END Includes****************************************************************/

/*Config Reporter*************************************************************/
#if(REPORT_CONFIG==TRUE)
	#if(ENABLE_FILESYSTEM==TRUE)
		#if(ENABLE_MEMORY_MANAGEMENT==FALSE)
			#error Please Enable The Memory Management Module First.
		#endif

		#if(ENABLE_RESOURCE==FALSE)
			#error Please Enable The Resource Management Module First.
		#endif

		#if((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_RESOURCE==TRUE))
			#warning The Filesystem Management Module Is Enabled.
		#endif 
	#else
		#warning The Filesyetem Management Module Is Disabled.
	#endif
#endif
/*End Config Reporter*********************************************************/

/*Begin Function:Sys_Mount_FS**************************************************
Description : Mount The Filesystem.
Input       : u8 Root -The Root Folder Identifier,Capital Characters Only,Like 'A'.
			  void* Absolute_Start_Address -The Absolute Start Address Of The Filesystem.
              u8 Resource_ID -The Resource To Mount As Filesystem.Must Be EXISTING,DEV_W,DEV_R,
			  And One Of The Below:(See defines.h)
			  #define SRAM		   0x01					 //Static RAM.
			  #define DRAM         0x02  				 //Dynamic RAM.

			  #define SERIAL_FLASH 0x03					 //SPI&IIC.
			  #define NOR_FLASH	   0x04					 //Nor Flash.
              #define NAND_FLASH   0x05					 //Nand Flash.
Output      : u8 -If Successful,Return The FS Identifier(A Number),Otherwise "EFSMNT"(0xFF).
******************************************************************************/
#if(ENABLE_FILESYSTEM==TRUE)
u8 Sys_Mount_FS(u8 Root,void* Absolute_Start_Address,u8 Resource_ID,struct File_Operations* Operations)
{
	u8 FS_Count=0;
	u8 FS_ID=EFSMNT;
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();
	for(FS_Count=0;FS_Count<MAX_FS_IN_SYS;FS_Count++)
	{
		if(FSCB[FS_Count].Root_Name==Root)
		{
			ENABLE_ALL_INTS();
			ENABLE_SYSTICK();
			return(FS_ID);                                                     //That Indicates That Two Filesystems Are Mounted Under The Same Root.Abort. 
		}
	}

	for(FS_Count=0;FS_Count<MAX_FS_IN_SYS;FS_Count++)
	{
		
		if(FSCB[FS_Count].Root_Name==0)                                        //If It Is "0",There Must Be No Filesystem.
		{	
			if(_Sys_Occupy_Resource(Resource_ID,1)==1)
				break;												           //That Suggests That The Resource Occupy Is failed.
			FSCB[FS_Count].Root_Name=Root;
			FSCB[FS_Count].Resource_ID=Resource_ID;					           //Fill In The Resource ID,Prepare For Unmounting.
			FSCB[FS_Count].Absolute_Start_Address=Absolute_Start_Address;
			//FSCB[FS_Count].In_Use_PID[0]=Current_PID;	                       //It Just Mounted The Filesystem. Doesn't Mean That It Is Using It.
			FSCB[FS_Count].Sys_Inner_Fopen=Operations->Sys_This_FS_Fopen;	   //Now The FS_Count Has The Empty Slot.
			FSCB[FS_Count].Sys_Inner_Fread=Operations->Sys_This_FS_Fread;
    		FSCB[FS_Count].Sys_Inner_Fwrite=Operations->Sys_This_FS_Fwrite;
			FSCB[FS_Count].Sys_Inner_Fclose=Operations->Sys_This_FS_Fclose;
			FSCB[FS_Count].Sys_Inner_Fseek=Operations->Sys_This_FS_Fseek;	   //There Is No "Fcopy" Because It Can Be Substituted By "Fopen+CREATE" And "Fread" And "Fwrite".
			FSCB[FS_Count].Sys_Inner_Fremove=Operations->Sys_This_FS_Fremove;
			FSCB[FS_Count].Sys_Inner_Frename=Operations->Sys_This_FS_Frename;
			FSCB[FS_Count].Sys_Inner_Get_Fproperty=Operations->Sys_This_FS_Get_Fproperty;
			FSCB[FS_Count].Sys_Inner_Set_Fproperty=Operations->Sys_This_FS_Set_Fproperty;
			FSCB[FS_Count].Sys_Inner_Format=Operations->Sys_This_FS_Format;
			FS_ID=FS_Count;													
			break;
		}
	}

	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();
	return(FS_ID); 
}
#endif
/*Finish Function:Sys_Mount_FS************************************************/

/*Begin Function:Sys_Unmount_FS************************************************
Description : Free The Allocated Memory.For Application Use.
Input       : u8 FS_ID -The Filesystem ID Returned By The "Sys_Mount_FS".
Output      : u8 -If Successful,Return"0",else "ENOUMT"(0x01).
******************************************************************************/
#if(ENABLE_FILESYSTEM==TRUE)
u8 Sys_Unmount_FS(u8 FS_ID)
{
	u8 In_Use_PID_Count=0;
	DISABLE_SYSTICK();	                                             		   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	
	 
	if((u32)FSCB[FS_ID].Sys_Inner_Fopen==0)
	{																           //The Filesystem is Not Mounted In Fact.
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOUMT); 
	}

	for(In_Use_PID_Count=0;In_Use_PID_Count<MAX_PID_USE_FS;In_Use_PID_Count++)
	{	
		if(FSCB[FS_ID].In_Use_PID[In_Use_PID_Count]!=0)						   //If Not All "0",There's Still Process Using The Filesystem.
		{
			ENABLE_ALL_INTS();
			ENABLE_SYSTICK();
			return(ENOUMT);
		}
	}
		
	_Sys_Release_Resource(FSCB[FS_ID].Resource_ID,1);                          //Release The Resource Used.

	FSCB[FS_ID].Root_Name=0;
	FSCB[FS_ID].Resource_ID=0;	
	FSCB[FS_ID].Absolute_Start_Address=0;     
	FSCB[FS_ID].Sys_Inner_Fopen=0;			                                   //Clear The Table Totally.
	FSCB[FS_ID].Sys_Inner_Fread=0;
  	FSCB[FS_ID].Sys_Inner_Fwrite=0;
	FSCB[FS_ID].Sys_Inner_Fclose=0;
	FSCB[FS_ID].Sys_Inner_Fseek=0;
	FSCB[FS_ID].Sys_Inner_Fremove=0;
	FSCB[FS_ID].Sys_Inner_Frename=0;
	FSCB[FS_ID].Sys_Inner_Get_Fproperty=0;
	FSCB[FS_ID].Sys_Inner_Set_Fproperty=0;
	FSCB[FS_ID].Sys_Inner_Format=0;

	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();
	return(0); 
}
#endif
/*Finish Function:Sys_Unmount_FS**********************************************/

/*Begin Function:Sys_Low_Level_Format_FS***************************************
Description : Low-Level Format The Filesystem At The Assigned Address.
Input       : u8 Root -The Root Of The Mounted Filesystem.
Output      : u8 -If Succeed,"0".else "ENOFSY"(0x01). 
******************************************************************************/
#if(ENABLE_FILESYSTEM==TRUE)
u8 Sys_Low_Level_Format_FS(u8 Root)
{
	u8 FSCB_Count=0,Find_Flag=0;
	u8 (*Here_Sys_Format)(void* Absolute_Start_Address);
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();
	
	for(FSCB_Count=0;FSCB_Count<MAX_FS_IN_SYS;FSCB_Count++)
	{
		if(FSCB[FSCB_Count].Root_Name==Root)
		{
			Find_Flag=1;
			break;              									           //This Indicates A Filesystem Is Mounted Under The Directory.Now The FSCB_Count Contains The FSID.
		}
	}

	if(Find_Flag==0)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();                                                      //The Filesystem does not exist.Abort.
		return(ENOFSY);
	}

	Here_Sys_Format=FSCB[FSCB_Count].Sys_Inner_Format;
	Here_Sys_Format(FSCB[FSCB_Count].Absolute_Start_Address);

	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();
	return(0);	
}
#endif
/*Finish Function:Sys_Low_Level_Format_FS*************************************/

/*Begin Function:Sys_Fopen*****************************************************
Description : The  "Fopen" Function For The System.
Input       : u8* Path -The Path Of The File.Like"A\Readme.txt".
              u8 Mode -The Open Modes.See "defines.h" for more details.
#define       READ       0x01
#define       WRITE      0x02
#define       ADD        0x04
#define       CREATE     0x08		//(Other Bits Are Reserved)
Output      : u8 -The IO Buffer ID Returned.(This Is Not Like The C Standard Library,But Sort Of Linux.)
              If The Function Should Fail,Return "ENOFIL"(MAX_FILE_OPEN+1) instead.
			  Take Note That The Return Value Is NOT The File ID In The Filesystem.
******************************************************************************/	
#if(ENABLE_FILESYSTEM==TRUE)
u8 Sys_Fopen(u8* Path,u8 Mode)
{	
	u8 IOBCB_Count=0,FSCB_Count=0,FSCB_PID_Count=0,Find_Flag=0;
	u8 (*Here_Sys_Fopen)(u8* Path,u8 Mode,u8 IO_Buffer,void* Absolute_Start_Address);
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();
	
	for(FSCB_Count=0;FSCB_Count<MAX_FS_IN_SYS;FSCB_Count++)
	{
		if(FSCB[FSCB_Count].Root_Name==*Path)
		{
			Find_Flag=1;
			break;              									           //This Indicates A Filesystem Is Mounted Under The Directory.Now The FSCB_Count Contains The FSID.
		}
	}

	if(Find_Flag==0)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOFIL);                                                        //The Filesystem does not exist.Abort.
	}

	for(FSCB_PID_Count=0;FSCB_PID_Count<MAX_PID_USE_FS;FSCB_PID_Count++)
	{
		if(FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]==0)
		{
			FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]=Current_PID;		   //Find An Empty Slot To Put The PID In.
			break; 													           //We Don't Check If The PID Is Already Registered Here,Because When Opening A New File Needs Two "Sys_Fclose" To Unregister.
		}
	}
																			   //Fill In The Chart.
	for(IOBCB_Count=0;IOBCB_Count<MAX_FILE_OPEN;IOBCB_Count++)
	{
		if(IOBCB[IOBCB_Count].Root_Name==0)                                    //This Suggests There is No File Under The I/O Control Block.If We Are Unable To Find One,Then It Is Impossible To Open.
		{
			Here_Sys_Fopen=FSCB[FSCB_Count].Sys_Inner_Fopen;                   //Assign The Pointer First.
			if(Here_Sys_Fopen(Path,Mode,IOBCB_Count,FSCB[FSCB_Count].Absolute_Start_Address)==1)
			{
				FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]=0;                 //Unregister The PID Now.The Open Failed.
				ENABLE_ALL_INTS();
				ENABLE_SYSTICK();											   //This Suggests That The Open Failed.
				return(ENOFIL);
			}
			else
			{
				ENABLE_ALL_INTS();
				ENABLE_SYSTICK();		
				return(IOBCB_Count);										   //Succeed. Return The File Identifier.
			}
		}
	}

	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();
	return(ENOFIL);															   //It Fails(Unable To Find Any Empty IO Buffer Slot) When Runs To Here.
}
#endif
/*Finish Function:Sys_Fopen***************************************************/

/*Begin Function:Sys_Fread*****************************************************
Description : The  "Fread" Function For The System.
Input       : void* Buffer -The Pointer To The Buffer Area To Read The Data In.
              u8 Size -The Size Of Each Unit That You Want To Read.
			  u32 Count -The Number Of Units That You Want To Read.
			  u8 IO_Buffer -The IO Buffer Control Block Number.
Output      : u8 -If 0,Success;"ENOFOP"(0x01) Means Failure.
******************************************************************************/	
#if(ENABLE_FILESYSTEM==TRUE)
u8 Sys_Fread(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer)
{	
	u8 FS_Count;                             								   //Counting Filesystem.
	u8 (*Here_Sys_Fread)(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);//The Pointer For The Fucntion Provided By The Filesystem.

	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	 

	if((IOBCB[IO_Buffer].Root_Name==0)||(IOBCB[IO_Buffer].Operation_PID!=Current_PID))	
	{   
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();        								
		return(ENOFOP);
	} 

	for(FS_Count=0;FS_Count<MAX_FS_IN_SYS;FS_Count++)
	{																		   //The Filesystem Must Exist,Because A File Is Opened. 
		if(FSCB[FS_Count].Root_Name==IOBCB[IO_Buffer].Root_Name)			   //Now The Corresponding Filesystem Is Found.
			break;
	}

	Here_Sys_Fread=FSCB[FS_Count].Sys_Inner_Fread;

	if(Here_Sys_Fread(Buffer,Size,Count,IO_Buffer,FSCB[FS_Count].Absolute_Start_Address)==1)
	{   
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();        								
		return(ENOFOP);														   //The Fread Failed Somehow.
	} 

	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();        								
	return(0);																   //The Operation Successfully Completed.
}
#endif
/*Finish Function:Sys_Fread***************************************************/

/*Begin Function:Sys_Fwrite****************************************************
Description : The  "Fwrite" Function For The System.For Application Use.
Input       : void* Buffer -The Pointer To The Buffer Area To Write The Data From.
              u8 Size -The Size Of Each Unit That You Want To Write.
			  u32 Count -The Number Of Units That You Want To Write.
			  u8 IO_Buffer -The IO Buffer Control Block Number.
Output      : u8 -If 0,Success;"ENOFOP"(0x01) Means Failure.
******************************************************************************/
#if(ENABLE_FILESYSTEM==TRUE)	
u8 Sys_Fwrite(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer)
{	
	u8 FS_Count;                             								   //Counting Filesystem.
	u8 (*Here_Sys_Fwrite)(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);//The Pointer For The Fucntion Provided By The Filesystem.

	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	 

	if((IOBCB[IO_Buffer].Root_Name==0)||(IOBCB[IO_Buffer].Operation_PID!=Current_PID))	
	{   
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();        								
		return(ENOFOP);
	} 

	for(FS_Count=0;FS_Count<MAX_FS_IN_SYS;FS_Count++)
	{																		   //The Filesystem Must Exist,Because A File Is Opened. 
		if(FSCB[FS_Count].Root_Name==IOBCB[IO_Buffer].Root_Name)			   //Now The Corresponding Filesystem Is Found.
			break;
	}

	Here_Sys_Fwrite=FSCB[FS_Count].Sys_Inner_Fwrite;

	if(Here_Sys_Fwrite(Buffer,Size,Count,IO_Buffer,FSCB[FS_Count].Absolute_Start_Address)==1)
	{   
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();        								
		return(ENOFOP);														   //The Fread Failed Somehow.
	} 

	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();        								
	return(0);																   //The Operation Successfully Completed.
}	
#endif													
/*Finish Function:Sys_Fwrite**************************************************/

/*Begin Function:Sys_Fclose****************************************************
Description : The  "Fclose" Function For The System.
Input       : u8 IO_Buffer -The IO Buffer Control Block Number Of The File To Be Closed.
Output      : u8 -If 0,Success;"ENOFOP"(0x01) Means Failure.
******************************************************************************/	
#if(ENABLE_FILESYSTEM==TRUE)
u8 Sys_Fclose(u8 IO_Buffer)
{	
	u8 Find_Flag=0,FSCB_Count,FSCB_PID_Count;
	u8 (*Here_Sys_Fclose)(u8 IO_Buffer,void* Absolute_Start_Address);
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();

	if(IOBCB[IO_Buffer].Operation_PID!=Current_PID)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOFOP);                                                        //The Process Has No Right To Close This File(It Is Not Opened By It).	
	}	

	for(FSCB_Count=0;FSCB_Count<MAX_FS_IN_SYS;FSCB_Count++)
	{
		if(FSCB[FSCB_Count].Root_Name==IOBCB[IO_Buffer].Root_Name)
		{
			Find_Flag=1;
			break;              								        	   //The Corresponding Filesystem ID Is Found.
		}
	}

	if(Find_Flag==0)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOFOP);                                                        //The Filesystem does not exist.Abort.
	}

	for(FSCB_PID_Count=0;FSCB_PID_Count<MAX_PID_USE_FS;FSCB_PID_Count++)
	{
		if(FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]==Current_PID)
		{
			FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]=0;			           //Find The PID And Clear It Out..
			break; 													           //We Don't Check If The PID Is Registered More Than Once Here,Because When Opening A New File Needs Two "Sys_Fclose" To Unregister.
		}
	}

	Here_Sys_Fclose=FSCB[FSCB_Count].Sys_Inner_Fclose;                         //Assign The Pointer First.

	if(Here_Sys_Fclose(IO_Buffer,FSCB[FSCB_Count].Absolute_Start_Address)==1)
	{
		FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]=Current_PID;               //Reregister The PID Now.The Close Failed.
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();												            
		return(ENOFOP);												           //This Suggests That The Close Failed.
	}
	else
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();		
		return(0);												               //Succeed.Return The File Identifier.
	}																
}
#endif
/*Finish Function:Sys_Fclose**************************************************/

/*Begin Function:_Sys_Fclose***************************************************
Description : The  "Fclose" Function For System Use.Don't Call It In User Apps.
Input       : u8 IO_Buffer -The IO Buffer Control Block Number Of The File To Be Closed.
Output      : u8 -If 0,Success;"ENOFOP"(0x01) Means Failure.
******************************************************************************/	
#if(ENABLE_FILESYSTEM==TRUE)
u8 _Sys_Fclose(u8 IO_Buffer)
{	
	u8 Find_Flag=0,FSCB_Count,FSCB_PID_Count;
	u8 (*Here_Sys_Fclose)(u8 IO_Buffer,void* Absolute_Start_Address);

	if(IOBCB[IO_Buffer].Operation_PID!=Current_PID)
		return(ENOFOP);                                                        //The Process Has No Right To Close This File(It Is Not Opened By It).		

	for(FSCB_Count=0;FSCB_Count<MAX_FS_IN_SYS;FSCB_Count++)
	{
		if(FSCB[FSCB_Count].Root_Name==IOBCB[IO_Buffer].Root_Name)
		{
			Find_Flag=1;
			break;              								        	   //The Corresponding Filesystem ID Is Found.
		}
	}

	if(Find_Flag==0)
		return(ENOFOP);                                                        //The Filesystem does not exist.Abort.

	for(FSCB_PID_Count=0;FSCB_PID_Count<MAX_PID_USE_FS;FSCB_PID_Count++)
	{
		if(FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]==Current_PID)
		{
			FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]=0;			           //Find The PID And Clear It Out..
			break; 													           //We Don't Check If The PID Is Registered More Than Once Here,Because When Opening A New File Needs Two "Sys_Fclose" To Unregister.
		}
	}

	Here_Sys_Fclose=FSCB[FSCB_Count].Sys_Inner_Fclose;                         //Assign The Pointer First.

	if(Here_Sys_Fclose(IO_Buffer,FSCB[FSCB_Count].Absolute_Start_Address)==1)
	{
		FSCB[FSCB_Count].In_Use_PID[FSCB_PID_Count]=Current_PID;               //Reregister The PID Now.The Close Failed.										            
		return(ENOFOP);												           //This Suggests That The Close Failed.
	}
	else
	{	
		return(0);												               //Succeed.Return The File Identifier.
	}																
}
#endif
/*Finish Function:_Sys_Fclose*************************************************/

/*Begin Function:_Sys_Close_All_Files******************************************
Description : The  "Fclose" Function For System Use.Don't Call It In User Apps.
Input       : u8 PID -The Process ID To Deal With.
Output      : Void.
******************************************************************************/	
#if(ENABLE_FILESYSTEM==TRUE)
void _Sys_Close_All_Files(u8 PID)
{												  
	u8 IOBCB_Count;
	for(IOBCB_Count=0;IOBCB_Count<MAX_FILE_OPEN;IOBCB_Count++)
		if(IOBCB[IOBCB_Count].Operation_PID==PID)							   //Check The PID Before The Action.
			_Sys_Fclose(IOBCB_Count);
}
#endif
/*Finish Function:_Sys_Close_All_Files****************************************/

/*Begin Function:Sys_Fseek*****************************************************
Description : The  "Fseek" Function For The System.
Input       : u8 IO_Buffer -The IO Buffer Control Block Number Of The File.
			  s32 Offset -The Offset That You Want To Move The Pointer.A Negative Number Means Moving Backwards.
			  u8 Position -The Position To Move The Pointer From.
Output      : u8 -If 0,Success;"ENOFOP"(0x01) Means Failure.
******************************************************************************/	
#if(ENABLE_FILESYSTEM==TRUE)
u8 Sys_Fseek(u8 IO_Buffer,s32 Offset,u8 Position)
{	
	u8 FSCB_Count;
	u8 (*Here_Sys_Fseek)(u8 IO_Buffer,s32 Offset,u8 Position,void* Absolute_Start_Address);
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();

	if((IOBCB[IO_Buffer].Root_Name==0)||(IOBCB[IO_Buffer].Operation_PID!=Current_PID))
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();												            
		return(ENOFOP);														   //The File Is Not Opened At All.
	}

	for(FSCB_Count=0;FSCB_Count<MAX_FS_IN_SYS;FSCB_Count++)
	{
		if(FSCB[FSCB_Count].Root_Name==IOBCB[IO_Buffer].Root_Name)
			break;															   //The Corresponding Filesystem Is Found.It Must Exist.
	}		

	Here_Sys_Fseek=FSCB[FSCB_Count].Sys_Inner_Fseek;

	if(Here_Sys_Fseek(IO_Buffer,Offset,Position,FSCB[FSCB_Count].Absolute_Start_Address)==1)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();												            
		return(ENOFOP);													       //The Fseek Failed For Some Reason.	
	}	
	
	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();												            
	return(0);																   //The Fseek Succeeded.
}
#endif
/*Finish Function:Sys_Fseek***************************************************/
//
///*Begin Function:Sys_Frename***************************************************
//Description : The  "Frename" Function For The System.
//Input       : u8* Path -The Path Of The File To Be Renamed.
//			  u8* New_Name -The New Name Of The File.
//Output      : u8 -If 0,Success;1 Means Failure.
//******************************************************************************/	
//#if(ENABLE_FILESYSTEM==TRUE)
//u8 Sys_Frename(u8* Path,u8* New_Name)
//{	
//														
//}
//#endif
///*Finish Function:Sys_Frename*************************************************/
//
///*Begin Function:Sys_Fremove***************************************************
//Description : The  "Fremove" Function For The System.
//Input       : u8* Path -The Path Of The File To Be Removed.
//Output      : u8 -If 0,Success;1 Means Failure.
//******************************************************************************/
//#if(ENABLE_FILESYSTEM==TRUE)	
//u8 Sys_Fremove(u8* Path)
//{	
//	;														
//}
//#endif
///*Finish Function:Sys_Fremove*************************************************/
//
///*Begin Function:Sys_Get_Fproperty*********************************************
//Description : The Function For Getting The File Properties.
//Input       : u8* Path -The Path Of The File To Be Operated.
//			  struct File_Property* Property -The Struct To Put The File Properties In.
//Output      : u8 -If 0,Success;1 Means Failure.
//******************************************************************************/
//#if(ENABLE_FILESYSTEM==TRUE)	
//u8 Sys_Get_Fproperty(u8* Path,struct File_Property* Property)
//{	
//														
//}
//#endif
///*Finish Function:Sys_Get_Fproperty*******************************************/
//
///*Begin Function:Sys_Set_Fproperty*********************************************
//Description : The Function For Setting The File Properties.
//Input       : u8* Path -The Path Of The File To Be Operated.
//			  struct File_Property* Property -The Struct To Put The File Properties In.
//Output      : u8 -If 0,Success;1 Means Failure.
//******************************************************************************/
//#if(ENABLE_FILESYSTEM==TRUE)	
//u8 Sys_Set_Fproperty(u8* Path,struct File_Property* Property)
//{	
//														
//}
//#endif
///*Finish Function:Sys_Set_Fproperty*******************************************/

/*End Of File*******************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.*****************************/
