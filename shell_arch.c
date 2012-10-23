/******************************************************************************
Filename    : shell_arch.c
Author      : pry 
Date        : 24/07/2012
Description : The Shell Functions Related To The Hardware.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 24/07/2012 Ver1.0.0
  Created The File Only.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "shell_arch.h"
/*End Includes****************************************************************/

/*Begin Function:_Sys_USART_Configuration**************************************
Description    : Configure Open407V's USART2/3 For Serial Port Communication. 
Input          : Void
Output         : Void
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_USART_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); 

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* USART configuration */
  USART_Init(USART1,&USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(USART1,ENABLE); 
}
#endif
/*Finish Function:_Sys_USART_Configuration************************************/

/*Begin Function:_Sys_SH_Getch*************************************************
Description : The Function For Getting A Character From The Standard Input.
              For System Use.
Input       : Void.
Output      : u8 -The Character That You Got. 
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
u8 _Sys_SH_Getch(void)
{	 
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);	       //Dead Loop To Wait For A Serial Reception.(RXNE:Receive Register Not Empty)
	return(USART_DATA_MASK&USART_ReceiveData(USART1));
}
#endif
/*Finish Function:_Sys_SH_Getch***********************************************/

/*Begin Function:_Sys_SH_Putch*************************************************
Description : The Function For Printing A Character To The Standard Output.
              For System Use.
Input       : u8 -The Character That You Want To Output.
Output      : Void. 
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_Putch(u8 Char)
{  
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);	       //Wait For The Transmission To Complete.
	USART_SendData(USART1,Char);
}
#endif
/*Finish Function:_Sys_SH_Putch***********************************************/
/*The Functions Above Are Concerned With The Hardware*************************/

/*The Functions Below Will Be Used By The Higher Layers***********************/
/*Begin Function:_Sys_StdIO_Init***********************************************
Description : Initialize The Standard IO(The Standard Input/Output,Such As USART).
              For System Use.
Input       : Void.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_StdIO_Init(void)
{
	_Sys_USART_Configuration();
}
#endif
/*Finish Function:_Sys_StdIO_Init*********************************************/
u8 Buffer_Out[SHELL_OUTPUT_BUF_LEN];
/*Begin Function:_Sys_SH_Output************************************************
Description : The Output Function For The Shell.It Works Just Like The "printf".
Input       : s8* String -The String Input,including The Format Control characters.
			  s32* Args_Int -The Integer Argument Array.Note That They Must All Be Integers.
			  double* Args_Double -The Double Argument Array.Note That They Must All Be Doubles.
			  s8** Args_String -The String Argument Array.Note That They Must All Be Strings.
			  Why It Doesn't Have Changeable Argument List is That The ARMCC Doesn't Support It.
Output      : Void.
******************************************************************************/
#if(ENABLE_SHELL==TRUE)
void _Sys_SH_Output(s8* String,s32* Args_Int,double* Args_Double,s8** Args_String)
{
	
	u8* Buffer_Out_Ptr=Buffer_Out;
	//Sys_Memset(Buffer_Out,'\0',SHELL_OUTPUT_BUF_LEN);
	Sys_Sprintf((s8*)Buffer_Out,String,Args_Int,Args_Double,Args_String);
	
	while(((*Buffer_Out_Ptr)!='\0')&&(Buffer_Out_Ptr-Buffer_Out<MAX_STR_LEN))
		_Sys_SH_Putch(*Buffer_Out_Ptr++);
}
#endif
/*Finish Function:_Sys_SH_Output**********************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
