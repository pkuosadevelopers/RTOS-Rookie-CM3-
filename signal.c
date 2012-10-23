/******************************************************************************
Filename   : signal.c
Author     : pry
Date       : 25/04/2012
Description: The Signal Part For The OS.
             Take Note That The SIGKILL is Treated Specially In The OS.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "signal.h"
/*END Includes****************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/04/2012 Ver1.1.0
  Created The Basic Functions.
2.Modified By pry                                        On 02/05/2012 Ver1.2.0
  Improved The Module.
3.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines,And Made The Module Configurable.
******************************************************************************/

/*Config Reporter*************************************************************/
#if(REPORT_CONFIG==TRUE)
	#if(ENABLE_RESOURCE==TRUE)
		#warning The Signal Management Module Is Enabled. 
	#else
		#warning The Signal Management Module Is Disabled.
	#endif
#endif
/*End Config Reporter*********************************************************/

/*Begin Function:_Sys_Signal_Handler*******************************************
Description : The System Signal Handler.
Input       : u8 PID -The Process ID.
Output      : Void.
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
void _Sys_Signal_Handler(u8 PID)	   										   //The System Signal Handler.
{
	if((PCB_SIGNAL[PID]&SIGKILL)==0)	   									   //There Is No SIGKILL.Note The Priority Of The C Operators.Use Small Brackets.
	{
		if((PCB_SIGNAL[PID]&SIGSLEEP)!=0)	 								   //There Is SIGSLEEP.
			_Sys_Sleep_The_Process(PID);
		if((PCB_SIGNAL[PID]&SIGWAKEUP)!=0)									   //There Is SIGWAKEUP.
			_Sys_Wakeup_The_Process(PID);
		if(((PCB_SIGNAL[PID]&SIGCUSTOM)!=0)&&((PCB_SIGCUSTOM_HANDLER[PID])!=0))//There Is SIGCUSTOM And The SIGCUSTOM Handler Is Not Null.
		{																       //Load The Function Pointer First.Don't Execute It Directly.
			_Sys_SIGCUSTOM_Handler=(volatile void(*)(void))PCB_SIGCUSTOM_HANDLER[PID]; 
			_Sys_SIGCUSTOM_Handler();	
		}
		PCB_SIGNAL[PID]=NOSIG;												   //Clear The Signal.
	}
	else
		_Sys_Kill_The_Process(PID);	  //There Is SIGKILL.
}
#endif
/*Finish Function:_Sys_Signal_Handler*****************************************/
 
/*Begin Function:_Sys_Kill_The_Process*****************************************
Description : The Function To Kill A Process,For System Use.
			  The Process Killed Won't Have a Return value.
              The SIGKILL Handler.
Input       : u8 PID -The Process to Kill.
Output      : Void.
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
void _Sys_Kill_The_Process(u8 PID)			                       			   //The SIGKILL Handler,Used By The System.It Can't Be Run In The "Init",because it may be too late.
{	
	//u32 Temp_Reload_Val_Storage;								   			   //Clean Up Its PCB,Delete All PCB data.

//	DISABLE_SYSTICK();	                                           			   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
//	DISABLE_ALL_INTS();											   			   //Because Here We Already Disabled The Systick,So We Don't Need To Disable It Again In The "_Sys_XXXX..."Function again.
#if(ENABLE_MEMORY_MANAGEMENT==TRUE)	
	_Sys_Free_All_Memory(PID);                                     			   //Free All The Memory Allocated By It.
#endif

#if((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))	
	_Sys_Free_All_Shared_Memory(PID);                              			   //Free All Shared Memory It Is Using.
#endif

#if(ENABLE_RESOURCE==TRUE)
	_Sys_Release_All_Resources(PID); 							   			   //Release All The Resources Occupied By It.
#endif

#if(ENABLE_FILESYSTEM==TRUE)
	_Sys_Close_All_Files(PID);									   			   //Close All Files It Opened.
#endif

	PCB_SP[PID]=0;				                                   			   //The SP Is Stored Here.+2,Because The "Init" And "Arch" Process Already Occupies 2 Slots.
	PCB_TIM[PID]=0;                                                			   //The time must be smaller than 0x00FFFFFF.The last is Dummy,0x00FFFFFE forever,for Fault Prevention..
	//PCB_TASK_SEQ_PTR;    
	PCB_LEAST_REQUIRED_TICKS[PID]=0;                               			   //The Minimum Timeslice For The Application,In Ticks.Used By The Kernel When Scheduling.
    PCB_SIGNAL[PID]=0;	                                           			   //The Array For Signal Storage.
    PCB_SIGCUSTOM_HANDLER[PID]=0;                                    		   //Stores The Entrance Of The Handler.
    PCB_PID_STATUS[PID]=0;                                         			   //Clean The Process Status Register At Last.
	PCB_NAME[PID]=0;														   //Clear The Pointer To The Name.

//	ENABLE_ALL_INTS();	                                     				   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
//	ENABLE_SYSTICK();

//	Temp_Reload_Val_Storage=RELOAD_VAL;											
//	
//	RELOAD_VAL=0x23;										       			   //The Value Is Precisely Trimmed For Jumping Out Of The Process For STM32(CM3).
//	RELOAD_NOW;
//	RELOAD_VAL=Temp_Reload_Val_Storage;                                    							   
}
#endif
/*Finish Function:_Sys_Kill_The_Process***************************************/

/*Begin Function:_Sys_Sleep_The_Process****************************************
Description : The Function To make A Process Sleep.
              For System Use.The SIGSLEEP Handler.
Input       : u8 PID -Make The Process Sleep.
Output      : Void.
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
void _Sys_Sleep_The_Process(u8 PID)											   //Make a Certain Process Sleep.
{
	PCB_PID_STATUS[PID]|=SLEEP;                                         	   //Elminate All The Timeslice For It From The Next Allocation,Not Permanently.
}
#endif
/*Finish Function:_Sys_Sleep_The_Process**************************************/

/*Begin Function:_Sys_Wakeup_The_Process***************************************
Description : The Function To Wake Up A Process.
              For System Use.The SIGWAKEUP handler.
Input       : u8 PID -The Process To Wake Up.
Output      : Void.
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
void _Sys_Wakeup_The_Process(u8 PID)										   //Make a Certain Process Wake Up.
{
	PCB_PID_STATUS[PID]&=~(SLEEP);                                         	   //Start To Allocate Timeslices.
}
#endif
/*Finish Function:_Sys_Wakeup_The_Process*************************************/

/*Begin Function:Sys_Send_Signal***********************************************
Description : The Function for sending Signals to other Processes.
Input       : u8 PID-The PID to Send The Signal To.
              u8 Signal-The Signal ID.
Output      : Void.
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
void Sys_Send_Signal(u8 PID,u8 Signal)
{
	if(Signal<0x10)	                                                  		   //System Signal.
	{													     
    	PCB_SIGNAL[PID]|=Signal;
	}
	else								                             		   //SIGNAL_(x) Signal.
		PCB_SIGNAL[PID]=(PCB_SIGNAL[PID]&SYSSIGMASK)|Signal<<4;
}
#endif
/*Finish Function:Sys_Send_Signal*********************************************/

/*Begin Function:Sys_Process_Quit**********************************************
Description : The Quit Function For Application Use,Can Produce a
              Return Value.Then The Process Becomes a Zombie.
Input       : u8 End_Val -The Return Value Of The Process.
                          The Last 4 Bits Are valid.
Output      : Void.
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
void Sys_Process_Quit(u8 End_Val)								   			   //The Caller Will Quit With a End Value.
{
	DISABLE_SYSTICK();	                                           			   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();											   			   //Because Here We Already Disabled The Systick,So We Don't Need To Disable It Again In The "_Sys_XXXX..."Function again.
	PCB_PID_STATUS[Current_PID]=(PCB_PID_STATUS[Current_PID]|(End_Val&END_VAL_MASK)|ZOMBIE);   
																			   //The Last 4 Bytes Of Retval Is valid.The Zombie Bit Indicates That It Is Dead.
#if(ENABLE_MEMORY_MANAGEMENT==TRUE)
	_Sys_Free_All_Memory(Current_PID);                             			   //Free All The Memory Allocated By It.
#endif

#if((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))
	_Sys_Free_All_Shared_Memory(Current_PID);                              	   //Free All Shared Memory It Is Using.
#endif

#if(ENABLE_RESOURCE==TRUE)
	_Sys_Release_All_Resources(Current_PID); 							       //Release All The Resources Occupied By It.
#endif

#if(ENABLE_FILESYSTEM==TRUE)
	_Sys_Close_All_Files(Current_PID);									       //Close All Files It Opened. 
#endif     
	
	ENABLE_ALL_INTS();	                                          			   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	ENABLE_SYSTICK();
	while(1);                                                      			   //Dead Loop.
}
#endif
/*Finish Function:Sys_Process_Quit********************************************/

/*Begin Function:Sys_Get_The_Process_Retval************************************
Description : The Function To Collect The Zombie's Return Value.
              For Application Use.
Input       : u8 PID -The Process to Collect And Eliminate.
Output      : u8 -It Will Be The Zombie Process's Return Value.
                  If The Function Failed,It Will Be "ENORET"(0xF0).
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
u8 Sys_Get_The_Process_Retval(u8 PID)							        	   //Get The End Value Of A Zombie Process.
{	
	u8 Ret_Val=0;

	DISABLE_SYSTICK();	                                                	   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	DISABLE_ALL_INTS();

	if((PCB_PID_STATUS[PID]&ZOMBIE)==ZOMBIE)                            	   //See if the Process is dead;
	{																	
		Ret_Val=(PCB_PID_STATUS[PID]&END_VAL_MASK);				    		   //If dead,get the retval,and Delete it completely.		
												
		PCB_SP[PID]=0;				                                    	   //The SP Is Stored Here.+2,Because The "Init" And "Arch" Process Already Occupies 2 Slots.
		PCB_TIM[PID]=0x00FFFFFE;                                        	   //Dummy 0x00FFFFFE for Fault Prevention.
		//PCB_TASK_SEQ_PTR;    
		PCB_LEAST_REQUIRED_TICKS[PID]=0;                                	   //The Minimum Timeslice For The Application,In Ticks.Used By The Kernel When Scheduling.
    	PCB_SIGNAL[PID]=0;	                                            	   //The Array For Signal Storage.
    	PCB_SIGCUSTOM_HANDLER[PID]=0;                                   	   //Stores The Entrance Of The Handler.
    	PCB_PID_STATUS[PID]=0;                                          	   //The Process Status Register.Clear Them All.

		ENABLE_ALL_INTS();	                                            	   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	    ENABLE_SYSTICK(); 

		return(Ret_Val);        	 										   //Retval is the Zombie Process's.
	}		   
	else
	{
		ENABLE_ALL_INTS();	                                            	   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	    ENABLE_SYSTICK();
		return(ENORET);												    	   //The Value Is 0xF0.
	}
}
#endif
/*Finish Function:Sys_Get_The_Process_Retval**********************************/

/*Begin Function:Sys_Register_Signal_Handler***********************************
Description : The Function for Registering Signal Processing Function,For Application Use..
              The Function Will Be Executed at The Process Switching Time,
			  At the Process's own memory space(That is,After The SP Switch).
			  The Time Will Be Included In The Process Timeslice.
			  The Signal Functions Must Be A Quick One.The Time It May Consume In the 
			  worst cases CAN'T Be Longer Than The Timeslice For The Process.
Input       : void (*Func)(void)-The Function Pointer.
Output      : Void.
******************************************************************************/
#if(ENABLE_SIGNAL==TRUE)
void Sys_Register_SIGCUSTOM_Handler(void (*Func)(void))					
{
	PCB_SIGCUSTOM_HANDLER[Current_PID]=(u32)Func;
}
#endif
/*Finish Function:Sys_Register_Signal_Handler*********************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
