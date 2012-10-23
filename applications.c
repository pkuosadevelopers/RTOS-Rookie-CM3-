/******************************************************************************
Filename   : applications.c
Author     : pry
Date       : 25/04/2012
Description: The Tasks&applications&Test Functions Is here.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 25/04/2012 Ver1.0.0
  Moved the Test Functions to Here.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "applications.h"
/*End Includes****************************************************************/

/*Begin Function:Task1*********************************************************
Description : The Task Process 1.
Input       : Void.
Output      : Void
******************************************************************************/
void Task1(void)
{  
	Sys_GUI_LCD_Init();
    Sys_Start_Process((void*)Task3,0x20003900,4,(u8*)"Driver_2");
	while(1)
	{
		Sys_GUI_LCD_Clear_Screen(GREEN);
	}
}
/*Finish Function:Task1*******************************************************/

/*Begin Function:Task2*********************************************************
Description : Test Process 2.
Input       : Void.
Output      : Void
******************************************************************************/
void Task2(void)
{ 
    while(1)
        Sys_GUI_LCD_Clear_Screen(RED);
}
/*Finish Function:Task2*******************************************************/

/*Begin Function:Task3*********************************************************
Description : Test Process 3.
Input       : Void.
Output      : Void
******************************************************************************/
void Task3(void)
{ 
    while(1)
        Sys_GUI_LCD_Clear_Screen(BLUE);
}
/*Finish Function:Task3*******************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
