/******************************************************************************
Filename    : shell.c
Author      : pry 
Date        : 17/07/2012
Description : The Shell For The Operating System.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 17/07/2012 Ver1.0.0
  Created The File Only.
2.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Created The Parser And The Executer.
3.Modified By pry                                        On 25/07/2012 Ver3.0.0
  The Basic Commands Can Now Be Used.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "shell.h"
/*End Includes****************************************************************/

/*Config Reporter*************************************************************/
#if(REPORT_CONFIG==TRUE)
	#if(ENABLE_SHELL==TRUE)
		#if(ENABLE_MEMORY_MANAGEMENT==FALSE)
			#error Please Enable The Memory Management Module Before Enabling The Shell Module.
		#endif

		#if(ENABLE_SYS_LIB==FALSE)
			#error Please Enable The System Library Module Before Enabling The Shell Module.
		#endif

		#if((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SYS_LIB==TRUE))
			#warning The Shell Module Is Enabled.
		#endif 
	#else
		#warning The Shell Module Is Disabled.
	#endif
#endif
/*End Config Reporter*********************************************************/

/*Begin Function:Sys_Shell*****************************************************
Description : The Entrance Of The Rookie Shell.
Input       : Void.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void Sys_Shell(void)
{
	s32 Function_ID=0;
	_Sys_StdIO_Init();

	while(1)
	{	
		_Sys_SH_Get_Command(Command);									       //Get The Command.

		Function_ID=_Sys_SH_Parse_Command(Command,Arguments); 				   //Parse The Command.
		                      							   
		_Sys_SH_Execute_Command(Function_ID,Arguments);						   //Execute The Command.		
	}
}
#endif
/*Finish Function:Sys_Shell***************************************************/

/*Begin Function:_Sys_SH_Get_Command*******************************************
Description : The Function To Get The Command.For System Use.
Input       : u8* Command -The Argument Is For Output.It Outputs The Command As A
              String.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_Get_Command(u8* Command)
{
	u8 Char_Got=0;
	u8 Char_Count=0;
	u8* Command_Original_Position=Command;									   //Stores The Original Position Of The Command Line Buffer.

	_Sys_SH_Output("[Root@Rookieroot] ",0,0,0);

	while(Char_Got!='\r')              										   //If The Character Got Is Not A "Enter".Here We Do Not Use The "\n" Because '\r' Follows It.
	{
		Char_Got=_Sys_SH_Getch();      										   //Then Load The Character Into The Command Area. The "\n\r" Is Also Loaded Into It.
		DISABLE_ALL_INTS();													   //Atomic Operation.

		if(Char_Got==0x08)													   //If The Input Is Backspace.
		{	
			if(Command!=Command_Original_Position)							   //If The Pointer Don't Move Back To The Beginning,Then Move It Backwards.
			{
				Command--;													   //Clear The Last Character.
				Char_Count--;												   //Decline The Character Count.

				_Sys_SH_Putch('\b');										   //Move The Cursor Backwards,But Do Not Delete The Character.
				_Sys_SH_Putch(' ');											   //Output A Space,It Looks As If The Character Is Deleted.
				_Sys_SH_Putch('\b');										   //Move Backward Again.
			}
																   
			*Command='\0';													   

		}
		else
		{		
			_Sys_SH_Putch(Char_Got);										   //Display The Character.
			*Command=Char_Got;
			Command++;
			Char_Count++;
		}

		if(Char_Count==COMMAND_SPACE-2) 									   //2 Spaces Is Reserved Here.If The Command Line Is Too Long,Cut It Off.
		{
			ENABLE_ALL_INTS();
			break;
		}

		ENABLE_ALL_INTS();
	}

	DISABLE_ALL_INTS();
	*Command='\0';

	_Sys_SH_Putch('\n');													   //For Displaying The "[Root@Rookieroot] " Correctly.
	_Sys_SH_Putch('\r');
	ENABLE_ALL_INTS();
}
#endif
/*Finish Function:_Sys_SH_Get_Command*****************************************/

/*Begin Function:_Sys_SH_Parse_Command*****************************************
Description : The Parser Of The Rookie Shell.
Input       : u8* Command -The Command Line.
              void* Arguments -The Arguments To Input.
Output      : int -The Function Identifier.Indicates The Function To Execute.
              Should It Be -1,The Shell Will Quit.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
s32 _Sys_SH_Parse_Command(u8* Command,void* Arguments)
{
	DISABLE_ALL_INTS();														   //Atomic Operation For Function Reentrance Avoidance.
	if(Sys_Strcmp(Command,STR("ls"),2)==0)
	{
		Command+=(2+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_LS);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("pwd"),3)==0)
	{
		Command+=(3+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_PWD);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("du"),2)==0)
	{
		Command+=(2+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_DU);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("mkdir"),5)==0)
	{
		Command+=(5+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		return(COMMAND_MKDIR);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("find"),4)==0)
	{
		Command+=(4+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_FIND);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("cp"),2)==0)
	{
		Command+=(2+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_CP);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("mv"),2)==0)
	{
		Command+=(2+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_MV);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("rm"),2)==0)
	{
		Command+=(2+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.                                         //Move The Command Pointer To Send In The Correct Argument.                                         //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_RM);                                     			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("edit"),4)==0)
	{
		Command+=(4+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_EDIT);                                   			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("top"),3)==0)
	{
		Command+=(3+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_TOP);                                    			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("signal"),6)==0)
	{
		Command+=(6+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_SIGNAL);                                    			   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("restart"),7)==0)
	{
		Command+=(7+1);                                         			   //Move The Command Pointer To Send In The Correct Argument.
		Sys_Strcpy(Command,(u8*)Arguments,MAX_STR_LEN);         			   //Copy The Arguments.
		ENABLE_ALL_INTS();
		return(COMMAND_RESTART);                                    		   //Return The Command Number.
	}

	if(Sys_Strcmp(Command,STR("quit"),4)==0)                    			   //The "Quit Shell" Command.
	{
		ENABLE_ALL_INTS();
		return(COMMAND_QUIT);
	}

	if(((*Command)!='\n')&&((*Command)!='\r')&&((*Command)!='\0'))			   //If The Command Contains Characters But Can't Be Parsed,Then It Is A Unknown One.
	{
		ENABLE_ALL_INTS();
		return(COMMAND_NOT_KNOWN);											   //The Command Is Unknown.
	}

	ENABLE_ALL_INTS();													       //The Parsing Failed.The Command Can't be Recognized.
	return(COMMAND_NONE);
}
#endif
/*Finish Function:_Sys_SH_Parse_Command***************************************/

/*Begin Function:_Sys_SH_Execute_Command***************************************
Description : The Executer Of The Rookie Shell.
Input       : s32 Function_ID -The Function ID.Returned By "_Sys_SH_Parse_Command".
              void* Arguments -The Arguments To Input.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_Execute_Command(s32 Function_ID,void* Arguments)
{
	switch(Function_ID)
	{
		case COMMAND_LS:_Sys_SH_ls(Arguments);break;
		case COMMAND_PWD:_Sys_SH_pwd(Arguments);break;
		case COMMAND_DU:_Sys_SH_du(Arguments);break;
		case COMMAND_MKDIR:_Sys_SH_mkdir(Arguments);break;
		case COMMAND_FIND:_Sys_SH_find(Arguments);break;
		case COMMAND_CP:_Sys_SH_cp(Arguments);break;
		case COMMAND_MV:_Sys_SH_mv(Arguments);break;
		case COMMAND_RM:_Sys_SH_rm(Arguments);break;
		case COMMAND_EDIT:_Sys_SH_edit(Arguments);break;
		case COMMAND_TOP:_Sys_SH_top(Arguments);break;
		case COMMAND_SIGNAL:_Sys_SH_signal(Arguments);break;
		case COMMAND_RESTART:_Sys_SH_restart(Arguments);break;

		case COMMAND_QUIT:Sys_Send_Signal(Sys_Get_PID(),SIGKILL);break;        //The Break Is Dummy.Send A Kill Signal To Itself To Quit Now.
		case COMMAND_NOT_KNOWN:_Sys_SH_Unknown_Command();break;
		case COMMAND_NONE:break;
		default:break;
	}
}
#endif
/*Finish Function:_Sys_SH_Execute_Command*************************************/

/*Begin Function:_Sys_SH_Unknown_Command***************************************
Description : The Function Which Displays Back The Information Of An Unknown Command.
Input       : Void.
Output      : Void.
******************************************************************************/
void _Sys_SH_Unknown_Command(void)
{
	_Sys_SH_Output("The Command Input Is Not Found.\n\r\0",0,0,0);
}
/*Finish Function:_Sys_SH_Unknown_Command*************************************/

/*Begin Function:_Sys_SH_ls****************************************************
Description : The Function For Further Executing The "ls" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_ls(void* Arguments)
{
	_Sys_SH_Output("ls Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_ls**************************************************/

/*Begin Function:_Sys_SH_pwd***************************************************
Description : The Function For Further Executing The "pwd" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_pwd(void* Arguments)
{
	_Sys_SH_Output("pwd Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_pwd*************************************************/

/*Begin Function:_Sys_SH_du****************************************************
Description : The Function For Further Executing The "du" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_du(void* Arguments)
{
	_Sys_SH_Output("du Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_du**************************************************/

/*Begin Function:_Sys_SH_mkdir*************************************************
Description : The Function For Further Executing The "mkdir" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_mkdir(void* Arguments)
{
	_Sys_SH_Output("mkdir Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_mkdir***********************************************/

/*Begin Function:_Sys_SH_find**************************************************
Description : The Function For Further Executing The "find" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_find(void* Arguments)
{
	_Sys_SH_Output("find Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_find************************************************/

/*Begin Function:_Sys_SH_cp****************************************************
Description : The Function For Further Executing The "cp" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_cp(void* Arguments)
{
	_Sys_SH_Output("cp Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_cp**************************************************/

/*Begin Function:_Sys_SH_mv****************************************************
Description : The Function For Further Executing The "mv" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_mv(void* Arguments)
{
	_Sys_SH_Output("mv Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_mv**************************************************/

/*Begin Function:_Sys_SH_rm****************************************************
Description : The Function For Further Executing The "rm" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_rm(void* Arguments)
{
	_Sys_SH_Output("rm Command Is Being Executed!\n\r\0",0,0,0);
}
#endif
/*Finish Function:_Sys_SH_rm**************************************************/

/*Begin Function:_Sys_SH_edit**************************************************
Description : The Function For Further Executing The "edit" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_edit(void* Arguments)
{
	_Sys_SH_Output("editor Will Be Started!\n\r\0",0,0,0);
}
#endif
/*Begin Function:_Sys_SH_edit*************************************************/

/*Begin Function:_Sys_SH_top***************************************************
Description : The Function For Further Executing The "top" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_top(void* Arguments)
{	
	s32 Count;
	s32 Memory_Count;
	s32 Print_Integer[2];
		
	double CPU_Percentage;
	s32 Occupied_Memory;	   
	s32 CPU_Total_Time;
																	       	   //This Is The Table Header.
	_Sys_SH_Output("| PID |     Name    |  Status  |  CPU  |  Memory   |\n\r\0",0,0,0);
	_Sys_SH_Output("|  0  |     Init    |  Running | 0.0%%  |  0 Byte   |\n\r\0",0,0,0);
		   
	CPU_Total_Time=0;
	for(Count=1;Count<MAX_NUMBER_OF_TASKS+1+1;Count++)						   //Calculate The Total CPU Time.
	{																		   //If The Process Is Running Now.
		if(((PCB_PID_STATUS[Count]&OCCUPY)!=0)&&((PCB_PID_STATUS[Count]&SLEEP)==0)&&((PCB_PID_STATUS[Count]&ZOMBIE)==0))     								   
			CPU_Total_Time+=PCB_TIM[Count];	
	}

	for(Count=1;Count<MAX_NUMBER_OF_TASKS+1+1;Count++)
	{		 
		if((PCB_PID_STATUS[Count]&OCCUPY)!=0)
		{
			CPU_Percentage=(((float)PCB_TIM[Count]/(float)CPU_Total_Time)*(float)100);		   //Calculate The CPU Percentage.

			Occupied_Memory=0;
			for(Memory_Count=0;Memory_Count<NUMBER_OF_MEM_PAGES;Memory_Count++)//Calculate The Memory Consumption.
				if(MCB_MEM[Memory_Count]==Count)							   //It Means That This Part Of Memory Is Used By The Process.
					Occupied_Memory+=SIZE_OF_EACH_PAGE;						

			Print_Integer[0]=Count;
			Print_Integer[1]=Occupied_Memory;

			if((PCB_PID_STATUS[Count]&SLEEP)!=0)							   //Classify The Process And Print Corresponding Messages.
				_Sys_SH_Output("|  %d  |     %s     | Sleeping | 0.0 %% |  %d Byte |\n\r\0",Print_Integer,0,(s8**)(&PCB_NAME[Count]));	
			else if((PCB_PID_STATUS[Count]&ZOMBIE)!=0)
				_Sys_SH_Output("|  %d  |     %s     |  Zombie  | 0.0 %% |   0 Byte |\n\r\0",Print_Integer,0,(s8**)(&PCB_NAME[Count]));	
			else
				_Sys_SH_Output("|  %d  |     %s     |  Running | %1f %% |  %d Byte |\n\r\0",Print_Integer,&CPU_Percentage,(s8**)(&PCB_NAME[Count]));	
		}				
	}
}
#endif
/*Begin Function:_Sys_SH_top**************************************************/

/*Begin Function:_Sys_SH_signal************************************************
Description : The Function For Further Executing The "signal" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_signal(void* Arguments)
{
	s32 PID=0;
	s32 Signal_Number;														   //The PID Must Be s32 For "Sys_Sscanf".

	Sys_Sscanf((s8*)Arguments,"%d",&PID,0);									   //Get The PID.

	if(PID==0)																   //If The Command Is Not Complete,Or Not Legal(Send Signal To The Init Process),It Will Fail.
	{	
		_Sys_SH_Output("The Signal Is Illegal.\n\r\0",0,0,0);	
		return;
	}

	if(Sys_Strcmp(Arguments,STR("SIGKILL"),7)==0)
	{
		Sys_Send_Signal((u8)PID,SIGKILL);
		return;
	}

	if(Sys_Strcmp(Arguments,STR("SIGSLEEP"),8)==0)
	{
		Sys_Send_Signal((u8)PID,SIGSLEEP);
		return;
	}

	if(Sys_Strcmp(Arguments,STR("SIGWAKEUP"),9)==0)
	{
		Sys_Send_Signal((u8)PID,SIGWAKEUP);
		return;
	}

	if(Sys_Strcmp(Arguments,STR("SIGCUSTOM"),9)==0)
	{
		Sys_Send_Signal((u8)PID,SIGCUSTOM);
		return;
	}

	if(Sys_Strcmp(Arguments,STR("SIGNAL_("),8)==0)
	{																		   //Sends Out The Custom Signal.
		Signal_Number=PID;													   //The "PID" Is The "x" In "SIGNAL_(x)" In Fact.
		PID=0;			   													   //Reset The PID To See If The Command Is Complete Or Legal.
		Sys_Sscanf((s8*)Arguments+11,"%d",&PID,0);							   //This One Should Be The PID.
		if(PID==0)
		{	
			_Sys_SH_Output("The Signal Is Illegal.\n\r\0",0,0,0);				   //Illegal.
			return;
		}
		Sys_Send_Signal((u8)PID,SIGNAL_((u8)Signal_Number));
		return;
	}

	_Sys_SH_Output("The Signal Is Illegal.\n\r\0",0,0,0);				           //If No Valid Signal Is Recognized,Then It Must Have Been Illegal.
}
#endif
/*Begin Function:_Sys_SH_signal***********************************************/

/*Begin Function:_Sys_SH_restart***********************************************
Description : The Function For Further Executing The "signal" Command And 
              Displaying Back The Information.
Input       : void* Arguments -Its Arguments.The Arguments.The Redundant
              Characters Will Be Automatically Eradicated By The Function.
Output      : Void.
******************************************************************************/
void _Sys_SH_restart(void* Arguments)
{
	Sys_Restart();
}
/*Finish Function:_Sys_SH_restart*********************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
