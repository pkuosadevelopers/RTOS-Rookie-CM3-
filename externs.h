/******************************************************************************
Filename   : externs.h
Author     : pry
Date       : 25/04/2012
Description: The Externs (Globals) For The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/04/2012 Ver1.0.0
  Standardized it.Better Looks.
2.Modified By pry                                        On 29/04/2012 Ver1.1.0
  Added Some Functions And Classified Them. 
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _externs_h_
#define _externs_h_

/*Global Variables************************************************************/
//PID
extern vu8 Global_Process_Spin_Lock;                                         //For Process Switching.
extern vu32 Current_PID;	          				             			   //For Application Use. These two Must Be u32 For LDR Instruction.
extern vu32 Next_PID;		                                                   //For Application Use.Already Fixed the errors.
//End PID

//PCB
extern vu32 PCB_SP[MAX_NUMBER_OF_TASKS+2];				                       //The SP Is Stored Here.+2,Because The "Init" And "Arch" Process Already Occupies 2 Slots.
extern vu32 PCB_TIM[MAX_NUMBER_OF_TASKS+2+1];                                  //The time must be smaller than 0x00FFFFFF.The last is Dummy,0x00FFFFFE forever,for Fault Prevention..
extern vu32 PCB_REMAINING_TIME[MAX_NUMBER_OF_TASKS+2+1];                       //The Storage Of The Remaining Time.
extern vu32 PCB_TASK_SEQ[MAX_NUMBER_OF_TASKS+2];                               //The sequence of task running list.the last must be 0(kernel).
extern vu32 PCB_TASK_SEQ_PTR;                                                  //The Pointer For PCB_SEQ.
extern vu32 PCB_LEAST_REQUIRED_TICKS[MAX_NUMBER_OF_TASKS+1+1];                 //The Minimum Timeslice For The Application,In Ticks.Used By The Kernel When Scheduling.
extern vu8  PCB_SIGNAL[MAX_NUMBER_OF_TASKS+1+1];	                           //The Array For Signal Storage.
extern vu32 PCB_SIGCUSTOM_HANDLER[MAX_NUMBER_OF_TASKS+1+1];                    //Stores The Entrance Of The SIGCUSTOM Handler.
extern vu8  PCB_PID_STATUS[MAX_NUMBER_OF_TASKS+1+1];                           //The Process Status Register.Also The PID Occupation Status.
extern vu32 PCB_TASK_ADDRESS[MAX_NUMBER_OF_TASKS+1+1];                         //The Starting Address Of Each Task.
extern vu8* PCB_NAME[MAX_NUMBER_OF_TASKS+2];								   //The Process Name Is Registered Here.
//End PCB

//Memory(MCB_SM)
extern vu8  MCB_MEM[NUMBER_OF_MEM_PAGES];                                      //The Memory Allocation Block. Use Paging Method,Each Page is 1/2560 of The Total memory.No Need To Use"u32".
extern volatile struct Shared_Memory MCB_SM[MAX_SHARED_MEM_BLOCKS];            //The Shared Memory Managing Block. 
//End Memory(MCB_SM)

//Signal&Semaphore
extern volatile void (*_Sys_SIGCUSTOM_Handler)(void);		                   //SIGCUSTOM Handler. 
//End Signal&Semaphore

//Device&Resource(RCB)
extern volatile struct Resource RCB[MAX_NUMBER_OF_RES];                        //The Resource Control Block.
//End Device&Resource(RCB)

//Filesystem(FSCB)
extern volatile struct File_System FSCB[MAX_FS_IN_SYS]; 			           //The Filesystem Control Block.
extern volatile struct IO_Buffer IOBCB[MAX_FILE_OPEN];				           //The IO Buffer Control Block.
//End Filesystem(FSCB)

//GUI																           //No "volatile" Is Needed Here Because There Is Only One GUI Process In The System.
extern struct List_Head Dialog_List_Head;
extern struct Dialog_Box DCB[MAX_NUMBER_OF_DIALOG];		                       //The Dialog Control Table.
extern struct List_Head* Dialog_Current_Ptr;                                   //A Pointer On The Dialog Box List.
extern u8 Dialog_List_Exist;								                   //When It Is "1",It Indicates That The List Exists.
//End GUI
/*End Global Variables********************************************************/

/*Assembly Functions Prototypes***********************************************/
extern void DISABLE_ALL_INTS(void);			                                   //Disable All Interrupts.
extern void ENABLE_ALL_INTS(void);			                                   //Enable All Interrupts.
extern void SWITCH_TO_INIT(void);				                    		   //The Critical Function-Task Switching.
extern void SWITCH_OUT_OF_INIT(void);                                          //The Function To Switch Out Of The Init Process.
extern void SYS_LOAD_SP(void);				                                   //Notice That When The Compiler Enters Interrupt,It Will Automatically Add"PUSH {R4,LR}",And When It Quits,"POP{R4,PC}".
extern void SYS_SAVE_SP(void);                                                 //Which May Result In A Trouble.In Experiments We Can Know That The Former LR is	0xFFFFFFF9,So When The Int.Routine Quits,
							                                                   //The PC Will Be 0xFFFFFFF9(This Indicates That The Routine Ends("RET_TO_BASE" Set),As In The case MAY BE?)
extern void DISABLE_SYSTICK(void);                                             //Disable The Systick Timer.
extern void ENABLE_SYSTICK(void);                                              //Enable The Systick Timer.
/*End Assembly Functions Prototypes*******************************************/

/*C Function Prototypes*******************************************************/
//From Kernel
extern void SysTick_Handler(void);					                           //The Task Switcher.It Must Be Exported For Overlaying Another One In The Assembly File.
extern u8 Sys_Start_Process(void* Task,u32 SP,u32 Ticks,u8* Name);             //Start A New Process.
extern u8 Sys_Get_PID(void);                                                   //Get The Process's PID.
extern void Sys_Switch_Now(void);									           //A Inline Function For Switching The Task Directly. 
//End From Kernel

//From Memory
#if (ENABLE_MEMORY_MANAGEMENT==TRUE)
extern void* Sys_Malloc(u32 Bytes);                                            //Allocate Memory.For Application Use.
extern void* _Sys_Malloc(u32 Bytes);                                           //Allocate memory.For System Use.
extern void Sys_Free_Memory(void* Mem_Ptr);                                    //Free The Allocated Memory.For Application Use.
extern void _Sys_Free_Memory(void* Mem_Ptr);                                   //Free The Allocated Memory.For System Use.
extern void Sys_Free_All_Memory(void);		                                   //Free All The Memory Allocated By A Certain Process.For Use In Application.
extern void _Sys_Free_All_Memory(u8 PID);					                   //Free All The Memory Allocated By A Certain Process.For Use In System.
#endif

#if ((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SHARED_MEMORY==TRUE))
extern void* Sys_Shared_Malloc(u8* SMID,u32 Bytes);			                   //Allocate Shared Memory.
extern void* Sys_Join_Shared_Malloc(u8 SMID,u32* Bytes);	                   //Join Shared Memory.
extern void Sys_Free_Shared_Memory(u8 SMID);				                   //Free Shared Memory.
extern void Sys_Free_All_Shared_Memory(void);				                   //Free All Shared Memory Used By A Certain Process.For Application Use.
extern void _Sys_Free_All_Shared_Memory(u8 PID);			                   //Free All Shared Memory Used By A Certain Process.For System Use.
#endif
//End From Memory

//From Signal
#if(ENABLE_SIGNAL==TRUE)
extern void _Sys_Signal_Handler(u8 PID);	                                   //The System Signal Handler.
extern void Sys_Process_Quit(u8 End_Val);		                               //Called The Process To Quit With a End Value.
extern void _Sys_Kill_The_Process(u8 PID);			                           //The SIGKILL Handler,Used By The System.
extern u8 Sys_Get_The_Process_Retval(u8 PID);					               //Get The End Value Of A Zombie Process.
extern void _Sys_Sleep_The_Process(u8 PID);						               //Make a Certain Process Sleep.
extern void _Sys_Wakeup_The_Process(u8 PID);						           //Wake Up A Certain Process.
extern void Sys_Send_Signal(u8 PID,u8 Signal);						           //Send A Signal.
extern void Sys_Register_SIGCUSTOM_Handler(void (*Func)(void));                //SIGCUSTOM Function Registry.
#endif
//End From Signal

//From Resource
#if(ENABLE_RESOURCE==TRUE)
extern u8 Sys_Register_Resource(u8 Resource_Type,u8 Number_Of_Resources);      //Register a resource.
extern u8 Sys_Remove_Resource(u8 Resource_ID);                                 //Remove,Or Unregister a Resource.
extern u8 Sys_Occupy_Resource(u8 Resource_ID,u8 Number_To_Occupy);             //Occupy Resource(s).
extern u8 _Sys_Occupy_Resource(u8 Resource_ID,u8 Number_To_Occupy);            //Occupy Resource(s). For System Use.
extern u8 Sys_Release_Resource(u8 Resource_ID,u8 Number_To_Release);           //Release Resource(s).		
extern u8 _Sys_Release_Resource(u8 Resource_ID,u8 Number_To_Release);	       //Release Resource(s). For System Use.
extern void Sys_Release_All_Resources(void);                                   //Release All Resources Occupied By The Process.For Application Use.
extern void _Sys_Release_All_Resources(u8 PID);						           //Release All Resources Occupied By The Process.For System Use.
extern u8 Sys_Query_Amount(u8 Resource_ID);                                    //Query The Resources Left.No Need to "DISABLE_XXX" And "ENABLE_XXX"
#endif
//End From Resource

//Filesystem
#if(ENABLE_FILESYSTEM==TRUE)												   //The Filesystem Mounting Function.
extern u8 Sys_Mount_FS(u8 Root,void* Absolute_Start_Address,u8 Resource_ID,struct File_Operations* Operations);	 	  							 
extern u8 Sys_Unmount_FS(u8 FS_ID);											   //The Filesystem Unmounting Function.
extern u8 Sys_Low_Level_Format_FS(u8 Root);									   //The Filesystem Low-Level Formatting Function.
extern u8 Sys_Fopen(u8* Path,u8 Mode);										   //The "Fopen" Function.
extern u8 Sys_Fread(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer);			   //The "Fread" Function.
extern u8 Sys_Fwrite(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer);			   //The "Fwrite" Function.
extern u8 Sys_Fclose(u8 IO_Buffer);											   //The "Fclose" Function.
extern u8 _Sys_Fclose(u8 IO_Buffer);										   //The "Fclose" Function For System Use.
extern void _Sys_Close_All_Files(u8 PID); 									   //Close All Files That The Process Opened. For System Use.
extern u8 Sys_Fseek(u8 IO_Buffer,s32 Offset,u8 Position);					   //The "Fseek" Function.
#endif
//End Filesystem

//From Miscellaneous
#if(ENABLE_LIST==TRUE)
extern void Sys_Create_List(struct List_Head* Head);
extern struct List_Head* _Sys_Next_In_List(struct List_Head* Current);
extern struct List_Head* _Sys_Prev_In_List(struct List_Head* Current);
extern struct List_Head* Sys_Search_The_List(struct List_Head* From,struct List_Head* To,u8 Step,u8* Sequence,u8 (*Stop_Condition)(struct List_Head*,void*),void* Custom_Argument);
extern void Sys_List_Delete_Node(struct List_Head* Prev,struct List_Head* Next);																																							   //The List Operating Functions-Delete Node.
extern void Sys_List_Insert_Node(struct List_Head* New,struct List_Head* Prev,struct List_Head* Next); 																																		   //The List Operating Functions-Insert Node.
extern struct List_Head* Sys_List_Seek(struct List_Head* Current,s8 Rewind_Steps);
#endif

#if(ENABLE_SYS_LIB==TRUE)
extern void Sys_Delay(u32 Time);                                                                      													       //The Old-Style Delay Function.
extern u32 Sys_Strlen(u8* String);										                              														   //The Custom "Strlen".
extern u8 Sys_Strcmp(u8* String_0,u8* String_1,u32 Length);                                           													       //The Custom "Strcmp".
extern void Sys_Strcpy(u8* String_0,u8* String_1,u32 Length);                                         													       //The Custom "Strcpy".
extern void Sys_Sprintf(s8* Buffer,s8* String,s32* Args_Int,double* Args_Double,s8** Args_String);    														   //The Custom "Sprintf".
extern void Sys_Sscanf(s8* String,s8* Format,s32* Args_Int,double* Args_Double);                      														   //The Custom "Sscanf".
#endif

extern void Sys_Memset(u8* Address,u8 Char,u32 Size);                                                 														   //The Custom "Memset".
//End From Miscellaneous.h

//From Error.h
void Sys_Restart(void);														   //Restart The System (Usually Used When A Fault Happens).
//End From Error.h

//From GUI.h
#if(ENABLE_GUI==TRUE)
//Arch																						  
extern void _Sys_GUI_LCD_Pin_Input(void);									   //Configure The LCD Pins As Input.For System Use.
extern void _Sys_GUI_LCD_Pin_Output(void);									   //Configure The LCD Pins As Output.For System Use.
extern void _Sys_GUI_LCD_Pin_Init(void);									   //Initialize The LCD Pins.For System Use.
extern void _Sys_GUI_LCD_Write_Data(u16 LCD_DATA);							   //Write Data Directly To LCD.For System Use.
extern u16 _Sys_GUI_LCD_Read_Data(void);									   //Read Data Directly From LCD.For System Use.

extern void _Sys_GUI_LCD_Write_Register(u16 LCD_Reg,u16 LCD_RegValue);		   //Write Data To A Register.For System Use.
extern u16 _Sys_GUI_LCD_Read_Register(u8 LCD_Reg);							   //Read Data From Register.For System Use.

extern u16 Sys_GUI_LCD_Init(void);											   //Initialize The LCD Screen.For System Use.
extern void _Sys_GUI_LCD_Set_Cursor(u16 x,u16 y);							   //Set The LCD Operation Cursor.(Using It In Application Is Deprecated).
extern void Sys_GUI_LCD_Set_Window(u16 StartX,u16 StartY,u16 EndX,u16 EndY);   //Set The Read/Write Window For The LCD.(Using It In Application Is Deprecated).
extern void Sys_GUI_LCD_Clear_Screen(u16 Color);							   //Clear Screen With A Certain Color.
extern void Sys_GUI_LCD_Set_Point(u16 Coord_X,u16 Coord_Y,u16 Color);		   //Draw A Point On The Screen.
extern u16 Sys_GUI_LCD_Get_Point(u16 Coord_X,u16 Coord_Y);					   //Get The Color Of A Certain Dot On The Screen.
extern void Sys_GUI_LCD_Back_Light(u8 Status);								   //Turn On/Off The LCD Backlight.
//End Arch

//API
extern void Sys_GUI_LCD_Draw_Line(u16 Start_Coord_X, u16 Start_Coord_Y, u16 End_Coord_X, u16 End_Coord_Y,u16 Color);										   //Draw A Line On The Screen.
extern void Sys_GUI_LCD_Print_Char(u16 Coord_X,u16 Coord_Y,u8 Char,u8 Size,u16 Char_Color,u16 Background_Color);										       //Print A Character On The Screen.
extern void Sys_GUI_LCD_Print_String(u16 Coord_X,u16 Coord_Y,u8* String,u8 Size,u16 Length,u16 Char_Color,u16 Background_Color);							   //Print A String On The Screen.
extern void Sys_GUI_LCD_GUI_Polygon(u16 Number_Of_Points,u16 (*Coord_Of_Dots)[2],u16 Color);																   //Draw Continue	Lines On The Screen.(Or Polygon).
extern void Sys_GUI_LCD_GUI_Filled_Rectangle(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u16 Border_Color,u16 Fill_Color);									   //Draw A Filled Rectangle On The Screen.
extern void Sys_GUI_LCD_GUI_Circle(u16 Center_Coord_X,u16 Center_Coord_Y,u16 Radius,u16 Color);																   //Draw A Circle On The Screen.
//void Sys_GUI_LCD_GUI_Arc(u16 Center_Coord_X,u16 Center_Coord_Y,u16 Radius,double Start_Angle,double End_Angle,u16 Color);									   //Draw A Arc On The Screen.
extern void Sys_GUI_LCD_Draw_Picture(u16 Start_Coord_X,u16 Start_Coord_Y,u16 Original_Length,u16 Original_Width,u16 *Picture);								   //Draw A Picture On The Screen.
extern void Sys_GUI_LCD_Draw_Dotted_Line(u16 Start_Coord_X, u16 Start_Coord_Y, u16 End_Coord_X, u16 End_Coord_Y,u16 Color);									   //Draw A Dotted Line On The Screen.
extern u16 Sys_GUI_Color_Mixer(u16 Color_1,u16 Color_2,float Ratio);																						   //Mix 2 5R6B5G Color Into One.

extern void Sys_GUI_Draw_Dialog_Box(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,void* Title,void* Icon);													   //Draw A Dialog Box As A Picture.
extern void Sys_GUI_Erase_Dialog_Box(u8 Dialog_ID,u16 Color);																								   //Erase A Dialog Box.
extern void Sys_GUI_Draw_Command_Button(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status);															   //Draw A Command Button As A Picture.
extern void Sys_GUI_Draw_Check_Box(u16 Coord_X,u16 Coord_Y,u8 Status);																						   //Draw A Check Box As A Picture.
extern void Sys_GUI_Draw_Scroll_Bar(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width);																			   //Draw A Scroll Bar As A Picture.
extern void Sys_GUI_Draw_Radio_Button_Group(u16 Coord_X,u16 Coord_Y,u8 Number,u16* Relative_Centers,u8 Status); 											   //Draw Radio Button Group As A Picture On The Screen.
extern void Sys_GUI_Draw_Tab_Group(u16 Coord_X,u16 Coord_Y,u16 Header_Width,u8 Number,u16* Header_Lengths,u16 Width_Of_Tabs,u8 Status);						   //Draw Tab Group As A Picture On The Screen.
extern void Sys_GUI_Draw_Mouse(u16 Coord_X,u16 Coord_Y,u8 Status);																							   //Draw The Mouse On The Screen.
extern void Sys_GUI_Draw_Selected_Bar(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status);																   //Draw A Selected Bar On The Screen.
extern void Sys_GUI_Draw_Progress_Bar(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u16 Front_Beginning_Color,u16 Front_End_Color,u16 Back_Color,double Progress);//Draw A Progress Bar On The Screen.

extern u8 Sys_GUI_Create_Dialog_Box(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status,void* Title,void* Icon);											   //Create A Dialog Box,But Don't Display It At Once.
extern u8 Sys_GUI_Delete_Dialog_Box(u8 Dialog_ID);																											   //Delete A Dialog Box.

extern u8 Sys_GUI_Refresh_Dialog_Box(u8 Dialog_ID);																										       //Refresh A Certain Dialog Box.
extern u8 Sys_GUI_Redefine_Dialog_Box(u8 Dialog_ID,u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status,void* Title,void* Icon);							   //Partially Redefine A Dialog Box.
extern u8 Sys_GUI_Move_Dialog_Box(u16 Coord_X,u16 Coord_Y,u8 Dialog_ID,u8 Mode);																			   //Move A Dialog Box.
extern u8 Sys_GUI_Refresh_All_Dialog_Boxes(void);																											   //Refresh All The Dialog Boxes On The Screen.

extern u8 Sys_GUI_Add_Control_To_Dialog(u8 Dialog_ID,struct Dialog_Control_Head* Control);																	   //Add A Control To The Dialog Box.
extern u8 Sys_GUI_Add_Control_To_Tab(struct Dialog_Control_Head* Tab,u8 Tab_ID,struct Dialog_Control_Head* Control);										   //Add A Control To The Tab.
extern u8 Sys_GUI_Delete_Control_From_Dialog(u8 Dialog_ID,struct Dialog_Control_Head* Control);																   //Delete A Control From The Dialog Box.
extern u8 Sys_GUI_Delete_Control_From_Tab(struct Dialog_Control_Head* Tab,u8 Tab_ID,struct Dialog_Control_Head* Control);									   //Delete A Control From The Tab.

extern u8 Sys_GUI_Get_Clicked_Dialog(u16 Coord_X,u16 Coord_Y);																								   //Get The ID Of The Clicked Dialog Box.
extern u16 Sys_GUI_Get_Clicked_Control(u8 Dialog_ID,u16 Coord_X,u16 Coord_Y);																				   //Get The Clicked Control Of The Dialog Box.
extern u16 Sys_GUI_Get_Clicked_Control_In_Tab(u8 Dialog_ID,u16 Compound_ID,u16 Coord_X,u16 Coord_Y);														   //Get The Clicked Control In A Certain Tab.

extern u8 Sys_GUI_Execute_Clicked_Control_Action_In_Tab(u8 Dialog_ID,u16 Dialog_Compound_ID,u16 Tab_Compound_ID,u8 Mode,u16 Coord_X,u16 Coord_Y);			   //Execute The Corresponding Action Function Of The Control In Tab. 
extern u8 Sys_GUI_Execute_Clicked_Control_Action(u8 Dialog_ID,u16 Compound,u8 Mode,u16 Coord_X,u16 Coord_Y);												   //Execute The Corresponding Action Function Of The Control In Dialog Box. 
//End API
#endif
//End From GUI.h

//From shell.h
#if(ENABLE_SHELL==TRUE)
extern void Sys_Shell(void);
extern void _Sys_StdIO_Init(void);
extern u8 _Sys_SH_Getch(void);
extern void _Sys_SH_Putch(u8 Char);
extern void _Sys_SH_Output(s8* String,s32* Args_Int,double* Args_Double,s8** Args_String);
#endif
//End From shell.h

//From Applications
extern void Task1(void);							                           //For Test Only.
extern void Task2(void); 
extern void Task3(void);                                  
extern void Sub1(void);
extern void Sub2(void);
//End From Applications
/*End C Function Prototypes***************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
