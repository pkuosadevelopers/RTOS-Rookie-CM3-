 /*****************************************************************************
Filename   : defines.h
Author     : pry
Date       : 25/04/2012
Description: The Definitions Of Constants For The OS.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/04/2012 Ver1.0.0
  Standardized it.Better Looks.
2.Modified By pry                                        On 30/04/2012 Ver1.1.0
  Added "Signal" And "Device".
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _defines_h_
#define _defines_h_

/*Defines*********************************************************************/
//Signals
/*Bit Assignment
     7       6      5      4         3         2         1          0
   SIGNAL_(x)[7        :    0]   SIGCUSTOM SIGWAKEUP  SIGSLEEP   SIGKILL*/
#define NOSIG      0x00                                                        //An Empty Signal.
#define SIGKILL    0x01                                                        //Kill the process Instantly.No Effect For Init And Arch.
#define SIGSLEEP   0x02                                                        //Stuck a process(sleep.)
#define SIGWAKEUP  0x04                                                        //Wakeup a process.
#define SIGCUSTOM  0x08                                                        //The Signals Will Be Dealt by the System.
#define SIGNAL_(x) (((x)<<4)+0x10)                                             //The Signals For Application Use.From 0 to 15.They Must Be Dealt With In The Process.
#define SYSSIGMASK 0x0F                                                        //The System Signal Mask.
//End Signals	

//Process Status
/*Bit Assignment
  7       6         5         4            3         2         1         0
Occupy  Zombie    Sleep    Reserved   End_Val[3             :              0]*///The End_Val Is The Return Value When It Quits.  
#define OCCUPY       0x80                                                      //The Process Slot Is Occupied.
#define SLEEP        0x20                                                      //The Process Is Sleeping.
#define ZOMBIE       0x40                                                      //The Process Is A Zombie Process.
#define INITED       0x10                                                      //Set This Bit To 1 Means That The Process Is Initialized Yet.
#define END_VAL_MASK 0x0F                                                      //The AND Mask For The Return Value Of The Process. 
//End Process Status

//Device/Resource
/*Bit Assignment
      7        6        5       4         3         2         1        0 
  EXISTING   READ    WRITE  CHIP_BOARD  Device Class[3         :        0] */
#define EXIST        0x80                    								   //Indicates The struct is occupied or not.
#define DEV_R        0x40                    								   //Readable
#define DEV_W        0x20                    								   //Writeable 
#define ON_CHIP      0x10                    								   //On The Chip.

#define ON_BOARD     0x00                    								   //On The Board.
#define DEV_NR       0x00                    								   //Non-readable.
#define DEV_NW       0x00                    								   //Non-writeable

#define DUMMY        0x00                                                      //Dummy Device.Acts As A Semaphore.

#define SRAM		 0x01					                                   //Static RAM.
#define DRAM         0x02  					                                   //Dynamic RAM.

#define SERIAL_FLASH 0x03					                                   //SPI&IIC.
#define NOR_FLASH	 0x04					                                   //Nor Flash.
#define NAND_FLASH	 0x05					                                   //Nand Flash.

#define INDICATOR    0x06                                                      //LED,Buzzer,etc.

#define MISC_INPUT   0x07                                                      //Custom Keypads,Sensors,Touchscreen Controllers,DAC,etc. 
#define MISC_OUTPUT  0x08					                                   //Serial Port,Simple Logic IC(74/CC/CD),ADC

#define LCD_TFT      0x09					                                   //TFT LCD Screen.
#define LCD_12864    0x0A                    								   //12864 LCD Screen.
#define LCD_1602	 0x0B                    								   //1602 LCD Screen.

#define CPU   		 0x0C					 								   //Another Microprocessor/Microcontroller.The Class is valid only When The Processor Acts As A Co-Processor.
                                                                               //e.g. If The Processor is Programmed To Be A Keypad Scanner(Just Like An ASIC),Then It Should Be In "INPUT" Class.
#define CPLD		 0x0D				     								   //A CPLD.
#define FPGA		 0x0E					 								   //A FPGA.

#define ANALOG       0x0F					 								   //Analog Devices,Such As A Adjustable Resistor.
//End Device/Resource

//Filesystem
#define MAX_FS_IN_SYS    2					 								   //Maximum Number Of Filesystems Mounted At The Same Time.It Can't Be Larger Than 0xFE,Because 0xFF Is Defined As "EFSMNT".	 [1-255]
#define MAX_PID_USE_FS   5					 							       //The Maximum Number Of Processes Using The Same Filesystem.
#define LONGEST_PATH    10					 								   //The Longest Filename That The OS Can Support.																			 [1-255]
#define MAX_FILE_OPEN   10					 								   //Maximum Number Of Files Opened At The Same Time.																			 [1-255]

#define READ            0x01				 								   //These Are Modes For "Fopen".
#define WRITE           0x02
#define ADD             0x04
#define CREATE_FILE     0x08		           								   //(Other Bits Are Reserved)

#define SEEK_SET        0x00                 								   //The "SEEK_SET" Macro For The "Sys_Fseek" Function.
#define SEEK_CUR		0x01 				 								   //The "SEEK_CUR" Macro For The "Sys_Fseek" Function.
#define SEEK_END        0x02				 								   //The "SEEK_END" Macro For The "Sys_Fseek" Function.
//End Filesystem

//GUI	

//Begin:LCD Color Definitions.(5R6G5B)
#define WHITE          0xFFFF
#define BLACK          0x0000
#define GREY           0xF7DE
#define BLUE_1         0x001F
#define BLUE           0x001F
#define BLUE_2         0x051F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define TRANSPARENT    0x0001  												   //The Transparent Color(In Fact,It Won't Be Printed On The Screen).
//End:LCD Color Definitions.

//Begin Character Size Definitions.
#define _8X16_         0x00
#define _16X24_        0x01
//End Character Size Definitions.

//Begin Polygon Definitions
#define PI             (double)3.14159265
//End Polygon Definitions

//Begin Dialog Box Settings
#define CLEAR_CONTROL_DIALOG_ID           0xFF								   //The Control's Dialog ID Clearer.
//End Dialog Box Settings.

//Begin Dialog Box Move Modes
#define REAL_MOVE              			  0x80								   //The Move Is A Real One(Will Update The DCB.).
#define SHOW_WHILE_MOVING       		  0x40								   //Show The Window Contents While Moving The Dialog Box.
#define DIALOG_ERASE_COLOR      		  GREEN								   //The Background Color Used When Erasing A Dialog Box.
//End Dialog Box Move Modes

//Begin Mouse Actions.
#define BREAK							0x00								   //The Action When The Mouse Press Breaks On It.
#define PRESS							0x01							       //The Action Function When It Is Clicked.It Will Be Executed At Once After Click.
#define BREAK_PASS						0x02								   //The Action When The Mouse Passes It Without Pressing It.
#define PRESS_PASS						0x03								   //The Action When The Mouse Passes It While Pressing It.
//End Mouse Actions.

//Begin Dialog Box Status
/*Bit Assignment
    7      6        5        4         3        2      1     0
  EXIST  ACTIVE  MOVEABLE  ENABLE   VISIBLE   Reserved[2   :   0]*/
#define DIALOG_EXIST                      0x80								   //The Window Exists.
#define DIALOG_VACANT                     0x00								   //The Window Does Not Exist.
#define DIALOG_ACTIVE                     0x40								   //The Window Is Active Now.
#define DIALOG_INACTIVE                   0x00								   //The Window Is Inactive Now.
#define DIALOG_MOVEABLE                   0x20								   //The Window Can Be Moved.
#define DIALOG_FIXED                      0x00								   //The Window Is Fixed.
#define DIALOG_ENABLED                    0x10								   //The Operations To The Window Is Enabled.
#define	DIALOG_DISABLED 			      0x00								   //The Operations To The Window Is Disabled.
#define DIALOG_VISIBLE                    0x08 								   //The Dialog Box Is Visible.
//End Dialog Box Status

//Begin Dialog Box Category
#define COMMAND_BUTTON			          0x01						           //A Command Button.
#define RADIO_BUTTON			          0x02								   //A Radio Button.
#define TOGGLE					          0x03								   //A Toggle Button.
#define CHECK_BOX				          0x04								   //A Checkbox.
#define TAB						          0x05								   //A Tab.
#define SCROLL_LIST				          0x06								   //A Scroll List.
#define DROPDOWN_MENU			          0x07								   //A DropDown Menu.
#define SPIN_BOX				          0x08								   //A Spin Box.
#define PROGRESS_BAR                      0x09                                 //A Progress Bar.
//#define TREE_VIEW				          0x0A								   //Unused For Now.
//#define WEB_BROWSER                     0x0B								   //Unused For Now.
//#define INPUT_GRID                      0x0C							  	   //Unused For Now.
//#define TOOLBAR                         0x0D							 	   //Unused For Now.
//End Dialog Box Category

//Begin Command Button
/*Bit Assignment
     7         6           5     4     3     2     1     0
   PUSHED   DOTLINE    RESERVED[5             :          0]*/
#define COMMAND_BUTTON_PUSHED                0x80					 		   //The Command Button -Pushed.
#define COMMAND_BUTTON_NORMAL                0x00					 		   //The Command Button -Normal.
#define COMMAND_BUTTON_WITH_DOTTED_LINE		 0x40                    		   //The Command Button With Dotted Line Around It.
//End Command Button

//Begin Check Box
/*Bit Assignment
     7            6              5     4     3     2     1     0
   CHECKED   BEINGCHECKED    RESERVED[5             :          0]*/
#define CHECK_BOX_CHECKED					 0x80							   //The Checked Check Box. 
#define CHECK_BOX_UNCHECKED					 0x00					 		   //The Unchecked Check Box.
#define CHECK_BOX_BEING_CHECKED				 0x40					 		   //The Check Box Is Being Checked Now.

#define CHECK_BOX_SIZE					 	 13						 		   //The Size Of Checkboxes.It Takes Effect When The Checkbox Is Being Drawn.
//End Check Box.

//Begin Radio Button
/*Bit Assignment
     7            6            5      4      3       2          1          0
   CHECKED   BEINGCHECKED    CHECKED_NUMBER[5:3]    BEING_CHECKED_NUMBER[2:0]*/
#define RADIO_BUTTON_CHECKED                 0x80					     	   //The Radio Button Group Is Checked.(Operated At Least Once Since Its Appearance)
#define RADIO_BUTTON_BEING_CHECKED           0x40					           //The Radio Button Group Is Being Checked.
#define RADIO_BUTTON_CHECKED_NUM_(x)		 ((x)<<3)				           //The Checked Number.
#define RADIO_BUTTON_BEING_CHECKED_NUM_(x)	 ((x))					           //The Being-Checked Number.

#define MAX_NUMBER_OF_RADIO_IN_GROUP      8                                    //The Maximum Number Of Radio Buttons In A Control Group.		  [0-8]
#define RADIO_BUTTON_SIZE       13						                       //The Size Of Radio Buttons.It Only Takes Effect When Deciding Which Radio Button Is Pressed.(The Graphics Is Fixed.)
//End Radio Button

//Begin Tabs
#define TAB_SHOWING_(x)                      ((x))                   		   //The Tabs Being Selected.
#define TAB_SELECTED_ADD_WIDTH               0x03							   //The Additional Width Of A Selected Tab Header.
#define MAX_NUMBER_OF_TAB_IN_GROUP		  4							 		   //The Maximum Number Of Tabs In A Control Group.				  [0-254]

#define MAX_CONTROL_IN_TAB                5                                    //The Maximum Number Of Controls In A Single Tab.				  [0-255]
#define TAB_NOT_HEADER                    0xFF                                 //If You Clicked A Tab,But You Didn't Click The Header,Then This Will Be Returned For Further Investigation.
#define TAB_AS_DIALOG_ID				  0xFF						           //The Tab's Dialog ID. Dummy In Fact.
//End Tabs

//Begin Mouse Types
#define MOUSE_NORMAL           			0x00					 			   //The Normal Mouse.
#define MOUSE_BUSY             		 	0x01								   //The Busy Mouse.
#define MOUSE_QUESTION	        		0x02								   //The Mouse With A Question Mark.

#define MOUSE_PRESS_HAND        		0x03								   //The Hand-Like Mouse For Hyperlinks.
#define MOUSE_ENTER_TEXT        		0x04								   //The Text Modifying Cursor.
#define MOUSE_STOP              		0x05								   //The "Not Available" Cursor.

#define MOUSE_MOVE              		0x06 								   //The Mouse For Moving The Window.
#define MOUSE_ADJ_LENGTH    			0x07								   //The Mouse For Adjusting The Length.
#define MOUSE_ADJ_WIDTH		    		0x08								   //The Mouse For Adjusting The Width.
#define MOUSE_ADJ_TOP_LEFT   			0x09								   //The Mouse For Adjusting The Top-Left To Bottom-Right Line.
#define MOUSE_ADJ_TOP_RIGHT     		0x0A								   //The Mouse For Adjusting The Bottom-Left To Top-Right Line.

#define MOUSE_ACCURATE_POSITION 		0x0B								   //The Mouse For Accurate Locating An Object.

#define MOUSE_INVISIBLE        			0xFF								   //A Invisible Mouse.
//End Mouse Types

//Begin Select Bar
#define	BAR_SELECTED       	  	    0x80								 	   //The Selected Bar.
#define BAR_UNSELECTED				0x00									   //The Unselected Bar.
#define BAR_WITH_DOTTED_LINE        0x40									   //The Bar With A Dotted Outline.
#define BARCOLOR                    (0<<11)|(0<<5)|(16)					 	   //The Blue Color Of The Bar.
//End Select Bar
//End GUI

//Miscellaneous
#define END_SEARCH      			0x01			        				   //The Return Value To Stop Searching For The Custom Function In "_Sys_Search_The_List".
//End Miscellaneous	

//Error
#define ENONOD         				0x00 									   //The "Error_No_Node(_In_List)" Error For "_Sys_Search_The_List".

#define ENOPID          			0x00				 					   //The "Error_No_PID" Error For "Sys_Start_Process" In <kernel.c>
#define ENOMEM          			0x00				 					   //The "Error_No_Memory" Error For "Sys_Malloc" In <memory.c>
#define ENOSHM          			0x00				 				       //The "Error_No_Shared_Memory" Error For "Sys_Shared_Malloc" And "Sys_Join_Shared_Malloc" In <memory.c>
#define ENORCB          			MAX_NUMBER_OF_RES+1	 					   //The "Error_No_Resource_Control_Block" Error For "Sys_Register_Resource" In <resource.c>.
#define ENORES          			0x01                 					   //The "Error_No_Resource"	Error For "Sys_Remove_Resource","Sys_Occupy_Resource","_Sys_Occupy_Resource","Sys_Release_Resource" And "_Sys_Release_Resource" In <resource.c>.
#define ENORET          			0xF0                 				 	   //The "Error_No_Return_Value" Error For "Sys_Get_The_Process_Retval" In <signal.c>.

#define EFSMNT						0xFF                 					   //The "Error_Filesystem_Mount"	Error For "Sys_Mount_FS" In <filesystem.c>.
#define ENOUMT						0x01                 					   //The "Error_No_(Filesystem_)Unmount" Error For "Sys_Unmount_FS" In <filesystem.c>.
#define ENOFSY						0x01				 					   //The "Error_No_Filesystem" Error For "Sys_Low_Level_Format_FS" In <filesystem.c>.
#define ENOFIL          			MAX_FILE_OPEN+1		 					   //The "Error_No_File" Error For "Sys_Fopen" In <filesystem.c>.
#define ENOFOP          			0x01                 					   //The "Error_No_File_Operations" Error For "Sys_Fread","Sys_Fwrite","Sys_Fclose","Sys_Fseek"(Preliminarily) In <filesystem.c>.

#define ENODIA          			0xFF			        				   //The "Error_No(_More)_Dialog" Error For "Sys_GUI_Create_Dialog_Box","_Sys_GUI_Draw_Tab_Controls","_Sys_GUI_Draw_Dialog_Controls","Sys_GUI_Refresh_All_Dialog_Boxes",
                                                                    		   //"Sys_GUI_Get_Clicked_Dialog","Sys_GUI_Get_Clicked_Control","Sys_GUI_Get_Clicked_Control_In_Tab" And "Sys_GUI_Delete_Dialog_Box" In <GUI.c>.  	    
#define ENODOP          			0x00                    				   //The "Error_No_Dialog_Operation" Error For "_Sys_GUI_Draw_Tab_Controls","_Sys_GUI_Draw_Any_Control","_Sys_GUI_Draw_Any_Control_For_Tabs","Sys_GUI_Refresh_Dialog_Box",
                                                                    		   //"Sys_GUI_Move_Dialog_Box","Sys_GUI_Add_Control_To_Dialog","Sys_GUI_Add_Control_To_Tab","Sys_GUI_Delete_Control_From_Tab","Sys_GUI_Delete_Control_From_Dialog",
																			   //"Sys_GUI_Execute_Clicked_Control_Action_In_Tab","Sys_GUI_Execute_Clicked_Control_Action" And "Sys_GUI_Redefine_Dialog_Box" In <GUI.c>.
//End Error	  
/*End Defines*****************************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
