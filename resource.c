 /*****************************************************************************
Filename    : resource.c
Author      : pry
Date        : 25/04/2012
Description : The Device&Semaphore Managing Module For The OS.
              In RTOS-Rookie,Usually A Device is A Entity That 
			  Can Complete A Certain Function,Bus Controllers 
			  Excluded.
			  A Semaphore Can Be Treated As An Dummy Resource.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/04/2012 Ver1.0.0
  Created The File Itself.
2.Modified By pry                                        On 30/04/2012 Ver1.1.0
  Created A Few basic Functions.
3.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines,And Made The Module Configurable.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "resource.h"
/*END Includes****************************************************************/

/*Config Reporter*************************************************************/
#if(REPORT_CONFIG==TRUE)
	#if(ENABLE_RESOURCE==TRUE)
		#warning The Resource/Semaphore Management Module Is Enabled. 
	#else
		#warning The Resource/Semaphore Management Module Is Disabled.
	#endif
#endif
/*End Config Reporter*********************************************************/ 

/*Begin Function:Sys_Register_Resource*****************************************
Description : Register a device.For Application Use.
Input       : u8 Resource_Type -The Type Of The Resource.See Definition
			  In "defines.h".
			  u8 Number_Of_Total_Res -Total number of Resources/semaphores.
Output      : u8 -The Resource ID of the Resource Registered.If Failed,It Will
                  Return "ENORCB"(MAX_NUMBER_OF_RES+1).
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
u8 Sys_Register_Resource(u8 Resource_Type,u8 Number_Of_Resources)
{
	u8 Resource_Count,Find_Flag=0;
	DISABLE_SYSTICK();	                                     				   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();
	for(Resource_Count=0;Resource_Count<MAX_NUMBER_OF_RES;Resource_Count++) 
	{	
		if(RCB[Resource_Count].Res_Type==0)
		{	
			RCB[Resource_Count].Res_Type=Resource_Type;
			RCB[Resource_Count].Total_Res=Number_Of_Resources;
			RCB[Resource_Count].Res_Left=Number_Of_Resources;
			Find_Flag=1;
			break;
		}
	}
	if(Find_Flag==0)											 			   //Didn't Find Any Available Slots.
	{
		ENABLE_ALL_INTS();	                                     			   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORCB);														   //Return a invalid value.
	}
	ENABLE_ALL_INTS();	                                     				   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	ENABLE_SYSTICK();
	return(Resource_Count);
} 
#endif
/*Finish Function:Sys_Register_Resource***************************************/

/*Begin Function:Sys_Remove_Resource*******************************************
Description : Remove a device.For Application Use.
Input       : u8 Resource_ID -The Resource to Remove,Or Unregister.
Output      : u8 -0 for Success,"ENORES"(0x01) For Fault.
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
u8 Sys_Remove_Resource(u8 Resource_ID)
{
	DISABLE_SYSTICK();	                                     				   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();
	
	if(Resource_ID>=MAX_NUMBER_OF_RES)
	{
		ENABLE_ALL_INTS();	                                     			  //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();	
		return(ENORES);														  //An Overboundary Operation.Abort.
	}
	if(RCB[Resource_ID].Res_Type==0)							 			  //The Resource Is Not Yet Registered.
	{
		ENABLE_ALL_INTS();	                                     			  //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();	
		return(ENORES);
	}											
	if(RCB[Resource_ID].Total_Res==RCB[Resource_ID].Res_Left)				   //If No Resource Is Being Occupied,Then Eliminate The Resource Completely.
	{
		RCB[Resource_ID].Res_Type=0;
		RCB[Resource_ID].Total_Res=0;				 
		RCB[Resource_ID].Res_Left=0;
		ENABLE_ALL_INTS();	                                     			   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();						 
		return(0);	
	}
	else														 			   //If The Two Number Is Not Equal,There Must Be Resources Occupied.Abort.
	{	
		ENABLE_ALL_INTS();	                                         		   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();													 
		return(ENORES);                                           
	}
} 
#endif
/*Finish Function:Sys_Remove_Resource*****************************************/

/*Begin Function:Sys_Occupy_Resource*******************************************
Description : Occupy Some Resources.For Application Use.
Input       : u8 Resource_ID -The ID Of The Resource,Returned By The "Sys_Register_Resource".
			  u8 Number_To_Occupy -Total number of Resources/semaphores you want To Occupy.
Output      : u8 -If Successful,0;Otherwise "ENORES"(0x00).
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
u8 Sys_Occupy_Resource(u8 Resource_ID,u8 Number_To_Occupy)
{
	u8 Res_PID_Count=0,Find_Flag=0;
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();

	if(Number_To_Occupy==0)									                   //If The System Call Is A Trick,Or Fault.
	{
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORES);
	}

	if(Resource_ID>=MAX_NUMBER_OF_RES)
	{
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();	
		return(ENORES);											               //An Overboundary Operation.Abort.
	}

	if(RCB[Resource_ID].Res_Type==0)						                   //That Indicates That The Resource Is Not Registered,Will Return A Fault.
	{	
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORES);
	}

	if(RCB[Resource_ID].Res_Left<Number_To_Occupy)			                   //There's Not Enough Resources To Occupy.
	{
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORES);					
	}
					 
	RCB[Resource_ID].Res_Left-=Number_To_Occupy;			                   //Occupy The Resources.

	for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
	{
		if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==Current_PID)
		{
			RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied+=Number_To_Occupy;   
			Find_Flag=1;													   //It Has Occupied Some Before.
			break;
		}		
	}

	if(Find_Flag==0)														   //It never Occupied Any Before.
	{
		for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
		{
			if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==0)
			{
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID=Current_PID;
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied=Number_To_Occupy;
				break;	
			}
		}
	}

	ENABLE_ALL_INTS();	                                            		   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	ENABLE_SYSTICK();
	return(0);
} 
#endif
/*Finish Function:Sys_Occupy_Resource*****************************************/

/*Begin Function:_Sys_Occupy_Resource******************************************
Description : Occupy Some Resources.For System Use.
Input       : u8 Resource_ID -The ID Of The Resource,Returned By The "Sys_Register_Resource".
			  u8 Number_To_Occupy -Total number of Resources/semaphores you want To Occupy.
Output      : u8 -If Successful,0;Otherwise "ENORES"(0x00).
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
u8 _Sys_Occupy_Resource(u8 Resource_ID,u8 Number_To_Occupy)
{
	u8 Res_PID_Count=0,Find_Flag=0;

	if(Number_To_Occupy==0)									                   //If The System Call Is A Trick,Or Fault.
		return(ENORES);

	if(Resource_ID>=MAX_NUMBER_OF_RES)	
		return(ENORES);											 			   //An Overboundary Operation.Abort.
	
	if(RCB[Resource_ID].Res_Type==0)									       //That Indicates That The Resource Is Not Registered,Will Return A Fault.
		return(ENORES);
	
	if(RCB[Resource_ID].Res_Left<Number_To_Occupy)			 			       //There's Not Enough Resources To Occupy.
		return(ENORES);					
					 
	RCB[Resource_ID].Res_Left-=Number_To_Occupy;			 				   //Occupy The Resources.

	for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
	{
		if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==Current_PID)
		{
			RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied+=Number_To_Occupy;   
			Find_Flag=1;													   //It Has Occupied Some Before.
			break;
		}		
	}

	if(Find_Flag==0)														   //It never Occupied Any Before.
	{
		for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
		{
			if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==0)
			{
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID=Current_PID;
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied=Number_To_Occupy;
				break;	
			}
		}
	}

	return(0);
} 
#endif
/*Finish Function:_Sys_Occupy_Resource****************************************/

/*Begin Function:Sys_Release_Resource******************************************
Description : Release Some Resources.For Application Use.
Input       : u8 Resource_ID -The ID Of The Resource,Returned By The "Sys_Register_Resource".
			  u8 Number_To_Release -Total number of Resources/semaphores you want To Release.
Output      : u8 -If Successful,0;Otherwise "ENORES"(0x01).
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
u8 Sys_Release_Resource(u8 Resource_ID,u8 Number_To_Release)
{
	u8 Res_PID_Count=0,Find_Flag=0;
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();

	if(Number_To_Release==0)								                   //If The System Call Is A Trick,Or Fault.
	{
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORES);
	}
	if(Resource_ID>=MAX_NUMBER_OF_RES)
	{
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();	
		return(ENORES);											               //An Overboundary Operation.Abort.
	}
	if(RCB[Resource_ID].Res_Type==0)						                   //That Indicates That The Resource Is Not Registered,Will Return A Fault.
	{	
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORES);
	}
	for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
	{
		if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==Current_PID)
		{		
		 	Find_Flag=1;
			break;												               //After Breaking The "Res_PID_Count" Contains The Process's place in"Current_Occupy".
		}
	}
	if(Find_Flag==0)												           //That Indicates That The Process Has No Right To Release,For It never Occupied Any.
	{	
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORES);
	}																		   //There's Not Enough Resources to Release.
	if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied<Number_To_Release) 
	{	
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();
		return(ENORES);
	}
							
	RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID=0;                      //Remove The PID.
	RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied=0;   //Clear The Number.				 
	RCB[Resource_ID].Res_Left+=Number_To_Release;			                   //Release The Resources.

	ENABLE_ALL_INTS();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	ENABLE_SYSTICK();
	return(0);
} 
#endif
/*Finish Function:Sys_Release_Resource****************************************/

/*Begin Function:_Sys_Release_Resource*****************************************
Description : Release Some Resources.For System Use.
Input       : u8 Resource_ID -The ID Of The Resource,Returned By The "Sys_Register_Resource".
			  u8 Number_To_Release -Total number of Resources/semaphores you want To Release.
Output      : u8 -If Successful,0;Otherwise "ENORES"(0x01).
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
u8 _Sys_Release_Resource(u8 Resource_ID,u8 Number_To_Release)
{
	u8 Res_PID_Count=0,Find_Flag=0;

	if(Number_To_Release==0)								                   //If The System Call Is A Trick,Or Fault.
		return(ENORES);

	if(Resource_ID>=MAX_NUMBER_OF_RES)
		return(ENORES);											               //An Overboundary Operation.Abort.
	
	if(RCB[Resource_ID].Res_Type==0)						                   //That Indicates That The Resource Is Not Registered,Will Return A Fault.
		return(ENORES);

	for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
	{
		if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==Current_PID)
		{		
		 	Find_Flag=1;
			break;												               //After Breaking The "Res_PID_Count" Contains The Process's place in"Current_Occupy".
		}
	}

	if(Find_Flag==0)												           //That Indicates That The Process Has No Right To Release,For It never Occupied Any.
		return(ENORES);
																			   //There's Not Enough Resources to Release.
	if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied<Number_To_Release) 
		return(ENORES);
							
	RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID=0;                      //Remove The PID.
	RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied=0;   //Clear The Number.				 
	RCB[Resource_ID].Res_Left+=Number_To_Release;			                   //Release The Resources.

	return(0);
} 
#endif
/*Finish Function:_Sys_Release_Resource***************************************/

/*Begin Function:Sys_Release_All_Resources*************************************
Description : Release All Resources The Application Used.For Application Use.
Input       : Void.
Output      : Void.
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
void Sys_Release_All_Resources(void)
{
	u8 Resource_ID=0,Res_PID_Count=0;
	DISABLE_SYSTICK();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();
	for(Resource_ID=0;Resource_ID<MAX_NUMBER_OF_RES;Resource_ID++)
	{
		if(RCB[Resource_ID].Res_Type==0)					                   //The Resource ID is Not Used(Not Registered).
			continue;      
		if(RCB[Resource_ID].Total_Res==RCB[Resource_ID].Res_Left) 
			continue;										                   //The Resource Is never Occupied.
		for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
		{
			if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==Current_PID)//Now The Res_PID_Count Contains The PID's Place In"Current_Occupy".
			{
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID=0;          //Remove The PID.
				RCB[Resource_ID].Res_Left+=RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied;	//Release The Resources.
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied=0; //Clear The Number.				 	                                    
				break;                                            
			}
		}		                        
	}
	ENABLE_ALL_INTS();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	ENABLE_SYSTICK();
} 
#endif
/*Finish Function:Sys_Release_All_Resources***********************************/

/*Begin Function:_Sys_Release_All_Resources************************************
Description : Release Some Resources.For System Use,In "SIGKILL Handler".
Input       : u8 PID -The Process Being Killed.
Output      : Void.
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
void _Sys_Release_All_Resources(u8 PID)
{
	u8 Resource_ID=0,Res_PID_Count=0;
	for(Resource_ID=0;Resource_ID<MAX_NUMBER_OF_RES;Resource_ID++)
	{
		if(RCB[Resource_ID].Res_Type==0)					                   //The Resource ID is Not Used(Not Registered).
			continue;      
		if(RCB[Resource_ID].Total_Res==RCB[Resource_ID].Res_Left) 
			continue;										                   //The Resource Is never Occupied.
		for(Res_PID_Count=0;Res_PID_Count<MAX_PID_OCCUPY_RES;Res_PID_Count++)
		{
			if(RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID==PID)	       //Now The Res_PID_Count Contains The PID's Place In"Current_Occupy".
			{
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].PID=0;          //Remove The PID.
				RCB[Resource_ID].Res_Left+=RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied;//Release The Resources.
				RCB[Resource_ID].Current_Occupy[Res_PID_Count].Number_Of_Res_Occupied=0;				 	                                    
				break;                                            			   //Clear The Number.
			}
		}		                        
	}
} 
#endif
/*Finish Function:_Sys_Release_All_Resources**********************************/

/*Begin Function:Sys_Query_Amount**********************************************
Description : Returns The Resources Remaining.For Application Use.
			  There's No Need To Disable&Enable The Int/Systick.
			  For The Function Doesn't Write To The Table.
Input       : u8 Resource_ID -The Resource's ID.
Output      : u8 -if There is No Resource Available,Or The Resource Is Not 
              Registered,Or The Operation is Over The Boundary,It will return 0.
			  If There Is Any Of It Left,It Will Return Its Available Amount.
******************************************************************************/
#if(ENABLE_RESOURCE==TRUE)
u8 Sys_Query_Amount(u8 Resource_ID)
{
	if(Resource_ID>=MAX_NUMBER_OF_RES)						 
	{
		return(0);											                   //An Overboundary Operation.Abort.
	}
	if(RCB[Resource_ID].Res_Type==0)						                   //The Resource Is Not Registered,Will Return A Fault.
	{	
		return(0);
	}
	return(RCB[Resource_ID].Res_Left);                                         //Return The Left Value.	
}
#endif
/*End Function:Sys_Query_Amount***********************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
