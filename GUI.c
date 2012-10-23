/******************************************************************************
Filename:   GUI.c 
Author:     www.armjishu.com,pry
Date:       07/04/2012
Description:The GUI Function List for the calculator.
            The Functions Listed Used The Functions In The "Sys_GUI_LCD.c"
			,which is more basic.
******************************************************************************/

/*Version Information**********************************************************
1.Created By   pry                                       On 06/04/2012 Ver1.0.0
  Copied some Functions From Sys_GUI_LCD.c,Whose Author is ARMjishu.
2.Modified By pry                                        On 07/04/2012 Ver1.1.0
  Knowing That The Pushing&Poping May cripple The Refresh,The 
  Sys_GUI_LCD_GUI_Filled_Rectangle,Sys_GUI_LCD_Draw_Picture And Sys_GUI_LCD_Draw_Picture_2x
  is Rewritten So That Sys_GUI_LCD_SetPoint Is No Longer Used.
3.Modified By pry					                     On 07/07/2012 Ver2.1.0
  Completed Most Drawing Functions,Only A Few Is Left.
4.Modified By pry					                     On 08/07/2012 Ver2.1.1
  Completed All Drawing Functions In Use Now.
5.Modified By pry                                        On 09/07/2012 Ver3.0.0
  Completed The WM Part.
6.Modified By pry					                     On 10/07/2012 Ver3.1.0
  Installed The GUI Module Into The Operating System.
7.Modified By pry                                        On 23/07/2012 Ver2.0.0
  Aligned The "***" Lines,And Made The Module Configurable.
******************************************************************************/

/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "GUI.h"
#include "GUI_arch.h"
#include "GUI_font.h"
#include "math.h" 															   //The "sqrt" is Used.
/*End Includes****************************************************************/

/*Config Reporter*************************************************************/
#if(REPORT_CONFIG==TRUE)
	#if(ENABLE_GUI==TRUE)
		#if(ENABLE_MEMORY_MANAGEMENT==FALSE)
			#error Please Enable The Memory Management Module Before Enabling The GUI Module.
		#endif

		#if(ENABLE_SYS_LIB==FALSE)
			#error Please Enable The System Library Module Before Enabling The GUI Module.
		#endif

		#if((ENABLE_MEMORY_MANAGEMENT==TRUE)&&(ENABLE_SYS_LIB==TRUE))
			#warning The GUI Module Is Enabled.
		#endif 
	#else
		#warning The GUI Module Is Disabled.
	#endif
#endif
/*End Config Reporter*********************************************************/
															 
/*Begin Function:Sys_GUI_LCD_Draw_Line*****************************************
Description : Draw a Line Between The Two Dots A And B.
Input       : u16 Start_Coord_X -The Line Coordination Of The A Point.
              u16 Start_Coord_Y -The Row Coordination Of The A Point.
              u16 End_Coord_X -The Line Coordination Of The B Point.
              u16 End_Coord_Y-The Row Coordination Of The B Point.
			  u16 Color -The Color Of The Line.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Draw_Line(u16 Start_Coord_X, u16 Start_Coord_Y, u16 End_Coord_X, u16 End_Coord_Y,u16 Color)
{
	double Linear,Constant;
	u16 Dot_Count;	

	if(Start_Coord_X==End_Coord_X)										  	   //This Indicates That The Slope Is Infinite. Can't Use Ordinary Ways. Check This First.
	{
		if(Start_Coord_Y==End_Coord_Y)									       //The Line Degraded Into A Dot.
			Sys_GUI_LCD_Set_Point(Start_Coord_X,Start_Coord_Y,Color);

		if(Start_Coord_Y<End_Coord_Y)									  	   //Normal.
		{
			for(Dot_Count=Start_Coord_Y;Dot_Count<=End_Coord_Y;Dot_Count++)
				Sys_GUI_LCD_Set_Point(Start_Coord_X,Dot_Count,Color);	
		}

		if(Start_Coord_Y>End_Coord_Y)									       //Abormal. Now Reverse The Process.
		{
			for(Dot_Count=End_Coord_Y;Dot_Count<=Start_Coord_Y;Dot_Count++)
				Sys_GUI_LCD_Set_Point(Start_Coord_X,Dot_Count,Color);	
		}
	}
	else
	{
		Linear=((double)(End_Coord_Y-Start_Coord_Y))/((double)(End_Coord_X-Start_Coord_X));
		Constant=(double)Start_Coord_Y-Linear*(double)Start_Coord_X;
		
		if(Start_Coord_X<End_Coord_X)
		{
			for(Dot_Count=Start_Coord_X;Dot_Count<=End_Coord_X;Dot_Count++)
				Sys_GUI_LCD_Set_Point(Dot_Count,(u32)(Linear*(double)Dot_Count+(double)Constant),Color);	
		}
		else
		{
			for(Dot_Count=End_Coord_X;Dot_Count<=Start_Coord_X;Dot_Count++)
				Sys_GUI_LCD_Set_Point(Dot_Count,(u32)(Linear*(double)Dot_Count+(double)Constant),Color);		
		}
	}
}
#endif
/*Finish Function:Sys_GUI_LCD_Draw_Line***************************************/


/*Begin Function:Sys_GUI_LCD_Print_Char****************************************
Description : Display A 16x24 ASCII At The Given Coordination.
Input       : u16 Coord_X-The Line Coordination Of The Top-Left Corner Of The Character.
              u16 Coord_Y-The Row Coordination Of The Top-Left Corner Of The Character.
			  u8 Char-The Character.
			  u8 Size -The Size Of The Character.
			  u16 Char_Color-The Color Of The Character.
			  u16 Background_Color-The color Of The Background.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Print_Char(u16 Coord_X,u16 Coord_Y,u8 Char,u8 Size,u16 Char_Color,u16 Background_Color)
{
	u16 Line_Count=0;
	u16 Column_Count=0;

	u16 This_Line_Dots=0;

	switch(Size)
	{
		case _8X16_:
		{
			for (Line_Count=0;Line_Count<16;Line_Count++)
			{
				This_Line_Dots=ASCII_Table_8x16[((Char-0x20)*16)+Line_Count];  //Get The Lines Of The Character To Display.
				for (Column_Count=0;Column_Count<8;Column_Count++)			   //These Are Dots.
				{
					if ((This_Line_Dots>>(7-Column_Count))& 0x01==0x01)
					{														   //The Color Of The Character.
				 		Sys_GUI_LCD_Set_Point(Coord_X+Column_Count,Coord_Y+Line_Count,Char_Color); 
				    }
				    else
				    {
					  	if(Background_Color!=TRANSPARENT)					   //The Color Of The Background.
				        	Sys_GUI_LCD_Set_Point(Coord_X+Column_Count,Coord_Y+Line_Count,Background_Color); 
				    }
			  	}
			}
			break;
		}

		case _16X24_:
		{
			for(Line_Count=0;Line_Count<24;Line_Count++)
			{
			This_Line_Dots=ASCII_Table_16x24[((Char-0x20)*24)+Line_Count];
		
				for (Column_Count=0;Column_Count<16;Column_Count++)
				{																	   
					if ((This_Line_Dots>>Column_Count)&0x01==0x01)			   //The Color Of The Characters.
						Sys_GUI_LCD_Set_Point(Coord_X+Column_Count,Coord_Y+Line_Count,Char_Color); 
					else
					{
						if(Background_Color!=TRANSPARENT)					   //The Color Of The Background.
							Sys_GUI_LCD_Set_Point(Coord_X+Column_Count,Coord_Y+Line_Count,Background_Color); 
					}
				}
			}
			break;
		}
		default:break;
	}
}
#endif
/*Finish Function:Sys_GUI_LCD_Print_Char**************************************/

/*Begin Function:Sys_GUI_LCD_Print_String**************************************
Description: To Put A String Of 8x16 ASCII Codes On The Screen.
Input      : u16 Coord_X -The Line Coordination Of The Top-Left Corner Of The String.
             u16 Coord_Y -The Row Coordination Of The Top-Left Corner Of The String.
	         u8* String -The Pointer For The String.
			 u8 Size -The Size Of The Characters Forming The String.
			 u16 Length -The Length Of The Character.
	         u16 Char_Color -The Color Of The Characters.
	         u16 Background_Color -The Color Of The Background.
Output     : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Print_String(u16 Coord_X,u16 Coord_Y,u8* String,u8 Size,u16 Length,u16 Char_Color,u16 Background_Color)
{
	u16 Char_Count;
	u8 *Char_Pointer=String;
	u8 Padding;

	switch(Size)
	{
		case _8X16_:Padding=8;break;
		case _16X24_:Padding=16;break;
		default:Padding=8;break;
	}

	for(Char_Count=0;(Char_Count<Length)&&((*Char_Pointer)!='\0');Char_Count++)
		Sys_GUI_LCD_Print_Char(Coord_X+Padding*Char_Count,Coord_Y,*Char_Pointer++,Size,Char_Color,Background_Color);	
}
#endif
/*Finish Function:Sys_GUI_LCD_Print_String************************************/


/*Begin Function:Sys_GUI_LCD_GUI_Polygon***************************************
Description: Draw a Polygon on The Screen,Which Can Be Used As Menu,Buttons,etc.
Input      : u16 Number_Of_Points-The number Of Points.
             u16 (*Coord_Of_Dots)[2] -The Coordinations Of The Points Stored In A 2-Dimension Array.
			 The First Dot And The Last Dot Must Be The Same If You Want To Draw A Polygon.
			 Therefore,Drawing A Triangle Will Cost 4 Points,Not 3.
			 u16 Color -The Color In 5R6G5B Form.
Output     : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_GUI_Polygon(u16 Number_Of_Points,u16 (*Coord_Of_Dots)[2],u16 Color)
{
 	int Number_Of_Lines;
	u16 (*Dot_Pointer)[2]=Coord_Of_Dots;
	u16 Start_Coord_X,Start_Coord_Y,End_Coord_X,End_Coord_Y;
	for(Number_Of_Lines=Number_Of_Points-1;Number_Of_Lines>0;Number_Of_Lines--)
	{
	    Start_Coord_X=(*Dot_Pointer)[0];								       //Change The Dimensions To Draw The Line Continuously.
		Start_Coord_Y=(*Dot_Pointer)[1];
		Dot_Pointer++;
		End_Coord_X=(*Dot_Pointer)[0];
		End_Coord_Y=(*Dot_Pointer)[1];										   //Draw a line.
		Sys_GUI_LCD_Draw_Line(Start_Coord_X,Start_Coord_Y,End_Coord_X,End_Coord_Y,Color);  
	}	
}
#endif
/*Finish Function:Sys_GUI_LCD_GUI_Polygon*************************************/

/*Begin Function:Sys_GUI_LCD_GUI_Filled_Rectangle******************************
Description: Draw a Filled Rectangle on The Screen.
Input      : u16 Coord_X -The X Position Of The Top-Left Corner.
             u16 Coord_Y -The Y Position Of The Top-Left Corner.
			 u16 Length -The Length Of The Rectangle.
			 u16 Width -The Width Of The Rectangle. 
			 u16 Border_Color -The Color Of Its Border.
			 u16 Fill_Color -The Color Within The Border.
Output     : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_GUI_Filled_Rectangle(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u16 Border_Color,u16 Fill_Color)
{
 	u16 Coord_X_Count,Coord_Y_Count;

	if(Fill_Color!=TRANSPARENT)
	{
		for(Coord_X_Count=0;Coord_X_Count<Length-1;Coord_X_Count++)
			for(Coord_Y_Count=0;Coord_Y_Count<Width-1;Coord_Y_Count++)
				Sys_GUI_LCD_Set_Point(Coord_X+1+Coord_X_Count,Coord_Y+1+Coord_Y_Count,Fill_Color);
	}
																			 //Draw The Border Now.
	Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,Border_Color);
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,Border_Color);//We Don't Judge If The Border Is Also "TRANSPARENT",Which Is Already Judged In "Sys_GUI_LCD_Draw_Line".
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,Border_Color);
	Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,Border_Color);		
}
#endif
/*Finish Function:Sys_GUI_LCD_GUI_Filled_Rectangle****************************/


/*Begin Function:Sys_GUI_LCD_GUI_Circle****************************************
Description: Draw An Circle on The Screen,Which can be
             Used As Menu,Buttons,etc.
Input      : u16 Center_Coord_X -The X Dimension Of Top-Left Corner Of The Center.
             u16 Center_Coord_Y -The Y Dimension Of Top-Left Corner Of The Center.
			 u16 Radius -The Radius Of The Circle.
			 u16 Color -The Color."Transparent" Can't Be Applied To This.
Output     : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_GUI_Circle(u16 Center_Coord_X,u16 Center_Coord_Y,u16 Radius,u16 Color)
{																		 
	u16 Dot_Amount=(u16)2*(PI*(double)Radius);								   //Use The Parametric Equation For Better Output.
	u16 Dot_Count=0;
	for(Dot_Count=0;Dot_Count<=Dot_Amount;Dot_Count++)
		Sys_GUI_LCD_Set_Point(Center_Coord_X+(s16)((double)Radius*sin((double)Dot_Count/(double)Dot_Amount*2*PI)),Center_Coord_Y+(s16)((double)Radius*cos((double)Dot_Count/(double)Dot_Amount*2*PI)),Color);																					
}
#endif
/*Finish Function:Sys_GUI_LCD_GUI_Circle**************************************/

///*Begin Function:Sys_GUI_LCD_GUI_Arc*******************************************
//Description: Draw An Arc(A Part Of Circle) on The Screen,Which can be
//             Used As Menu,Buttons,etc.
//Input      : u16 Center_Coord_X -The X Dimension Of Top-Left Corner Of The Center.
//             u16 Center_Coord_Y -The Y Dimension Of Top-Left Corner Of The Center.
//			 u16 Radius -The Radius Of The Arc.
//			 double Start_Angle -The Start Angle Of The Arc.From 0~2PI.
//			 double End_Angle -The End Angle Of The Arc.From 0~2PI.
//			 u16 Color -The Color."Transparent" Can't Be Applied To This.
//Output     : Void.
//******************************************************************************/
//#if(ENABLE_GUI==TRUE)
//void Sys_GUI_LCD_GUI_Arc(u16 Center_Coord_X,u16 Center_Coord_Y,u16 Radius,double Start_Angle,double End_Angle,u16 Color)
//{																		 
//	u16 Dot_Amount=2*(u16)(abs(End_Angle-Start_Angle)*(double)Radius);											//Multiply 2 For Better View.													  //Use The Parametric Equation For Better Output.
//	u16 Dot_Count=0;
//	for(Dot_Count=0;Dot_Count<=Dot_Amount;Dot_Count++)
//		Sys_GUI_LCD_Set_Point(Center_Coord_X+(s16)((double)Radius*sin((double)Dot_Count/(double)Dot_Amount*2*PI+Start_Angle)),Center_Coord_Y+(s16)((double)Radius*cos((double)Dot_Count/(double)Dot_Amount*2*PI+Start_Angle)),Color);																					
//}
//#endif
///*Finish Function:Sys_GUI_LCD_GUI_Arc*****************************************/

/*Begin Function:Sys_GUI_LCD_Draw_Picture**************************************
Description : To Show A Picture In The Given Area Of The Screen.Note That The Arguments Must Be Precise.
              Didn't Use Sys_GUI_LCD_SetPoint,for it is Slow.
              The Correct Picture Mode Is Given In The Img2Lcd Software,Scan From Top-Left To Bottom-Right Row By Row.
Input       : u16 Start_Coord_X -The X Coordination Of The Smaller(Coordination) Corner.
              u16 Start_Coord_Y -The Y Coordination Of The Smaller(Coordination) Corner.
              u16 Original_Length -The Horizonal(X) Length Of The Original Picture.
              u16 Original_Width -The Vertical(Y) Length Of The Original Picture.						 
			  u16 Picture -The Pointer For The Picture.
Output      : Void
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Draw_Picture(u16 Start_Coord_X,u16 Start_Coord_Y,u16 Original_Length,u16 Original_Width,u16 *Picture)
{
	u16 Color;
	u8* Picture_Pointer =(u8*)Picture;
	u16 Line_Count;
	u16 Dot_Count;

	for(Line_Count=0;Line_Count<Original_Width;Line_Count++)				   //This Fucntion Needs To Be Modified To Fit A Specific Screen.
	{
		for(Dot_Count=0;Dot_Count<Original_Length;Dot_Count++)
		{
			Color=0x00;
			Color|=(*Picture_Pointer++)<<8;
			Color|=(*Picture_Pointer++);
			Sys_GUI_LCD_Set_Point(Start_Coord_X+Dot_Count,Start_Coord_Y+Line_Count,Color);	
		}	
	}
}
#endif
/*Finish Function:Sys_GUI_LCD_Draw_Picture************************************/

/*Begin Function:Sys_GUI_LCD_Draw_Dotted_Line**********************************
Description : Draw A Dotted Line Between The Two Points Given.
Input       : u16 Start_Coord_X -The X Coordination For The Start Point.
              u16 Start_Coord_Y -The Y Coordination For The Start Point.
			  u16 End_Coord_X -The X Coordination For The End Point.
              u16 End_Coord_Y -The Y Coordination For The End Point.
			  u16 Color -The Color Of The Dotted Line.For The Dots Only.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_LCD_Draw_Dotted_Line(u16 Start_Coord_X, u16 Start_Coord_Y, u16 End_Coord_X, u16 End_Coord_Y,u16 Color)
{
	double Linear,Constant;
	u16 Dot_Count;	

	if(Start_Coord_X==End_Coord_X)										       //This Indicates That The Slope Is Infinite. Can't Use Ordinary Ways. Check This First.
	{
		if(Start_Coord_Y==End_Coord_Y)									       //The Line Degraded Into A Dot.
			Sys_GUI_LCD_Set_Point(Start_Coord_X,Start_Coord_Y,Color);

		if(Start_Coord_Y<End_Coord_Y)									       //Normal.
		{
			for(Dot_Count=Start_Coord_Y;Dot_Count<=End_Coord_Y;Dot_Count++)
				if((Dot_Count%2)==0)
					Sys_GUI_LCD_Set_Point(Start_Coord_X,Dot_Count,Color);	
		}

		if(Start_Coord_Y>End_Coord_Y)									       //Abormal. Now Reverse The Process.
		{
			for(Dot_Count=End_Coord_Y;Dot_Count<=Start_Coord_Y;Dot_Count++)
				if((Dot_Count%2)==0)
					Sys_GUI_LCD_Set_Point(Start_Coord_X,Dot_Count,Color);	
		}
	}
	else
	{
		Linear=((double)(End_Coord_Y-Start_Coord_Y))/((double)(End_Coord_X-Start_Coord_X));
		Constant=(double)Start_Coord_Y-Linear*(double)Start_Coord_X;
		
		if(Start_Coord_X<End_Coord_X)
		{
			for(Dot_Count=Start_Coord_X;Dot_Count<=End_Coord_X;Dot_Count++)
				if((Dot_Count%2)==0)
					Sys_GUI_LCD_Set_Point(Dot_Count,(u32)(Linear*(double)Dot_Count+(double)Constant),Color);	
		}
		else
		{
			for(Dot_Count=End_Coord_X;Dot_Count<=Start_Coord_X;Dot_Count++)
				if((Dot_Count%2)==0)
					Sys_GUI_LCD_Set_Point(Dot_Count,(u32)(Linear*(double)Dot_Count+(double)Constant),Color);		
		}
	}
}
#endif
/*Finish Function:Sys_GUI_LCD_Draw_Dotted_Line********************************/

/*Begin Function:Sys_GUI_Color_Mixer*******************************************
Description : Return The Mixed Color Of Color_1 And Color_2,The 
              Destination Color is "Color_1*(1-Ratio)+Color_2*Ratio".
              The Color Format Is "5R6G5B".It Is A Inline Function.(Pending...)
			  "Color_1(0.00)----------0.33----------0.67----------(0.99)Color_2".
Input       : u16 Color_1 -The First Color.
              u16 Color_2 -The Second Color.
Output      : u16 -The Mixed Color In "5R6G5B" Format.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
u16 Sys_GUI_Color_Mixer(u16 Color_1,u16 Color_2,float Ratio)
{
	u8 Red,Green,Blue;
	if(Ratio<0)
		return(Color_1);
	if(Ratio>1)
		return(Color_2);
	
	Blue=(u16)((Color_1&0x1F)*(1-Ratio)+(Color_2&0x1F)*(Ratio));			   //Mix The 3 Dimensions Separately.
	Green=(u16)(((Color_1>>5)&0x3F)*(1-Ratio)+((Color_2>>5)&0x3F)*(Ratio));
	Red=(u16)(((Color_1>>11)&0x1F)*(1-Ratio)+((Color_2>>11)&0x1F)*(Ratio));

	return((Red<<11)|(Green<<5)|(Blue));
}
#endif
/*Finish Function:Sys_GUI_Color_Mixer*****************************************/

/*Begin Function:_Sys_GUI_Draw_Dialog_Title************************************
Description : Draw A Dialog Title Box,With A Omittable Icon.The Box Contents Is Not
              Included(That Is The Job For"_Sys_GUI_Draw_Dialog_Box_Without_Title").
Input       : u16 Coord_X -The X Coordination Of The Top-Left Corner Of The Bar.
              u16 Coord_Y -The Y Coordination Of The Top-Left Corner Of The Bar.
			  u16 Length -The Length Of The Bar.
			  u16 Width -The Width Of The Bar.
			  void* Title -The Title Of The Dialog Box.It Is Shown On The Title.
			  void* Icon -The Omittable Icon. If The Argument Is Zero,No Icon Will Be Displayed.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void _Sys_GUI_Draw_Dialog_Title(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,void* Title,void* Icon)
{
	u16 Coord_X_Count,Coord_Y_Count;
	for(Coord_X_Count=0;Coord_X_Count<Length;Coord_X_Count++)
	{
		for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
		{
			Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,Sys_GUI_Color_Mixer(TITLE_LEFTSIDE_COLOR,TITLE_RIGHTSIDE_COLOR,(float)Coord_X_Count/(float)Length));
		}
	}

	if(Icon!=0)
	{																		   //Draw The Icon.
		Sys_GUI_LCD_Draw_Picture(Coord_X+1,Coord_Y+1,ICON_SIZE,ICON_SIZE,(u16*)Icon); 
	}

	if(Title!=0)
	{
		Sys_GUI_LCD_Print_String(Coord_X+ICON_SIZE+4,Coord_Y+1,(u8*)Title,DIALOG_TITLE_FONT,Sys_Strlen(Title),WHITE,TRANSPARENT);
	}
}
#endif
/*Finish Function:_Sys_GUI_Draw_Dialog_Title**********************************/

/*Begin Function:_Sys_GUI_Draw_Dialog_Box_Without_Title************************
Description : Draw A Dialog Box,With Out Its Title.
Input       : u16 Coord_X -The X Coordination Of The Top-Left Corner Of The Bar.
              u16 Coord_Y -The Y Coordination Of The Top-Left Corner Of The Bar.
			  u16 Length -The Length Of The Bar.
			  u16 Width -The Width Of The Bar.
			  void* Icon -The Omittable Icon. If The Argument Is Zero,No Icon Will Be Displayed.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void _Sys_GUI_Draw_Dialog_Box_Without_Title(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width)
{
	u16 Coord_X_Count,Coord_Y_Count;

	for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
	{	
		for(Coord_X_Count=0;Coord_X_Count<Length;Coord_X_Count++)
		{
			Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,BACKCOLOR);
		}
	}
	
	Sys_GUI_LCD_Draw_Line(Coord_X+1,Coord_Y+1,Coord_X+Length-2,Coord_Y+1,BLACK);
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-2,Coord_Y+1,Coord_X+Length-2,Coord_Y+Width-2,BLACK);
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-2,Coord_Y+Width-2,Coord_X+1,Coord_Y+Width-2,BLACK);
	Sys_GUI_LCD_Draw_Line(Coord_X+1,Coord_Y+Width-2,Coord_X+1,Coord_Y+1,BLACK);	
}
#endif
/*Finish Function:_Sys_GUI_Draw_Dialog_Box_Without_Title**********************/

/*Begin Function:Sys_GUI_Draw_Dialog_Box***************************************
Description : Draw A Dialog Box,With A Omittable Icon.
Input       : u16 Coord_X -The X Coordination Of The Top-Left Corner Of The Bar.
              u16 Coord_Y -The Y Coordination Of The Top-Left Corner Of The Bar.
			  u16 Length -The Length Of The Bar.
			  u16 Width -The Width Of The Box.The Width Of The Title Is Included.
			  void* Title -The Title Of The Dialog Box.Must End With A '\0'.
			  void* Icon -The Omittable Icon. If The Argument Is Zero,No Icon Will Be Displayed.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Dialog_Box(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,void* Title,void* Icon)
{
	_Sys_GUI_Draw_Dialog_Title(Coord_X,Coord_Y,Length,DIALOG_TITLE_WIDTH,Title,Icon);
	_Sys_GUI_Draw_Dialog_Box_Without_Title(Coord_X,Coord_Y+DIALOG_TITLE_WIDTH,Length,Width-DIALOG_TITLE_WIDTH);	
}
#endif
/*Finish Function:Sys_GUI_Draw_Dialog_Box*************************************/

/*Begin Function:Sys_GUI_Erase_Dialog_Box**************************************
Description : Erase A Dialog Box.
Input       : u8 Dialog_ID -The Dialog Box To Be Erased.
			  u16 Color -The Color Used To Cover It.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Erase_Dialog_Box(u8 Dialog_ID,u16 Color)
{
	Sys_GUI_LCD_GUI_Filled_Rectangle(DCB[Dialog_ID].Dialog_Coord_X,DCB[Dialog_ID].Dialog_Coord_Y,DCB[Dialog_ID].Dialog_Length,DCB[Dialog_ID].Dialog_Width,Color,Color);	
}
#endif
/*Finish Function:Sys_GUI_Erase_Dialog_Box************************************/

/*Begin Function:Sys_GUI_Draw_Command_Button***********************************
Description : Draw A Blank Command Button According To The Arguments.
Input       : u16 Coord_X -The X Coordination Of The Top-Left Corner Of The Button.
              u16 Coord_Y -The Y Coordination Of The Top-Left Corner Of The Button.
			  u16 Length -The Length Of The Button.
			  u16 Width -The Width Of The Button.
			  u8 Status -The Status Of The Command Button.Can Be The Combination Of The Following:
			  COMMAND_BUTTON_PUSHED                [0x80]					 //The Command Button -Pushed.
 			  COMMAND_BUTTON_NORMAL                [0x00]					 //The Command Button -Normal.
			  COMMAND_BUTTON_WITH_DOTTED_LINE	   [0x40]                    //The Command Button With Dotted Line Around It.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Command_Button(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status)
{
	u16 Coord_X_Count,Coord_Y_Count;

	for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
	{	
		for(Coord_X_Count=0;Coord_X_Count<Length;Coord_X_Count++)
		{
			Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,BACKCOLOR);
		}
	}

	if((Status&COMMAND_BUTTON_PUSHED)!=0)
	{
		Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,BLACK);
		Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,WHITE);
		Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,WHITE);
		Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,BLACK);
																			   //The Shadow.						   	 
		Sys_GUI_LCD_Draw_Line(Coord_X+1,Coord_Y+Width-2,Coord_X+1,Coord_Y+1,DARKER_BACKCOLOR);	
		Sys_GUI_LCD_Draw_Line(Coord_X+1,Coord_Y+1,Coord_X+Length-2,Coord_Y+1,DARKER_BACKCOLOR);						
	}	
	else
	{
		Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,WHITE);
		Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,BLACK);
		Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,BLACK);
		Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,WHITE);
																			   //The Shadow.							   	 
		Sys_GUI_LCD_Draw_Line(Coord_X+Length-2,Coord_Y+Width-2,Coord_X+1,Coord_Y+Width-2,DARKER_BACKCOLOR);	
		Sys_GUI_LCD_Draw_Line(Coord_X+Length-2,Coord_Y+1,Coord_X+Length-2,Coord_Y+Width-2,DARKER_BACKCOLOR);		
	}

	if((Status&COMMAND_BUTTON_WITH_DOTTED_LINE)!=0)
	{
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+3,Coord_Y+3,Coord_X+Length-4,Coord_Y+3,BLACK);
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+Length-4,Coord_Y+3,Coord_X+Length-4,Coord_Y+Width-4,BLACK);
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+Length-4,Coord_Y+Width-4,Coord_X+3,Coord_Y+Width-4,BLACK);
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+3,Coord_Y+Width-4,Coord_X+3,Coord_Y+3,BLACK);			
	}
}
#endif
/*Finish Function:Sys_GUI_Draw_Command_Button*********************************/


/*Begin Function:Sys_GUI_Draw_Check_Box****************************************
Description : Draw A Blank Check Box According To The Arguments.
Input       : u16 Coord_X -The X Coordination Of The Top-Left Corner Of The Box.
              u16 Coord_Y -The Y Coordination Of The Top-Left Corner Of The Box.
			  u16 Length -The Length Of The Box.
			  u16 Width -The Width Of The Box.
			  u8 Status -The Status Of The Check Box.Can Be The Combination Of The Following:
			  CHECK_BOX_CHECKED					 [0x80]					 //The Checked Check Box. 
			  CHECK_BOX_UNCHECKED				 [0x00]					 //The Unchecked Check Box.
			  CHECK_BOX_BEING_CHECKED		     [0x40]					 //The Check Box Is Being Checked Now.
Output      : Void.
******************************************************************************/		
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Check_Box(u16 Coord_X,u16 Coord_Y,u8 Status)		           //The Default Size Is 13.
{
	u16 Coord_X_Count,Coord_Y_Count;

	if((Status&CHECK_BOX_BEING_CHECKED)!=0)
	{
		for(Coord_Y_Count=0;Coord_Y_Count<CHECK_BOX_SIZE;Coord_Y_Count++) 
			for(Coord_X_Count=0;Coord_X_Count<CHECK_BOX_SIZE;Coord_X_Count++) 
				Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,BACKCOLOR); 				 
	}																		   //The Box Is Being Checked Now,So We Make It Gray.
	else
	{
		for(Coord_Y_Count=0;Coord_Y_Count<CHECK_BOX_SIZE;Coord_Y_Count++) 
			for(Coord_X_Count=0;Coord_X_Count<CHECK_BOX_SIZE;Coord_X_Count++) 
				Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,WHITE);                    
	}																		   //Clear Out The White Area.
																			   //Draw The Outline.
	Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y,Coord_X+CHECK_BOX_SIZE-1,Coord_Y,DARKER_BACKCOLOR);			     
	Sys_GUI_LCD_Draw_Line(Coord_X+CHECK_BOX_SIZE-1,Coord_Y,Coord_X+CHECK_BOX_SIZE-1,Coord_Y+CHECK_BOX_SIZE-1,WHITE);
	Sys_GUI_LCD_Draw_Line(Coord_X+CHECK_BOX_SIZE-1,Coord_Y+CHECK_BOX_SIZE-1,Coord_X,Coord_Y+CHECK_BOX_SIZE-1,WHITE);
	Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+CHECK_BOX_SIZE-1,Coord_X,Coord_Y,DARKER_BACKCOLOR);

	Sys_GUI_LCD_Draw_Line(Coord_X+1,Coord_Y+1,Coord_X+CHECK_BOX_SIZE-2,Coord_Y+1,BLACK);
	Sys_GUI_LCD_Draw_Line(Coord_X+CHECK_BOX_SIZE-2,Coord_Y+1,Coord_X+CHECK_BOX_SIZE-2,Coord_Y+CHECK_BOX_SIZE-2,DARKER_BACKCOLOR);
	Sys_GUI_LCD_Draw_Line(Coord_X+CHECK_BOX_SIZE-2,Coord_Y+CHECK_BOX_SIZE-2,Coord_X,Coord_Y+CHECK_BOX_SIZE-2,DARKER_BACKCOLOR);
	Sys_GUI_LCD_Draw_Line(Coord_X+1,Coord_Y+CHECK_BOX_SIZE-2,Coord_X+1,Coord_Y+1,BLACK);

	if((Status&CHECK_BOX_CHECKED)!=0)
	{																		   //The "Correct" Mark.
		Sys_GUI_LCD_Set_Point(Coord_X+1+2,Coord_Y+1+4,BLACK);				   //(2,4)
		Sys_GUI_LCD_Set_Point(Coord_X+1+2,Coord_Y+1+5,BLACK);				   //(2,5)
		Sys_GUI_LCD_Set_Point(Coord_X+1+2,Coord_Y+1+6,BLACK);			   	   //(2,6)

		Sys_GUI_LCD_Set_Point(Coord_X+1+3,Coord_Y+1+5,BLACK);				   //(3,5)
		Sys_GUI_LCD_Set_Point(Coord_X+1+3,Coord_Y+1+6,BLACK);				   //(3,6)																											 
		Sys_GUI_LCD_Set_Point(Coord_X+1+3,Coord_Y+1+7,BLACK);			 	   //(3,7)

		Sys_GUI_LCD_Set_Point(Coord_X+1+4,Coord_Y+1+6,BLACK);				   //(4,6)
		Sys_GUI_LCD_Set_Point(Coord_X+1+4,Coord_Y+1+7,BLACK);				   //(4,7)
		Sys_GUI_LCD_Set_Point(Coord_X+1+4,Coord_Y+1+8,BLACK);				   //(4,8)

		Sys_GUI_LCD_Set_Point(Coord_X+1+5,Coord_Y+1+5,BLACK);				   //(5,5)																											
		Sys_GUI_LCD_Set_Point(Coord_X+1+5,Coord_Y+1+6,BLACK);				   //(5,6) 
		Sys_GUI_LCD_Set_Point(Coord_X+1+5,Coord_Y+1+7,BLACK);				   //(5,7)

		Sys_GUI_LCD_Set_Point(Coord_X+1+6,Coord_Y+1+4,BLACK);				   //(6,4)
		Sys_GUI_LCD_Set_Point(Coord_X+1+6,Coord_Y+1+5,BLACK);				   //(6,5)
		Sys_GUI_LCD_Set_Point(Coord_X+1+6,Coord_Y+1+6,BLACK);				   //(6,6)

		Sys_GUI_LCD_Set_Point(Coord_X+1+7,Coord_Y+1+3,BLACK);				   //(7,3)
		Sys_GUI_LCD_Set_Point(Coord_X+1+7,Coord_Y+1+4,BLACK);				   //(7,4)
		Sys_GUI_LCD_Set_Point(Coord_X+1+7,Coord_Y+1+5,BLACK);				   //(7,5)

		Sys_GUI_LCD_Set_Point(Coord_X+1+8,Coord_Y+1+2,BLACK);				   //(8,2)
		Sys_GUI_LCD_Set_Point(Coord_X+1+8,Coord_Y+1+3,BLACK);				   //(8,3)
		Sys_GUI_LCD_Set_Point(Coord_X+1+8,Coord_Y+1+4,BLACK);				   //(8,4)
	}
}
#endif
/*Finish Function:Sys_GUI_Draw_Check_Box**************************************/

/*Begin Function:Sys_GUI_Draw_Scroll_Bar***************************************
Description : Draw A Scroll Bar According To The Arguments.
Input       : u16 Coord_X -The X Coordination Of The Top-Left Corner Of The Bar.
              u16 Coord_Y -The Y Coordination Of The Top-Left Corner Of The Bar.
			  u16 Length -The Length Of The Bar.
			  u16 Width -The Width Of The Bar.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Scroll_Bar(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width)
{
	u16 Coord_X_Count,Coord_Y_Count;

	for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
	{	
		for(Coord_X_Count=0;Coord_X_Count<Length;Coord_X_Count++)
		{																	   //The Rectangular. 
			Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,BACKCOLOR);						 
		}
	}
																										         
	Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,WHITE);	   //The Outline.									  
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,BLACK);
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,BLACK);
	Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,WHITE);
																			   //The Shadow.								   	 
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-2,Coord_Y+Width-2,Coord_X+1,Coord_Y+Width-2,DARKER_BACKCOLOR);	         
	Sys_GUI_LCD_Draw_Line(Coord_X+Length-2,Coord_Y+1,Coord_X+Length-2,Coord_Y+Width-2,DARKER_BACKCOLOR);		
}
#endif
/*Finish Function:Sys_GUI_Draw_Scroll_Bar*************************************/

/*Begin Function:Sys_GUI_Draw_Radio_Button_Group*******************************
Description : Draw Relative Radio Buttons According To The Arguments.Supports 8 Radio Buttons At The Same Time.
Input       : u16 Coord_X -The X Coordination Of The Reference Point.
              u16 Coord_Y -The Y Coordination Of The Reference Point.
			  u16 Number -The Number Of Radio Buttons.
			  u16 Relative_Centers -The Relative Center Positions Of The Radio Buttons,In X1,Y1,X2,Y2,...Format.
			  u8 Status -The Status Of The Radio Button Group.Can Be The Combination Of The Following:
			  RADIO_BUTTON_CHECKED                 [0x80]					 //The Radio Button Group Is Checked.(Operated At Least Once Since Its Appearance)
			  RADIO_BUTTON_BEING_CHECKED           [0x40]					 //The Radio Button Group Is Being Checked.
			  RADIO_BUTTON_CHECKED_NUM_(x)		   [((x)<<3)]				 //The Checked Number.
			  RADIO_BUTTON_BEING_CHECKED_NUM_(x)   [((x))]					 //The Being-Checked Number.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Radio_Button_Group(u16 Coord_X,u16 Coord_Y,u8 Number,u16* Relative_Centers,u8 Status)                 
{
	u8 Number_Count,Point_Count;

	const u8  DARKER_BACKCOLOR_Dot[]=										   //These Are Positions Codes Of The Picture."(X,Y)".
	{
		0x15,0x16,0x17,0x18,0x23,0x24,0x29,0x2A,0x32,0x42,0x51,0x61,0x71,0x81,0x92,0xA2	
	};

	const u8 WHITE_Dot[]=
	{
		0x3B,0x4B,0x5C,0x6C,0x7C,0x8C,0x9B,0xAB,0xB3,0xB4,0xB9,0xBA,0xC5,0xC6,0xC7,0xC8
	};

	const u8 BLACK_Dot[]=
	{
		0x25, 0x26, 0x27, 0x28, 0x33, 0x34, 0x39, 0x43, 0x52, 0x62, 0x72, 0x82, 0x93, 0xA3
	};

	const u8 BACKCOLOR_Dot[]=
	{
		0x3A, 0x4A, 0x5B, 0x6B, 0x7B, 0x8B, 0x9A, 0xA9, 0xAA, 0xB5, 0xB6, 0xB7, 0xB8
	};

	const u8 Central_Mark[]=
	{
		0x56,0x57,0x65,0x66,0x67,0x68,0x75,0x76,0x77,0x78,0x86,0x87
	};

	const u8 WHITE_Area[]=
	{																	       //The Position Of The White Area. 
		0x35,0x36,0x37,0x38,0x44,0x45,0x46,0x47,0x48,0x49,0x53,0x54,0x55,0x58,0x59,0x5A,0x63,0x64,0x69,0x6A,  
		0x73,0x74,0x79,0x7A,0x83,0x84,0x85,0x88,0x89,0x8A,0x94,0x95,0x96,0x97,0x98,0x99,0xA5,0xA6,0xA7,0xA8			
	};

	for(Number_Count=0;Number_Count<Number;Number_Count++)
	{																		   //The Darker Background Part.
		for(Point_Count=0;Point_Count<sizeof(DARKER_BACKCOLOR_Dot);Point_Count++)							  
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(DARKER_BACKCOLOR_Dot[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(DARKER_BACKCOLOR_Dot[Point_Count]&0x0F),DARKER_BACKCOLOR);		
		
		for(Point_Count=0;Point_Count<sizeof(WHITE_Dot);Point_Count++)
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(WHITE_Dot[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(WHITE_Dot[Point_Count]&0x0F),WHITE);
		
		for(Point_Count=0;Point_Count<sizeof(BLACK_Dot);Point_Count++)
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(BLACK_Dot[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(BLACK_Dot[Point_Count]&0x0F),BLACK);
			
		for(Point_Count=0;Point_Count<sizeof(BACKCOLOR_Dot);Point_Count++)
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(BACKCOLOR_Dot[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(BACKCOLOR_Dot[Point_Count]&0x0F),BACKCOLOR);			
	
		for(Point_Count=0;Point_Count<sizeof(WHITE_Area);Point_Count++)
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(WHITE_Area[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(WHITE_Area[Point_Count]&0x0F),WHITE);	
	
		for(Point_Count=0;Point_Count<sizeof(Central_Mark);Point_Count++)
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(Central_Mark[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(Central_Mark[Point_Count]&0x0F),WHITE);	
	}

	if((Status&RADIO_BUTTON_BEING_CHECKED)!=0)
	{
		Number_Count=Status&0x07;
	    
		for(Point_Count=0;Point_Count<sizeof(WHITE_Area);Point_Count++)
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(WHITE_Area[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(WHITE_Area[Point_Count]&0x0F),BACKCOLOR);
		
		for(Point_Count=0;Point_Count<sizeof(Central_Mark);Point_Count++)	   //Must Test The "Being Checked" First,Or It Will Cause Flaws In The Buttons.
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(Central_Mark[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(Central_Mark[Point_Count]&0x0F),BACKCOLOR);			

	}

	if((Status&RADIO_BUTTON_CHECKED)!=0)
	{
		Number_Count=(Status>>3)&0x07;

		for(Point_Count=0;Point_Count<sizeof(Central_Mark);Point_Count++)
			Sys_GUI_LCD_Set_Point(Coord_X+*(Relative_Centers+Number_Count*2)+(Central_Mark[Point_Count]>>4),Coord_Y+*(Relative_Centers+Number_Count*2+1)+(Central_Mark[Point_Count]&0x0F),BLACK);	
	}						
}
#endif
/*Finish Function:Sys_GUI_Draw_Radio_Button_Group*****************************/

/*Begin Function:Sys_GUI_Draw_Tab_Group****************************************
Description : Draw Relative Tabs According To The Arguments.Supports 8 Tabs At The Same Time.
Input       : u16 Coord_X -The X Coordination Of The Reference Point.
              u16 Coord_Y -The Y Coordination Of The Reference Point.
			  u16 Header_Width -The Width Of A Unselected Tab Header.The Selected One Will Be "TAB_SELECTED_ADD_WIDTH+Header_Width".
			  u8 Number -The Number Of Tabs.
			  u16* Header_Lengths -The Length Of Each Tab Header.The Total Length Of The Tab Group Is Decided By The Sum Of This.
			  u16 Width_Of_Tabs -The Width Of Tabs.The Header Of The Tabs Is Not Included In This.(Only The Width Of The Blank Area Below.)
			  The Normal Width Of The Whole Tab Group Will Be "Header_Width+Width_Of_Tabs". 
			  u8 Status -The Status Of The Tab Group.Can Be The Combination Of The Following:
			  TAB_SHOWING_(x)                    [((x))]                              //The Tabs Being Selected.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Tab_Group(u16 Coord_X,u16 Coord_Y,u16 Header_Width,u8 Number,u16* Header_Lengths,u16 Width_Of_Tabs,u8 Status)
{
	u16 Coord_X_Count,Coord_Y_Count;
	u16 Coord_X_Offset=0;													   //The Offset value For Each Tab.
	u8 Tab_Count;

	u16 Total_Length=0;

	if(Number==0)
		return;																   //An Error.

	if(Status<Number)														   //The Selected Tab Must Exist In The System.
	{
		for(Tab_Count=0;Tab_Count<Number;Tab_Count++)						   //Calculate The Total Length Of The Tabs.
			Total_Length+=*(Header_Lengths+Tab_Count);		
		
		for(Coord_X_Count=0;Coord_X_Count<Total_Length;Coord_X_Count++)														        
		{																	   //Draw The Box Below.
			for(Coord_Y_Count=0;Coord_Y_Count<Width_Of_Tabs+Header_Width;Coord_Y_Count++)
				Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count+TAB_SELECTED_ADD_WIDTH,BACKCOLOR);             	
		}	
	
		for(Tab_Count=0;Tab_Count<Number;Tab_Count++)						   //Draw The Standard Tab Header Style First.
		{																																 
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,Coord_X+Coord_X_Offset,Coord_Y+TAB_SELECTED_ADD_WIDTH,WHITE);
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset,Coord_Y+TAB_SELECTED_ADD_WIDTH,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-1,Coord_Y+TAB_SELECTED_ADD_WIDTH,WHITE);
	
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-1,Coord_Y+TAB_SELECTED_ADD_WIDTH,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,BLACK);
	
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-2,Coord_Y+TAB_SELECTED_ADD_WIDTH+1,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-2,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,DARKER_BACKCOLOR);
			
			Coord_X_Offset+=*(Header_Lengths+Tab_Count);
		}
																			   //Draw The Tab Window Outline.
		Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+Width_Of_Tabs,Coord_X,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,WHITE);									
		Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,Coord_X+Total_Length-1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,WHITE);
		Sys_GUI_LCD_Draw_Line(Coord_X+Total_Length-1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,Coord_X+Total_Length-1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+Width_Of_Tabs,BLACK);
		Sys_GUI_LCD_Draw_Line(Coord_X+Total_Length-1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+Width_Of_Tabs,Coord_X,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+Width_Of_Tabs,BLACK);
	
		Sys_GUI_LCD_Draw_Line(Coord_X+Total_Length-2,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+1,Coord_X+Total_Length-2,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+Width_Of_Tabs-1,DARKER_BACKCOLOR);
		Sys_GUI_LCD_Draw_Line(Coord_X+Total_Length-2,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+Width_Of_Tabs-1,Coord_X+1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH+Width_Of_Tabs-1,DARKER_BACKCOLOR);
		
		Coord_X_Offset=0;													   //Reset The Offset
		for(Tab_Count=0;Tab_Count<Status;Tab_Count++)						   //Recalculate The Total Length Of The Tabs.After That,The "Tab_Count" Equals "Status".
			Coord_X_Offset+=*(Header_Lengths+Tab_Count);	

		if(Tab_Count!=Number-1)												   //The Tab Is Not The Last One.Make It Look Wider.
		{																	   //Fill The Area Of The Selected Tab.
			for(Coord_X_Count=0;Coord_X_Count<*(Header_Lengths+Tab_Count)+1;Coord_X_Count++)														       
			{
				for(Coord_Y_Count=0;Coord_Y_Count<Header_Width+TAB_SELECTED_ADD_WIDTH+1;Coord_Y_Count++)
					Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count+Coord_X_Offset,Coord_Y+Coord_Y_Count,BACKCOLOR);              	
			}		
																			   //Begin To Draw The Outline.
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,Coord_X+Coord_X_Offset,Coord_Y,WHITE);					 
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset,Coord_Y,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)+1-1,Coord_Y,WHITE);
		
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)+1-1,Coord_Y,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)+1-1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,BLACK);
		
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)+1-2,Coord_Y+1,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)+1-2,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,DARKER_BACKCOLOR);
    	}
		else
		{																	   //Fill The Area Of The Selected Tab.
			for(Coord_X_Count=0;Coord_X_Count<*(Header_Lengths+Tab_Count);Coord_X_Count++)														        
			{
				for(Coord_Y_Count=0;Coord_Y_Count<Header_Width+TAB_SELECTED_ADD_WIDTH+1;Coord_Y_Count++)
					Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count+Coord_X_Offset,Coord_Y+Coord_Y_Count,BACKCOLOR);              	
			}		
																			   //Begin To Draw The Outline.
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,Coord_X+Coord_X_Offset,Coord_Y,WHITE);					 
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset,Coord_Y,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-1,Coord_Y,WHITE);
		
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-1,Coord_Y,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-1,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,BLACK);
		
			Sys_GUI_LCD_Draw_Line(Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-2,Coord_Y+1,Coord_X+Coord_X_Offset+*(Header_Lengths+Tab_Count)-2,Coord_Y+Header_Width+TAB_SELECTED_ADD_WIDTH,DARKER_BACKCOLOR);
		};
	}
}
#endif
/*Finish Function:Sys_GUI_Draw_Tab_Group**************************************/

/*Begin Function:Sys_GUI_Draw_Mouse********************************************
Description : Draw A Mouse According To The Arguments.
Input       : u16 Coord_X -The X Coordination Of The Mouse Top-Left Corner.
              u16 Coord_Y -The Y Coordination Of The Mouse Top-Left Corner.
			  u8 Status -The Status Of The Mouse.Can Be One Of The Following:
			  MOUSE_NORMAL       	  	      [0x00]								 //The Normal Mouse.
			  MOUSE_BUSY                      [0x01]								 //The Busy Mouse.
			  MOUSE_QUESTION	              [0x02]								 //The Mouse With A Question Mark.

			  MOUSE_PRESS_HAND                [0x03]								 //The Hand-Like Mouse For Hyperlinks.
   			  MOUSE_ENTER_TEXT                [0x04]								 //The Text Modifying Cursor.
			  MOUSE_STOP                      [0x05]								 //The "Not Available" Cursor.

			  MOUSE_MOVE                      [0x06] 								 //The Mouse For Moving The Window.
			  MOUSE_ADJ_LENGTH    	          [0x07]								 //The Mouse For Adjusting The Length.
			  MOUSE_ADJ_WIDTH		          [0x08]								 //The Mouse For Adjusting The Width.
			  MOUSE_ADJ_TOP_LEFT   	          [0x09]								 //The Mouse For Adjusting The Top-Left To Bottom-Right Line.
			  MOUSE_ADJ_TOP_RIGHT             [0x0A]								 //The Mouse For Adjusting The Bottom-Left To Top-Right Line.

	    	  MOUSE_ACCURATE_POSITION         [0x0B]								 //The Mouse For Accurate Locating An Object.

			  MOUSE_INVISIBLE                 [0xFF]								 //A Invisible Mouse.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Mouse(u16 Coord_X,u16 Coord_Y,u8 Status)
{
	u8 Point_Count;

	//Normal Mouse
	const u8 MOUSE_NORMAL_Dot_BLACK[]=										   //The Column Is The First Argument Here"(Y,X)".
	{
		0x00,0x10,0x11,0x20,0x22,0x30,0x33,0x40,0x44,0x50,0x55,0x60,0x66,0x70,0x77,0x80,0x88,0x90,0x95,0x96,0x97,
		0x98,0x99,0xA0,0xA2,0xA3,0xA5,0xB0,0xB1,0xB3,0xB6,0xC0,0xC1,0xC4,0xC6,0xD0,0xD4,0xD7,0xE5,0xE7,0xF5,0xF6
	};
	const u8 MOUSE_NORMAL_Dot_WHITE[]=
	{
		0x21,0x31,0x32,0x41,0x42,0x43,0x51,0x52,0x53,0x54,0x61,0x62,0x63,0x64,0x65,0x71,0x72,0x73,0x74,0x75,
		0x76,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x91,0x92,0x93,0x94,0xA1,0xA4,0xB4,0xB5,0xC5,0xD5,0xD6,0xE6
	};
	//End Normal Mouse

	//Additional "Busy" Mark
	const u8 BUSY_Add_Dot_BLACK[]=
	{
		0x0A,0x0B,0x0C,0x0D,0x0E,0x1A,0x1B,0x1D,0x1E,0x2A,0x2C,0x2E,0x3B,0x3D,0x4C,0x5B,0x5D,0x6A,0x6E,0x7A,0x7C,0x7E,0x8A,0x8B,0x8C,0x8D,0x8E
	};
	const u8 BUSY_Add_Dot_WHITE[]=
	{
		0x1C,0x2B,0x2D,0x3C,0x5C,0x6B,0x6C,0x6D,0x7B,0x7D
	};
	//End Additional "Busy" Mark

	//Additional "Question" Mark
	const u8 QUESTION_Add_Dot_BLACK[]=								           //It Has No White Part.
	{
		0x0B,0x0C,0x0D,0x1A,0x1B,0x1C,0x1D,0x1E,0x29,0x2A,0x2B,0x2D,0x2E,0x2F,0x39,0x3A,0x3E,0x3F,0x4A,0x4D,0x4E,0x4F,0x5C,0x5D,0x5E,0x6C,0x6D,0x8D,0x8E,0x9D,0x9E
	};
	//End Additional "Question" Mark

	//Hand-Shaped Mouse
	const u8 MOUSE_PRESS_HAND_Dot_BLACK[]=
	{
		0x05,0x06,0x14,0x17,0x24,0x27,0x34,0x37,0x39,0x44,0x47,0x48,0x4A,0x4C,0x51,0x52,0x54,0x57,0x5A,0x5B,0x5D,0x5E,
		0x60,0x63,0x64,0x67,0x6A,0x6D,0x6F,0x70,0x74,0x77,0x7A,0x7D,0x7F,0x80,0x8F,0x90,0x9F,0xA0,0xAF,0xB0,0xBF,0xC1,0xCE,
		0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xE1,0xEE,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD
	};
	const u8 MOUSE_PRESS_HAND_Dot_WHITE[]=
	{
		0x15,0x16,0x25,0x26,0x35,0x36,0x45,0x46,0x49,0x55,0x56,0x58,0x59,0x5C,0x61,0x62,0x65,0x66,0x68,0x69,0x6B,0x6C,0x6E,0x71,0x72,0x73,0x75,0x76,0x78,0x79,0x7B,0x7C,0x7E,
		0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,
		0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED
	};
	//End Hand-Shaped Mouse

	//The Mouse For Text Editing
	const u8 MOUSE_ENTER_TEXT_Dot[]=										   //It Has No White Part.
	{
		0x00,0x01,0x02,0x04,0x05,0x06,0x13,0x23,0x33,0x43,0x53,0x63,0x73,0x83,0x93,0xA3,0xB3,0xC3,0xD3,0xE3,0xF0,0xF1,0xF2,0xF4,0xF5,0xF6
	};
	//End The Mouse For Text Editing

	//The "Forbidden" Or "Invalid" Mouse
	const u8 MOUSE_STOP_Dot_BLACK[]=
	{
		0x05,0x06,0x07,0x08,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x22,0x23,0x24,0x29,0x2A,0x2B,0x31,0x32,0x33,0x34,0x3A,0x3B,0x3C,0x41,0x42,0x43,0x44,0x45,0x4B,0x4C,
		0x50,0x51,0x54,0x55,0x56,0x5C,0x5D,0x60,0x61,0x65,0x66,0x67,0x6C,0x6D,0x70,0x71,0x76,0x77,0x78,0x7C,0x7D,0x80,0x81,0x87,0x88,0x89,0x8C,0x8D,0x91,0x92,0x98,0x99,0x9A,0x9B,0x9C,
		0xA1,0xA2,0xA3,0xA9,0xAA,0xAB,0xAC,0xB2,0xB3,0xB4,0xB9,0xBA,0xBB,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD5,0xD6,0xD7,0xD8	
	};
	const u8 MOUSE_STOP_Dot_WHITE[]=
	{
		0x25,0x26,0x27,0x28,0x35,0x36,0x37,0x38,0x39,0x46,0x47,0x48,0x49,0x4A,0x52,0x53,0x57,0x58,0x59,0x5A,0x5B,0x62,0x63,0x64,0x68,0x69,0x6A,0x6B,
		0x72,0x73,0x74,0x75,0x79,0x7A,0x7B,0x82,0x83,0x84,0x85,0x86,0x8A,0x8B,0x93,0x94,0x95,0x96,0x97,0xA4,0xA5,0xA6,0xA7,0xA8,0xB5,0xB6,0xB7,0xB8
	};
	//End The "Forbidden" Or "Invalid" Mouse

   	//The Mouse For Moving Windows.
	const u8 MOUSE_MOVE_Dot[]=
	{
		0x07,0x16,0x17,0x18,0x25,0x26,0x27,0x28,0x29,0x37,0x47,0x52,0x57,0x5C,0x61,0x62,0x67,0x6C,0x6D,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,
		0x81,0x82,0x87,0x8C,0x8D,0x92,0x97,0x9C,0xA7,0xB7,0xC5,0xC6,0xC7,0xC8,0xC9,0xD6,0xD7,0xD8,0xE7
	};	
	//End The Mouse For Moving Windows.

	//The Length Adjusting Mouse.
	const u8 MOUSE_ADJ_LENGTH_Dot[]=
	{																		   //It Has No White Part.
		0x52,0x5D,0x61,0x62,0x6D,0x6E,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,0x81,0x82,0x8D,0x8E,0x92,0x9D
	};
	//End The Length Adjusting Mouse.

	//The Width Adjusting Mouse.
	const u8 MOUSE_ADJ_WIDTH_Dot[]=
	{																		   //It Has No White Part.
		0x07,0x16,0x17,0x18,0x25,0x26,0x27,0x28,0x29,0x37,0x47,0x57,0x67,0x77,0x87,0x97,0xA7,0xB7,0xC7,0xD5,0xD6,0xD7,0xD8,0xD9,0xE6,0xE7,0xE8,0xF7
	};
	//End The Width Adjusting Mouse.

	//The Mouse For Adjusting The Top-Left To Bottom-Right Line.
	const u8 MOUSE_ADJ_TOP_LEFT_Dot[]=
	{																		   //It Has No White Part.
		0x00,0x01,0x02,0x03,0x04,0x10,0x11,0x12,0x13,0x20,0x21,0x22,0x30,0x31,0x33,0x40,0x44,0x55,0x66,0x77,
		0x88,0x99,0xAA,0xBB,0xBF,0xCC,0xCE,0xCF,0xDD,0xDE,0xDF,0xEC,0xED,0xEE,0xEF,0xFB,0xFC,0xFD,0xFE,0xFF
	};
	//End The Mouse For Adjusting The Top-Left To Bottom-Right Line.

	//The Mouse For Adjusting The Bottom-Left To Top-Right Line.
	const u8 MOUSE_ADJ_TOP_RIGHT_Dot[]=
	{																	       //It Has No White Part.
		0x0B,0x0C,0x0D,0x0E,0x0F,0x1C,0x1D,0x1E,0x1F,0x2D,0x2E,0x2F,0x3C,0x3E,0x3F,0x4B,0x4F,0x5A,0x69,0x78,
		0x87,0x96,0xA5,0xB0,0xB4,0xC0,0xC1,0xC3,0xD0,0xD1,0xD2,0xE0,0xE1,0xE2,0xE3,0xF0,0xF1,0xF2,0xF3,0xF4
	};
	//End The Mouse For Adjusting The Bottom-Left To Top-Right Line.

   	//The Mouse For Accurate Locating An Object.
	const u8 MOUSE_ACCURATE_POSITION_Dot[]=									   //It Has No White Part.
	{
		0x07,0x17,0x27,0x37,0x46,0x47,0x48,0x55,0x57,0x59,0x64,0x6A,
		0x70,0x71,0x72,0x73,0x74,0x75,0x77,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,
		0x84,0x8A,0x95,0x97,0x99,0xA6,0xA7,0xA8,0xB7,0xC7,0xD7,0xE7
	};
	//End The Mouse For Accurate Locating An Object.

	switch(Status)
	{
	 	case MOUSE_NORMAL:                                                     //The Normal Mouse.   
		{																	   //The Black Outline Part.
		 	for(Point_Count=0;Point_Count<sizeof(MOUSE_NORMAL_Dot_BLACK);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_NORMAL_Dot_BLACK[Point_Count]&0x0F),Coord_Y+(MOUSE_NORMAL_Dot_BLACK[Point_Count]>>4),BLACK);
																			   //The White Central Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_NORMAL_Dot_WHITE);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_NORMAL_Dot_WHITE[Point_Count]&0x0F),Coord_Y+(MOUSE_NORMAL_Dot_WHITE[Point_Count]>>4),WHITE);
			
			break;	
		}
								 
		case MOUSE_BUSY:														 //The Busy Mouse.																  
		{																		 //The Black Outline Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_NORMAL_Dot_BLACK);Point_Count++)							                                  
					Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_NORMAL_Dot_BLACK[Point_Count]&0x0F),Coord_Y+(MOUSE_NORMAL_Dot_BLACK[Point_Count]>>4),BLACK);
																				 //The White Central Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_NORMAL_Dot_WHITE);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_NORMAL_Dot_WHITE[Point_Count]&0x0F),Coord_Y+(MOUSE_NORMAL_Dot_WHITE[Point_Count]>>4),WHITE);
																				 //The Black Outline Part.
			for(Point_Count=0;Point_Count<sizeof(BUSY_Add_Dot_BLACK);Point_Count++)							                                  
					Sys_GUI_LCD_Set_Point(Coord_X+(BUSY_Add_Dot_BLACK[Point_Count]&0x0F),Coord_Y+(BUSY_Add_Dot_BLACK[Point_Count]>>4),BLACK);
																				 //The White Central Part.
			for(Point_Count=0;Point_Count<sizeof(BUSY_Add_Dot_WHITE);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(BUSY_Add_Dot_WHITE[Point_Count]&0x0F),Coord_Y+(BUSY_Add_Dot_WHITE[Point_Count]>>4),WHITE);

			break;
		}    
		         	         
		case MOUSE_QUESTION:													 //The Mouse With A Question Mark.															   
		{																		 //The Black Outline Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_NORMAL_Dot_BLACK);Point_Count++)							                                 
					Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_NORMAL_Dot_BLACK[Point_Count]&0x0F),Coord_Y+(MOUSE_NORMAL_Dot_BLACK[Point_Count]>>4),BLACK);
																				 //The White Central Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_NORMAL_Dot_WHITE);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_NORMAL_Dot_WHITE[Point_Count]&0x0F),Coord_Y+(MOUSE_NORMAL_Dot_WHITE[Point_Count]>>4),WHITE);
																				 //The Black Outline Part.
			for(Point_Count=0;Point_Count<sizeof(QUESTION_Add_Dot_BLACK);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(QUESTION_Add_Dot_BLACK[Point_Count]&0x0F),Coord_Y+(QUESTION_Add_Dot_BLACK[Point_Count]>>4),BLACK);
			
			break;  
		}	       				 
																			   //The Hand-Like Mouse For Hyperlinks.															      
		case MOUSE_PRESS_HAND:
		{																	   //The Black Outline Part.
		 	for(Point_Count=0;Point_Count<sizeof(MOUSE_PRESS_HAND_Dot_BLACK);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_PRESS_HAND_Dot_BLACK[Point_Count]&0x0F),Coord_Y+(MOUSE_PRESS_HAND_Dot_BLACK[Point_Count]>>4),BLACK);
																			   //The White Central Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_PRESS_HAND_Dot_WHITE);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_PRESS_HAND_Dot_WHITE[Point_Count]&0x0F),Coord_Y+(MOUSE_PRESS_HAND_Dot_WHITE[Point_Count]>>4),WHITE);
			
			break;	
		}          			     
																																					  
		case MOUSE_ENTER_TEXT:												   //The Text Modifying Cursor.																	  
		{																	   //The Black Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_ENTER_TEXT_Dot);Point_Count++)							                                      
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_ENTER_TEXT_Dot[Point_Count]&0x0F),Coord_Y+(MOUSE_ENTER_TEXT_Dot[Point_Count]>>4),BLACK);
			
			break;		
		}         		         

		case MOUSE_STOP:													   //The "Not Available" Cursor.																	  
		{																	   //The Black Outline Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_STOP_Dot_BLACK);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_STOP_Dot_BLACK[Point_Count]&0x0F),Coord_Y+(MOUSE_STOP_Dot_BLACK[Point_Count]>>4),BLACK);
																			   //The White Central Part.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_STOP_Dot_WHITE);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_STOP_Dot_WHITE[Point_Count]&0x0F),Coord_Y+(MOUSE_STOP_Dot_WHITE[Point_Count]>>4),WHITE);
			
			break;		
		}   					 

		case MOUSE_MOVE:													   //The Mouse For Moving The Window.																 
		{																																		 
			for(Point_Count=0;Point_Count<sizeof(MOUSE_MOVE_Dot);Point_Count++)//The Black Part.It Has No White Area.							                                         
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_MOVE_Dot[Point_Count]&0x0F),Coord_Y+(MOUSE_MOVE_Dot[Point_Count]>>4),BLACK);
			
			break;		
		}				 	     

		case MOUSE_ADJ_LENGTH:												   //The Mouse For Adjusting The Length. 																 
		{																	   //The Black Part.It Has No White Area.																 
			for(Point_Count=0;Point_Count<sizeof(MOUSE_ADJ_LENGTH_Dot);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_ADJ_LENGTH_Dot[Point_Count]&0x0F),Coord_Y+(MOUSE_ADJ_LENGTH_Dot[Point_Count]>>4),BLACK);
			
			break;	
		}    			         

		case MOUSE_ADJ_WIDTH:												   //The Mouse For Adjusting The Width.
		{																	   //The Black Part.It Has No White Area.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_ADJ_WIDTH_Dot);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_ADJ_WIDTH_Dot[Point_Count]&0x0F),Coord_Y+(MOUSE_ADJ_WIDTH_Dot[Point_Count]>>4),BLACK);
			
			break;	
		}						 
																																				  
		case MOUSE_ADJ_TOP_LEFT:											   //The Mouse For Adjusting The Top-Left To Bottom-Right Line.
		{																	   //The Black Part.It Has No White Area.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_ADJ_TOP_LEFT_Dot);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_ADJ_TOP_LEFT_Dot[Point_Count]&0x0F),Coord_Y+(MOUSE_ADJ_TOP_LEFT_Dot[Point_Count]>>4),BLACK);
			
			break;		
		}   					 

		case MOUSE_ADJ_TOP_RIGHT:											   //The Mouse For Adjusting The Bottom-Left To Top-Right Line.																   
		{																	   //The Black Part.It Has No White Area.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_ADJ_TOP_RIGHT_Dot);Point_Count++)							                                  
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_ADJ_TOP_RIGHT_Dot[Point_Count]&0x0F),Coord_Y+(MOUSE_ADJ_TOP_RIGHT_Dot[Point_Count]>>4),BLACK);
			
			break;
		}    					 

		case MOUSE_ACCURATE_POSITION:										   //The Mouse For Accurate Locating An Object.
		{																	   //The Black Part.It Has No White Area.
			for(Point_Count=0;Point_Count<sizeof(MOUSE_ACCURATE_POSITION_Dot);Point_Count++)							                            
				Sys_GUI_LCD_Set_Point(Coord_X+(MOUSE_ACCURATE_POSITION_Dot[Point_Count]&0x0F),Coord_Y+(MOUSE_ACCURATE_POSITION_Dot[Point_Count]>>4),BLACK);
			
			break;
		} 	                     
																																					
		case MOUSE_INVISIBLE:												   //A Invisible Mouse.
		{
			break;
		}      					 

		default:
		{
			break;
		}
	}
	
}
#endif
/*Finish Function:Sys_GUI_Draw_Mouse******************************************/

/*Begin Function:Sys_GUI_Draw_Selected_Bar*************************************
Description : Draw A Selected Text Bar According To The Arguments.
Input       : u16 Coord_X -The X Coordination Of The Bar Top-Left Corner.
              u16 Coord_Y -The Y Coordination Of The Bar Top-Left Corner.
			  u16 Length -The Length Of The Bar.
			  u16 Width -The Width Of The Bar.
			  u8 Status -The Status Of The Mouse.Can Be The Combination Of The Following:  
			  BAR_SELECTED       	  	      [0x80]								 //The Selected Bar.
			  BAR_UNSELECTED				  [0x00]								 //The Unselected Bar.
			  BAR_WITH_DOTTED_LINE            [0x40]								 //The Busy Mouse.
Output      : Void.
*****************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Selected_Bar(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status)
{
	u16 Coord_X_Count,Coord_Y_Count;

	if((Status&BAR_SELECTED)!=0)											   //The Bar Is Selected.
	{
		for(Coord_X_Count=1;Coord_X_Count<Length-1;Coord_X_Count++)			   //The Outline Will Be Drawn Below.
			for(Coord_Y_Count=1;Coord_Y_Count<Width-1;Coord_Y_Count++)
				Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,BARCOLOR);		

		if((Status&BAR_WITH_DOTTED_LINE)!=0)
		{
			Sys_GUI_LCD_Draw_Dotted_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,BLACK);
			Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,BLACK);
			Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,BLACK);
			Sys_GUI_LCD_Draw_Dotted_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,BLACK);		
		}
		else
		{
		 	Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,BARCOLOR);
			Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,BARCOLOR);
			Sys_GUI_LCD_Draw_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,BARCOLOR);
			Sys_GUI_LCD_Draw_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,BARCOLOR);
		}		
	}

	if((Status&BAR_WITH_DOTTED_LINE)!=0)
	{
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X,Coord_Y,Coord_X+Length-1,Coord_Y,BLACK);
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+Length-1,Coord_Y,Coord_X+Length-1,Coord_Y+Width-1,BLACK);
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X+Length-1,Coord_Y+Width-1,Coord_X,Coord_Y+Width-1,BLACK);
		Sys_GUI_LCD_Draw_Dotted_Line(Coord_X,Coord_Y+Width-1,Coord_X,Coord_Y,BLACK);		
	}		
}
#endif
/*Finish Function:Sys_GUI_Draw_Selected_Bar***********************************/

/*Begin Function:Sys_GUI_Draw_Progress_Bar*************************************
Description : Draw A Progress Bar According To The Arguments.
Input       : u16 Coord_X -The X Coordination Of The Bar Top-Left Corner.
              u16 Coord_Y -The Y Coordination Of The Bar Top-Left Corner.
			  u16 Length -The Length Of The Bar.
			  u16 Width -The Width Of The Bar.
			  u16 Front_Beginning_Color -The Front Color Of The Bar,At The Beginning Of The Progress.
			  u16 Front_End_Color -The Front Color Of The Bar,At The End Of The Progress.
			  u16 Back_Color -The Background Color Of The Bar.
			  (0%)|-----Front Color------|(Progress%)-----------Back Color------------|(100%)
			  double Progress -The Progress Of The Bar.0 Is Empty,1 Is Full.
Output      : Void.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
void Sys_GUI_Draw_Progress_Bar(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u16 Front_Beginning_Color,u16 Front_End_Color,u16 Back_Color,double Progress)
{
	u16 Coord_X_Count,Coord_Y_Count;
	u16 Front_Color_Mix;													   //The Mixed Front Color.

	if(Front_Beginning_Color==Front_End_Color)								   //The Two Color Are Identical,So We Don't Use The Color Mixer.
	{
		for(Coord_X_Count=0;Coord_X_Count<Length;Coord_X_Count++)
		{
			if((double)Coord_X_Count<Progress*(double)Length)
			{
				for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
					Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,Front_Beginning_Color);	
			}
			else
			{
				for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
					Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,Back_Color);
			}
		}		
	}
	else																	   //Use The Color Mixer Now.
	{																		   //Mix It First.
		Front_Color_Mix=Sys_GUI_Color_Mixer(Front_Beginning_Color,Front_End_Color,Progress);						 

		for(Coord_X_Count=0;Coord_X_Count<Length;Coord_X_Count++)
		{
			if((double)Coord_X_Count<Progress*(double)Length)
			{
				for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
					Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,Front_Color_Mix);	
			}
			else
			{
				for(Coord_Y_Count=0;Coord_Y_Count<Width;Coord_Y_Count++)
					Sys_GUI_LCD_Set_Point(Coord_X+Coord_X_Count,Coord_Y+Coord_Y_Count,Back_Color);
			}
		}		
	}
}
#endif
/*Finish Function:Sys_GUI_Draw_Progress_Bar***********************************/

/*Begin Function:Sys_GUI_Create_Dialog_Box*************************************
Description : Create a Dialog Box According To The Arguments
			  The Function Just Creates The Dialog Box.It Doesn't Display It Right Away.
			  The Created Dialog Box Will Be Empty After Creation.
Input       : u16 Coord_X -The X Coordination Of The Dialog Box.The Top-Left Corner.
              u16 Coord_Y -The Y Coordination Of The Dialog Box.The Top-Left Corner.
			  u16 Length -The Length Of It.
			  u16 Width -The Width Of It.
			  u8 Status -The Initial Status Of The Dialog Box.Can Be The Combination Of The Following:
			  DIALOG_EXIST                      [0x80]								 //The Window Exists.
			  DIALOG_ACTIVE                     [0x40]								 //The Window Is Active Now.
			  DIALOG_MOVEABLE                   [0x20]								 //The Window Can Be Moved.
	 		  DIALOG_ENABLED                    [0x10]								 //The Operations To The Window Is Enabled.
			  void* Title -The Title Of The Box.Can't Be Longer Than The Macro "MAX_LEN_OF_DIALOG_TITLE". It Must End With A "\0"
			  void* Icon -The Icon For The Dialog Box(Must Be In 10*10 Size.) 
Output      : u8 -If Successful,It Will Return The Dialog Box Number;Else It Will Return "ENODIA",(0xFF).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Create_Dialog_Box(u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status,void* Title,void* Icon)
{
	u8 DCB_Count;

	for(DCB_Count=0;DCB_Count<MAX_NUMBER_OF_DIALOG;DCB_Count++)
	{
		if(((DCB[DCB_Count].Dialog_Status)&DIALOG_EXIST)==0)		
			break;                                                             //Now The "DCB_Count" Contains The Empty One.
	}

	if(DCB_Count==MAX_NUMBER_OF_DIALOG)
	{
		 												  					   //There Is No Vacancy Available.
		return(ENODIA);
	}

	if(Status&DIALOG_EXIST==0x00)
	{
		 											  						   //The Initial Status Is Not Correct(The "DIALOG_EXIST" Is Not Set.).
		return(ENODIA);	
	}

	Sys_Strcpy(Title,DCB[DCB_Count].Dialog_Title,MAX_LEN_OF_DIALOG_TITLE); 	   //Fill The Title.

	DCB[DCB_Count].Dialog_ID=DCB_Count;									   	   //This Is For Use In The List.

	DCB[DCB_Count].Dialog_Status=Status;
	DCB[DCB_Count].Dialog_Icon=Icon;
	DCB[DCB_Count].Dialog_Coord_X=Coord_X;								   	   //Fill The Correspoding Table In DCB.
	DCB[DCB_Count].Dialog_Coord_Y=Coord_Y;
	DCB[DCB_Count].Dialog_Length=Length;
	DCB[DCB_Count].Dialog_Width=Width;
	DCB[DCB_Count].Dialog_Number_Of_Controls=0;							       //We Haven't Registered Any Controls Yet.
	
	if(Dialog_List_Exist==0)
	{
		Sys_Create_List(&Dialog_List_Head);						   			   //The List Does Not Exist-Create The List Now.
		Dialog_Current_Ptr=&Dialog_List_Head;
		Sys_List_Insert_Node(&(DCB[DCB_Count].List),Dialog_Current_Ptr->Prev,Dialog_Current_Ptr);		
		Dialog_List_Exist=1;					  		
	}
	else
	{
		Dialog_Current_Ptr=Sys_List_Seek(&Dialog_List_Head,1);				   //Go To The First Valid Node.After The List Is Cleaned Up,If Only The Head Remains,It Will Work Properly.
		Sys_List_Insert_Node(&(DCB[DCB_Count].List),Dialog_Current_Ptr->Prev,Dialog_Current_Ptr);	
	}																		   //Insert The Node To The First One.
		
	return(DCB_Count);	
}
#endif
/*Finish Function:Sys_GUI_Create_Dialog_Box***********************************/

/*Begin Function:Sys_GUI_Delete_Dialog_Box*************************************
Description : Delete a Dialog Box According To The Arguments
			  The Function Just Deletes The Dialog Box.It Doesn't Vanish Right Away.
Input       : u8 Dialog_ID -The ID Of The Dialog Box. 
Output      : u8 -If Successful,It Will Return 0;Else It Will Return "ENODIA"(0xFF).
******************************************************************************/
#if(ENABLE_GUI==TRUE) 
u8 Sys_GUI_Delete_Dialog_Box(u8 Dialog_ID)
{	
	 

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{	 												 					   //The Dialog Box Does Not Exist.
		return(ENODIA);
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)
	{											 							   //The Dialog Box Does Not Exist.
		return(ENODIA);	
	}
	
	Sys_Memset(DCB[Dialog_ID].Dialog_Title,0,MAX_LEN_OF_DIALOG_TITLE);	       //Clear The Title First.

	DCB[Dialog_ID].Dialog_ID=0;									               //This Is For Use In The List. Clear It As A Habit.

	DCB[Dialog_ID].Dialog_Status=0;
	DCB[Dialog_ID].Dialog_Icon=0;
	DCB[Dialog_ID].Dialog_Coord_X=0;								           //Clear The Correspoding Table In DCB.
	DCB[Dialog_ID].Dialog_Coord_Y=0;
	DCB[Dialog_ID].Dialog_Length=0;
	DCB[Dialog_ID].Dialog_Width=0;
	DCB[Dialog_ID].Dialog_Number_Of_Controls=0;							       //Clear The Control Count.

	Sys_Memset((u8*)(DCB[Dialog_ID].CCB),0,sizeof(DCB[Dialog_ID].CCB));

	Sys_List_Delete_Node(DCB[Dialog_ID].List.Prev,DCB[Dialog_ID].List.Next);   //Delete The Node In The Dialog Box List.

	DCB[Dialog_ID].List.Prev=0;											       //Clear The Pointer Information At Last.
	DCB[Dialog_ID].List.Next=0;

	return(0);
}
#endif
/*Finish Function:Sys_GUI_Delete_Dialog_Box***********************************/

/*Begin Function:_Sys_GUI_Draw_Tab_Controls************************************
Description : Draw The Tab Controls According To The Tab_CCB In The Tab's Property Struct.
Input       : struct Dialog_Control_Head* Control -The Control Header Of The Tab's Property Struct.
Output      : u8 -If Successful,Return 1;Else "ENODOP"(0x00,When The Operation Can't Be Done),Or "ENODIA"
              (0xFF,When The Dialog Box Doesn't Exist At All).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 _Sys_GUI_Draw_Tab_Controls(struct Dialog_Control_Head* Control)
{
	u8 Control_Count;
	struct Tab_Property* Tab=(struct Tab_Property*)(Control);
	 

	if(Control->Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{												                           //The Dialog ID Does Not Exist At All.
		return(ENODIA);	
	}

	if((DCB[Control->Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)				   //The Dialog Box Does Not Exist At All.
	{
		 
		return(ENODIA);
	}

	if(DCB[Control->Dialog_ID].Dialog_Number_Of_Controls==0)				   //There Are Already No Controls.
	{
		 
		return(ENODIA);
	}

	if((Control->Category)!=TAB)											   //The Head Is Not From A Tab's Struct.
		return(ENODOP);

	for(Control_Count=0;Control_Count<(Tab->Number_Of_Controls_In_Tabs[Tab->Status]);Control_Count++)
	{
		if((Tab->Tab_CCB[Tab->Status][Control_Count])->Category==TAB)		   //We Don't Allow Secondary Tabs in The OS.
			continue;
		_Sys_GUI_Draw_Any_Control_For_Tabs((DCB[Control->Dialog_ID].Dialog_Coord_X)+(Tab->Relative_Coord_X),(DCB[Control->Dialog_ID].Dialog_Coord_Y)+(Tab->Relative_Coord_Y),Tab->Tab_CCB[Tab->Status][Control_Count]);
	}	
	 
	return(1);
}
#endif
/*Finish Function:_Sys_GUI_Draw_Tab_Controls**********************************/

/*Begin Function:_Sys_GUI_Draw_Any_Control*************************************
Description : Draw Any Control According To The Arguments. It Is Used To Draw Dialog Controls And Tab Controls.
Input       : u16 Coord_X -The X Coordination Of The Reference Point.
			  u16 Coord_Y -The Y Coordination Of The Reference Point.
			  struct Dialog_Control_Head* Control -The Pointer To The Head Of The Struct.
Output      : u8 -If Successful,Return 1;Else "ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 _Sys_GUI_Draw_Any_Control(u16 Coord_X,u16 Coord_Y,struct Dialog_Control_Head* Control)
{
	u8 Radio_Count;
	u8 Tab_Count;
	u16 Tab_Total_Length;
	union Control_Pointer Control_Pointer_Union;
																			   //No Need To Disable INTS Here.
	switch(*((u8*)(Control)))
	{
		case COMMAND_BUTTON:			          						       //A Command Button.
		{	
			Control_Pointer_Union.Command_Button=(struct Command_Button_Property*)(Control);			
			Sys_GUI_Draw_Command_Button((Coord_X)+(Control_Pointer_Union.Command_Button->Relative_Coord_X),
			                                    (Coord_Y)+(Control_Pointer_Union.Command_Button->Relative_Coord_Y),
												Control_Pointer_Union.Command_Button->Length,
												Control_Pointer_Union.Command_Button->Width,
												Control_Pointer_Union.Command_Button->Status);
													
			Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Command_Button->Relative_Coord_X+5),
			                         (Coord_Y)+(Control_Pointer_Union.Command_Button->Relative_Coord_Y+5),
									 Control_Pointer_Union.Command_Button->Head.Name,DIALOG_CONTROL_FONT,
									 Sys_Strlen(Control_Pointer_Union.Command_Button->Head.Name),
									 BLACK,TRANSPARENT);
		
			break;
		}
		case RADIO_BUTTON:			          								   //A Radio Button.
		{
			Control_Pointer_Union.Radio_Button=(struct Radio_Button_Property*)(Control);	
				
			Sys_GUI_Draw_Radio_Button_Group((Coord_X)+(Control_Pointer_Union.Radio_Button->Relative_Coord_X),
			                                 (Coord_Y)+(Control_Pointer_Union.Radio_Button->Relative_Coord_Y),
											 Control_Pointer_Union.Radio_Button->Number,
											 Control_Pointer_Union.Radio_Button->Relative_Centers,
											 Control_Pointer_Union.Radio_Button->Status);
											 			
			for(Radio_Count=0;Radio_Count<(Control_Pointer_Union.Radio_Button->Number);Radio_Count++)
			{
				Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Radio_Button->Relative_Coord_X)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*(Radio_Count))+14,
				                         (Coord_Y)+(Control_Pointer_Union.Radio_Button->Relative_Coord_Y)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*(Radio_Count)+1),
										 Control_Pointer_Union.Radio_Button->Radio_Names[Radio_Count],DIALOG_CONTROL_FONT,
										 Sys_Strlen(Control_Pointer_Union.Radio_Button->Radio_Names[Radio_Count]),BLACK,TRANSPARENT);
			}

			break;
		}
		case TOGGLE:					          							   //A Toggle Button.
		{
			break;
		}
		case CHECK_BOX:				          								   //A Checkbox.
		{
			Control_Pointer_Union.Check_Box=(struct Check_Box_Property*)(Control); 
			
			Sys_GUI_Draw_Check_Box((Coord_X)+(Control_Pointer_Union.Check_Box->Relative_Coord_X),
			                        (Coord_Y)+(Control_Pointer_Union.Check_Box->Relative_Coord_Y),
									Control_Pointer_Union.Check_Box->Status);	

			Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Radio_Button->Relative_Coord_X+14),
				                     (Coord_Y)+(Control_Pointer_Union.Radio_Button->Relative_Coord_Y),
									 Control_Pointer_Union.Radio_Button->Head.Name,DIALOG_CONTROL_FONT,
									 Sys_Strlen(Control_Pointer_Union.Radio_Button->Head.Name),BLACK,TRANSPARENT);
		}
		case TAB:						      								   //A Tab.
		{
 			Control_Pointer_Union.Tab=(struct Tab_Property*)(Control);

			Sys_GUI_Draw_Tab_Group((Coord_X)+(Control_Pointer_Union.Tab->Relative_Coord_X),
			                        (Coord_Y)+(Control_Pointer_Union.Tab->Relative_Coord_Y),
									Control_Pointer_Union.Tab->Header_Width,Control_Pointer_Union.Tab->Number,Control_Pointer_Union.Tab->Header_Lengths,
									Control_Pointer_Union.Tab->Width_Of_Tabs,Control_Pointer_Union.Tab->Status);

			Tab_Total_Length=0;
			for(Tab_Count=0;Tab_Count<(Control_Pointer_Union.Tab->Number);Tab_Count++)
			{
				Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Tab->Relative_Coord_X)+Tab_Total_Length+4,
				                         (Coord_Y)+(Control_Pointer_Union.Tab->Relative_Coord_Y)+3,
										 Control_Pointer_Union.Tab->Header_Names[Tab_Count],DIALOG_CONTROL_FONT,
										 Sys_Strlen(Control_Pointer_Union.Tab->Header_Names[Tab_Count]),BLACK,TRANSPARENT);
																			   //Add It Up.
				Tab_Total_Length+=Control_Pointer_Union.Tab->Header_Lengths[Tab_Count];					
			}

			_Sys_GUI_Draw_Tab_Controls(Control);

			break;
		}
		case SCROLL_LIST:				      								   //A Scroll List.
		{
			break;
		}
		case DROPDOWN_MENU:			          								   //A DropDown Menu.
		{
			break;
		}
		case SPIN_BOX:				          								   //A Spin Box.
		{
			break;
		}
		case PROGRESS_BAR:                                                     //A Progress Bar.	
		{
			break;	
		}

		default:break;	
	}

	return(1);																   //Always Succeed Now.
}
#endif						
/*Finish Function:_Sys_GUI_Draw_Any_Control***********************************/

/*Begin Function:_Sys_GUI_Draw_Any_Control_For_Tabs****************************
Description : Draw Any Control In A Tab According To The Arguments. It Is Only Used To Draw Tab Controls.
              We Don't Use "_Sys_GUI_Draw_Any_Control" For Reentrant Problems.
			  The Function Is The Same As "_Sys_GUI_Draw_Any_Control" Except That It Can't Draw Tabs.
Input       : u16 Coord_X -The X Coordination Of The Reference Point.
			  u16 Coord_Y -The Y Coordination Of The Reference Point.
.			  struct Dialog_Control_Head* Control -The Pointer To The Head Of The Struct.
Output      : u8 -If Successful,Return 1;Else "ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 _Sys_GUI_Draw_Any_Control_For_Tabs(u16 Coord_X,u16 Coord_Y,struct Dialog_Control_Head* Control)
{
	u8 Radio_Count;
//	u8 Tab_Count;															   //Do Not Support Secondary Tabs.
//	u16 Tab_Total_Length;
	union Control_Pointer Control_Pointer_Union;
																			   //No Need To Disable INTS Here.
	switch(*((u8*)(Control)))
	{
		case COMMAND_BUTTON:			          						       //A Command Button.
		{	
			Control_Pointer_Union.Command_Button=(struct Command_Button_Property*)(Control);			
			Sys_GUI_Draw_Command_Button((Coord_X)+(Control_Pointer_Union.Command_Button->Relative_Coord_X),
			                                    (Coord_Y)+(Control_Pointer_Union.Command_Button->Relative_Coord_Y),
												Control_Pointer_Union.Command_Button->Length,
												Control_Pointer_Union.Command_Button->Width,
												Control_Pointer_Union.Command_Button->Status);
													
			Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Command_Button->Relative_Coord_X+5),
			                         (Coord_Y)+(Control_Pointer_Union.Command_Button->Relative_Coord_Y+5),
									 Control_Pointer_Union.Command_Button->Head.Name,DIALOG_CONTROL_FONT,
									 Sys_Strlen(Control_Pointer_Union.Command_Button->Head.Name),
									 BLACK,TRANSPARENT);
		
			break;
		}
		case RADIO_BUTTON:			          								   //A Radio Button.
		{
			Control_Pointer_Union.Radio_Button=(struct Radio_Button_Property*)(Control);	
				
			Sys_GUI_Draw_Radio_Button_Group((Coord_X)+(Control_Pointer_Union.Radio_Button->Relative_Coord_X),
			                                 (Coord_Y)+(Control_Pointer_Union.Radio_Button->Relative_Coord_Y),
											 Control_Pointer_Union.Radio_Button->Number,
											 Control_Pointer_Union.Radio_Button->Relative_Centers,
											 Control_Pointer_Union.Radio_Button->Status);
											 			
			for(Radio_Count=0;Radio_Count<(Control_Pointer_Union.Radio_Button->Number);Radio_Count++)
			{
				Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Radio_Button->Relative_Coord_X)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*(Radio_Count))+14,
				                         (Coord_Y)+(Control_Pointer_Union.Radio_Button->Relative_Coord_Y)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*(Radio_Count)+1),
										 Control_Pointer_Union.Radio_Button->Radio_Names[Radio_Count],DIALOG_CONTROL_FONT,
										 Sys_Strlen(Control_Pointer_Union.Radio_Button->Radio_Names[Radio_Count]),BLACK,TRANSPARENT);
			}

			break;
		}
		case TOGGLE:					          							   //A Toggle Button.
		{
			break;
		}
		case CHECK_BOX:				          								   //A Checkbox.
		{
			Control_Pointer_Union.Check_Box=(struct Check_Box_Property*)(Control); 
			
			Sys_GUI_Draw_Check_Box((Coord_X)+(Control_Pointer_Union.Check_Box->Relative_Coord_X),
			                        (Coord_Y)+(Control_Pointer_Union.Check_Box->Relative_Coord_Y),
									Control_Pointer_Union.Check_Box->Status);	

			Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Radio_Button->Relative_Coord_X+14),
				                     (Coord_Y)+(Control_Pointer_Union.Radio_Button->Relative_Coord_Y),
									 Control_Pointer_Union.Radio_Button->Head.Name,DIALOG_CONTROL_FONT,
									 Sys_Strlen(Control_Pointer_Union.Radio_Button->Head.Name),BLACK,TRANSPARENT);
		}
		case TAB:						      								   //A Tab.However,We Don't Allow Secondary Tabs,So There's No Tab Allowed.
		{
// 			Control_Pointer_Union.Tab=(struct Tab_Property*)(Control);
//
//			Sys_GUI_Draw_Tab_Group((Coord_X)+(Control_Pointer_Union.Tab->Relative_Coord_X),
//			                        (Coord_Y)+(Control_Pointer_Union.Tab->Relative_Coord_Y),
//									Control_Pointer_Union.Tab->Header_Width,Control_Pointer_Union.Tab->Number,Control_Pointer_Union.Tab->Header_Lengths,
//									Control_Pointer_Union.Tab->Width_Of_Tabs,Control_Pointer_Union.Tab->Status);
//
//			Tab_Total_Length=0;
//			for(Tab_Count=0;Tab_Count<(Control_Pointer_Union.Tab->Number);Tab_Count++)
//			{
//				Sys_GUI_LCD_Print_String((Coord_X)+(Control_Pointer_Union.Tab->Relative_Coord_X)+Tab_Total_Length+4,
//				                         (Coord_Y)+(Control_Pointer_Union.Tab->Relative_Coord_Y)+3,
//										 Control_Pointer_Union.Tab->Header_Names[Tab_Count],DIALOG_CONTROL_FONT,
//										 Sys_Strlen(Control_Pointer_Union.Tab->Header_Names[Tab_Count]),BLACK,TRANSPARENT);
//																			   //Add It Up.
//				Tab_Total_Length+=Control_Pointer_Union.Tab->Header_Lengths[Tab_Count];					
//			}
//
//			_Sys_GUI_Draw_Tab_Controls(Control);
			break;
		}
		case SCROLL_LIST:				      								   //A Scroll List.
		{
			break;
		}
		case DROPDOWN_MENU:			          								   //A DropDown Menu.
		{
			break;
		}
		case SPIN_BOX:				          								   //A Spin Box.
		{
			break;
		}
		case PROGRESS_BAR:                                                     //A Progress Bar.	
		{
			break;	
		}

		default:break;	
	}

	return(1);																   //Always Succeed Now.
}
#endif
/*Finish Function:_Sys_GUI_Draw_Any_Control_For_Tabs**************************/

/*Begin Function:_Sys_GUI_Draw_Dialog_Controls*********************************
Description : Draw The Dialog Controls According To The DCB.
Input       : u8 Dialog_ID -The Dialog ID To Draw The Controls.
Output      : u8 -If Successful,Return The Dialog Box Status;Else "ENODIA"(0xFF).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 _Sys_GUI_Draw_Dialog_Controls(u8 Dialog_ID)
{
	u8 Control_Count;
																			   //No Need To Disable INTS Here.
	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{						                                                   //The Dialog ID Does Not Exist At All.
		return(ENODIA);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{
		return(ENODIA);
	}

	if(DCB[Dialog_ID].Dialog_Number_Of_Controls==0)							   //There Are Already No Controls.
	{
		return(ENODIA);
	}

	for(Control_Count=0;Control_Count<DCB[Dialog_ID].Dialog_Number_Of_Controls;Control_Count++)
		_Sys_GUI_Draw_Any_Control(DCB[Dialog_ID].Dialog_Coord_X,DCB[Dialog_ID].Dialog_Coord_Y,DCB[Dialog_ID].CCB[Control_Count]);	

	return(DCB[Dialog_ID].Dialog_Status);
}
#endif
/*Finish Function:_Sys_GUI_Draw_Dialog_Controls*******************************/

/*Begin Function:Sys_GUI_Refresh_Dialog_Box************************************
Description : Refresh The Dialog Box According To The Arguments In The Corresponding Control Block. 
Input       : u8 Dialog_ID -The ID Of The Dialog Box Returned By "Sys_GUI_Create_Dialog_Box".
Output      : u8 -If Successful,It Will Return The Dialog Box Status;Else It Will Return
              "ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Refresh_Dialog_Box(u8 Dialog_ID)							 		   //Under Current Condition,All Dialog Boxes Will Be Refreshed.
{
	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{	 												 					   //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{	 
		return(ENODOP);
	}
	
	Sys_GUI_Draw_Dialog_Box(DCB[Dialog_ID].Dialog_Coord_X,DCB[Dialog_ID].Dialog_Coord_Y,DCB[Dialog_ID].Dialog_Length,DCB[Dialog_ID].Dialog_Width,DCB[Dialog_ID].Dialog_Title,DCB[Dialog_ID].Dialog_Icon);
	_Sys_GUI_Draw_Dialog_Controls(Dialog_ID);
 																			   //Now We Redraw The Controls.
	return(DCB[Dialog_ID].Dialog_Status);	
}
#endif
/*Finish Function:Sys_GUI_Refresh_Dialog_Box**********************************/

/*Begin Function:Sys_GUI_Redefine_Dialog_Box***********************************
Description : Redefine The Dialog Box According To The Arguments. 
Input       : u8 Dialog_ID -The ID Of The Dialog Box Returned By "Sys_GUI_Create_Dialog_Box".
              u16 Coord_X -The X Coordination Of The Dialog Box.The Top-Left Corner.
              u16 Coord_Y -The Y Coordination Of The Dialog Box.The Top-Left Corner.
			  u16 Length -The Length Of It.
			  u16 Width -The Width Of It.
			  u8 Status -The Initial Status Of The Dialog Box.
			  void* Title -The Title Of The Box.Can't Be Longer Than The Macro "MAX_LEN_OF_DIALOG_TITLE". 
Output      : u8 -If Successful,It Will Return The Dialog Box Status;Else It Will Return
              "ENODOP"(0x00).
******************************************************************************/
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Redefine_Dialog_Box(u8 Dialog_ID,u16 Coord_X,u16 Coord_Y,u16 Length,u16 Width,u8 Status,void* Title,void* Icon)
{
	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{		 												 				   //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{
		return(ENODOP);
	}
	
	if((Status&DIALOG_EXIST)==0)
	{	 		                  						   					   //The Redefined Status Is Not Correct(The "DIALOG_EXIST" Is Not Set.).
		return(ENODOP);	
	}

	Sys_Memset(DCB[Dialog_ID].Dialog_Title,0,MAX_LEN_OF_DIALOG_TITLE);	       //Clear The Title First.

	Sys_Strcpy(Title,DCB[Dialog_ID].Dialog_Title,MAX_LEN_OF_DIALOG_TITLE);     //Fill The Title.
	//DCB[DCB_Count].Dialog_ID=Dialog_ID;								       //We Can't Redefine It.It Is Fixed.
	DCB[Dialog_ID].Dialog_Status=Status;
	DCB[Dialog_ID].Dialog_Icon=Icon;
	DCB[Dialog_ID].Dialog_Coord_X=Coord_X;								       //Fill The Correspoding Table In DCB.
	DCB[Dialog_ID].Dialog_Coord_Y=Coord_Y;
	DCB[Dialog_ID].Dialog_Length=Length;
	DCB[Dialog_ID].Dialog_Width=Width;

	return(DCB[Dialog_ID].Dialog_Status);
}
#endif
/*Finish Function:Sys_GUI_Redefine_Dialog_Box*********************************/

/*Begin Function:Sys_GUI_Move_Dialog_Box***************************************
Description : Move a Dialog Box To A Certain Position According To The Arguments.
Input       : u16 Coord_X -The X Coordination Of The New Position.The Top-Left Corner.
              u16 Coord_Y -The Y Coordination Of The New Position.The Top-Left Corner.
			  u8 Dialog_ID -The ID Of The Dialog Box.
			  u8 Mode -The Move Mode.
Output      : If Successful,It Will Return The Dialog Status;Else "ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Move_Dialog_Box(u16 Coord_X,u16 Coord_Y,u8 Dialog_ID,u8 Mode)
{
	 

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{
		 												    				   //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{
		 
		return(ENODOP);
	}
																			   //We Don't Detect Other Properties Now.
	Sys_GUI_Erase_Dialog_Box(Dialog_ID,DIALOG_ERASE_COLOR);				       //Erase The Dialog Box Now.

	if((Mode&REAL_MOVE)!=0)
	{
		DCB[Dialog_ID].Dialog_Coord_X=Coord_X;								   //Update The Correspoding Table In DCB.
		DCB[Dialog_ID].Dialog_Coord_Y=Coord_Y;								   //Draw It At The New Position.
	}

	if((Mode&SHOW_WHILE_MOVING)!=0)
	{
		Sys_GUI_Draw_Dialog_Box(Coord_X,Coord_Y,DCB[Dialog_ID].Dialog_Length,DCB[Dialog_ID].Dialog_Width,DCB[Dialog_ID].Dialog_Title,DCB[Dialog_ID].Dialog_Icon);
	}
	else
	{																		   //Only Show Its Outline While Moving It.
	    Sys_GUI_LCD_GUI_Filled_Rectangle(Coord_X,Coord_Y,DCB[Dialog_ID].Dialog_Length,DCB[Dialog_ID].Dialog_Width,BLACK,TRANSPARENT);
	}

	return(DCB[Dialog_ID].Dialog_Status);
}
#endif
/*Finish Function:Sys_GUI_Move_Dialog_Box*************************************/

/*Begin Function:Sys_GUI_Refresh_All_Dialog_Boxes******************************
Description : Refresh All The Dialog Boxes According To Their Respective Properties.They Will Be Refreshed In
              The Sequence Of Their Position The List.
Input       : Void.
Output      : If Successful,It Will Return 0;Else "ENODIA"(0xFF).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Refresh_All_Dialog_Boxes(void)
{
	u8 Dialog_Refresh_Table[MAX_NUMBER_OF_DIALOG];
	struct List_Head* Current_List_Pointer=&Dialog_List_Head;
	u8 Table_Count=0;

	while(1)
	{
		Current_List_Pointer=Sys_List_Seek(Current_List_Pointer,1);
		if(Current_List_Pointer==&Dialog_List_Head)		                       //If We Returned To The Head Of The List,Then The Search Is Over.
			break;
		Dialog_Refresh_Table[Table_Count]=*((u8*)(Current_List_Pointer)+8);
		Table_Count++;														   //Add The Table Sequence Value.
	}																		   //Now The "Table_Count" Contains The Number Of The Windows To Refresh,The Array Storing Their Sequence.

	if(Table_Count==0)
		return(ENODIA);														   //There Are No Dialog Boxes In The System Now.

	for(;Table_Count>0;Table_Count--)										   //The First Dialog Box Will Be Refreshed At Last For The Cascade Effect.
		Sys_GUI_Refresh_Dialog_Box(Dialog_Refresh_Table[Table_Count-1]);	
	
	return(0);	
}
#endif
/*Finish Function:Sys_GUI_Refresh_All_Dialog_Boxes****************************/

/*Begin Function:Sys_GUI_Add_Control_To_Dialog*********************************
Description : Add A Control To The Dialog Box.
Input       : u8 The ID Of The Dialog Box To Add The Control To.
			  struct Dialog_Control_Head* Control -The Pointer To The General Head In The Struct.
Output      : u8 -If Successful,1;Else,"ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Add_Control_To_Dialog(u8 Dialog_ID,struct Dialog_Control_Head* Control)
{
	u8 Control_Count;

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{	 												                       //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{
		return(ENODOP);
	}

	if((DCB[Dialog_ID].Dialog_Number_Of_Controls+1)>=MAX_CONTROL_IN_DIALOG)
	{ 													 					   //There's No Empty "CCB" Space.
		return(ENODOP);
	}						 

	for(Control_Count=0;Control_Count<MAX_CONTROL_IN_DIALOG;Control_Count++)
	{
		if((DCB[Dialog_ID].CCB[Control_Count])==0)							   //The Slot Is Empty.
		{
			DCB[Dialog_ID].CCB[Control_Count]=Control;
			break;	
		}
	}

	DCB[Dialog_ID].Dialog_Number_Of_Controls+=1;							   //Increase.
	Control->Dialog_ID=Dialog_ID;											   //Fill The Control's Dialog ID.
													      					   //Success.
	return(0);
}
#endif
/*Finish Function:Sys_GUI_Add_Control_To_Dialog*******************************/

/*Begin Function:Sys_GUI_Add_Control_To_Tab************************************
Description : Add A Control To The One Tab In The Tab Group.
Input       : struct Dialog_Control_Head* Tab -The Tab Group To Add The Control To.
              u8 Tab_ID -The ID Of The Tab To Add The Control To.
			  struct Dialog_Control_Head* Control -The Pointer To The General Head In The Struct.
Output      : u8 -If Successful,1;Else,"ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Add_Control_To_Tab(struct Dialog_Control_Head* Tab,u8 Tab_ID,struct Dialog_Control_Head* Control)
{
	u8 Control_Count;

	struct Tab_Property* Tab_Property_Pointer=(struct Tab_Property*)Tab;

	if(Tab_ID>=MAX_NUMBER_OF_TAB_IN_GROUP)
	{											    						   //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((Tab->Category)!=TAB)					                         	   //The Control Is Not A Tab.
	{ 
		return(ENODOP);
	}

	if((Tab_Property_Pointer->Number_Of_Controls_In_Tabs[Tab_Property_Pointer->Status])>=MAX_CONTROL_IN_TAB)
	{
		 													 				   //There's No Empty "CCB" Space.
		return(ENODOP);
	}						 

	for(Control_Count=0;Control_Count<MAX_CONTROL_IN_TAB;Control_Count++)
	{
		if((Tab_Property_Pointer->Tab_CCB[Tab_ID][Control_Count])==0)		   //The Slot Is Empty.
		{
			(Tab_Property_Pointer->Tab_CCB[Tab_ID][Control_Count])=Control;
			break;	
		}
	}

	Tab_Property_Pointer->Number_Of_Controls_In_Tabs[Tab_ID]+=1;			   //Increase.
	(Control->Dialog_ID)=TAB_AS_DIALOG_ID;									   //Assignment.The ID Is Dummy,0xFF In Fact.
	 													      				   //Success.
	return(0);
}
#endif
/*Finish Function:Sys_GUI_Add_Control_To_Tab**********************************/

/*Begin Function:Sys_GUI_Delete_Control_From_Tab*******************************
Description : Delete A Control From The Dialog Box.
Input       : struct Dialog_Control_Head* Tab -The Head Of The Tab.
              u8 Tab_ID -The ID Of The Tab To Delete The Control From.The Tab ID Is The Sequence Of The Tab In The Tab Group.
			  struct Dialog_Control_Head* Control -The Pointer To The General Head In The Control Struct.
Output      : u8 -If Successful,1;Else,"ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Delete_Control_From_Tab(struct Dialog_Control_Head* Tab,u8 Tab_ID,struct Dialog_Control_Head* Control)
{
	u8 Control_Count;

	struct Tab_Property* Tab_Pointer=(struct Tab_Property*)Tab;

	if((Tab->Category)!=TAB)
	{	 												             		   //The First Tab Is Not A Tab.Abort.
		return(ENODOP);	
	}

	if(Tab_Pointer->Number_Of_Controls_In_Tabs[Tab_ID]==0)					   //There Are Already No Controls.Can't Delete Any More.
	{	 
		return(ENODOP);
	}

	for(Control_Count=0;Control_Count<MAX_CONTROL_IN_TAB;Control_Count++)
	{
		if((Tab_Pointer->Tab_CCB[Tab_ID][Control_Count])==Control)
			(Tab_Pointer->Tab_CCB[Tab_ID][Control_Count])=0;				   //Clear The Table.
	}

	if(Control_Count==MAX_CONTROL_IN_TAB)								       //The Control Does Not Exist In The Table.Abort.
	{		 
		return(ENODOP);		
	}

	(Tab_Pointer->Number_Of_Controls_In_Tabs[Tab_ID])-=1;					   //Decrease By 1.
	(Control->Dialog_ID)=CLEAR_CONTROL_DIALOG_ID;							   //Clear It.(In Fact It Didn't Get Cleared.OxFF Is Empty In Fact.)
		 														          	   //Success.
	return(0);
}
#endif
/*Finish Function:Sys_GUI_Delete_Control_From_Tab*****************************/

/*Begin Function:Sys_GUI_Delete_Control_From_Dialog****************************
Description : Delete A Control From The Dialog Box.
Input       : u8 The ID Of The Dialog Box To Delete The Control From.
			  struct Dialog_Control_Head* Control -The Pointer To The General Head In The Struct.
Output      : u8 -If Successful,1;Else,"ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Delete_Control_From_Dialog(u8 Dialog_ID,struct Dialog_Control_Head* Control)
{
	u8 Control_Count;

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{
		 												    				   //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{
		 
		return(ENODOP);
	}

	if(DCB[Dialog_ID].Dialog_Number_Of_Controls==0)							   //There Are Already No Controls.
	{
		 
		return(ENODOP);
	}

	for(Control_Count=0;Control_Count<MAX_CONTROL_IN_DIALOG;Control_Count++)
	{
		if(DCB[Dialog_ID].CCB[Control_Count]==Control)
			DCB[Dialog_ID].CCB[Control_Count]=0;							   //Clear The Table.
	}

	if(Control_Count==MAX_CONTROL_IN_DIALOG)								   //The Control Does Not Exist In The Table.Abort.
	{
		 
		return(ENODOP);		
	}

	DCB[Dialog_ID].Dialog_Number_Of_Controls-=1;							   //Decrease The Number By 1.
	Control->Dialog_ID=CLEAR_CONTROL_DIALOG_ID;								   //Clear The Dialog ID.
	 														 				   //Success.
	return(0);
}
#endif 
/*Finish Function:Sys_GUI_Delete_Control_From_Dialog**************************/


/*Begin Function:Sys_GUI_Get_Clicked_Dialog************************************
Description : Delete A Control From The Dialog Box.
Input       : u16 Coord_X -The X Coordination Of The Clicked Point.
              u16 Coord_Y -The Y Coordination Of The Clicked Point.
Output      : u8 -If There Is A Window,It Will Return The Dialog ID;Else It Will Return"ENODIA"
              (0xFF).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Get_Clicked_Dialog(u16 Coord_X,u16 Coord_Y)
{
	struct Dialog_Box* Dialog;												   //The Pointer To The Found Dialog Box Control Block.
	u16 Coord_Compound=(Coord_X<<8)|Coord_Y;
	 												  
	Dialog=(struct Dialog_Box*)Sys_Search_The_List(Dialog_List_Head.Next,Dialog_List_Head.Prev,
	                                               0xFF,0,_Sys_GUI_Search_Clicked_Dialog_Condition,&Coord_Compound);		
	if(Dialog==ENONOD)
	{	
		 																	   //The Node Is Not Found,It Indicates That There Are No Dialog Box Containing The Position.
		return(ENODIA);
	}
	else
	{
		 
		return(Dialog->Dialog_ID);
	}
}
#endif
/*Finish Function:Sys_GUI_Get_Clicked_Dialog**********************************/

/*Begin Function:_Sys_GUI_Search_Clicked_Dialog_Condition**********************
Description : The Search Condition For The Clicked Dialog Box.
Input       : struct List_Head* Current_Node -The Current Node That Is Being Searched.
              void* Coord_Compound -The Compound Of The Coordination.In Fact It Is Organized
			  In A "u16" Type:The First Byte Is The "Coord_X",The Second Is "Coord_Y".
Output      : u8 -If The Condition Is Met,Then It Will Return A "END_SEARCH"(0x01).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 _Sys_GUI_Search_Clicked_Dialog_Condition(struct List_Head* Current_Node,void* Coord_Compound)
{
	u8 Clicked_Coord_X=(*((u16*)(Coord_Compound)))>>8;
	u8 Clicked_Coord_Y=(*((u16*)(Coord_Compound)))&0xFF;
	struct Dialog_Box* Dialog=(struct Dialog_Box*)Current_Node;

	if(((Dialog->Dialog_Coord_X)<=Clicked_Coord_X)&&(Clicked_Coord_X<=(Dialog->Dialog_Coord_X+Dialog->Dialog_Length-1)))
		if(((Dialog->Dialog_Coord_Y)<=Clicked_Coord_Y)&&(Clicked_Coord_Y<=(Dialog->Dialog_Coord_Y+Dialog->Dialog_Width-1)))
			 return(END_SEARCH);											   //The Point Is In The Range.Stop Searching.

	return(0);																   //The Search Hasn't Succeeded Yet.
}
#endif
/*Finish Function:_Sys_GUI_Search_Clicked_Dialog_Condition********************/

/*Begin Function:Sys_GUI_Get_Clicked_Control***********************************
Description : Get The Clicked Control In The Dialog Box.
Input       : u8 Dialog_ID -The Dialog ID Of The Clicked Point.
			  u16 Coord_X -The X Coordination Of The Clicked Point.
              u16 Coord_Y -The Y Coordination Of The Clicked Point.
Output      : u16 -A Compound Return Value.The First 8 Bit Is The Control ID
              (The Identifier(Sequence) In The CCB.)The Last 8 Bit Is The Specific
			  Identifier For The Control Itself.If The Value Is Not Needed,It Will Be Zero.
			  If The Function Fails,It Will Return "ENODIA"(0xFF) In Both Two Bytes.
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u16 Sys_GUI_Get_Clicked_Control(u8 Dialog_ID,u16 Coord_X,u16 Coord_Y)
{
	u8 Control_Count;
	u8 Radio_Count,Tab_Count;
	u16 Tab_Total_Length=0;
	union Control_Pointer Control_Pointer_Union;

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{
		 												    				   //The Dialog ID Does Not Exist At All.
		return((ENODIA<<8)|ENODIA);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{
		 
		return((ENODIA<<8)|ENODIA);
	}
																			   //Process Them Separately With This Function.
   	for(Control_Count=0;Control_Count<DCB[Dialog_ID].Dialog_Number_Of_Controls;Control_Count++)		   
	{
		switch(DCB[Dialog_ID].CCB[Control_Count]->Category)
		{
			case COMMAND_BUTTON:			       					           //A Command Button.
			{
				Control_Pointer_Union.Command_Button=(struct Command_Button_Property*)(DCB[Dialog_ID].CCB[Control_Count]);		
				
				if(((Control_Pointer_Union.Command_Button->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
				   (Coord_X<=(Control_Pointer_Union.Command_Button->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+(Control_Pointer_Union.Command_Button->Length)))
				{
					if(((Control_Pointer_Union.Command_Button->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
					   (Coord_Y<=(Control_Pointer_Union.Command_Button->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Command_Button->Width)));
					{
						  
						return(((u16)Control_Count)<<8);
					}
				}

				break;
			}

			case RADIO_BUTTON:			       								   //A Radio Button.
			{
				Control_Pointer_Union.Radio_Button=(struct Radio_Button_Property*)(DCB[Dialog_ID].CCB[Control_Count]);

				for(Radio_Count=0;Radio_Count<(Control_Pointer_Union.Radio_Button->Number);Radio_Count++)
				{
					if(((Control_Pointer_Union.Radio_Button->Relative_Coord_X)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
					   (Coord_X<=(Control_Pointer_Union.Radio_Button->Relative_Coord_X)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count)+(DCB[Dialog_ID].Dialog_Coord_X)+RADIO_BUTTON_SIZE))
					{
						if(((Control_Pointer_Union.Radio_Button->Relative_Coord_Y)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count+1)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
					   	   (Coord_Y<=(Control_Pointer_Union.Radio_Button->Relative_Coord_Y)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count+1)+(DCB[Dialog_ID].Dialog_Coord_Y)+RADIO_BUTTON_SIZE))
						{   
							  
						    return((((u16)Control_Count)<<8)|(Radio_Count));
						}
					}
				}

				break;
			}

			case TOGGLE:					       							   //A Toggle Button.
			{
				break;
			}

			case CHECK_BOX:				         							   //A Checkbox.
			{	
				Control_Pointer_Union.Check_Box=(struct Check_Box_Property*)(DCB[Dialog_ID].CCB[Control_Count]);

				if(((Control_Pointer_Union.Check_Box->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
				   (Coord_X<=(Control_Pointer_Union.Check_Box->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+CHECK_BOX_SIZE))
				{
					if(((Control_Pointer_Union.Check_Box->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
					   (Coord_Y<=(Control_Pointer_Union.Check_Box->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+RADIO_BUTTON_SIZE))
					{   
						  
						return(((u16)Control_Count)<<8);
					}
				}

				break;
			}

			case TAB:						       						       //A Tab.If A Tab Is Clicked,It Means That One Of Its Part(Including The Blank Part)Is Clicked.
			{
				Control_Pointer_Union.Tab=(struct Tab_Property*)(DCB[Dialog_ID].CCB[Control_Count]);

				for(Tab_Count=0;Tab_Count<(Control_Pointer_Union.Tab->Number);Tab_Count++)
				{
					if(((Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+Tab_Total_Length<=Coord_X)&&
					   (Coord_X<=(Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+Tab_Total_Length+(Control_Pointer_Union.Tab->Header_Lengths[Tab_Count])))
					{
						if(((Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
					   	   (Coord_Y<=(Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Tab->Header_Width)))
						{    
							 
							return((((u16)Control_Count)<<8)|(Tab_Count));
						}
					}
																			   //Temporarily Finished.
					Tab_Total_Length+=(Control_Pointer_Union.Tab->Header_Lengths[Tab_Count]);		  
				}
				
  				if(((Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
				   (Coord_X<=(Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+Tab_Total_Length))
				{
					if(((Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Tab->Header_Width)<=Coord_Y)&&
				  	   (Coord_Y<=(Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Tab->Header_Width)+(Control_Pointer_Union.Tab->Width_Of_Tabs)))
					{
						 
						return((((u16)Control_Count)<<8)|(TAB_NOT_HEADER));
					}
				}

				break;
			}

			case SCROLL_LIST:				        						   //A Scroll List.
			{
				break;
			}

			case DROPDOWN_MENU:			         							   //A DropDown Menu.
			{
				break;
			}

			case SPIN_BOX:				         							   //A Spin Box.
			{
				break;
			}

			case PROGRESS_BAR:                                                 //A Progress Bar.
			{
			 	break;
			}

			default:break;
		}
	}
	 
	return((ENODIA<<8)|ENODIA);												   //It Must Have Failed When It Gets Here.
}
#endif
/*Finish Function:Sys_GUI_Get_Clicked_Control*********************************/

/*Begin Function:Sys_GUI_Get_Clicked_Control_In_Tab****************************
Description : Get The Clicked Control In A Tab.
Input       : u8 Dialog_ID -The Dialog ID Of The Clicked Point.
			  u16 Compound_ID -The Compound Identifier Returned By "Sys_GUI_Get_Clicked_Control".
			  u16 Coord_X -The X Coordination Of The Clicked Point.
              u16 Coord_Y -The Y Coordination Of The Clicked Point.
Output      : u16 -A Compound Return Value.The First 8 Bit Is The Control ID
              (The Identifier(Sequence) In The Tab_CCB.)The Last 8 Bit Is The Specific
			  Identifier For The Control Itself.If The Value Is Not Needed,It Will Be Zero.
			  If The Function Fails,It Will Return "ENODIA"(0xFF) In Both Two Bytes.
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u16 Sys_GUI_Get_Clicked_Control_In_Tab(u8 Dialog_ID,u16 Compound_ID,u16 Coord_X,u16 Coord_Y)
{
	u8 Control_ID_In_Dialog=(Compound_ID<<8);
	struct Tab_Property* Tab=(struct Tab_Property*)(DCB[Dialog_ID].CCB[Control_ID_In_Dialog]);

	u8 Control_Count;
	u8 Radio_Count;															   //The GUI Doesn't Support Secondary Tabs.
	union Control_Pointer Control_Pointer_Union;

	if((DCB[Dialog_ID].CCB[Control_ID_In_Dialog]->Category)!=TAB)			   //The Category Is Not Tab.Abort.
	{	 												    
		return((ENODIA<<8)|ENODIA);	
	}

	if((Compound_ID&0xFF)!=TAB_NOT_HEADER)
	{ 												    					   //The Clicked Area Is Not Valid Here.
		return((ENODIA<<8)|ENODIA);	
	}

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{	 												    				   //The Dialog ID Does Not Exist At All.
		return((ENODIA<<8)|ENODIA);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{	 
		return((ENODIA<<8)|ENODIA);
	}
																			   //Process Them Separately With This Function.
   	for(Control_Count=0;Control_Count<(Tab->Number_Of_Controls_In_Tabs[Tab->Status]);Control_Count++)		   
	{
		switch(Tab->Tab_CCB[Tab->Status][Control_Count]->Category)
		{
			case COMMAND_BUTTON:			       					           //A Command Button.
			{
				Control_Pointer_Union.Command_Button=(struct Command_Button_Property*)(Tab->Tab_CCB[Tab->Status][Control_Count]);		
				
				if(((Control_Pointer_Union.Command_Button->Relative_Coord_X)+(Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
				   (Coord_X<=(Control_Pointer_Union.Command_Button->Relative_Coord_X)+(Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+(Control_Pointer_Union.Command_Button->Length)))
				{
					if(((Control_Pointer_Union.Command_Button->Relative_Coord_Y)+(Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
					   (Coord_Y<=(Control_Pointer_Union.Command_Button->Relative_Coord_Y)+(Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Command_Button->Width)))
					{	  
						return(((u16)Control_Count)<<8);
					}
				}

				break;
			}

			case RADIO_BUTTON:			       								   //A Radio Button.
			{
				Control_Pointer_Union.Radio_Button=(struct Radio_Button_Property*)(Tab->Tab_CCB[Tab->Status][Control_Count]);

				for(Radio_Count=0;Radio_Count<(Control_Pointer_Union.Radio_Button->Number);Radio_Count++)
				{
					if(((Control_Pointer_Union.Radio_Button->Relative_Coord_X)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count)+(Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
					   (Coord_X<=(Control_Pointer_Union.Radio_Button->Relative_Coord_X)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count)+(Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+RADIO_BUTTON_SIZE))
					{
						if(((Control_Pointer_Union.Radio_Button->Relative_Coord_Y)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count+1)+(Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
					   	   (Coord_Y<=(Control_Pointer_Union.Radio_Button->Relative_Coord_Y)+*(Control_Pointer_Union.Radio_Button->Relative_Centers+2*Radio_Count+1)+(Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+RADIO_BUTTON_SIZE))
						{	  
							return((((u16)Control_Count)<<8)|(Radio_Count));
						}
					}
				}

				break;
			}

			case TOGGLE:					       							   //A Toggle Button.
			{
				break;
			}

			case CHECK_BOX:				         							   //A Checkbox.
			{	
				Control_Pointer_Union.Check_Box=(struct Check_Box_Property*)(Tab->Tab_CCB[Tab->Status][Control_Count]);

				if(((Control_Pointer_Union.Check_Box->Relative_Coord_X)+(Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
				   (Coord_X<=(Control_Pointer_Union.Check_Box->Relative_Coord_X)+(Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+CHECK_BOX_SIZE))
				{
					if(((Control_Pointer_Union.Check_Box->Relative_Coord_Y)+(Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
					   (Coord_Y<=(Control_Pointer_Union.Check_Box->Relative_Coord_Y)+(Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+RADIO_BUTTON_SIZE))
					{    
						return(((u16)Control_Count)<<8);
					}
				}

				break;
			}

			case TAB:						       						       //Don't Support Secondary Tabs.
			{
//				Control_Pointer_Union.Tab=(struct Tab_Property*)(Tab->Tab_CCB[Tab->Status][Control_Count]);
//
//				for(Tab_Count=0;Tab_Count<(Control_Pointer_Union.Tab->Number);Tab_Count++)
//				{
//					if(((Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+Tab_Total_Length<=Coord_X)&&
//					   (Coord_X<=(Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+Tab_Total_Length+(Control_Pointer_Union.Tab->Header_Lengths[Tab_Count])))
//					{
//						if(((Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)<=Coord_Y)&&
//					   	   (Coord_Y<=(Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Tab->Header_Width)))
//						    return((((u16)Control_Count)<<8)|(Tab_Count));
//					}
//																			   //Temporarily Finished.
//					Tab_Total_Length+=(Control_Pointer_Union.Tab->Header_Lengths[Tab_Count]);		  
//				}
//				
//  				if(((Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)<=Coord_X)&&
//				   (Coord_X<=(Control_Pointer_Union.Tab->Relative_Coord_X)+(DCB[Dialog_ID].Dialog_Coord_X)+Tab_Total_Length))
//				{
//					if(((Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Tab->Header_Width)<=Coord_Y)&&
//				  	   (Coord_Y<=(Control_Pointer_Union.Tab->Relative_Coord_Y)+(DCB[Dialog_ID].Dialog_Coord_Y)+(Control_Pointer_Union.Tab->Header_Width)+(Control_Pointer_Union.Tab->Width_Of_Tabs)))
//						return((((u16)Control_Count)<<8)|(TAB_NOT_HEADER));
//				}

				break;
			}

			case SCROLL_LIST:				        						   //A Scroll List.
			{
				break;
			}

			case DROPDOWN_MENU:			         							   //A DropDown Menu.
			{
				break;
			}

			case SPIN_BOX:				         							   //A Spin Box.
			{
				break;
			}

			case PROGRESS_BAR:                                                 //A Progress Bar.
			{
			 	break;
			}

			default:break;
		}
	}
 
	return((ENODIA<<8)|ENODIA);												   //It Must Have Failed When It Gets Here.
}
#endif
/*Finish Function:Sys_GUI_Get_Clicked_Control_In_Tab**************************/

/*Begin Function:Sys_GUI_Execute_Clicked_Control_Action_In_Tab*****************
Description : Execute Action Function Of A Control In The Tab.
Input       : u8 Dialog_ID -The Dialog ID Of The Clicked Point.
			  u16 Dialog_Compound_ID -The Compound Value Returned By "Sys_GUI_Get_Clicked_Control".
			  u16 Tab_Compound_ID -The Compound Value Returned By "Sys_GUI_Get_Clicked_Control_In_Tab".
			  u8 Mode -The Mode When The Mouse Passes It.Can Be The Four Values Below:
			  BREAK						[0x00]									//The Action When The Mouse Press Breaks On It.
			  PRESS						[0x01]							        //The Action Function When It Is Clicked.It Will Be Executed At Once After Click.
			  BREAK_PASS				[0x02]									//The Action When The Mouse Passes It Without Pressing It.
			  PRESS_PASS				[0x03]									//The Action When The Mouse Passes It While Pressing It.
			  u16 Coord_X -The X Coordination Of The Clicked Point.
              u16 Coord_Y -The Y Coordination Of The Clicked Point.
			  The Two Coordinations Are Dummy.For Continuity.
Output      : u8 -The Return Value Is The Same As That Of The Executed Function.
              (The Function Should Be "u8 (*)(void)" Type.) 
			  Should The Function Fail,It Will Return "ENODOP"(0x00).
******************************************************************************/ 
#if(ENABLE_GUI==TRUE)
u8 Sys_GUI_Execute_Clicked_Control_Action_In_Tab(u8 Dialog_ID,u16 Dialog_Compound_ID,u16 Tab_Compound_ID,u8 Mode,u16 Coord_X,u16 Coord_Y)
{
	u8 Dialog_Control_ID=Dialog_Compound_ID>>8;

	u8 Tab_Control_ID=Tab_Compound_ID>>8;
	u8 Tab_Specific_Identifier=(Tab_Compound_ID&0xFF);

	struct Tab_Property* Tab=(struct Tab_Property*)(DCB[Dialog_ID].CCB[Dialog_Control_ID]);

	union Control_Pointer Control_Pointer_Union;

	u8 (*Execute_Function)(void);

	if((Dialog_Compound_ID&0xFF)!=TAB_NOT_HEADER)
	{	 												    				   //The Tab ID Is In Fact A Failed One.
		return(ENODOP);	
	}

	if((Tab_Specific_Identifier==ENODIA)&&(Tab_Control_ID==ENODIA))
	{	 												    				   //The Tab ID Is In Fact A Failed One.
		return(ENODOP);	
	}

	if((Tab->Number_Of_Controls_In_Tabs[Tab->Status])==0)                      //There Are No Controls In The Tab.
	{	 
		return(ENODOP);
	}

	if((Tab_Specific_Identifier==ENODIA)&&(Dialog_Control_ID==ENODIA))
	{ 												    					   //The Dialog ID Is In Fact A Failed One.
		return(ENODOP);	
	}

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{	 												    				   //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)						   //The Dialog Box Does Not Exist At All.
	{		 
		return(ENODOP);
	}

	if(DCB[Dialog_ID].Dialog_Number_Of_Controls==0)							   //There Are Already No Controls.
	{		 
		return(ENODOP);
	}
	
	switch(Tab->Tab_CCB[Tab->Status][Tab_Control_ID]->Category)
	{
		case COMMAND_BUTTON:			       					         	   //A Command Button.
		{
			Control_Pointer_Union.Command_Button=(struct Command_Button_Property*)(Tab->Tab_CCB[Tab->Status][Tab_Control_ID]);		
			switch(Mode)
			{																   //Get The Function To Execute.
				case BREAK:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Break);break;	
				case PRESS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Press);break;
				case BREAK_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Break_Pass);break;
				case PRESS_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Press_Pass);break;
				default:Execute_Function=_Sys_GUI_NOP;break;
			}
			  
			return(Execute_Function());

			//break;													       //Dummy
		}

		case RADIO_BUTTON:			       								       //A Radio Button.
		{
			Control_Pointer_Union.Radio_Button=(struct Radio_Button_Property*)(Tab->Tab_CCB[Tab->Status][Tab_Control_ID]);

			switch(Mode)
			{																   //Get The Function To Execute.
				case BREAK:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Break[Tab_Specific_Identifier]);break;	
				case PRESS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Press[Tab_Specific_Identifier]);break;
				case BREAK_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Break_Pass[Tab_Specific_Identifier]);break;
				case PRESS_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Press_Pass[Tab_Specific_Identifier]);break;
				default:Execute_Function=_Sys_GUI_NOP;break;
			}
			  
			return(Execute_Function());				  

			//break;													       //Dummy
		}

		case TOGGLE:					       							       //A Toggle Button.
		{
			break;
		}

		case CHECK_BOX:				         							       //A Checkbox.
		{	
			Control_Pointer_Union.Check_Box=(struct Check_Box_Property*)(Tab->Tab_CCB[Tab->Status][Tab_Control_ID]);

			switch(Mode)
			{																   //Get The Function To Execute.
				case BREAK:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Break);break;	
				case PRESS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Press);break;
				case BREAK_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Break_Pass);break;
				case PRESS_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Press_Pass);break;
				default:Execute_Function=_Sys_GUI_NOP;break;
			}
		
			  
			return(Execute_Function());

			//break;													       //Dummy
		}

		case TAB:						       						     	   //Do Not Support Secondary Tabs.
		{
//			if(Dialog_Special_Identifier!=TAB_NOT_HEADER)
//			{
//				Control_Pointer_Union.Tab=(struct Tab_Property*)(Tab->Tab_CCB[Tab->Status][Tab_Control_ID]);
//	
//				Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Tab->Select_Action[Tab_Special_Identifier]);
//	
//				return(Execute_Function());
//			}
//			else
//			{
//				Compound_In_Tab=Sys_GUI_Get_Clicked_Control_In_Tab(Dialog_ID,Compound,Coord_X,Coord_Y);
//
//			}

			break;												       
		}

		case SCROLL_LIST:				        						       //A Scroll List.
		{
			break;
		}

		case DROPDOWN_MENU:			         							       //A DropDown Menu.
		{
			break;
		}

		case SPIN_BOX:				         							       //A Spin Box.
		{
			break;
		}

		case PROGRESS_BAR:                                                     //A Progress Bar.
		{
		 	break;
		}

		default:break;
	}	
		 													      			   //If It Gets Here,It Must Have Failed.
	return(ENODOP);	
}
#endif
/*Finish Function:Sys_GUI_Execute_Clicked_Control_Action_In_Tab***************/

/*Begin Function:Sys_GUI_Execute_Clicked_Control_Action************************
Description : Execute Action Function Of A Control In The Dialog Box.
Input       : u8 Dialog_ID -The Dialog ID Of The Clicked Point.
			  u16 Compound -The Compound Value Returned By "Sys_GUI_Get_Clicked_Control".
			  u8 Mode -The Mode When The Mouse Passes It.Can Be The Four Values Below:
			  BREAK						[0x00]									//The Action When The Mouse Press Breaks On It.
			  PRESS						[0x01]							        //The Action Function When It Is Clicked.It Will Be Executed At Once After Click.
			  BREAK_PASS				[0x02]									//The Action When The Mouse Passes It Without Pressing It.
			  PRESS_PASS				[0x03]									//The Action When The Mouse Passes It While Pressing It.
			  u16 Coord_X -The X Coordination Of The Clicked Point.
              u16 Coord_Y -The Y Coordination Of The Clicked Point.
			  The Two Coordinations Are Necessary When You Clicked A Tab.
Output      : u8 -The Return Value Is The Same As That Of The Executed Function.
              (The Function Should Be "u8 (*)(void)" Type.) 
			  Should The Function Fail,It Will Return "ENODOP"(0x00).
******************************************************************************/
#if(ENABLE_GUI==TRUE) 
u8 Sys_GUI_Execute_Clicked_Control_Action(u8 Dialog_ID,u16 Compound,u8 Mode,u16 Coord_X,u16 Coord_Y)
{
	u8 Control_ID=Compound>>8;
	u8 Specific_Identifier=Compound&0xFF;
	u16 Compound_In_Tab;
	union Control_Pointer Control_Pointer_Union;
	u8 (*Execute_Function)(void);

	 

	if((Specific_Identifier==ENODIA)&&(Control_ID==ENODIA))
	{
		 												    				   //The Dialog ID Is In Fact A Failed One.
		return(ENODOP);	
	}

	if(Dialog_ID>=MAX_NUMBER_OF_DIALOG)
	{
		 												    				   //The Dialog ID Does Not Exist At All.
		return(ENODOP);	
	}

	if((DCB[Dialog_ID].Dialog_Status&DIALOG_EXIST)==0)					       //The Dialog Box Does Not Exist At All.
	{
		 
		return(ENODOP);
	}

	if(DCB[Dialog_ID].Dialog_Number_Of_Controls==0)							 //There Are Already No Controls.
	{
		 
		return(ENODOP);
	}
	
	switch(DCB[Dialog_ID].CCB[Control_ID]->Category)
	{
		case COMMAND_BUTTON:			       					               //A Command Button.
		{
			Control_Pointer_Union.Command_Button=(struct Command_Button_Property*)(DCB[Dialog_ID].CCB[Control_ID]);		
			
			switch(Mode)
			{																   //Get The Function To Execute.
				case BREAK:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Break);break;	
				case PRESS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Press);break;
				case BREAK_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Break_Pass);break;
				case PRESS_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Command_Button->Action_Press_Pass);break;
				default:Execute_Function=_Sys_GUI_NOP;break;
			}
			  
			return(Execute_Function());

			//break;													  	   //Dummy
		}

		case RADIO_BUTTON:			       								       //A Radio Button.
		{
			Control_Pointer_Union.Radio_Button=(struct Radio_Button_Property*)(DCB[Dialog_ID].CCB[Control_ID]);

			switch(Mode)
			{																   //Get The Function To Execute.
				case BREAK:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Break[Specific_Identifier]);break;	
				case PRESS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Press[Specific_Identifier]);break;
				case BREAK_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Break_Pass[Specific_Identifier]);break;
				case PRESS_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Radio_Button->Action_Press_Pass[Specific_Identifier]);break;
				default:Execute_Function=_Sys_GUI_NOP;break;
			}
			  
			return(Execute_Function());				  

			//break;													        //Dummy
		}

		case TOGGLE:					       							       //A Toggle Button.
		{
			break;
		}

		case CHECK_BOX:				         							       //A Checkbox.
		{	
			Control_Pointer_Union.Check_Box=(struct Check_Box_Property*)(DCB[Dialog_ID].CCB[Control_ID]);

			switch(Mode)
			{																   //Get The Function To Execute.
				case BREAK:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Break);break;	
				case PRESS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Press);break;
				case BREAK_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Break_Pass);break;
				case PRESS_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Check_Box->Action_Press_Pass);break;
				default:Execute_Function=_Sys_GUI_NOP;break;
			}
			  
			return(Execute_Function());

			//break;													       //Dummy
		}

		case TAB:						       						     	   //A Tab.
		{
			if(Specific_Identifier!=TAB_NOT_HEADER)
			{
				Control_Pointer_Union.Tab=(struct Tab_Property*)(DCB[Dialog_ID].CCB[Control_ID]);
	
				switch(Mode)
				{															   //Get The Function To Execute.
					case BREAK:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Tab->Action_Break[Specific_Identifier]);break;	
					case PRESS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Tab->Action_Press);break;
					case BREAK_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Tab->Action_Break_Pass[Specific_Identifier]);break;
					case PRESS_PASS:Execute_Function=(u8 (*)(void))(Control_Pointer_Union.Tab->Action_Press_Pass[Specific_Identifier]);break;
					default:Execute_Function=_Sys_GUI_NOP;break;
				}
				  
				return(Execute_Function());
			}
			else
			{
				Compound_In_Tab=Sys_GUI_Get_Clicked_Control_In_Tab(Dialog_ID,Compound,Coord_X,Coord_Y);

				if((Compound_In_Tab)!=((ENODIA<<8)|ENODIA))
				{
					 
					return(Sys_GUI_Execute_Clicked_Control_Action_In_Tab(Dialog_ID,Compound,Compound_In_Tab,Mode,Coord_X,Coord_Y));
				}
				else
				{
					 													       //If It Gets Here,It Must Have Failed.
					return(ENODOP);	
				}
			}

			//break;												           //Dummy.
		}

		case SCROLL_LIST:				        						       //A Scroll List.
		{
			break;
		}

		case DROPDOWN_MENU:			         							       //A DropDown Menu.
		{
			break;
		}

		case SPIN_BOX:				         							       //A Spin Box.
		{
			break;
		}

		case PROGRESS_BAR:                                                     //A Progress Bar.
		{
		 	break;
		}

		default:break;
	}	

	 													         			   //If It Gets Here,It Must Have Failed.
	return(ENODOP);	
}
#endif
/*Finish Function:Sys_GUI_Execute_Clicked_Control_Action**********************/

/*Begin Function:_Sys_GUI_NOP**************************************************
Description : The "No Operation" Function For The GUI Subsystem.
Input       : Void.
Output      : u8 -Always Return 0.
******************************************************************************/
#if(ENABLE_GUI==TRUE)
u8 _Sys_GUI_NOP(void)
{
	return(0);
}
#endif
/*Finish Function:_Sys_GUI_NOP************************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
