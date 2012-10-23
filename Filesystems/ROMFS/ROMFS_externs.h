
struct On_ROM_MFT
{
	u32 FID;
	u8 Name[8];
	u8 Mode;

	u32 Length;
	u32 File_Start_Address;
};

struct On_ROM_Scatter_File
{
	u32 FID
	u32	File_Prev_Address
	u8  Data[20];
	u32 File_Next_Address
};