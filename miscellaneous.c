 /*****************************************************************************
Filename    : miscellaneous.c
Author      : pry
Date        : 29/04/2012
Description : The Miscellaneous Part For The OS.It Includes 
              A Large part Of The C-Standard Like Functions.
******************************************************************************/

/*Version Information**********************************************************
1.Created By pry                                         On 29/04/2012 Ver1.0.0
   Moved The "Sys_Delay" to here from kernel.c.
2.Modified By pry                                        On 03/05/2012 Ver1.1.0
  Added The "Sys_Strlen" And "Sys_Strcmp"
3.Modified By pry                                        On 24/06/2012 Ver1.2.0
  Corrected The "Sys_Memset".
4.Modified By pry			                             On 23/07/2012 Ver2.0.0
  Corrected Some Spelling Mistakes And Aligned The "***" Lines.
  The Module Was Also Made Configurable.
5.Modified By pry                                        On 26/07/2012 Ver3.0.0
  Corrected A Severe Mistake In The "Sys_Sprintf"(When Printing '%').
******************************************************************************/
							 
/*Includes********************************************************************/
#include "sysconfig.h"
#include "arch.h"
#include "miscellaneous.h"
/*END Includes****************************************************************/

/*Config Reporter*************************************************************/
#if(REPORT_CONFIG==TRUE)
	#if(ENABLE_LIST==TRUE)
		#warning The System(Rookie) List Is Enabled. 
	#else
		#warning The System(Rookie) List Is Disabled.
	#endif

	#if(ENABLE_SYS_LIB==TRUE)
		#warning The System Library Module Is Enabled.
	#else		
		#warning The System Library Module Is Disabled.
	#endif
#endif
/*End Config Reporter*********************************************************/

/*Begin Function:Sys_Create_List***********************************************
Description : Create A Linux-like List.
Input       : struct List_Head* Head-The Pointer To The Head Of The List.The Head Is A Special Node.
Output      : void.
******************************************************************************/
#if(ENABLE_LIST==TRUE)
void Sys_Create_List(struct List_Head* Head)
{
	Head->Prev=Head;
	Head->Next=Head;
}
#endif
/*Finish Function:Sys_Create_List*********************************************/

/*Begin Function:_Sys_Next_In_List*********************************************
Description : Move The Pointer Of The System List To The Next Node Of It.
Input       : struct List_Head* Current -The Current Pointer.
Output      : struct List_Head* -The Pointer To The Next Node.
******************************************************************************/
#if(ENABLE_LIST==TRUE)
INLINE struct List_Head* _Sys_Next_In_List(struct List_Head* Current)
{
	return(Current->Next);
}
#endif
/*Finish Function:_Sys_Next_In_List*******************************************/

/*Begin Function:_Sys_Prev_In_List*********************************************
Description : Move The Pointer Of The System List To The Previous Node Of It.
Input       : struct List_Head* Current -The Current Pointer.
Output      : struct List_Head* -The Pointer To The Previous Node.
******************************************************************************/
#if(ENABLE_LIST==TRUE)
INLINE struct List_Head* _Sys_Prev_In_List(struct List_Head* Current)
{
	return(Current->Prev); 
}
#endif
/*Finish Function:_Sys_Next_In_List*******************************************/

/*Begin Function:Sys_Search_The_List*******************************************
Description : Search The Linux-like List.The Search End Function Should Be Provided By The User In The Following Form:
              u8 Function(struct List_Head* Current,void* Custom_Argument);.The Search Will Stop When The Function Return A "END_SEARCH"(0x01).
			  The Node "From" And "To" Will All Be Searched. 
Input       : struct List_Head* From-The Node To Search From.
              struct List_Head* Next-The Node To Search To.
			  u8 Step -The Maximum Number Of Nodes To Search. 
			  u8* Sequence -The Sequence Of The Node In The List Search."1" Means The First.
			  It Outputs The Value.If The Pointer Is 0,Then It Will Not Give Out The Value.
			  u8 (*Stop_Condition)(struct List_Head*,void*) -The Stop Condition Function.
			  void* Custom_Argument -The Custom Argument. 
Output      : struct List_Head* -The Pointer To The "List" Element Of The Node Found.If Nothing Is Found,It Will Return A "ENONOD"(0x00).
******************************************************************************/
#if(ENABLE_LIST==TRUE)
struct List_Head* Sys_Search_The_List(struct List_Head* From,struct List_Head* To,u8 Step,u8* Sequence,u8 (*Stop_Condition)(struct List_Head*,void*),void* Custom_Argument)	 //Find The Dialog Box In The List. Return The Pointer To The Struct.
{	
	struct List_Head* Current=From;
	u8 Node_Count;
	DISABLE_ALL_INTS();

	if(Sequence!=0)															   //If You Want To Get The Sequence,Then Reset It.
	{
		*Sequence=0;
	}


	for(Node_Count=0;Node_Count<Step;Node_Count++)					           //Only Search Limited Times.															   //Make Sure It Won't Be A Dead One.
	{
		if(Sequence!=0)														   //If You Want To Get The Sequence,Then Increase It.
		{
			*Sequence++;
		}
		if(Stop_Condition(Current,Custom_Argument)==1)						   //The Node Is Found,We Return Now.
			return(Current);																   
		if(Current==To)
			return(ENONOD);													   //The Node Is Not Found.

		Current=Current->Next;												   //Move To The Next Node.
	}

	ENABLE_ALL_INTS();
	return(ENONOD);															   //In Fact,The Function Wouldn't Run To Here.
}
#endif
/*Finish Function:Sys_Search_The_List*****************************************/

/*Begin Function:Sys_List_Delete_Node******************************************
Description : Delete A Node From The Rookie Kernel List.The List Is From Linux.
              It Is A Special Two-Way Circulation List.
Input       : struct List_Head* Prev-The Node Before The Node To Be Deleted.
              struct List_Head* Next-The Node After The Node To Be Deleted.
Output      :
******************************************************************************/
#if(ENABLE_LIST==TRUE)
void Sys_List_Delete_Node(struct List_Head* Prev,struct List_Head* Next)
{
    Next->Prev=Prev;
    Prev->Next=Next;
}
#endif
/*Finish Function:Sys_List_Delete_Node****************************************/

/*Begin Function:Sys_List_Insert_Node******************************************
Description : Insert A Node to The Linux-like list.
              We use static so that it can only be used here.
			  The "inline" is deleted here for code reduction.
Input       : struct List_Head* New-The new node to insert.
			  struct List_Head* Prev-The Previous Node.
			  struct List_Head* Next-The Next Node.
Output      : Void.
******************************************************************************/
#if(ENABLE_LIST==TRUE)
void Sys_List_Insert_Node(struct List_Head* New,struct List_Head* Prev,struct List_Head* Next)
{
	Next->Prev=New;
	New->Next=Next;
	New->Prev=Prev;
	Prev->Next=New;
}
#endif
/*Finish Function:Sys_List_Insert_Node****************************************/

/*Begin Function:Sys_List_Seek*************************************************
Description : Rewind The Pointer Of A Certain List.
Input       : struct List_Head* Current -The Current Pointer To The "List" In The Struct.
			  s8 Rewind_Steps -The Nodes To Rewind.A Negative Number Means Move Backwards.
Output      : struct List_Head* -The Pointer To The "List" In The First Struct.
******************************************************************************/
#if(ENABLE_LIST==TRUE)
struct List_Head* Sys_List_Seek(struct List_Head* Current,s8 Rewind_Steps)
{
	struct List_Head* Current_Pointer=Current;
	u8 Step_Count;
	DISABLE_ALL_INTS();													       //Atomic Operation.

	if(Rewind_Steps>0)													       //Rewind Forward.
	{
		for(Step_Count=0;Step_Count<(Rewind_Steps);Step_Count++)
			Current_Pointer=_Sys_Next_In_List(Current_Pointer);
	}
	else
	{
		for(Step_Count=0;Step_Count<(-Rewind_Steps);Step_Count++)		        //Rewind Backward.		
			Current_Pointer=_Sys_Prev_In_List(Current_Pointer);
	}

	ENABLE_ALL_INTS();		   											        //Atomic Operation.
	return(Current_Pointer);
}
#endif
/*Finish Function:Sys_List_Seek***********************************************/

/*Begin Function:Sys_Delay*****************************************************
Description : Old-Style Delay Some Time.Wastes The CPU Cycle.
              For Application Use.
Input       : u32 Time -The Delay Time,Decided By The Application.Not Accurate.
Output      : Void. 
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
void Sys_Delay(u32 Time)
{
	u32 Time_0;
	u8	Time_1;
	for(Time_0=Time;Time_0>0;Time_0--)
		for(Time_1=DELAYTRIM;Time_1>0;Time_1--);
}
#endif
/*Finish Function:Sys_Delay***************************************************/

/*Begin Function:Sys_Memset****************************************************
Description : The Memset Function.Fills A Certain Part Of Memory With
              The Character Intended.For Application Use.
			  Warning:The Function Has No Memory Overflow Check.
			  This Function Must Be Compiled.
Input       : u8* Address -The Fill Start Address.
			  u8 Char -The Intended Character.
			  u32 Size -The Size To Fill.
Output      : Void. 
******************************************************************************/
void Sys_Memset(u8* Address,u8 Char,u32 Size)		                           //The Type Of The Pointer (Should And) Must Be u8* .If It Is u32*,The Memory Fill Will Easily Cause A Fault Somewhere. 
{
	u8* Address_Ptr=Address;
	u32 Size_Count;
	for(Size_Count=Size;Size_Count>0;Size_Count--)
		*Address_Ptr++=Char;						                           //Fill The Memory.
}
/*Finish Function:Sys_Memset**************************************************/

/*Begin Function:Sys_Strlen****************************************************
Description : The Function For Getting The String Length.
              For Application Use.
Input       : u8* String -The String.
Output      : u32 -The Number Of Characters In The String. 
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
u32 Sys_Strlen(u8* String)
{
	u32 Length_Count=0;
	for(Length_Count=0;(String[Length_Count]!='\0')&&(Length_Count<MAX_STR_LEN);Length_Count++);
	return Length_Count;
}
#endif
/*Finish Function:Sys_Strlen**************************************************/

/*Begin Function:Sys_Strcmp****************************************************
Description : The Function For Comparing The Two Strings.
              For Application Use.
Input       : u8* String_0 -The First String.
              u8* String_1 -The Second String.
			  u32 Length -The Length To Compare.In Characters.If The 
			  Length Is Bigger Than One Of The Strings,It Will Not Be Valid.
Output      : u8 -The Result.
              0  -The Two Are The Same Within The Compare Length.
			  1  -The First Is Bigger Than The Second.
			  2  -The First Is Smaller Than The Second. 
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
u8 Sys_Strcmp(u8* String_0,u8* String_1,u32 Length)
{
	u32 Length_Count=0;
	for(Length_Count=0;Length_Count<((Length>MAX_STR_LEN)?MAX_STR_LEN:Length);Length_Count++)
	{
		if(String_0[Length_Count]>String_1[Length_Count])
			return(1);
		if(String_0[Length_Count]<String_1[Length_Count])
			return(2);
		if(String_0[Length_Count]=='\0')
			return(0);										                   //If It Ever Gets Here,Then The Two Strings Must Be Totally The Same,And The "Length" Exceeded Both Of Their True Length.
	}
	return(0);												                   //This Inicates That The Two Are The Same Within The "Length".
}
#endif
/*Finish Function:Sys_Strcmp**************************************************/

/*Begin Function:Sys_Strcpy****************************************************
Description : The Function For Duplicating The Strings.
              For Application Use.
Input       : u8* String_0 -The First String,Source.
              u8* String_1 -The Second String,Duplication.
			  u32 Length -The Length To Copy.In Characters,The'\0' Included.
			  If The Length Is Bigger Than "MAX_STR_LEN",It Will Not Be Valid.
			  Take Note The Function Has NO Memory Overflow Check.Take EXTRA care!
Output      : Void. 
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
void Sys_Strcpy(u8* String_0,u8* String_1,u32 Length)
{
	u32 Length_Count=0;
	for(Length_Count=0;Length_Count<((Length>MAX_STR_LEN)?MAX_STR_LEN:Length)-1;Length_Count++)
	{
		String_1[Length_Count]=String_0[Length_Count];		                   //If It Ever Gets Here,Then The Two Strings Must Be Totally The Same,And The "Length" Exceeded Both Of Their True Length.
	}
	String_1[((Length>MAX_STR_LEN)?MAX_STR_LEN:Length)-1]='\0';                //Add The '\0'.
}
#endif
/*Finish Function:Sys_Strcpy**************************************************/

/*Begin Function:Sys_Sprintf***************************************************
Description : The Sprintf Function.C-standard Like.
              Format Control String: (x)is A Integer From 0~9.
			  %(x)d -Print As Integer. The (x) Indicates The Digit Format,If It Is Bigger Than The Real One,
			  Then The Higher Digits Will Be '0'.Otherwise The (x) Won't take Effect.(x)Can Be Omitted.
			  %(x)f -Print As Float.The (x) Indicates The Digit Format---The Digits Of The Fraction Part.
			  When Omitted,The Default Value Is 6.
			  %s -Print As String.
Input       : s8* Buffer -The Pointer To The Buffer,Which Must Be Big Enough.
              Take extra Note That The Function has No memory Overflow Check.
			  s8* String -The String Input,including The Format control characters.
			  s32* Args_Int -The Integer Argument Array.Note That They Must All Be Integers.
			  double* Args_Double -The Double Argument Array.Note That They Must All Be Doubles.
			  s8** Args_String -The String Argument Array.Note That They Must All Be Strings.
			  Why It Doesn't Have Changeable Argument List is That The ARMCC Doesn't Support It.
Output      : Void.
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
void Sys_Sprintf(s8* Buffer,s8* String,s32* Args_Int,double* Args_Double,s8** Args_String)
{
	s32* Ints_Ptr=Args_Int;
	s32 Int_Ptr_Value;                                                         //The Value Of The"Ints_Ptr".
	s32 Integer_High_Pos;       											   //The MSD(Most Significant Digit) Position Of The Integer.

	double* Doubles_Ptr=Args_Double;
	double Double_Ptr_Value;           										   //The Integer Part Of The Double Number.
	s32 Double_To_Int;                										   //Convert The "double" into "s32",e.g. 12.73-->12.
	s32 Double_To_Int_High_Pos;       										   //The MSD Position of Integer value Of "Double".

	s8** Strings_Ptr=Args_String;
	s8* String_Ptr_Value=*Strings_Ptr;

	s8* Buffer_Ptr=Buffer;
	s8* String_Ptr=String;
	
	while(*String_Ptr!='\0')
	{
		if(*String_Ptr!='%')
		{
			*Buffer_Ptr++=*String_Ptr++;
			continue;
		}

		switch(*(String_Ptr+1))												   //When It Goes Here,The "*String_Ptr" Must Be "%".
		{
			case '%':														   //If The Output Intended is '%'.The String Pointer Should Increase 2,Not 1.
			{																   
				*Buffer_Ptr++='%';	
				String_Ptr+=2;	
				break;
			}   
			      													
			case 'd':
			{
				String_Ptr+=2;                                         		   //Increase The Ptr By 2.
				Int_Ptr_Value=*Ints_Ptr;

				if(Int_Ptr_Value<0)                                            //If It Is A Number Smaller Than 0.
				{
					*Buffer_Ptr++='-';                                         //A Negative Number.
					Int_Ptr_Value=-Int_Ptr_Value;                      		   //Reverse Itself.
				}													   		   //Get The Highest Position Of The Integer.

				if(Int_Ptr_Value==0)										   //If The Number Is Zero.(Treat It Differently).
				{
					*Buffer_Ptr++='0';										   //Print The "0".
					Ints_Ptr++;
					break;													   //Break Directly.
				}

				Integer_High_Pos=_Sys_Query_Positive_Int_Highest(Int_Ptr_Value);
				for(Integer_High_Pos-=1;Integer_High_Pos>=0;Integer_High_Pos--)
					*Buffer_Ptr++=Int_Ptr_Value%_Sys_Sci(Integer_High_Pos+1)/_Sys_Sci(Integer_High_Pos)+0x30;

				Ints_Ptr++;											   		   //Turn Into ASCII
				break;
			}

			case 'f':
			{
				String_Ptr+=2;									       		   //Increase The Ptr By 2.
				Double_Ptr_Value=*Doubles_Ptr;

				if(Double_Ptr_Value<0)                                 		   //If It Is A Number Smaller Than 0.
				{
					*Buffer_Ptr++='-';							       		   //A Negative Number.
					Double_Ptr_Value=-Double_Ptr_Value;                		   //Reverse Itself.
				}

				Double_To_Int=(s32)(Double_Ptr_Value);

				if(Double_To_Int==0)
				{
					*Buffer_Ptr++='0';							       		   //The Number Has No Integer Field.
				}

				Double_To_Int_High_Pos=_Sys_Query_Positive_Int_Highest(Double_To_Int);
																	   		   //Get The Highest Position Of The Integer
				for(Double_To_Int_High_Pos-=1;Double_To_Int_High_Pos>=0;Double_To_Int_High_Pos--)
					*Buffer_Ptr++=Double_To_Int%_Sys_Sci(Double_To_Int_High_Pos+1)/_Sys_Sci(Double_To_Int_High_Pos)+0x30;
																	   		   //Turn Into ASCII
				*Buffer_Ptr++='.';                                     		   //The Dot.
				
				Double_Ptr_Value-=(double)Double_To_Int;               		   //Get The Fraction part.
				for(Double_To_Int_High_Pos=4;Double_To_Int_High_Pos>=0;Double_To_Int_High_Pos--)
				{
					Double_Ptr_Value*=10;                              		   //When The Float Number is stored in the computer,it is not definitely accurate(e.g. 7.3342 is 7.33419991).So We Can't use The Double-(s32)(Double) Method.
					*Buffer_Ptr++=((s32)Double_Ptr_Value%10)+0x30;     		   //Turn Into ASCII
				}
				Doubles_Ptr++;

				break;
			}
			case 's':
			{
				String_Ptr+=2;
				while(*String_Ptr_Value!='\0')
					*Buffer_Ptr++=*String_Ptr_Value++;
			}

			default:
			{
				switch(*(2+String_Ptr))
				{
					case 'd':
					{
						String_Ptr+=3;                         				   //Increase The Ptr By 3.(3 characters.)
						Int_Ptr_Value=*Ints_Ptr;
						if(Int_Ptr_Value<0)                    				   //If It Is A Number Smaller Than 0.
						{
							*Buffer_Ptr++='-';                 				   //A Negative Number.
							Int_Ptr_Value=-Int_Ptr_Value;      				   //Reverse Itself.
						}									   				   //Get The Highest Position Of The Integer.Choose The Bigger Between The Two.			   
						Integer_High_Pos=((*(String_Ptr-2)-0x30)>_Sys_Query_Positive_Int_Highest(Int_Ptr_Value))?(*(String_Ptr-2)-0x30):_Sys_Query_Positive_Int_Highest(Int_Ptr_Value);
						for(Integer_High_Pos-=1;Integer_High_Pos>=0;Integer_High_Pos--)
							*Buffer_Ptr++=Int_Ptr_Value%_Sys_Sci(Integer_High_Pos+1)/_Sys_Sci(Integer_High_Pos)+0x30;
						Ints_Ptr++;							   				   //Turn Into ASCII.If The Input Format Asks For More Digits than The MSD Position,output'0'instead.
						break;
					}

					case 'f':
					{
						String_Ptr+=3;					       				   //Increase The Ptr By 3.

						Double_Ptr_Value=*Doubles_Ptr;

						if(Double_Ptr_Value<0)                 				   //If It Is A Number Smaller Than 0.
						{
							*Buffer_Ptr++='-';				   				   //A Negative Number.
							Double_Ptr_Value=-Double_Ptr_Value;				   //Reverse Itself.
						}

						Double_To_Int=(s32)(Double_Ptr_Value);

						if(Double_To_Int==0)
						{
						*Buffer_Ptr++='0';					   				   //The Number Has No Integer Field.
						}

						Double_To_Int_High_Pos=_Sys_Query_Positive_Int_Highest(Double_To_Int);
															   				   //Get The Highest Position Of The Integer.
						for(Double_To_Int_High_Pos-=1;Double_To_Int_High_Pos>=0;Double_To_Int_High_Pos--)
							*Buffer_Ptr++=Double_To_Int%_Sys_Sci(Double_To_Int_High_Pos+1)/_Sys_Sci(Double_To_Int_High_Pos)+0x30;
															   				   //Turn Into ASCII
						*Buffer_Ptr++='.';                     				   //The Dot.
															   				   //Get The Fraction part.
						Double_Ptr_Value-=(double)Double_To_Int;
						for(Double_To_Int_High_Pos=(*(String_Ptr-2)-0x30-1);Double_To_Int_High_Pos>=0;Double_To_Int_High_Pos--)
						{
							Double_Ptr_Value*=10;              				   //When The Float Number is stored in the computer,it is not definitely accurate(e.g. 7.3342 is 7.33419991).So We Can't use The Double-(s32)(Double) Method.
							*Buffer_Ptr++=((s32)Double_Ptr_Value%10)+0x30;
						}									   				   //Turn Into ASCII
						Doubles_Ptr++;

						break;
					}

					default:break;
				}

				String_Ptr+=1;
				break;												   		   //No Format Follows It.Abort.
			}
		}
	}

	*Buffer_Ptr='\0';                                                          //Add The String End Identifier.
}
#endif
/*Finish Function:Sys_Sprintf*************************************************/

/*Begin Function:Sys_Sscanf****************************************************
Description : The Sscanf Function.C-standard Like.The Reverse Process Of "sprintf".
              Format Control String:
			  %d -Read The String As A Integer.
			  %f -Read As Float.
Input       : s8* String -The Pointer To The String To Be Read.
              Take extra Note That The Function has No memory Overflow Check.
			  s8* Foramt -The Format Control String.
			  s32* Args_Int -The Integer Argument Array.Stores All The Output Integers.
			  double* Args_Double -The Double Argument Array.Note That They Must All Be Doubles.
Output      : u8 -If Successful,return"0";else"1".
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
void Sys_Sscanf(s8* String,s8* Format,s32* Args_Int,double* Args_Double)		
{
	s8 Sign_Flag;

	s32* Ints_Ptr=Args_Int;
	s32 Int_Ptr_Value;			                                               //The Value Of The"Ints_Ptr".

	double* Doubles_Ptr=Args_Double;
	double Double_Ptr_Value;                                                   //The Integer Part Of The Double Number.
	s8 Dot_Count;
	s8* String_Ptr=String;
	s8* Format_Ptr=Format;
	
	while(*Format_Ptr!='\0')      											   //If The Format did not end...
	{
		if(((*String_Ptr)>'9')||((*String_Ptr)<'0'))
		{
			String_Ptr++;                                                      //Not A Number.Next Character.
			continue;
		}
		switch(*(Format_Ptr+1))
		{
			case 'd':
			{
				Format_Ptr+=2;                                         		   //Increase The Ptr By 2.
				if(*(String_Ptr-1)=='-')
					Sign_Flag=(s8)-1;
				else
					Sign_Flag=1;                                       		   //Whether It Is A Negative Number.

				Int_Ptr_Value=0;                                       		   //Clear The Value First.
																	   		   //Directly Write.
				for(;((*String_Ptr)>='0')&&((*String_Ptr)<='9');String_Ptr++)
				{
					Int_Ptr_Value=Int_Ptr_Value*10+(*String_Ptr)-0x30; 		   //Turn Into Plain Number.
				}

				*Ints_Ptr++=Int_Ptr_Value*Sign_Flag;
				break;
			}
			case 'f':
			{
				Format_Ptr+=2;									       		   //Increase The Ptr By 2.
					
				if(*(String_Ptr-1)=='-')                               		   //Fractions Like ".01","-.01" is Not Allowed.
					Sign_Flag=(s8)-1;
				else
					Sign_Flag=1;                                       		   //Whether It Is A Negative Number.

				Double_Ptr_Value=0;                                    		   //Clear The Value First.
																	   		   //Directly Write.
				for(;((*String_Ptr)>='0')&&((*String_Ptr)<='9');String_Ptr++)
					Double_Ptr_Value=Double_Ptr_Value*10+*String_Ptr-0x30; 
																	   		   //Turn Into Plain Number.
				if(*(String_Ptr++)!='.')                               		   //If There Is No Fraction Dot,Abort.
					break;
																	   		   //Directly Write.
				for(Dot_Count=0;((*String_Ptr)>='0')&&((*String_Ptr)<='9');String_Ptr++,Dot_Count++)
					Double_Ptr_Value=Double_Ptr_Value*10+(*String_Ptr)-0x30; 
																	   		   //Turn Into Plain Number.
				Double_Ptr_Value=Double_Ptr_Value*Sign_Flag/(double)_Sys_Sci(Dot_Count);

				*Doubles_Ptr++=Double_Ptr_Value;

				break;
			}
			default:break;
		}
	}
}
#endif
/*End Function:Sys_Sscanf*****************************************************/

/*Begin Private Function:_Sys_Query_Positive_Int_Highest*
Description : Get The Digits Of A Positive Integer.
Input       : s32 Integer -The Integer To Get The Digits.
Output      : u8 -The Digits Of The Integer.
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
u8 _Sys_Query_Positive_Int_Highest(s32 Integer)
{
	s8 Count=1;
	for(Count=1;Integer!=0;Integer/=10,Count++);
	return(Count-1);
}
#endif
/*Finish Private Function:_Sys_Query_Positive_Int_Highest*********************/

/*Begin Private Function:_Sys_Sci**********************************************
Description : The Function Calculating The 10-Based Exponent,all-integer.
Input       : s32 Exp -The Exponent.
Output      : s32 -The Result.
******************************************************************************/
#if(ENABLE_SYS_LIB==TRUE)
s32 _Sys_Sci(s32 Exp)     
{
	s32 Ret_Val=1;
	for(;Exp>0;Exp--)
		Ret_Val*=10;
	return(Ret_Val);
}
#endif
/*Finish Private Function:_Sys_Sci********************************************/

/*End Of File*****************************************************************/

/*Copyright (C) 2011-2013 pry. All rights reserved.***************************/
