/******************************************************************************
Filename:   GUI.h 
Author:     pry
Date:       07/04/2012
Description:The GUI Function List for the Operating System.
            The Functions Listed Used The Functions In The "GUI_arch.c",which are more basic.
******************************************************************************/

/*Version Information**********************************************************
1.Created By   pry                                    On 07/04/2012    Ver1.0.0
  Created The File And Added Some Functions To It.
******************************************************************************/

/*Preprocessor Control********************************************************/
#ifndef _GUI_h_
#define _GUI_h_																   //We Can't Use '.' In The Preprocessor Control Strings.

/*Includes********************************************************************/
#include "defines.h"		            									   //The Sequence Of Include Is Fixed.
#include "typedefs.h"
#include "externs.h"		            									
#include "filesystems_in_use.h"		 
#include "math.h" 															   //Some Functions Are Used,Such As "sin" And "cos".
/*End Includes****************************************************************/

/*Private C Function Prototypes***********************************************/	
static void _Sys_GUI_Draw_Dialog_Title(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,void* Title,void* Icon);	 											   //The Function For Drawing The Dialog Title.
static void _Sys_GUI_Draw_Dialog_Box_Without_Title(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width);				 											   //The Function For Drawing The Dialog Box Withgout Title.

static u8 _Sys_GUI_Draw_Tab_Controls(struct Dialog_Control_Head* Control);										 										       //The Function	For Drawing Controls In A Tab.
static u8 _Sys_GUI_Draw_Dialog_Controls(u8 Dialog_ID);															 											   //The Function For Drawing Controls In A Dialog Box.

static u8 _Sys_GUI_Draw_Any_Control(u16 Coord_X,u16 Coord_Y,struct Dialog_Control_Head* Control);				 											   //The Function For Drawing A Control In A Dialog Box.
static u8 _Sys_GUI_Draw_Any_Control_For_Tabs(u16 Coord_X,u16 Coord_Y,struct Dialog_Control_Head* Control);		 											   //The Function For Drawing A Control In A Tab.

static u8 _Sys_GUI_Search_Clicked_Dialog_Condition(struct List_Head* Current_Node,void* Coord_Compound);		 											   //The Search End Condition For The Clicked Dialog.

static u8 _Sys_GUI_NOP(void);																					 											   //The "No Action" Function Used In The "default :".
/*End Private C Function Prototypes*******************************************/

#endif
/*End Preprocessor Control****************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/ 
