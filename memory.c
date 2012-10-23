 /*****************************************************************************
Filename    : memory.c
Author      : pry
Date        : 25/04/2012
Description : The Memory Managing Module For The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/04/2012 Ver1.0.0
  Moved The "Sys_Malloc" and"Sys_Free_Memory" To Here from kernel.c
2.Modified By pry                                        On 30/04/2012 Ver1.1.0
  Completed The Shared Memory Module,And Added Free_All_XXX Functions.
3.Modified By pry                                        On 03/05/2012 Ver1.1.2
  Created"_Sys_Malloc" And "_Sys_Free_Memory".
4.Modified By pry                                        On 26/06/2012 Ver1.1.3
  Fixed Some Logic Mistakes.
5.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines,And Made The Module Configurable.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "memory.h"
/*End Includes****************************************************************/

/*Config Reporter*************************************************************/
#if(REPORT_CONFIG==TRUE)
	#if(ENABLE_MEMORY_MANAGEMENT==TRUE)
		#warning The Memory Management Module Is Enabled. 
	#else
		#warning The Memory Management Module Is Disabled.
	#endif

	#if(ENABLE_SHARED_MEMORY==TRUE)
		#if(ENABLE_MEMORY_MANAGEMENT==FALSE)
			#error Please Enable The Memory Management Module.
		#else 
			#warning The Shared Memory Module Is Enabled.
		#endif
	#else		
		#warning The Shared Memory Module Is Disabled.
	#endif
#endif
/*End Config Reporter*********************************************************/

/*Begin Function:Sys_Malloc****************************************************
Description : Allocate Some Memory.For Application Use.
Input       : u32 Bytes -The Amount Of RAM That The Application Need.
Output      : void* -The Pointer To The Memory,At The Beginning Of It.Should The Function Fail,
              It Will Return "ENOMEM"(0x00).
******************************************************************************/
#if (ENABLE_MEMORY_MANAGEMENT==TRUE)
void* Sys_Malloc(u32 Bytes)									                   //Check The Whole List To Find The Best Fit One.If There Are many ones,The first One Will Be Used.
{	
	u32 Page_Count=0,Mem_Count=0;                                              //The Variable Declarations must Be At The Beginning of the function.
	u8 Find_Flag=0;
	void* Ret_Ptr=(void*)0;	 									               //Use u32 to Ensure The Largest Memory Area.

	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	  
	for(Page_Count=0;Page_Count<NUMBER_OF_MEM_PAGES;Page_Count++)
	{
		if(MCB_MEM[Page_Count]==0)		                                       //"0" means That it is not Occupied.
			Mem_Count++;
		if(MCB_MEM[Page_Count]!=0)
			Mem_Count=0;
		if(Mem_Count==(Bytes/SIZE_OF_EACH_PAGE)+1+1)	                       //"break" means that There's Enough memory. +1 is for making a whole,and another"1" is for separating the two segment of memory.
		{
			Find_Flag=1;
			break;
		}
	}									                                       //Now "Count" stores The Value Of The End Page Number.

	if(Find_Flag==0)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOMEM);		                                                   //Retval. No Memory Available.
	}
	
	for(Mem_Count--;Mem_Count>0;Mem_Count--)               	                   //Now Let The "Mem_Count" Decline By 1,Leaving Enough Space For The Separation.
	{
		MCB_MEM[Page_Count--]=Current_PID;                                     //Fill The Page Allocation Table With Its PID.
	}
	Ret_Ptr=(void*)((Page_Count+1)*SIZE_OF_EACH_PAGE+APP_AVAIL_MEM_START);
	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();

	return Ret_Ptr;		                                                       //Retval	
}
#endif
/*Finish Function:Sys_Malloc**************************************************/

/*Begin Function:_Sys_Malloc***************************************************
Description : Allocate Some Memory.For System Use.
Input       : u32 Bytes -The Amount Of RAM That The Application Need.
Output      : void* -The Pointer To The Memory,At The Beginning Of It.
			  Should It Fail, It Will Return "ENOMEM"(0x00).
******************************************************************************/
#if (ENABLE_MEMORY_MANAGEMENT==TRUE)
void* _Sys_Malloc(u32 Bytes)									               //Check The Whole List To Find The Best Fit One.If There Are many ones,The first One Will Be Used.
{	
	u32 Page_Count=0,Mem_Count=0;                                              //The Variable Declarations must Be At The Beginning of the function.
	u8 Find_Flag=0;
	void* Ret_Ptr=(void*)0;	 									               //Use u32 to Ensure The Largest Memory Area.
	  															  
	for(Page_Count=0;Page_Count<NUMBER_OF_MEM_PAGES;Page_Count++)
	{
		if(MCB_MEM[Page_Count]==0)		                                       //"0" means That it is not Occupied.
			Mem_Count++;
		if(MCB_MEM[Page_Count]!=0)
			Mem_Count=0;
		if(Mem_Count==(Bytes/SIZE_OF_EACH_PAGE)+1+1)	                       //"break" means that There's Enough memory. +1 is for making a whole,and another"1" is for separating the two segment of memory.
		{
			Find_Flag=1;
			break;
		}
	}									                                       //Now"Count" stores The Value Of The End Page Number.
	
	if(Find_Flag==0)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOMEM);		                                                   //Retval. No Memory Available.
	}

	for(Mem_Count--;Mem_Count>0;Mem_Count--)               	                   //Now Let The "Mem_Count" Decline By 1,Leaving Enough Space For The Separation.
	{
		MCB_MEM[Page_Count--]=Current_PID;                                     //Fill The Page Allocation Table With Its PID.
	}
	Ret_Ptr=(void*)((Page_Count+1)*SIZE_OF_EACH_PAGE+APP_AVAIL_MEM_START);

	return Ret_Ptr;		                                                       //Retval	
}
#endif
/*Finish Function:_Sys_Malloc*************************************************/

/*Begin Function:Sys_Free_Memory***********************************************
Description : Free The Allocated Memory.For Application Use.
Input       : void* Mem_Ptr -The Pointer Returned By The "Sys_Malloc".
Output      : Void.
******************************************************************************/
#if (ENABLE_MEMORY_MANAGEMENT==TRUE)
void Sys_Free_Memory(void* Mem_Ptr)
{	 
	u32 Page_Count;                                                            //The Variable Declarations must Be At The Beginning of the function.
																               //Use u32 to Ensure The Largest Memory Area.
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	
	Page_Count=((u32)(Mem_Ptr)-APP_AVAIL_MEM_START)/SIZE_OF_EACH_PAGE;         //The Calculation Can't Be Done In Advance ,Or When The SysTick Timer Is Going To Be 0,The Error Can Occur.	
	while(MCB_MEM[--Page_Count]==Current_PID);
	while(MCB_MEM[++Page_Count]==Current_PID)
		MCB_MEM[Page_Count]=0;                                                 //Clear The Occupy Sign.	
	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();
}
#endif
/*Finish Function:Sys_Free_Memory*********************************************/

/*Begin Function:_Sys_Free_Memory**********************************************
Description : Free The Allocated Memory.For System Use.
Input       : void* Mem_Ptr -The Pointer Returned By The "Sys_Malloc".
Output      : Void.
******************************************************************************/
#if (ENABLE_MEMORY_MANAGEMENT==TRUE)
void _Sys_Free_Memory(void* Mem_Ptr)
{	 
	u32 Page_Count;                                                            //The Variable Declarations must Be At The Beginning of the function.
																               //Use u32 to Ensure The Largest Memory Area.
	Page_Count=((u32)(Mem_Ptr)-APP_AVAIL_MEM_START)/SIZE_OF_EACH_PAGE;         //The Calculation Can't Be Done In Advance ,Or When The SysTick Timer Is Going To Be 0,The Error Can Occur.	
	while(MCB_MEM[--Page_Count]==Current_PID);
	while(MCB_MEM[++Page_Count]==Current_PID)
		MCB_MEM[Page_Count]=0;                                                 //Clear The Occupy Sign.	
}
#endif
/*Finish Function:_Sys_Free_Memory********************************************/

/*Begin Function:Sys_Free_All_Memory*******************************************
Description : Free All Allocated Memory.For Application Use.
Input       : Void.
Output      : Void.
******************************************************************************/
#if (ENABLE_MEMORY_MANAGEMENT==TRUE)
void Sys_Free_All_Memory(void)									     
{
	u32 Page_Count;											                   //Use u32 to Ensure The Largest Memory Area.
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();
	for(Page_Count=0;Page_Count<NUMBER_OF_MEM_PAGES;Page_Count++)
	{
		if(MCB_MEM[Page_Count]==Current_PID)
			MCB_MEM[Page_Count]=0;
	}
	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();
}
#endif
/*Finish Function:Sys_Free_All_Memory*****************************************/

/*Begin Function:_Sys_Free_All_Memory******************************************
Description : Free All Allocated Memory.For System Use,In "SIGKILL Handler".
              The Systick is Disabled/Enabled In Other Ways,
			  So here We Don't Operate it again.
Input       : u8 PID-The Process's PID.
Output      : Void.
******************************************************************************/
#if (ENABLE_MEMORY_MANAGEMENT==TRUE)
void _Sys_Free_All_Memory(u8 PID)									     
{
	u32 Page_Count;					                 
	for(Page_Count=0;Page_Count<NUMBER_OF_MEM_PAGES;Page_Count++)
	{
		if(MCB_MEM[Page_Count]==PID)
			MCB_MEM[Page_Count]=0;
	}
}
#endif
/*Finish Function:_Sys_Free_All_Memory****************************************/

/*Begin Function:Sys_Shared_Malloc*********************************************
Description : Allocate Shared Memory.For Application Use.
Input       : u8* SMID-Gives Out The Shared Memory ID.
              u32 Bytes -The Amount Of Shared RAM That The Application Needs.
Output      : void* -The Pointer To The Memory,At The Beginning Of It.
			  Should The Process Fail,It Will Return"ENOSHM"
******************************************************************************/
#if ((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))
void* Sys_Shared_Malloc(u8* SMID,u32 Bytes)									   //Check The Whole List To Find The Best Fit One.If There Are many ones,The first One Will Be Used.
{	
	u32 Page_Count=0,Mem_Count=0;                                              //The Variable Declarations must Be At The Beginning of the function.
	u8 Shared_Page_Count=0;
	u8 Find_Flag=0;
	void* Ret_Ptr=(void*)0;	 
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	  

	for(Page_Count=0;Page_Count<NUMBER_OF_MEM_PAGES;Page_Count++)
	{
		if(MCB_MEM[Page_Count]==0)		                                       //"0" means That it is not Occupied.
			Mem_Count++;
		if(MCB_MEM[Page_Count]!=0)
			Mem_Count=0;
		if(Mem_Count==(Bytes/SIZE_OF_EACH_PAGE)+1+1)	     	               //"break" means that There's Enough memory. +1 is for making a whole,and another"1" is for separating the two segment of memory.
		{
			Find_Flag=1;
			break;
		}
	}

	if(Find_Flag==0)
	{
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOSHM);		                                                   //The Operation Failed. Return "ENOSHM".
	}
										                                       //Now"Count" stores The Value Of The End Page Number.
	for(Shared_Page_Count=0;Shared_Page_Count<MAX_SHARED_MEM_BLOCKS;Shared_Page_Count++)
	{
		if(MCB_SM[Shared_Page_Count].SM_End_Page==0)		                   //End Page "0" means That it is not Occupied. If Occupied,The End Page Won't Be "0".
			break;
	}

	MCB_SM[Shared_Page_Count].SM_End_Page=Page_Count;		                   //Now"Count" stores The Value Of The End Page Number.

	for(Mem_Count--;Mem_Count>0;Mem_Count--)                       	           //Now Let The "Mem_Count" Decline By 1,Leaving Enough Space For The Separation.
	{																  
		MCB_MEM[Page_Count--]=0x01;                                            //Fill The Page Allocation Table With PID "1".The Memory Is Literally Allocated By "Arch".
	}

	MCB_SM[Shared_Page_Count].SM_Start_Page=Page_Count+1;		               //Write The MCB_SM.
    MCB_SM[Shared_Page_Count].SM_PID[0]=Current_PID; 
	              
	Ret_Ptr=(void*)((Page_Count+1)*SIZE_OF_EACH_PAGE+APP_AVAIL_MEM_START);
	*SMID=Shared_Page_Count;
		
	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();

	return Ret_Ptr;		                                                       //Retval	
}
#endif
/*Finish Function:Sys_Shared_Malloc*******************************************/

/*Begin Function:Sys_Join_Shared_Malloc****************************************
Description : Gain The Right To Use The Allocated Shared Memory.For Application Use.
              Before Joining A Shared Memory,You Need To Specify Which Block Of 
			  Shared Memory You Want To Use,And It Can Be Done By Reading The MCB
			  Or The Shared Memory Itself.Communicating Through A Shared Memory
			  Needs A good Protocol.
Input       : u8 SMID-Sends In The Shared Memory ID.
 			  u32* Bytes -Outputs The Amount Of Shared RAM That The Block Has.			  
Output      : void* -The Pointer To The Memory,At The Beginning Of It. If The Shared Memory
              Is Not Found,Then The Function Will Return"ENOSHM"(0x00) In Both The Return Value And
			  The "u32* Bytes".
******************************************************************************/
#if ((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))
void* Sys_Join_Shared_Malloc(u8 SMID,u32* Bytes)							   //Check The Whole List To Find The Best Fit One.If There Are many ones,The first One Will Be Used.
{	
	u8 Shared_Page_PID_Count=0,Success_Flag=0;
	void* Ret_Ptr=(void*)0;	 								 
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	  
	if(MCB_SM[SMID].SM_End_Page!=0)
	{
		for(Shared_Page_PID_Count=0;Shared_Page_PID_Count<MAX_PROCESS_SHARE_MEM;Shared_Page_PID_Count++)
		{
			if(MCB_SM[SMID].SM_PID[Shared_Page_PID_Count]==Current_PID)
			{
				Success_Flag=1;                                                //Count It As Success.In Fact,It Has Succeeded yet. 
				break;														   //If The Process Has Joined It,Don't Join It Again.
			}
	
			if(MCB_SM[SMID].SM_PID[Shared_Page_PID_Count]==0)
			{
				Success_Flag=1;
				MCB_SM[SMID].SM_PID[Shared_Page_PID_Count]=Current_PID;		   //If Not,Join It.
				break;	
			}	
		}
	}

	if(Success_Flag==1)														   //Success.
	{
		*Bytes=(MCB_SM[SMID].SM_End_Page-MCB_SM[SMID].SM_Start_Page+1)*SIZE_OF_EACH_PAGE;
		Ret_Ptr=(void*)(MCB_SM[SMID].SM_Start_Page*SIZE_OF_EACH_PAGE+APP_AVAIL_MEM_START);
	}	
	else
	{	
		*Bytes=ENOSHM;														   //Fail.
		ENABLE_ALL_INTS();
		ENABLE_SYSTICK();
		return(ENOSHM);		                                                   //Retval
	}

	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();

	return Ret_Ptr;		                                                       //Retval	
}
#endif
/*Finish Function:Sys_Shared_Malloc*******************************************/

/*Begin Function:Sys_Free_Shared_Memory****************************************
Description : Free The Allocated Shared Memory.For Application Use.
Input       : u8 SMID -The SMID Returned By The "Sys_Shared_Malloc".
Output      : Void.
******************************************************************************/
#if ((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))
void Sys_Free_Shared_Memory(u8 SMID)
{	
	u8 Shared_Page_PID_Count=0,Free_Flag=1;                                    //The Variable Declarations must Be At The Beginning of the function.
	u32 Mem_Count=0;

	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	

	for(Shared_Page_PID_Count=0;Shared_Page_PID_Count<MAX_PROCESS_SHARE_MEM;Shared_Page_PID_Count++)
	{
		if(MCB_SM[SMID].SM_PID[Shared_Page_PID_Count]==Current_PID)
			MCB_SM[SMID].SM_PID[Shared_Page_PID_Count]=0;				       //If There Is The Current PID,Clear It At Once,So We Don't Need To Judge It Below.
		if(MCB_SM[SMID].SM_PID[Shared_Page_PID_Count]!=0)				       //If There Is Another Positive Number,It Must Be Others' PID.
			Free_Flag=0;								                       //If There Is Another Process Occupying The Block,Then It Can't Be Deleted Completely. 
	}

	if(Free_Flag==1)
	{
		for(Mem_Count=MCB_SM[SMID].SM_Start_Page;Mem_Count<=MCB_SM[SMID].SM_End_Page;Mem_Count++)
			MCB_MEM[Mem_Count]=0x00;							               //Eliminate The Shared Memory In The Memory Allocation Table.
		MCB_SM[SMID].SM_Start_Page=0;									       //The Start Page Of The Shared Memory.
		MCB_SM[SMID].SM_End_Page=0;										       //The End Page Of The Shared Memory.The PIDs of Processes sharing the memory Has Been Totally Cleared Above.
	}
	ENABLE_ALL_INTS();
	ENABLE_SYSTICK();
}
#endif
/*Finish Function:Sys_Free_Shared_Memory**************************************/

/*Begin Function:Sys_Free_All_Shared_Memory************************************
Description : Free All Allocated Shared Memory.For Application Use.
Input       : Void.
Output      : Void.
******************************************************************************/
#if ((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))
void Sys_Free_All_Shared_Memory(void)
{	
	u8 Shared_Memory_Count=0,Shared_Page_PID_Count=0,Free_Flag=1;              //The Variable Declarations must Be At The Beginning of the function.
	u32 Mem_Count=0;

	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();	

	for(Shared_Memory_Count=0;Shared_Memory_Count<MAX_SHARED_MEM_BLOCKS;Shared_Memory_Count++)
	{
		if(MCB_SM[Shared_Memory_Count].SM_End_Page==0)					       //The Block Is never Used,Skip.
			continue;
		for(Shared_Page_PID_Count=0;Shared_Page_PID_Count<MAX_PROCESS_SHARE_MEM;Shared_Page_PID_Count++)
		{
			if(MCB_SM[Shared_Memory_Count].SM_PID[Shared_Page_PID_Count]==Current_PID)
				MCB_SM[Shared_Memory_Count].SM_PID[Shared_Page_PID_Count]=0;   //Clear The PID.
			if(MCB_SM[Shared_Memory_Count].SM_PID[Shared_Page_PID_Count]!=0)   //If There Is Another Positive Number,It Must Be Others' PID.
				Free_Flag=0;												   //There Are Other Processes Using The Shared Memory,It Can't Be Freed Completely.
		}

		if(Free_Flag==1)
		{	
			for(Mem_Count=MCB_SM[Shared_Memory_Count].SM_Start_Page;Mem_Count<=MCB_SM[Shared_Memory_Count].SM_End_Page;Mem_Count++)
				MCB_MEM[Mem_Count]=0x00;							       	   //Eliminate The Shared Memory In The Allocation Table.
			MCB_SM[Shared_Memory_Count].SM_Start_Page=0;
			MCB_SM[Shared_Memory_Count].SM_End_Page=0;						   //The End Page Of The Shared memory.The MCB_SM.SM_PID Is Already Cleared Above.
		}																   
	}

	ENABLE_ALL_INTS();														   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	ENABLE_SYSTICK();
}
#endif
/*Finish Function:Sys_Free_All_Shared_Memory**********************************/

/*Begin Function:_Sys_Free_All_Shared_Memory***********************************
Description : Free The Allocated Shared Memory.For Application Use.
Input       : u8 PID -The Process Being Killed.
Output      : Void.
******************************************************************************/
#if ((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))
void _Sys_Free_All_Shared_Memory(u8 PID)
{	
	u8 Shared_Memory_Count=0,Shared_Page_PID_Count=0,Free_Flag=1;              //The Variable Declarations must Be At The Beginning of the function.
	u32 Mem_Count=0;

	for(Shared_Memory_Count=0;Shared_Memory_Count<MAX_SHARED_MEM_BLOCKS;Shared_Memory_Count++)
	{
		if(MCB_SM[Shared_Memory_Count].SM_End_Page==0)					       //The Block Is never Used,Skip.
			continue;
		for(Shared_Page_PID_Count=0;Shared_Page_PID_Count<MAX_PROCESS_SHARE_MEM;Shared_Page_PID_Count++)
		{
			if(MCB_SM[Shared_Memory_Count].SM_PID[Shared_Page_PID_Count]==PID)
				MCB_SM[Shared_Memory_Count].SM_PID[Shared_Page_PID_Count]=0;   //Clear The PID.
			if(MCB_SM[Shared_Memory_Count].SM_PID[Shared_Page_PID_Count]!=0)   //If There Is Another Positive Number,It Must Be Others' PID.
				Free_Flag=0;												   //There Are Other Processes Using The Shared Memory,It Can't Be Freed Completely.
		}

		if(Free_Flag==1)
		{	
			for(Mem_Count=MCB_SM[Shared_Memory_Count].SM_Start_Page;Mem_Count<=MCB_SM[Shared_Memory_Count].SM_End_Page;Mem_Count++)
				MCB_MEM[Mem_Count]=0x00;							           //Eliminate The Shared Memory In The Allocation Table.
			MCB_SM[Shared_Memory_Count].SM_Start_Page=0;
			MCB_SM[Shared_Memory_Count].SM_End_Page=0;						   //The End Page Of The Shared memory.The MCB_SM.SM_PID Is Already Cleared Above.
		}																   
	}
}
#endif
/*Finish Function:_Sys_Free_All_Shared_Memory*********************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
