/*****************************************************************************
Filename   : typedefs.h
Author     : pry
Date       : 29/04/2012
Description: The Header For The Variable Typedefs. 
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 29/04/2012 Ver1.1.0
  Moved The Definitions From "globals.h" to here.
2.Modified By pry                                        On 30/04/2012 Ver1.2.0
  Modified The "struct Shared_Memory" and The "struct Device".
3.Modified By pry			                             On 27/06/2012 ver1.3.0
  Deleted The "struct List_Head" And Added The Error Managing Module.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _typedefs_h_
#define _typedefs_h_

/*Structs*********************************************************************/
//From Kernel
struct List_Head															   //The Two-Way List For Storing The Window Cascade Sequence.
{
	struct List_Head* Prev;
	struct List_Head* Next;
};
//End From Kernel

//From Memory
struct Shared_Memory
{
	u8 SM_Start_Page;      													   //The size of the shared memory.
	u8 SM_End_Page;
	u8 SM_PID[MAX_PROCESS_SHARE_MEM];       		    				       //The PIDs of Processes sharing the memory.Maximum.
};
//End From Memory

//From Semaphore

//End From Semaphore

//From Filesystem
struct IO_Buffer
{	
	u8    Root_Name;                    									   //The Root Name.A Capital Letter.
	u8	  Operation_PID;													   //The Only PID Allowed To Access It.
	u32   FID_In_FS;												           //The File Identifier In The Filesystem.
	u32   File_Size;				    									   //The Size Of The File,In Bytes.
    void* File_Current_Ptr;													   //This Is a Real Address On The Memory.
																			   //The Buffer Was Cancelled For Saving Memory.
};

struct File_Operations 
{																			   //The C Stardard-like Functions Which Should Be Provided By The Filsystems.
	u8 (*Sys_This_FS_Fopen)(u8* Path,u8 Mode,u8 IO_Buffer,void* Absolute_Start_Address);				
	u8 (*Sys_This_FS_Fread)(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);	  
	u8 (*Sys_This_FS_Fwrite)(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);
	u8 (*Sys_This_FS_Fclose)(u8 IO_Buffer,void* Absolute_Start_Address);
	u8 (*Sys_This_FS_Fseek)(u8 IO_Buffer,s32 Offset,u8 Position,void* Absolute_Start_Address);
	u8 (*Sys_This_FS_Frename)(u8* Path,u8* New_Name,void* Absolute_Start_Address);
	u8 (*Sys_This_FS_Fremove)(u8* Path,void* Absolute_Start_Address);
	u8 (*Sys_This_FS_Get_Fproperty)(u8* Path,void* Property,void* Absolute_Start_Address);
	u8 (*Sys_This_FS_Set_Fproperty)(u8* Path,void* Property,void* Absolute_Start_Address);
	u8 (*Sys_This_FS_Format)(void* Absolute_Start_Address);					   //Low-Level Format The Filesystem At The Assigned Address.	  
};

struct File_System 
{	
	s8 Root_Name;
	u8 Resource_ID;															   //Filesystem Is A Resource.It Has A Resource ID.
	void* Absolute_Start_Address;
	u8 In_Use_PID[MAX_PID_USE_FS];
	u8 (*Sys_Inner_Fopen)(u8* Path,u8 Mode,u8 IO_Buffer,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Fread)(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Fwrite)(void* Buffer,u8 Size,u32 Count,u8 IO_Buffer,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Fclose)(u8 IO_Buffer,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Fseek)(u8 IO_Buffer,s32 Offset,u8 Position,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Frename)(u8* Path,u8* Name,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Fremove)(u8* Path,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Get_Fproperty)(u8* Path,void* Property,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Set_Fproperty)(u8* Path,void* Property,void* Absolute_Start_Address);
	u8 (*Sys_Inner_Format)(void* Absolute_Start_Address);					   //Low-Level Format The Filesystem At The Assigned Address.
};
//End From Filesystem


//From Device
struct Occupy_Resource														   //The Child struct for "struct Semaphore".
{
	u8 PID;               													   //The Process ID.
	u8 Number_Of_Res_Occupied;												   //The Number Of Resources Used.
};	

struct Resource
{
	u8 Res_Type;                                        					   //The Type Of The Resource.
	u8 Total_Res; 															   //Total number of resources/semaphores.
	u8 Res_Left;           							    					   //Resources left.
	struct Occupy_Resource Current_Occupy[MAX_PID_OCCUPY_RES];				   //The Process Occupying The Semaphores Are Registered here.
};
//End From Device

//From GUI
struct Dialog_Box
{
	struct List_Head List;													   //The Control Block Is Organized In A List.
	
	u8 Dialog_ID;  															   //It Must Be Here,The Sequence Can't Be Changed,Or It Will Cause A Fault.

	u8 Dialog_Title[MAX_LEN_OF_DIALOG_TITLE];
	u8 Dialog_Status;												           //The Status Of The Dialog Box.

	void* Dialog_Icon;											               //The Icon Of The Dialog Box.Can Be Omitted.(Must Be In 10*10 Size.) 

	u16 Dialog_Coord_X;													       //These Are The Arguments For The Dialog Box.
	u16	Dialog_Coord_Y;
	u16	Dialog_Length;
	u16	Dialog_Width;

	u8 Dialog_Number_Of_Controls;											   //The Number Of Controls In A Dialog Box.

	struct Dialog_Control_Head* CCB[MAX_CONTROL_IN_DIALOG];				       //The Pointers Point To The Properties Of The Items.It Saves The Memory.
};																 

struct Dialog_Control_Head													   //The Control Block Primitives For The Dialog Controls.
{
	u8 Category;													           //The Category Of The Control.
	u8 Dialog_ID;															   //The ID Of The Dialog Box It Is In.
	u8* Name;								                                   //The Name Of The Control.Can't Be Longer Than The "MAX_LEN_OF_CONTROL". 
};

struct Command_Button_Property
{
	struct Dialog_Control_Head Head;										   //The Common Head For The Dialog Boxes.

	u16 Relative_Coord_X;													   //The Relative Coordinations.
	u16	Relative_Coord_Y;

	u16 Length;																   //Its Size.
	u16 Width;

	u8 Status;																   //Its Status.See The "#define"s Above.

	void* Action_Break;														   //The Action When The Mouse Press Breaks On It.
	void* Action_Press;													       //The Action Function When It Is Clicked.It Will Be Executed At Once After Click.

	void* Action_Break_Pass;												   //The Action When The Mouse Passes It Without Pressing It.
	void* Action_Press_Pass;												   //The Action When The Mouse Passes It While Pressing It.
};

struct Check_Box_Property
{
	struct Dialog_Control_Head Head;										   //The Common Head For The Dialog Boxes.

	u16 Relative_Coord_X;													   //The Relative Coordinations.
	u16	Relative_Coord_Y;

	u8 Status;																   //Its Status.See The "#define"s Above.

	void* Action_Break;														   //The Action When The Mouse Press Breaks On It.
	void* Action_Press;													       //The Action Function When It Is Clicked.It Will Be Executed At Once After Click.

	void* Action_Break_Pass;												   //The Action When The Mouse Passes It Without Pressing It.
	void* Action_Press_Pass;												   //The Action When The Mouse Passes It While Pressing It.
};

struct Radio_Button_Property
{
	struct Dialog_Control_Head Head;										   //The Common Head For The Dialog Boxes.

	u16 Relative_Coord_X;													   //The Relative Coordinations.
	u16	Relative_Coord_Y;
	u8 Number;																   //The Number Of Radio Buttons In A Group.
	u8 Status;																   //Its Status.See The "#define"s Above.

	u16* Relative_Centers;							                           //The Relative Centers Of Radio Buttons.
	u8* Radio_Names[MAX_NUMBER_OF_RADIO_IN_GROUP];							   //The Names Of Radio Buttons.

	void* Action_Break[MAX_NUMBER_OF_RADIO_IN_GROUP];						   //The Action When The Mouse Press Breaks On It.
	void* Action_Press[MAX_NUMBER_OF_RADIO_IN_GROUP];						   //The Action Function When It Is Clicked.It Will Be Executed At Once After Click.

	void* Action_Break_Pass[MAX_NUMBER_OF_RADIO_IN_GROUP];					   //The Action When The Mouse Passes It Without Pressing It.
	void* Action_Press_Pass[MAX_NUMBER_OF_RADIO_IN_GROUP];					   //The Action When The Mouse Passes It While Pressing It.
};

struct Tab_Property
{
	struct Dialog_Control_Head Head;										   //The Common Head For The Dialog Boxes.

	u16 Relative_Coord_X;													   //The Relative Coordinations.
	u16	Relative_Coord_Y;	 

	u16 Header_Width;  														   //The Width Of Its Headers
	u16 Header_Lengths[MAX_NUMBER_OF_TAB_IN_GROUP];							   //The Respective Lengths Of The Headers.
	u8* Header_Names[MAX_NUMBER_OF_TAB_IN_GROUP];							   //The Names Of The Tabs In The Same Group.
	u16 Width_Of_Tabs;														   //The Width Of The Tabs.
	u8 Number;																   //The Number Of Tabs.
	u8 Status;																   //Its Status.
	
	u8 Number_Of_Controls_In_Tabs[MAX_NUMBER_OF_TAB_IN_GROUP];				   //The Number Of Controls In Each Tab.
	struct Dialog_Control_Head* Tab_CCB[MAX_NUMBER_OF_TAB_IN_GROUP][MAX_CONTROL_IN_TAB];
																			   //The Pointer To The Controls Of Each Tab.
	void* Action_Break[MAX_NUMBER_OF_TAB_IN_GROUP];							   //The Action When The Mouse Press Breaks On It.
	void* Action_Press[MAX_NUMBER_OF_TAB_IN_GROUP];							   //The Action Function When It Is Clicked.It Will Be Executed At Once After Click.

	void* Action_Break_Pass[MAX_NUMBER_OF_TAB_IN_GROUP];					   //The Action When The Mouse Passes It Without Pressing It.
	void* Action_Press_Pass[MAX_NUMBER_OF_TAB_IN_GROUP];
};

union Control_Pointer														   //The Union Is Used For Diaplaying The Controls.
{
	struct Dialog_Control_Head* Head;

	struct Command_Button_Property* Command_Button;	
	struct Check_Box_Property* Check_Box;
	struct Radio_Button_Property* Radio_Button;
	struct Tab_Property* Tab;
};
//End From GUI

/*End Structs*****************************************************************/

/*Typedefs********************************************************************/
typedef struct Resource SYS_RESOURCE;
typedef struct IO_Buffer SYS_FILE;
typedef struct File_Property SYS_FILE_PROPERTY;
/*End Typedefs****************************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
