/******************************************************************************
Filename    : kernel.c
Author      : pry
Date        : 07/04/2012
Description : The Kernel For The Operating System.
              The kernel Can Swap The Tasks So They Can
			  Run Properly.
			  It Works As A Processor Manager.
			  The System Supports 256 Processes Maximum.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 07/04/2012 Ver1.0.0
  The Kernel Can Roughly Run Now.Primitive.
2.Modified By pry                                        On 21/04/2012 Ver1.2.1
  Mainly Introduced The PCB_TIM and PCB_SEQ,Which makes 
  it possible to allocate different time to each process.
3.Modified By pry                                        On 25/04/2012 Ver1.4.0
  Completed The Memory Managing Module,And Task Switcher 
  The Kernel Has Gone Through Big Changes.
4.Modified By pry                                        On 29/04/2012 Ver1.6.0
  Preliminarily Completed The Semaphore Module.
  Completed The Semaphore Module And The Memory Module.
5.Modified By pry                                        On 25/06/2012 Ver1.6.1
  Modified Some Values&Names In This Section. Added The
  Error Managing Module. 
6.Greatly Modified By pry                                On 27/06/2012 Ver2.0.0
  Fixed A Severe Kernel Error In "_Sys_Init".
7.Modified By pry				                         On 01/07/2012 Ver2.1.0
  Fixed A Severe Error In The Kernel.	 
8.Modified By pry                                        On 22/07/2012 Ver2.1.1
  Ported To The Cortex-M4 Platform.
9.Modified By pry                                        On 22/07/2012 Ver3.0.0
  Added The "arch.h" And "sysconfig.h" ,Which Make The
  Operating System Configurable.
10.Modified By pry                                       On 23/07/2012 Ver3.1.0
  Fixed Some Grammar&Spelling Mistakes.A Lot More May Still Remain.
  Aligned The "***" Lines To Standardize The System.
11.Modified By pry                                  	 On 25/07/2012 Ver3.3.0
  The Processes Can Have A Configurable Name Now.
12.Modified By pry                                       On 26/07/2012 Ver3.4.0
  The Signal Process Method Is Changed.
13.Modified By pry                                       On 04/10/2012 Ver4.0.0
  The Kernel Scheduler Is Greatly Modified To Be More Stable And Support
  The Priority Scheduling Method.
  The Stability Of The "Sys_Switch_Now" Fucntion Also Improved.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "kernel.h"
/*End Includes****************************************************************/

/*Config Reporter*************************************************************/

/*End Config Reporter*********************************************************/

/*Begin Function:_Sys_Kernel_Load**********************************************
Description : The Kernel Loader.Loads The Kernel.Critical.           
Input       : u32 SP-The Initial Stack Pointer Of The Init Process. 
Output      : Void
******************************************************************************/
void _Sys_Kernel_Load(u32 SP)                                                  //The PID of "Init" is 0,Forever.
{
	PCB_PID_STATUS[0]|=OCCUPY|INITED;                                          //Indicates That This PID Is In Use,And It Is Initalized..
	PCB_TIM[MAX_NUMBER_OF_TASKS+2]=0xFFFFFFFF;		                           //Fault Prevention,When The Last Application Function Is Finished And Switching To The kernel.
	PCB_TASK_ADDRESS[0]=(u32)_Sys_Init;                                        //The Kernel's Address.
    PCB_SP[0]=SP;	                                                           //The Kernel's Stack Pointer Address.
	PCB_TIM[0]=0;		                                                       //The Timeslice Is For The Init Process Runonce. It Can Only Be 0.
	PCB_NAME[0]="Init";														   //The Name For The System Process.
}
/*Finish Function:_Sys_Kernel_Load********************************************/

/*Begin Function:_Sys_Proc_Load************************************************
Description : The Process Loader.Loads The Arguments into Corresponding 
              Slots.If There is Already An Task,The Former One Will Be
			  Lost,But The System Will Not Encounter Errors.
Input       : void*Task -The Pointer Of The Task Function.
              u32 SP-The Initial Stack Pointer Of The Task Function. 
			  u8 PID-The PID For The Process.
			  u32 Ticks -The Initial Timeslice For It,In Ticks.The TimeSlice
			  Must Be Bigger Than The Process Switching Period.
			  u8* Name -The Name Of The Process.A Pointer To A String.
			  Every Process Must Have A Name.
Output      : Void.
******************************************************************************/
void _Sys_Proc_Load(void* Task,u32 SP,u8 PID,u32 Ticks,u8* Name)
{	
	PCB_PID_STATUS[PID]|=OCCUPY;                                               //Indicates That This PID Is In Use.
	PCB_SP[PID]=SP;	
	PCB_TIM[PID]=Ticks;                                                        //The number of Ticks to run. Must Be smaller than 0x00FFFFFF
	PCB_NAME[PID]=Name;														   //Fill In The Name.
    PCB_TASK_ADDRESS[PID]=(u32)Task;                                           //The Starting Address Of The Task.
}

/*Finish Function:_Sys_Proc_Load**********************************************/

/*Begin Function:_Sys_Systick_Init*********************************************
Description : Initiate The Timer For Switching Tasks.
Input       : u8 Presc-The Prescaler value to the System Clock.
Output      : Void.
******************************************************************************/
void _Sys_Systick_Init(u32 Ticks)
{		 
	 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00000);			               // Set the Vector Table base address at 0x08000000 
	 if(SysTick_Config(Ticks))
	 {
	 	while(1);                                                              //"1" indicates that there is an error.
	 }
	 NVIC_SetPriority(SysTick_IRQn,0x00);                                      //Set its Priority to the highest.
}
/*Finish Function:_Sys_Systick_Init*******************************************/

/*Begin Function:_Sys_Process_Initialization***********************************
Description : Initiate The Process For Task Switching.Never Call It In User Application.
Input       : u8 PID -The PID Of The Process To Initialize..
Output      : Void.
******************************************************************************/
void _Sys_Process_Initialization(u8 PID)
{
    void(*Process_Pointer)(void)=(void(*)(void))PCB_TASK_ADDRESS[PID];
    *((u32*)(PCB_SP[PID]+0x04*(9-8)))=0xFFFFFFF9;	                           //Note That When The Compiler Enters Interrupt,It Will Automatically Add"PUSH {R4,LR}",And When It Quits,"POP{R4,PC}".
	*((u32*)(PCB_SP[PID]+0x04*(17-8)))=0x01000200;                             //for xPSR. fill the T bit,or an INVSTATE will happen.	 
	*((u32*)(PCB_SP[PID]+0x04*(16-8)))=(u32)Process_Pointer;                   //Set The Function Pointer.
}
/*Finish Function:_Sys_Process_Initialization*********************************/

/*Begin Function:_Sys_Start****************************************************
Description : The Function For Starting The Operating System.
Input       : Void.
Output      : Void.
******************************************************************************/
void _Sys_Start(void)
{
    Current_PID=0;
    PCB_PID_STATUS[0]|=INITED;                                                 //Set The "INITED" Flag.
    _Sys_Process_Initialization(Current_PID);                                  //The Current_PID Is 0 Here.We're Starting From The Init Process.
    SYS_LOAD_SP();                                                             //Load Its Stack Pointer.
    _Sys_Init();
}
/*Finish Function:_Sys_Start**************************************************/

/*Begin Function:SysTick_Handler***********************************************
Description : The Function For Switching Tasks.
Input       : Void.
Output      : Void.
******************************************************************************/
void SysTick_Handler(void)
{	   
    DISABLE_ALL_INTS();	                                                       //Disable All Interrupts.
    						                                                   
    Global_Process_Spin_Lock++;                                                //Shift The Spin Lock.
    
    if(PCB_REMAINING_TIME[Current_PID]==0)                                     //If Its Time Has Been Used Up.
    {
        SYS_SAVE_SP();                                                         //Save The SP To PCB_SP Now.
        
        Current_PID=PCB_TASK_SEQ[PCB_TASK_SEQ_PTR++];                          //Switch To The Next Process.
        Next_PID=PCB_TASK_SEQ[PCB_TASK_SEQ_PTR];                               //The Next_PID Is Purely For Information Acquire Now.
        
        if((PCB_PID_STATUS[Current_PID]&INITED)==0)                            //The Next Process Is Not Initialized Yet.Switching To It Directly Will Cause A Hardfault.
        {
            PCB_PID_STATUS[Current_PID]|=INITED;                               //Set The "INITED" Flag.
            _Sys_Process_Initialization(Current_PID);                          //Initialize The Process Now.
        }

        SYS_LOAD_SP();
        ENABLE_ALL_INTS();                               
    }
    else
    {    
        PCB_REMAINING_TIME[Current_PID]-=1;                                    //The Remaining Runtime Decrease By 1. A Cycle Has Passed.
        ENABLE_ALL_INTS();
    }  
} 
/*Finish Function:Systick_Handler*********************************************/

/*Begin Function:_Sys_Memory_Init**********************************************
Description : Initiate Memory Pages In A Certain Area.
Input       : u32 Start -The Start Address Of The Area.
			  u32 End -The End Address Of The Area.
Output      : Void.
******************************************************************************/
void _Sys_Memory_Init(u32 Start,u32 End)
{	
	Sys_Memset((u8*)Start,0,End-Start+1);				                       //The Sys_Memset Is Based On Byte.If It Is Based On Word,It Will Cause A HardFault Implicitly When Used To Clear The Last Byte Of The Memory.
}
/*Finish Function:_Sys_Memory_Init********************************************/

/*Begin Function:Sys_Start_Process*********************************************
Description : The Task Loader.Loads The Arguments into Corresponding 
              Slots.If There is Already An Task,The Former One Will Be
			  Lost,But The System Will Not Encounter Errors. 
			  The Initial Stack Pointer Of The Task Function.And The PID For The Process
			  Will Be Allocated By The System. 			  
Input       : void*Task-The Pointer Of The Task Function.  
			  u32 Ticks-The Initial Timeslice For It,In Ticks.
			  u8* Name -The Name Of The Process.A Pointer To A String.
			  Every Process Must Have A Name.
Output      : u8- The PID Of The New Process.Should It Fail,It Will Return "ENOPID"(0x00).
******************************************************************************/
u8 Sys_Start_Process(void* Task,u32 SP,u32 Ticks,u8* Name)	                   //The SP Still Stays Here.In The Next Version,May Be Dummy.
{	
	u8 New_PID,Find_Flag=0;

	DISABLE_SYSTICK();	                                                       //Make Sure It Is Completed Correctly.Atomic Operation.This May Be Reentrantable.
	DISABLE_ALL_INTS();
	
	for(New_PID=2;New_PID<MAX_NUMBER_OF_TASKS+2;New_PID++)                     //From 2. 0 Is Init,1 Is Arch.
		if((PCB_PID_STATUS[New_PID]&OCCUPY)==0)	                               //Note The ANSI C Operator Priority.
		{
			Find_Flag=1;
			break;                                                             //Find An Unoccupied PID.
		}

	if(Find_Flag==0)
	{
		ENABLE_ALL_INTS();	                                                   //Make sure it is completed correctly.atomic operation.This may be reentrantable.
		ENABLE_SYSTICK();	
		return(ENOPID);
	}
	PCB_TASK_ADDRESS[New_PID]=(u32)Task;									   //Fill The Task Pointer.
	PCB_PID_STATUS[New_PID]=OCCUPY|INITED;                                     //Indicates That This PID Is In Use.
	PCB_SP[New_PID]=SP;	
	PCB_TIM[New_PID]=Ticks;                                                    //The number of Ticks to run. Must Be smaller than 0x00FFFFFF
	PCB_NAME[New_PID]=Name;													   //Fill In The Name.

	_Sys_Process_Initialization(New_PID);                                      //Initialize The Task's Stack.
	
	ENABLE_ALL_INTS();	                                                       //Make sure it is completed correctly.atomic operation.This may be reentrantable.
	ENABLE_SYSTICK();

	return(New_PID);                                                           //Return The PID Of The New Process.
}

/*Finish Function:Sys_Start_Process*******************************************/

/*Begin Function:_Sys_Init*****************************************************
Description : Initiate The Application Memory Page Allocation.
Input       : Void.
Output      : Void.
******************************************************************************/
void _Sys_Init(void)						                                   //The Paramount Process;Its PID is 0,forever.	  
{
	u16 Count;
  
    while(1)
    {
        DISABLE_ALL_INTS();
#if(ENABLE_SIGNAL==TRUE)
    	for(Count=1;Count<MAX_NUMBER_OF_TASKS+1;Count++)				       //Process The Signals Here.
	    	_Sys_Signal_Handler(Count);
#endif
                                                                               //The Code Below Sets The Task Sequence.
       PCB_TASK_SEQ_PTR=0;			                                           //Reset The Process PTR.
       for(Count=1;Count<MAX_NUMBER_OF_TASKS+2;Count++)
       {		                                                               //From 1,For "Init" Do Not Count Here.
       		if(((PCB_PID_STATUS[Count]&OCCUPY)!=0)&&((PCB_PID_STATUS[Count]&SLEEP)==0)&&((PCB_PID_STATUS[Count]&ZOMBIE)==0))
            {	
                PCB_REMAINING_TIME[Count]=PCB_TIM[Count];
                PCB_TASK_SEQ[PCB_TASK_SEQ_PTR++]=Count;                        //Find Out The PIDs Occupied&Not Sleeping&Not Zombie And Create The Next Schedule Table.
            }
       }
       PCB_TASK_SEQ[PCB_TASK_SEQ_PTR]=0;			                           //The Last Must Be The Init process,Or The System Will Crash.Remember To Decrease The value By One,Or The System To Crash.
    
       PCB_TASK_SEQ_PTR=0;			                                           //Reset The Process PTR.
      
       ENABLE_ALL_INTS();  
       
       Sys_Switch_Now(); 
    }       
}
/*Finish Function:_Sys_Init***************************************************/

/*Begin Function:_Sys_Arch*****************************************************
Description : The Process Which Does Some Chores.
			  It Has The Highest Priority,"Init" Excluded.
			  However,It Still Use The Common "Task_Load".
              Now It Will Work As A Shell Normally.
Input       : Void.
Output      : Void.
******************************************************************************/
void _Sys_Arch(void)							            
{   
    Sys_Shell();
}
/*Finish Function:_Sys_Arch***************************************************/

/*Begin Function:Sys_Switch_Now************************************************
Description : The Function For Directly Switching The Process,
			  For Giving Up The Remaining Timeslice Of The Caller.
			  In Other Words,It Will Switch To The Next Task Directly.
			  It Is A Inline Function.
Input       : Void.
Output      : Void.
******************************************************************************/
void Sys_Switch_Now(void)
{
    vu8 Old_Process_Spin_Lock;                                                 //Contains The Old Process Spin Lock.
    PCB_REMAINING_TIME[Current_PID]=0;                                         //Switch To The Init Process Directly For Context Switching.
    Old_Process_Spin_Lock=Global_Process_Spin_Lock;                            
    while(Old_Process_Spin_Lock==Global_Process_Spin_Lock);	                   //Halt The Process Before A Switch.
    Old_Process_Spin_Lock=Global_Process_Spin_Lock;
}
/*Finish Function:Sys_Switch_Now**********************************************/

/*Begin Function:Sys_Get_PID***************************************************
Description : Get The Process ID.
Input       : Void.
Output      : u8-The Current PID.
******************************************************************************/
u8 Sys_Get_PID(void)
{
	return(Current_PID);
}
/*Finish Function:Sys_Get_PID*************************************************/

/*Begin Function:main**********************************************************
Description : The Entrance Of The OS.Must Be Set Here.
Input       : Void.
Output      : int-dummy.
******************************************************************************/
int main(void)		                                                           //The main function will only be run ONCE.
{	
	//_Sys_Memory_Init(APP_AVAIL_MEM_START,APP_AVAIL_MEM_END);				   //Not 0x20005000,Or It Will Cause A Implicit Hard Fault.
	_Sys_Kernel_Load(0x20004F50);                                              //The Timeslice Is Dummy.The Init Process Use A Different Way of Context Switching.
	_Sys_Proc_Load(_Sys_Arch,0x20004E00,1,2,(u8*)"Arch");                      //The Arch Process,Which Does Some Chores.Its PID is 1,forever.
	_Sys_Proc_Load(Task1,0x20004C00,2,2,(u8*)"Main_GUI");                      //The GUI Process,Normally.
	_Sys_Proc_Load(Task2,0x20004A00,3,2,(u8*)"Driver_1");			           //The Driver Process 1,Normally.
	//_Sys_Proc_Load(Task3,0x20003900,4,2,(u8*)"Driver_2");                      //The Driver Process 2,Normally.
    _Sys_Systick_Init(0x00010000);                                             //Initialize The System Clock.
	
    _Sys_Start();                                                              //Start From The First PID.
	while(1);												                   //Dead Loop.
	//return(0);											                   //Unreachable Statement.Commented Out.
}
/*Finish Function:main********************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
