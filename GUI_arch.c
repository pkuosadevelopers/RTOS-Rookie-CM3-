/******************************************************************************
Filename    : GUI_arch.c
Author      : www.armjishu.com,pry 
Date        : 03/16/2010
Description : TFT LCD Driver.The Functions Related To Hardware For The GUI Interface.
              Some Of Them Are Atomic Operations.They Will Only Disable All Interrupts;
			  They Will Not Disable The Systick Timer.
******************************************************************************/

/****************************Version Information*******************************
1.Created By  www.armjishu.com                           On 03/16/2010 Ver1.0.0
2.Modified By pry                                        On 04/04/2012 Ver1.0.1
  Cleared The File.
3.Modified By pry                                        On 29/06/2012 Ver2.0.0
  Installed The Module Into The OS. Made Great Changes To It.
4.Modified By pry					                     On 10/07/2012 Ver2.1.0
  Changed Some Function To Inline Ones.
5.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines,And Made The Module Configurable.
******************************************************************************/

/*Hardware Connection Demonstration********************************************
STM32         Sys_GUI_LCD 
PC0~7  <----> DB0~7  
PB8~15	<----> DB8~15
PC11   <----> nRD
PC9    <----> RS        
PC10   <----> nWR        
PC8    <----> nCS        
Reset  <----> nReset     
PC12    <----> BK_LED    
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "GUI_arch.h"
/*End Includes****************************************************************/

/*Begin Function:_Sys_GUI_LCD_Pin_Input****************************************
Description    : Configure MCU LCD_DB Pins As InPut
Input          : Void
Output         : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void INLINE _Sys_GUI_LCD_Pin_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
#endif
/*Finish Function:_Sys_GUI_LCD_Pin_Input**************************************/

/*Begin Function:_Sys_GUI_LCD_Pin_Output***************************************
Description    : Configure MCU LCD_DB pins AS OutPut
Input          : Void
Output         : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void INLINE _Sys_GUI_LCD_Pin_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
#endif
/*Finish Function:_Sys_GUI_LCD_Pin_Output*************************************/

/*Begin Function:_Sys_GUI_LCD_Pin_Init*****************************************
Description    : Configures LCD Pins Push-Pull(Totem) mode.
Input          : Void
Output         : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void INLINE _Sys_GUI_LCD_Pin_Init(void)
{						
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LCD_CLK_RS | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC|
	                     LCD_CLK_WR | LCD_CLK_RD | LCD_CLK_CS | LCD_CLK_BL, ENABLE);
	
	// DB15--0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//LCD_Pin_BL
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_BL;
//	GPIO_Init(LCD_PORT_BL, &GPIO_InitStructure);	   //The Pin Of The Processor Is Probably Damaged Due To Overclocking.
	//LCD_Pin_WR
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_WR;
	GPIO_Init(LCD_PORT_WR, &GPIO_InitStructure);
	
	//LCD_Pin_CS
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_CS;
	GPIO_Init(LCD_PORT_CS, &GPIO_InitStructure);
	
	//LCD_Pin_RS
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_RS;
	GPIO_Init(LCD_PORT_RS, &GPIO_InitStructure);
	
	//LCD_Pin_RD
	GPIO_InitStructure.GPIO_Pin = LCD_Pin_RD;
	GPIO_Init(LCD_PORT_RD, &GPIO_InitStructure);
	
	_Sys_GUI_Pin_Set_Cs();
}
#endif
/*Finish Function:_Sys_GUI_LCD_Pin_Init***************************************/

/*Begin Function:_Sys_GUI_LCD_Write_Data***************************************
Description    : Writes Data Directly To The LCD.
Input          : u16 Data -The Data To Write.
Output         : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void INLINE _Sys_GUI_LCD_Write_Data(u16 LCD_DATA)  
{
	GPIO_Write(GPIOC, ((GPIOC->ODR&0XFF00)|(LCD_DATA&0x00FF)));
	GPIO_Write(GPIOB, ((GPIOB->ODR&0X00FF)|(LCD_DATA&0xFF00)));		  		   //Data Output.The Function Is No Longer Used In The CM4-FSMC Architecture.
}
#endif
/*Finish Function:_Sys_GUI_LCD_Write_Data*************************************/

/*Begin Function:_Sys_GUI_LCD_Read_Data****************************************
Description    : Reads Data Directly From The LCD.
Input          : u16 Data -The Data To Write.
Output         : Void
******************************************************************************/	  
#if(ENABLE_GUI==TRUE) 
u16 INLINE _Sys_GUI_LCD_Read_Data(void)
{
	return (GPIO_ReadInputData(GPIOB)&0XFF00)|(GPIO_ReadInputData(GPIOC)&0X00FF);//Data Input.The Function Is No Longer Used In The CM4-FSMC Architecture.
}
#endif
/*Finish Function:_Sys_GUI_LCD_Read_Data**************************************/

/*Begin Function:_Sys_GUI_LCD_Write_Register***********************************
Description    : Writes to the selected LCD register.
Input          : u16 LCD_Reg: address of the selected register.
                 u16 LCD_RegValue: value to write to the selected register.
Output         : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void INLINE _Sys_GUI_LCD_Write_Register(u16 LCD_Reg,u16 LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	_Sys_GUI_Pin_Clear_Cs();
	_Sys_GUI_Pin_Clear_Rs();
	_Sys_GUI_Pin_Clear_Wr();
	_Sys_GUI_LCD_Write_Data(LCD_Reg);
	_Sys_GUI_Pin_Set_Wr();
	/* Write 16-bit Reg */
	_Sys_GUI_Pin_Set_Rs();
	_Sys_GUI_Pin_Clear_Wr();
	_Sys_GUI_LCD_Write_Data(LCD_RegValue);
	_Sys_GUI_Pin_Set_Wr();
	_Sys_GUI_Pin_Set_Cs();
}
#endif
/*Finish Function:_Sys_GUI_LCD_Write_Register*********************************/

/*Begin Function:_Sys_GUI_LCD_Read_Register************************************
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : LCD Register Value.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
u16 INLINE _Sys_GUI_LCD_Read_Register(u8 LCD_Reg)
{
	u16 Data;
  
	/* Write 16-bit Index (then Read Reg) */
	// _Sys_GUI_LCD_Light_On()
	_Sys_GUI_Pin_Clear_Cs();
	_Sys_GUI_Pin_Clear_Rs();
	_Sys_GUI_Pin_Clear_Wr();
	_Sys_GUI_LCD_Write_Data(LCD_Reg);
	_Sys_GUI_Pin_Set_Wr();
	
	/* Read 16-bit Reg */
	_Sys_GUI_Pin_Set_Rs();
	_Sys_GUI_Pin_Clear_Rd();
	_Sys_GUI_Pin_Set_Rd();
	Data=_Sys_GUI_LCD_Read_Data(); 
	_Sys_GUI_Pin_Set_Cs();
	
	return Data;
}
#endif	
/*Finish Function:_Sys_GUI_LCD_Read_Register**********************************/

/*Begin Function:_Sys_GUI_LCD_Set_Cursor***************************************
Description : The Function For Setting The Screen Cursor.
Input       : u16 x-The Line Address,u16 y-The Row Address 
Output      : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void INLINE _Sys_GUI_LCD_Set_Cursor(u16 x,u16 y)
{
#if CONTROLLER_ID==0x8989
	_Sys_GUI_LCD_Write_Register(0x004e,y);                     				   //Line
  //_Sys_GUI_LCD_Write_Register(0x004f,0x13f-x);               				   //Row
	_Sys_GUI_LCD_Write_Register(0x004f,x);                     				   //Row

#elif CONTROLLER_ID==0x9320
	_Sys_GUI_LCD_Write_Register(0x0020,y);                     				   //Line
	_Sys_GUI_LCD_Write_Register(0x0021,0x13f-x);               				   //Row

#elif CONTROLLER_ID==0x9919
	_Sys_GUI_LCD_Write_Register(0x004e,x);                     				   //Line
  	_Sys_GUI_LCD_Write_Register(0x004f,y);                     				   //Row	

#elif CONTROLLER_ID==0x9325
	_Sys_GUI_LCD_Write_Register(0x0020,x);                      			   //Line
  	_Sys_GUI_LCD_Write_Register(0x0021,y);                      			   //Row
			
#else
  	_Sys_GUI_LCD_Write_Register(0x0020,y);                      			   //Line
  	_Sys_GUI_LCD_Write_Register(0x0021,0x13f-x);                			   //Row
#endif
}
#endif
/*Finish Function:_Sys_GUI_LCD_Set_Cursor*************************************/

/*Begin Function:Sys_GUI_LCD_Set_Window****************************************
Description : Set The Window Area.
Input       : u16 StartX-The Line Start Coordination 
              u16 StartY-The Row  Start Coordination
              u16 EndX-  The Line End   Coordination
              u16 EndY-  The Row End Coordination
Output		: Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void INLINE Sys_GUI_LCD_Set_Window(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
	DISABLE_ALL_INTS();														   //Atomic Operation.

	_Sys_GUI_LCD_Set_Cursor(StartX,StartY);
	_Sys_GUI_LCD_Write_Register(0x0050, StartX);
	_Sys_GUI_LCD_Write_Register(0x0052, StartY);
	_Sys_GUI_LCD_Write_Register(0x0051, EndX);
	_Sys_GUI_LCD_Write_Register(0x0053, EndY);

	ENABLE_ALL_INTS();
}
#endif
/*Finish Function:Sys_GUI_LCD_Set_Window**************************************/
/*The Inline Functions Above Will Only Be Used By The Functions Below*********/

/*The Functions Below Will Be Used By Higher Layers***************************/ 
/*Begin Function:Sys_GUI_LCD_Init**********************************************
Description  : Initiate The Sys_GUI_LCD LCD Controller.
Input        : Void.
Output       : u16 -The Screen Controller's ID.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
u16 Sys_GUI_LCD_Init(void)
{
	u16 Screen_Controller_ID;

	DISABLE_ALL_INTS();											          	   //Atomic Operation.

	_Sys_GUI_LCD_Pin_Init();

	Sys_Delay(50);                                                        	   //Delay For 50ms. 
	_Sys_GUI_LCD_Write_Register(0x0000,0x0001);  
	Sys_Delay(50);                                                             //Delay For 50 ms.
	
	_Sys_GUI_LCD_Pin_Input();
	Sys_Delay(10);                                                        	   //Delay 50ms For The Mode Change.
	Screen_Controller_ID=_Sys_GUI_LCD_Read_Register(0x0000);		      	   //Read Out The Screen Controller Device ID
	_Sys_GUI_LCD_Pin_Output();
	Sys_Delay(10);                                                        	   //Delay 50ms For The Mode Change.

#if (CONTROLLER_ID==0x9325||CONTROLLER_ID==0x9328)
	_Sys_GUI_LCD_Write_Register(0x00e3,0x3008);
	_Sys_GUI_LCD_Write_Register(0x00e7,0x0012);
	_Sys_GUI_LCD_Write_Register(0x00ef,0x1231);                                //Set the internal vcore voltage
  //_Sys_GUI_LCD_Write_Register(0x00e7,0x0010);      
	_Sys_GUI_LCD_Write_Register(0x0000,0x0001);  			                   //Start Internal Oscillator
	_Sys_GUI_LCD_Write_Register(0x0001,0x0100);     
	_Sys_GUI_LCD_Write_Register(0x0002,0x0700); 				          	   //Power on sequence                     
	_Sys_GUI_LCD_Write_Register(0x0003,(1<<12)|(1<<5)|(1<<4) ); 	           //65K 
	_Sys_GUI_LCD_Write_Register(0x0004,0x0000);                                   
	_Sys_GUI_LCD_Write_Register(0x0008,0x0207);	           
	_Sys_GUI_LCD_Write_Register(0x0009,0x0000);         
	_Sys_GUI_LCD_Write_Register(0x000a,0x0000); 				          	   //Display Setting         
	_Sys_GUI_LCD_Write_Register(0x000c,0x0001);				                   //Display Setting          
	_Sys_GUI_LCD_Write_Register(0x000d,0x0000); 				               //0F3C          
	_Sys_GUI_LCD_Write_Register(0x000f,0x0000);
	                                                                           //Power On sequence
	_Sys_GUI_LCD_Write_Register(0x0010,0x0000);   
	_Sys_GUI_LCD_Write_Register(0x0011,0x0007);
	_Sys_GUI_LCD_Write_Register(0x0012,0x0000);                                                                 
	_Sys_GUI_LCD_Write_Register(0x0013,0x0000); 
	                
	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x0010,0x1590);   
	_Sys_GUI_LCD_Write_Register(0x0011,0x0227);

	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x0012,0x009c);
	                  
	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x0013,0x1900);   
	_Sys_GUI_LCD_Write_Register(0x0029,0x0023);
	_Sys_GUI_LCD_Write_Register(0x002b,0x000e);

	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x0020,0x0000);                                                            
	_Sys_GUI_LCD_Write_Register(0x0021,0x0000);           
     
	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x0030,0x0007); 
	_Sys_GUI_LCD_Write_Register(0x0031,0x0707);   
	_Sys_GUI_LCD_Write_Register(0x0032,0x0006);
	_Sys_GUI_LCD_Write_Register(0x0035,0x0704);
	_Sys_GUI_LCD_Write_Register(0x0036,0x1f04); 
	_Sys_GUI_LCD_Write_Register(0x0037,0x0004);
	_Sys_GUI_LCD_Write_Register(0x0038,0x0000);        
	_Sys_GUI_LCD_Write_Register(0x0039,0x0706);     
	_Sys_GUI_LCD_Write_Register(0x003c,0x0701);
	_Sys_GUI_LCD_Write_Register(0x003d,0x000f);

	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x0050,0x0000);        
	_Sys_GUI_LCD_Write_Register(0x0051,0x00ef);   
	_Sys_GUI_LCD_Write_Register(0x0052,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0053,0x013f);
	_Sys_GUI_LCD_Write_Register(0x0060,0xa700);        
	_Sys_GUI_LCD_Write_Register(0x0061,0x0001); 
	_Sys_GUI_LCD_Write_Register(0x006a,0x0000);
	_Sys_GUI_LCD_Write_Register(0x0080,0x0000);
	_Sys_GUI_LCD_Write_Register(0x0081,0x0000);
	_Sys_GUI_LCD_Write_Register(0x0082,0x0000);
	_Sys_GUI_LCD_Write_Register(0x0083,0x0000);
	_Sys_GUI_LCD_Write_Register(0x0084,0x0000);
	_Sys_GUI_LCD_Write_Register(0x0085,0x0000);	
	_Sys_GUI_LCD_Write_Register(0x0090,0x0010);     
	_Sys_GUI_LCD_Write_Register(0x0092,0x0600); 
		 
	#if CONTROLLER_ID==0x9328 
		_Sys_GUI_LCD_Write_Register(0x0093,0x0003);
		_Sys_GUI_LCD_Write_Register(0x0095,0x0110);
		_Sys_GUI_LCD_Write_Register(0x0097,0x0000);        
		_Sys_GUI_LCD_Write_Register(0x0098,0x0000);  
	#endif   

	_Sys_GUI_LCD_Write_Register(0x0007,0x0133);					               //Display on sequence 
	_Sys_GUI_LCD_Write_Register(0x0020,0x0000);                                                            
	_Sys_GUI_LCD_Write_Register(0x0021,0x0000);

#elif (CONTROLLER_ID==0x9320||CONTROLLER_ID==0x9300)
	_Sys_GUI_LCD_Write_Register(0x00,0x0001);
	_Sys_GUI_LCD_Write_Register(0x01,0x0100);	                               //Driver Output Control.
	_Sys_GUI_LCD_Write_Register(0x02,0x0700);	                               //LCD Driver Waveform Control.
  //_Sys_GUI_LCD_Write_Register(0x03,0x1030);	                               //Entry Mode Set.
  //_Sys_GUI_LCD_Write_Register(0x03,0x1018);	                               //Entry Mode Set.
	_Sys_GUI_LCD_Write_Register(0x03,0x1028);	                               //Entry Mode Set.
	
	_Sys_GUI_LCD_Write_Register(0x04,0x0000);	                               //Scalling Control.
	_Sys_GUI_LCD_Write_Register(0x08,0x0202);	                               //Display Control 2.(0x0207)
	_Sys_GUI_LCD_Write_Register(0x09,0x0000);	                               //Display Control 3.(0x0000)
	_Sys_GUI_LCD_Write_Register(0x0a,0x0000);	                               //Frame Cycle Control.(0x0000)
	_Sys_GUI_LCD_Write_Register(0x0c,(1<<0));	                               //Extern Display Interface Contral 1.(0x0000)
	_Sys_GUI_LCD_Write_Register(0x0d,0x0000);	                               //Frame Maker Position.
	_Sys_GUI_LCD_Write_Register(0x0f,0x0000);	                               //Extern Display Interface Contral 2.
	
	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x07,0x0101);	                               //Display Contral.

	Sys_Delay(3);
	
	_Sys_GUI_LCD_Write_Register(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));     //Power Control 1.(0x16b0)
	_Sys_GUI_LCD_Write_Register(0x11,0x0007);							       //Power Control 2.(0x0001)
	_Sys_GUI_LCD_Write_Register(0x12,(1<<8)|(1<<4)|(0<<0));				       //Power Control 3.(0x0138)
	_Sys_GUI_LCD_Write_Register(0x13,0x0b00);							       //Power Control 4.
	_Sys_GUI_LCD_Write_Register(0x29,0x0000);							       //Power Control 7.
	
	_Sys_GUI_LCD_Write_Register(0x2b,(1<<14)|(1<<4));
	
	_Sys_GUI_LCD_Write_Register(0x50,0);		                               //Set X Start.
	_Sys_GUI_LCD_Write_Register(0x51,239);	                                   //Set X End.
	_Sys_GUI_LCD_Write_Register(0x52,0);		                               //Set Y Start.
	_Sys_GUI_LCD_Write_Register(0x53,319);	                                   //Set Y End.
	
	_Sys_GUI_LCD_Write_Register(0x60,0x2700);	                               //Driver Output Control.
	_Sys_GUI_LCD_Write_Register(0x61,0x0001);	                               //Driver Output Control.
	_Sys_GUI_LCD_Write_Register(0x6a,0x0000);	                               //Vertical Srcoll Control.
	
	_Sys_GUI_LCD_Write_Register(0x80,0x0000);	                               //Display Position:Partial Display 1.
	_Sys_GUI_LCD_Write_Register(0x81,0x0000);	                               //RAM Address Start:Partial Display 1.
	_Sys_GUI_LCD_Write_Register(0x82,0x0000);	                               //RAM Address End:Partial Display 1.
	_Sys_GUI_LCD_Write_Register(0x83,0x0000);	                               //Displsy Position:Partial Display 2.
	_Sys_GUI_LCD_Write_Register(0x84,0x0000);	                               //RAM Address Start:Partial Display 2.
	_Sys_GUI_LCD_Write_Register(0x85,0x0000);	                               //RAM Address End:Partial Display 2.
	
	_Sys_GUI_LCD_Write_Register(0x90,(0<<7)|(16<<0));	                       //Frame Cycle Control.(0x0013)
	_Sys_GUI_LCD_Write_Register(0x92,0x0000);	                               //Panel Interface Control 2.(0x0000)
	_Sys_GUI_LCD_Write_Register(0x93,0x0001);	                               //Panel Interface Control 3.
	_Sys_GUI_LCD_Write_Register(0x95,0x0110);	                               //Frame Cycle Control.(0x0110)
	_Sys_GUI_LCD_Write_Register(0x97,(0<<8));	                          
	_Sys_GUI_LCD_Write_Register(0x98,0x0000);	                               //Frame Cycle Control.
	Sys_Delay(1);
	_Sys_GUI_LCD_Write_Register(0x07,0x0173);	                          	   //0x0173
	Sys_Delay(1);

#elif CONTROLLER_ID==0x9331
	_Sys_GUI_LCD_Write_Register(0x00E7, 0x1014);
	_Sys_GUI_LCD_Write_Register(0x0001, 0x0100);                          	   //Set SS and SM bit   0x0100
	_Sys_GUI_LCD_Write_Register(0x0002, 0x0200);                          	   //Set 1 line inversion
	_Sys_GUI_LCD_Write_Register(0x0003, 0x1030);                          	   //Set GRAM write direction and BGR=1.     0x1030
	_Sys_GUI_LCD_Write_Register(0x0008, 0x0202);                          	   //Set the back porch and front porch
	_Sys_GUI_LCD_Write_Register(0x0009, 0x0000);                          	   //Set non-display area refresh cycle ISC[3:0]
	_Sys_GUI_LCD_Write_Register(0x000A, 0x0000);                          	   //FMARK function
	_Sys_GUI_LCD_Write_Register(0x000C, 0x0000);                          	   //RGB interface setting
	_Sys_GUI_LCD_Write_Register(0x000D, 0x0000);                          	   //Frame marker Position
	_Sys_GUI_LCD_Write_Register(0x000F, 0x0000);                          	   //RGB interface polarity
	                                                                      	   //Power On sequence
	_Sys_GUI_LCD_Write_Register(0x0010, 0x0000);                          	   //SAP, BT[3:0], AP, DSTB, SLP, STB
	_Sys_GUI_LCD_Write_Register(0x0011, 0x0007);                          	   //DC1[2:0], DC0[2:0], VC[2:0]
	_Sys_GUI_LCD_Write_Register(0x0012, 0x0000);                          	   //VREG1OUT voltage
	_Sys_GUI_LCD_Write_Register(0x0013, 0x0000);                          	   //VDV[4:0] for VCOM amplitude

	Sys_Delay(20);                                                        	   //Dis-charge capacitor power voltage

	_Sys_GUI_LCD_Write_Register(0x0010, 0x1690);                          	   //SAP, BT[3:0], AP, DSTB, SLP, STB
	_Sys_GUI_LCD_Write_Register(0x0011, 0x0227);                          	   //DC1[2:0], DC0[2:0], VC[2:0]
	
	Sys_Delay(5);                                                         	   //Sys_Delay 50ms

	_Sys_GUI_LCD_Write_Register(0x0012, 0x000C);                          	   //Internal reference voltage= Vci;

	Sys_Delay(5);                                                         	   //Sys_Delay 50ms

	_Sys_GUI_LCD_Write_Register(0x0013, 0x0800);                          	   //Set VDV[4:0] for VCOM amplitude
	_Sys_GUI_LCD_Write_Register(0x0029, 0x0011);                          	   //Set VCM[5:0] for VCOMH
	_Sys_GUI_LCD_Write_Register(0x002B, 0x000B);                          	   //Set Frame Rate

	Sys_Delay(5);                                                         	   //Sys_Delay 50ms

	_Sys_GUI_LCD_Write_Register(0x0020, 0x0000);                          	   //GRAM horizontal Address
	_Sys_GUI_LCD_Write_Register(0x0021, 0x0000);                          	   //GRAM Vertical Address
                                              	                          	   //Adjust the Gamma Curve
	_Sys_GUI_LCD_Write_Register(0x0030, 0x0000);
	_Sys_GUI_LCD_Write_Register(0x0031, 0x0106);
	_Sys_GUI_LCD_Write_Register(0x0032, 0x0000);
	_Sys_GUI_LCD_Write_Register(0x0035, 0x0204);
	_Sys_GUI_LCD_Write_Register(0x0036, 0x160A);
	_Sys_GUI_LCD_Write_Register(0x0037, 0x0707);
	_Sys_GUI_LCD_Write_Register(0x0038, 0x0106);
	_Sys_GUI_LCD_Write_Register(0x0039, 0x0707);
	_Sys_GUI_LCD_Write_Register(0x003C, 0x0402);
	_Sys_GUI_LCD_Write_Register(0x003D, 0x0C0F);
	                                                                      	   //Set GRAM Area
	_Sys_GUI_LCD_Write_Register(0x0050, 0x0000);                          	   //Horizontal GRAM Start Address
	_Sys_GUI_LCD_Write_Register(0x0051, 0x00EF);                          	   //Horizontal GRAM End Address
	_Sys_GUI_LCD_Write_Register(0x0052, 0x0000);                          	   //Vertical GRAM Start Address
	_Sys_GUI_LCD_Write_Register(0x0053, 0x013F);                          	   //Vertical GRAM Start Address
	_Sys_GUI_LCD_Write_Register(0x0060, 0x2700);                          	   //Gate Scan Line
	_Sys_GUI_LCD_Write_Register(0x0061, 0x0001);                          	   //NDL,VLE, REV
	_Sys_GUI_LCD_Write_Register(0x006A, 0x0000);                          	   //Set scrolling line
	                                                                      	   //Partial Display Control
	_Sys_GUI_LCD_Write_Register(0x0080, 0x0000);
	_Sys_GUI_LCD_Write_Register(0x0081, 0x0000);
	_Sys_GUI_LCD_Write_Register(0x0082, 0x0000);
	_Sys_GUI_LCD_Write_Register(0x0083, 0x0000);
	_Sys_GUI_LCD_Write_Register(0x0084, 0x0000);
	_Sys_GUI_LCD_Write_Register(0x0085, 0x0000);
	                                                                      	   //Panel Control
	_Sys_GUI_LCD_Write_Register(0x0090, 0x0010);
	_Sys_GUI_LCD_Write_Register(0x0092, 0x0600);
	_Sys_GUI_LCD_Write_Register(0x0007,0x0021);		
	Sys_GUI_LCD_Delay(50);
	_Sys_GUI_LCD_Write_Register(0x0007,0x0061);
	Sys_GUI_LCD_Delay(50);
	_Sys_GUI_LCD_Write_Register(0x0007,0x0133);                           	   //262K color and display ON
	Sys_GUI_LCD_Delay(50);

#elif CONTROLLER_ID==0x9919	                                                                      
	_Sys_GUI_LCD_Write_Register(0x28,0x0006);							  	   //POWER ON &RESET DISPLAY OFF
	_Sys_GUI_LCD_Write_Register(0x00,0x0001);
	_Sys_GUI_LCD_Write_Register(0x10,0x0000);
	_Sys_GUI_LCD_Write_Register(0x01,0x72ef);
	_Sys_GUI_LCD_Write_Register(0x02,0x0600);
	_Sys_GUI_LCD_Write_Register(0x03,0x6a38);
	_Sys_GUI_LCD_Write_Register(0x11,0x6874);                              	   //70                                                        
	_Sys_GUI_LCD_Write_Register(0x0f,0x0000); 							   	   //RAM WRITE DATA MASK
	_Sys_GUI_LCD_Write_Register(0x0b,0x5308); 							   	   //RAM WRITE DATA MASK
	_Sys_GUI_LCD_Write_Register(0x0c,0x0003);
	_Sys_GUI_LCD_Write_Register(0x0d,0x000a);
	_Sys_GUI_LCD_Write_Register(0x0e,0x2e00);                             	   //0030
	_Sys_GUI_LCD_Write_Register(0x1e,0x00be);
	_Sys_GUI_LCD_Write_Register(0x25,0x8000);
	_Sys_GUI_LCD_Write_Register(0x26,0x7800);
	_Sys_GUI_LCD_Write_Register(0x27,0x0078);
	_Sys_GUI_LCD_Write_Register(0x4e,0x0000);
	_Sys_GUI_LCD_Write_Register(0x4f,0x0000);
	_Sys_GUI_LCD_Write_Register(0x12,0x08d9);
	                                                                      	   //Adjust the Gamma Curve
	_Sys_GUI_LCD_Write_Register(0x30,0x0000);	                               //0007
	_Sys_GUI_LCD_Write_Register(0x31,0x0104);	                          	   //0203
	_Sys_GUI_LCD_Write_Register(0x32,0x0100);	 	                      	   //0001
	_Sys_GUI_LCD_Write_Register(0x33,0x0305);	                          	   //0007
	_Sys_GUI_LCD_Write_Register(0x34,0x0505);	                          	   //0007
	_Sys_GUI_LCD_Write_Register(0x35,0x0305);		                      	   //0407
	_Sys_GUI_LCD_Write_Register(0x36,0x0707);		                      	   //0407
	_Sys_GUI_LCD_Write_Register(0x37,0x0300);		                      	   //0607
	_Sys_GUI_LCD_Write_Register(0x3a,0x1200);		                      	   //0106
	_Sys_GUI_LCD_Write_Register(0x3b,0x0800);		 
	_Sys_GUI_LCD_Write_Register(0x07,0x0033);

#elif CONTROLLER_ID==0x1505
	                                                                      	   //second release on 3/5,luminance is acceptable,waves appear during camera preview
	_Sys_GUI_LCD_Write_Register(0x0007,0x0000);
	Sys_Delay(3);
	_Sys_GUI_LCD_Write_Register(0x0012,0x011C);                           	   //0x011A
	_Sys_GUI_LCD_Write_Register(0x00A4,0x0001);                           	   //NVM
	_Sys_GUI_LCD_Write_Register(0x0008,0x000F);
	_Sys_GUI_LCD_Write_Register(0x000A,0x0008);
	_Sys_GUI_LCD_Write_Register(0x000D,0x0008);
		                                                                        
	_Sys_GUI_LCD_Write_Register(0x0030,0x0707);							  	   //GAMMA CONTROL
	_Sys_GUI_LCD_Write_Register(0x0031,0x0007);                           	   //0x0707
	_Sys_GUI_LCD_Write_Register(0x0032,0x0603); 
	_Sys_GUI_LCD_Write_Register(0x0033,0x0700); 
	_Sys_GUI_LCD_Write_Register(0x0034,0x0202); 
	_Sys_GUI_LCD_Write_Register(0x0035,0x0002);                           	   //?0x0606
	_Sys_GUI_LCD_Write_Register(0x0036,0x1F0F);
	_Sys_GUI_LCD_Write_Register(0x0037,0x0707);                                //0x0f0f  0x0105
	_Sys_GUI_LCD_Write_Register(0x0038,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0039,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x003A,0x0707); 
	_Sys_GUI_LCD_Write_Register(0x003B,0x0000);                                //0x0303
	_Sys_GUI_LCD_Write_Register(0x003C,0x0007);                                //?0x0707
	_Sys_GUI_LCD_Write_Register(0x003D,0x0000);                                //0x1313//0x1f08

	Sys_Delay(3);

	_Sys_GUI_LCD_Write_Register(0x0007,0x0001);
	_Sys_GUI_LCD_Write_Register(0x0017,0x0001);                                //Power supply startup enable

	Sys_Delay(3);
	
	                                                                      	   //Power Control
	_Sys_GUI_LCD_Write_Register(0x0010,0x17A0); 
	_Sys_GUI_LCD_Write_Register(0x0011,0x0217);                           	   //reference voltage VC[2:0]   Vciout = 1.00*Vcivl
	_Sys_GUI_LCD_Write_Register(0x0012,0x011E);                           	   //0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
	_Sys_GUI_LCD_Write_Register(0x0013,0x0F00);                           	   //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
	_Sys_GUI_LCD_Write_Register(0x002A,0x0000);  
	_Sys_GUI_LCD_Write_Register(0x0029,0x000A);                           	   //0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
	_Sys_GUI_LCD_Write_Register(0x0012,0x013E);                           	   //0x013C  power supply on
	                                                                           //Coordinates Control
	_Sys_GUI_LCD_Write_Register(0x0050,0x0000);                           	   //0x0e00
	_Sys_GUI_LCD_Write_Register(0x0051,0x00EF); 
	_Sys_GUI_LCD_Write_Register(0x0052,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0053,0x013F); 
	                                                                      	   //Pannel Image Control
	_Sys_GUI_LCD_Write_Register(0x0060,0x2700); 
	_Sys_GUI_LCD_Write_Register(0x0061,0x0001); 
	_Sys_GUI_LCD_Write_Register(0x006A,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0080,0x0000); 
	                                                                      	   //Partial Image Control
	_Sys_GUI_LCD_Write_Register(0x0081,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0082,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0083,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0084,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0085,0x0000); 
	                                                                       	   //Panel Interface Control
	_Sys_GUI_LCD_Write_Register(0x0090,0x0013);                            	   //0x0010 frenqucy
	_Sys_GUI_LCD_Write_Register(0x0092,0x0300); 
	_Sys_GUI_LCD_Write_Register(0x0093,0x0005); 
	_Sys_GUI_LCD_Write_Register(0x0095,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0097,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0098,0x0000); 
	
	_Sys_GUI_LCD_Write_Register(0x0001,0x0100); 
	_Sys_GUI_LCD_Write_Register(0x0002,0x0700); 
	_Sys_GUI_LCD_Write_Register(0x0003,0x1030); 
	_Sys_GUI_LCD_Write_Register(0x0004,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x000C,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x000F,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0020,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0021,0x0000); 
	_Sys_GUI_LCD_Write_Register(0x0007,0x0021); 
	Sys_Delay(2);
	_Sys_GUI_LCD_Write_Register(0x0007,0x0061); 
	Sys_Delay(2);
	_Sys_GUI_LCD_Write_Register(0x0007,0x0173); 
	Sys_Delay(2);

#elif CONTROLLER_ID==0x8989
	_Sys_GUI_LCD_Write_Register(0x0000,0x0001);		                      	   //Start The Osciliator
	_Sys_GUI_LCD_Write_Register(0x0003,0xA8A4);		                      	   //0xA8A4
	_Sys_GUI_LCD_Write_Register(0x000C,0x0000);        
	_Sys_GUI_LCD_Write_Register(0x000D,0x080C);        
	_Sys_GUI_LCD_Write_Register(0x000E,0x2B00);        
	_Sys_GUI_LCD_Write_Register(0x001E,0x00B0);        
	_Sys_GUI_LCD_Write_Register(0x0001,0x293F);		                      	   //Output-320*240 Control Code: 0x693F  0x2B3F
	_Sys_GUI_LCD_Write_Register(0x0002,0x0600);                        	  	   //LCD Driving Waveform control
	_Sys_GUI_LCD_Write_Register(0x0010,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0011,0x6070);		                      	   //0x4030.Define The Display Mode-16 Bit Color:0x6058
	_Sys_GUI_LCD_Write_Register(0x0005,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0006,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0016,0xEF1C);     
	_Sys_GUI_LCD_Write_Register(0x0017,0x0003);     
	_Sys_GUI_LCD_Write_Register(0x0007,0x0233);		                      	   //0x0233       
	_Sys_GUI_LCD_Write_Register(0x000B,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x000F,0x0000);		                      	   //Scanning Begin Address.
	_Sys_GUI_LCD_Write_Register(0x0041,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0042,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0048,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0049,0x013F);     
	_Sys_GUI_LCD_Write_Register(0x004A,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x004B,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0044,0xEF00);     
	_Sys_GUI_LCD_Write_Register(0x0045,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0046,0x013F);     
	_Sys_GUI_LCD_Write_Register(0x0030,0x0707);     
	_Sys_GUI_LCD_Write_Register(0x0031,0x0204);     
	_Sys_GUI_LCD_Write_Register(0x0032,0x0204);     
	_Sys_GUI_LCD_Write_Register(0x0033,0x0502);     
	_Sys_GUI_LCD_Write_Register(0x0034,0x0507);     
	_Sys_GUI_LCD_Write_Register(0x0035,0x0204);     
	_Sys_GUI_LCD_Write_Register(0x0036,0x0204);     
	_Sys_GUI_LCD_Write_Register(0x0037,0x0502);     
	_Sys_GUI_LCD_Write_Register(0x003A,0x0302);     
	_Sys_GUI_LCD_Write_Register(0x003B,0x0302);     
	_Sys_GUI_LCD_Write_Register(0x0023,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0024,0x0000);     
	_Sys_GUI_LCD_Write_Register(0x0025,0x8000);     
	_Sys_GUI_LCD_Write_Register(0x004e,0);                                	   //X Begin Address(0)
	_Sys_GUI_LCD_Write_Register(0x004f,0);                                	   //Y Begin Address(0)
#endif																	  	
	ENABLE_ALL_INTS();											      		   //Atomic Operation.
	return(Screen_Controller_ID);
}
#endif
/*Finish Function:Sys_GUI_LCD_Init********************************************/


/*Begin Function:Sys_GUI_LCD_Clear_Screen**************************************
Description : To Fill The Screen With Certain Color.Namely,"Clear."
              The Function Is Extremely Fast,Because It Directly Operates 
			  The Hardware.
Input       : u16 Color-The Color That You Want In 16-Bit 5R6G5B Mode.
Output      : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Clear_Screen(u16 Color)
{
	u32 index=0; 

	DISABLE_ALL_INTS();											      //Atomic Operation.
	_Sys_GUI_LCD_Set_Cursor(0,0); 
	/* Write 16-bit Index, then Write Reg */   /* Prepare to write GRAM */
	// _Sys_GUI_LCD_Light_On()
	_Sys_GUI_Pin_Clear_Cs();
	_Sys_GUI_Pin_Clear_Rs();
	_Sys_GUI_Pin_Clear_Wr();
	_Sys_GUI_LCD_Write_Data(R34);
	_Sys_GUI_Pin_Set_Wr();
	_Sys_GUI_Pin_Set_Cs();
	
	_Sys_GUI_Pin_Clear_Cs();
	_Sys_GUI_Pin_Set_Rs();
	for(index=0;index<76800;index++)
	{
		_Sys_GUI_Pin_Clear_Wr();
		_Sys_GUI_LCD_Write_Data(Color);
		_Sys_GUI_Pin_Set_Wr();
	}
	_Sys_GUI_Pin_Set_Cs();

	ENABLE_ALL_INTS();											      //Atomic Operation.
}
#endif
/*Finish Function:Sys_GUI_LCD_Clear_Screen************************************/

/*Begin Function:Sys_GUI_LCD_Set_Point*****************************************
Description : Draw a Dot At The Coordination Given.
              Note That This Function Should Only Be Used In Drawing
			  Complicated Pictures,Or it Will Be Exceptionally SLOW.
Input       : u16 Coord_X -The Line Coordination
              u16 Coord_Y -The Row Coordination
              u16 Color -The Color Of The Dot In 5R6B5G Code.
Output      : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Set_Point(u16 Coord_X,u16 Coord_Y,u16 Color)
{
//	DISABLE_ALL_INTS();											                //Atomic Operation.Add These Adjustments To Make The Program Safer,But Much Slower.The Best Way Is To Disable It In Higher Class Functions.

//	if((Coord_X>320)||(Coord_Y>240)) 											//The Value Is Out Of The Screen Border.
//		return;
	_Sys_GUI_LCD_Set_Cursor(Coord_X,Coord_Y);									//Don't Modify Here If The Screen Orientation Is Not Correct. Modify The Screen Registers.
		                                                                           
	_Sys_GUI_Pin_Clear_Cs();																		//Prepare To Write GRAM. 
	_Sys_GUI_Pin_Clear_Rs();
	_Sys_GUI_Pin_Clear_Wr();
	_Sys_GUI_LCD_Write_Data(R34);								                // Write 16-bit Index Register.
	_Sys_GUI_Pin_Set_Wr();
	_Sys_GUI_Pin_Set_Cs();
	
	_Sys_GUI_Pin_Clear_Cs();
	_Sys_GUI_Pin_Set_Rs();																		
	_Sys_GUI_Pin_Clear_Wr();
	_Sys_GUI_LCD_Write_Data(Color);						                        //Write GRAM.
	_Sys_GUI_Pin_Set_Wr();
	_Sys_GUI_Pin_Set_Cs();;

//	ENABLE_ALL_INTS();											               //Atomic Operation.
}
#endif
/*Finish Function:Sys_GUI_LCD_SetPoint****************************************/

/*Begin Function:Sys_GUI_LCD_Get_Point*****************************************
Description : Get The Color Of a Certain Pixel.
Input       : u16 Coord_X -The Line Coordination Of The Pixel.
              u16 Coord_Y -The Row Coordination Of The Pixel.
Output      : u16 The 16-Bit Color In 5R6B5G Code.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
u16 Sys_GUI_LCD_Get_Point(u16 Coord_X,u16 Coord_Y)
{
//	DISABLE_ALL_INTS();														   //Atomic Operation.
																				
	_Sys_GUI_LCD_Set_Cursor(Coord_X,Coord_Y);

//	ENABLE_ALL_INTS();
	return (0);							                                       //Not Ready Now.
}
#endif
/*Finish Function:Sys_GUI_LCD_Get_Point***************************************/

/*Begin Function:Sys_GUI_LCD_Back_Light****************************************
Description : Turn The LCD Backlight On Or Off.
Input       : u8 Status-When "1",The Backlight is On,Otherwise it Will Be Off.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Back_Light(u8 Status)
{
//	DISABLE_ALL_INTS();														   //Atomic Operation.
	
//	if(Status>=1)
//	{
//		_Sys_GUI_LCD_Light_On();
//	}
//	else
//	{
//		_Sys_GUI_LCD_Light_Off();
//	}
	;     																	   //The Function Doesn't Function Now.
//	ENABLE_ALL_INTS();
}
#endif
/*Finish Function:Sys_GUI_LCD_Back_Light**************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/ 
