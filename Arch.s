;/*****************************************************************************
;Filename    : arch.s
;Author      : pry
;Date        : 10/04/2012
;Description : The assembly part of the Rookie OS,Which Is Related 
;              Directly To Hardware.
;*****************************************************************************/

;/*Version Information*********************************************************
;1.Created By pry                                        On 10/04/2012 Ver1.0.0 
;  There may still be defects.
;2.Modified By pry                                       On 13/04/2012 Ver1.0.1
;  Modified the "startup_stm32f10x_md.s" for thread mode initialization.
;  Still Encounter Hard Faults.						   
;3.Modified By pry                                       On 14/04/2012 Ver1.1.0
;  Still some faults(The Systick INT is not reentrantable.)
;4.Modified By pry                                       On 16/04/2012 Ver1.2.0
;  The Faults above is solved by correctly setting The stack.
;5.Modified	By pry                                       On 26/06/2012 Ver1.3.0
;  Eliminated The Fault Of Unable To Set The Segment In Rom.
;6.Modified By pry                                       On 23/07/2012 Ver2.0.0
;  Aligned The "***" Lines,And Fixed A Few Spelling Mistakes.
;7.Modified By pry                                       On 04/10/2012 Ver3.0.0
;  Changed The Sceduler To Support The Priority Schedule Mode.The Kernel Is Updated
;  To Be More Stable.
;*****************************************************************************/

;/*The ARM Cortex-M3/4 Structure***********************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:general purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main	  Stack pointer
;R14:LR					 	  Link Register(used for returning from a subfunction)
;R15:PC						  Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The Above 3 registers are saved into the stack in combination(xPSR).
;
;The ARM Cortex-M4 Also Include A Single-Accuracy FPU.
;*****************************************************************************/
			
;/*Begin FileHead*************************************************************/			
				AREA            ARCH,CODE,READONLY,ALIGN=3                     ;The Align Is "(2^3)/8=1(Byte)." In Fact It Does Not Take Effect.
				
				THUMB
    			REQUIRE8
    		    PRESERVE8
;/*End FileHead***************************************************************/

;/*Begin Exports**************************************************************/
				EXPORT			DISABLE_ALL_INTS                               ;The Assembly Function Used In C.
				EXPORT			ENABLE_ALL_INTS                                ;The Assembly Function Used In C.
			   	EXPORT          SYS_SAVE_SP                                    ;The Assembly Function Used In C.
                EXPORT          SYS_LOAD_SP                                    ;The Assembly Function Used In C.
				EXPORT  		DISABLE_SYSTICK		                           ;The Assembly Function Used In C.
				EXPORT 			ENABLE_SYSTICK		                           ;The Assembly Function Used In C.
;/*End Exports****************************************************************/

;/*Begin Imports**************************************************************/
				IMPORT          Next_PID			                           ;The PID Of the Next Thread.
				IMPORT          Current_PID                                    ;The PID Of the Current Thread.
				IMPORT          PCB_SP				                           ;The SP Of PCB.
;/*End Imports****************************************************************/

;/*Begin Function:SYS_SAVE_SP**************************************************
;Description :The Function For Setting The SP Of Each Task.
;Input       :Current_PID -The Current PID.
;Output      :Void.	
;Register Usage:
;R0-The Address Of The SP Table.									  
;*****************************************************************************/
SYS_SAVE_SP      
                LDR       R0,=PCB_SP
                LDR       R1,=Current_PID
                LDR       R1,[R1]
                STR       SP,[R0,R1,LSL #2]                                   ;Save The SP.
                
                BX        LR                                                  ;Jump Back.
;/*Finish Function:SYS_SAVE_SP************************************************/

;/*Begin Function:SYS_LOAD_SP**************************************************
;Description :The Function For Setting The SP Of Each Task.
;Input       :Current_PID -The Current PID.
;Output      :Void.	
;Register Usage:
;R0-The Address Of The SP Table.									  
;*****************************************************************************/
SYS_LOAD_SP      
                LDR       R0,=PCB_SP
                LDR       R1,=Current_PID
                LDR       R1,[R1]
                LDR       SP,[R0,R1,LSL #2]                                   ;Set The SP Correctly.
                
                BX        LR                                                  ;Jump Back.
;/*Finish Function:SYS_LOAD_SP************************************************/

;/*Begin Function:DISABLE_ALL_INTS*********************************************
;Description :The Function For Disabling all interrupts.
;Input       :Void.
;Output      :Void.	
;Register Usage:None.								  
;*****************************************************************************/	
DISABLE_ALL_INTS
				CPSID     I		                                               ;Disable all interrupts (I is primask,F is Faultmask.)

				BX        LR						                           ;Return.
;/*Finish Function:DISABLE_ALL_INTS*******************************************/

;/*Begin Function:ENABLE_ALL_INTS**********************************************
;Description :The Function For Enabling all interrupts.
;Input       :Void.
;Output      :Void.	
;Register Usage:None.								  
;*****************************************************************************/
ENABLE_ALL_INTS
	            CPSIE    I				           	                           ;Enable all interrupts.

				BX       LR						                               ;Return.
;/*Finish Function:DISABLE_ALL_INTS*******************************************/

;/*Begin Function:DISABLE_SYSTICK**********************************************
;Description :The Function For Disabling Systick timer.
;Input       :Void.
;Output      :Void.	
;Register Usage:R0-The ST_CTRL_STAT Register Address,
;               R1-Its Value.
;               Note That The "ENABLE_SYSTICK"	Must Be Used Together
;               With It,And The ST_CURRENT Register Can't Change 
;				During The Procedures Between Them.							  
;*****************************************************************************/	
DISABLE_SYSTICK
				LDR       R0,=0xE000E010              ;The ST_CTRL_STAT Register.		                     
				LDR       R1,[R0]
				AND       R1,#0xFFFFFFFE
				STR       R1,[R0]				      ;Clear The "Enable" Bit.

				BX        LR						  ;Return.
;/*Finish Function:DISABLE_SYSTICK********************************************/

;/*Begin Function:ENABLE_SYSTICK***********************************************
;Description :The Function For Enabling Systick timer.
;Input       :Void.
;Output      :Void.	
;Register Usage:R0-The RELOAD_VAL Address Register.
;				R1-The Other Address Register.
;               R2,3-The Value Register.								  
;*****************************************************************************/
ENABLE_SYSTICK
	            LDR       R1,=0xE000E010                                       ;The ST_CTRL_STAT Register.		                      
				LDR       R2,[R1]				                               ;Get The Configuration.
				ORR       R2,#0x00000001
				STR       R2,[R1]				                               ;Set The "Enable" Bit.
                     
				BX        LR					                               ;Return.
;/*Finish Function:ENABLE_SYSTICK*********************************************/

				END
;/*End Of File****************************************************************/

;/*Copyright (C) 2011-2013 pry. All rights reserved.**************************/
