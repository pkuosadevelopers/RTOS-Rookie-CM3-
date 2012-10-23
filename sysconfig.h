/******************************************************************************
Filename    : sysconfig.h
Author      : pry 
Date        : 22/07/2012
Description : The Configuration Of The Operating System Is Logged Here.
              These "#define"s Are Not Included In The "defines.h".
******************************************************************************/

/*Version Information**********************************************************
1.Created By  pry                                        On 22/07/2012 Ver1.0.0
 Created The File To Standardize The Operating System.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _sysconfig_h_
#define _sysconfig_h_

/*Compiler Configuration******************************************************/
//Keywords
#define INLINE                      __inline
//End Keywords
/*End Compiler Configuration**************************************************/

/*System Module Configuration*************************************************/
//Configuration Choice
#define TRUE                        0x01
#define FALSE                       0x00
//End Configuration Choice

//Config Reporter
#define REPORT_CONFIG               TRUE                                       //[Independent]
//End Config Reporter

//Basic Types
#define DEFINE_BASIC_TYPES		    FALSE									   //[Independent]
//End Basic Types

//Kernel
//End Kernel

//Memory
#define ENABLE_MEMORY_MANAGEMENT 	TRUE								       //[Independent]
#define ENABLE_SHARED_MEMORY	    TRUE									   //[Dependency:ENABLE_MEMORY_MANAGEMENT]
//End Memory

//Miscellaneous
#define ENABLE_LIST                 TRUE								       //[Independent]
#define ENABLE_SYS_LIB				TRUE								       //[Independent]
//End Miscellaneous

//Resource											   
#define ENABLE_RESOURCE             TRUE			   						   //[Independent]
//End Resource

//Signal
#define ENABLE_SIGNAL			    TRUE					                   //[Independent]
//End Signal

//Filesystem
#define ENABLE_FILESYSTEM           FALSE									   //[Dependency:ENABLE_MEMORY_MANAGEMENT,ENABLE_RESOURCE]
//End Filesystem

//GUI
#define ENABLE_GUI                  TRUE								       //[Dependency:ENABLE_LIST,ENABLE_SYS_LIB]
//End GUI

//Shell
#define ENABLE_SHELL			    TRUE									   //[Dependency:ENABLE_]
//End Shell
/*End System Module Configuration*********************************************/

/*Kernel Configuration********************************************************/
//Tasks/Processes
//#define NULL                 0							                   //The General "Null".//Already Defined In Other Headers.
#define MAX_NUMBER_OF_TASKS   4                                                //The Maximum Number Of Application(Process) Running at the same time.  	                            [1-250]
#define MAX_STACK_DEP         100                                              //The Maximum Stack Depth,In Bytes,Must Be Bigger Than 18.							 [18-250]
//End Tasks/Processes

//Systick
#define MINIMAL_TIMESLICE_TICK 100                                             //The Timeslice Unit For Scheduler.
//End Systick			                            
/*End Kernel Configuration****************************************************/

/*Memory Management Configuration*********************************************/
#define	SYS_MEM_AVAIL_START   0x20000000					                   //The Allocateable Memory For Kernel Start Address.					                            [X]
#define SYS_MEM_AVAIL_END	  0x20000FFF					                   //The Allocateable Memory For Kernel End Address.													[X]
#define APP_AVAIL_MEM_START	  0x20001000				                       //The Allocateable Memory For Applications Start Address.												[X]
#define APP_AVAIL_MEM_END     0x20004FFF					                   //The Allocateable Memory For Applications End Address.											[X]
#define NUMBER_OF_MEM_PAGES   256						                       //The Number Of Memory Pages.																        [1-0xFFFF_FFFF]
#define SIZE_OF_EACH_PAGE     ((APP_AVAIL_MEM_END-APP_AVAIL_MEM_START)/NUMBER_OF_MEM_PAGES)	//The Size Of Each page. Will be Automatically Calculated.          [Auto]										  
#define MAX_SHARED_MEM_BLOCKS 10							                   //The Maximum Number Of Shared Memory Blocks.							                            [1-255]
#define MAX_PROCESS_SHARE_MEM 10                                               //The Maximum Number Of Processes Sharing The Same Memory Area.								    [1-255]
/*End Memory Manegement Configuration*****************************************/

/*Miscellaneous Configuration*************************************************/
#define DELAYTRIM       			110					    		           //The "Sys_Delay" Trim Value.
#define MAX_STR_LEN     			1024                    	               //The Maximum String Length That The "Sys_Strxxx"functions Can Deal With.						  [1-0xFFFF_FFFF]
/*End Miscellaneous Configuration*********************************************/

/*Resource Configuration******************************************************/
#define MAX_NUMBER_OF_RES    10				                                   //The Maximum Number Of Resources In The System.							   [1-255]
#define MAX_PID_OCCUPY_RES   10				                                   //The Maximum Number Of Processes Occupying The Same Resource.				   [1-255]
/*End Resource Configuration**************************************************/

/*Filesystem Configuration****************************************************/

/*End Filesystem Configuration************************************************/

/*GUI Configuration***********************************************************/
//Begin:LCD Size Definitions.
#define LCD_LENGTH      320
#define LCD_WIDTH       240
//End:LCD Size Definitions.

//Begin:Define The Screen Rotation Mode According To PDF.
#define ID_AM           110	  
//End:Define The Screen Rotation Mode According To PDF.

//Screen Controllers
#define CONTROLLER_ID  0x8989
//End Screen Controllers

//Dialog Box
#define MAX_NUMBER_OF_DIALOG              0x04							       //The Maximum Number Of Dialog Buttons In The System.			  [0-254]
#define MAX_CONTROL_IN_DIALOG             0x05						           //The Maximum Number Of Controls In A Dialog.					  [0-254]
#define MAX_LEN_OF_DIALOG_TITLE           20						           //The Longest Dialog Title.										  [0-MAX_STR_LEN]

#define BACKCOLOR               		  (23<<11)|(46<<5)|(23)	 			   //The Background Color	(192R,192G,192B)
#define DARKER_BACKCOLOR        		  (15<<11)|(31<<5)|(15)				   //The Darker Background.(128R,128G,128B)
#define TITLE_LEFTSIDE_COLOR    		  (0<<11)|(0<<5)|(16)				   //The Color Of The Bar -The Left Side.
#define TITLE_RIGHTSIDE_COLOR			  (3<<11)|(33<<5)|(26)	 			   //The Color Of The Bar	-The Right Side.
#define DIALOG_TITLE_WIDTH     			  18						 		   //The Width Of The Title(Bar).
#define DIALOG_TITLE_FONT      			  _8X16_					 		   //The Font Of The Dialog Box Title.
#define DIALOG_CONTROL_FONT     		  _8X16_							   //The Font Of Dialog Box.
#define ICON_SIZE               		  10								   //The Size Of The Icon.
//End Dialog Box
/*End GUI Configuration*******************************************************/

/*Shell Configuration*********************************************************/
#define	COMMAND_SPACE                     500
#define SHELL_OUTPUT_BUF_LEN              500
/*End Shell Configuration*****************************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
