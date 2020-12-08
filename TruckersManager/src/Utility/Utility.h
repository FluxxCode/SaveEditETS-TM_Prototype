#pragma once

namespace fs = std::filesystem;

struct Colors
{
	static const ImVec4 Blue;
	static const ImVec4 Green;
	static const ImVec4 White;
	static const ImVec4 Black;
	static const ImVec4 Grey;
	static const ImVec4 DarkOrange;
	static const ImVec4 Yellow;

	static const ImVec4 btnHovered;
	static const ImVec4 btnActive;
	static const ImVec4 btnDeselected;
	static const ImVec4 btnSelected;

	static const ImVec4 btnDeactive;
	static const ImVec4 btnDeactiveHovered;
	static const ImVec4 btnDeactivePressed;

	static const ImVec4 btnDeselected2;

	static const ImVec4 btnSelectedHovered;
	static const ImVec4 btnSelectedPressed;

	static const ImVec4 transperant;
};

std::string GetUsername();

void AddLog(std::string log); //Write log in file and print it to the console
void Print(std::string str); //Prints str to the console with current time

std::string GetTime(); //Return the current time

int RenameFolder(std::string path, std::string newPath); //Renames a folder

std::string StringToHex(    const std::string str); //Converts the string to hex
int			StringToInt(    const std::string str); //Converts a string to a int
long		StrinToLong(    const std::string str); //Converts a string to a long
long long   StrinToLongLong(const std::string str); //Converts a string to a long long
ImU64		StringToImU64(  const std::string str); //Converts a string to a ImU64
float		StringToFloat(  const std::string str); //Converts a string to a float
bool		StringToBool(   const std::string str); //Converts a string to a bool
bool		CharToBool(     const char c); //Converts a char to bool. Char has to be 0 or 1
std::string IntToBinary(    int i); //Conerts a int into binary
std::string BoolToString(   const bool b); //Converts a bool to a string
float		HexToFloat(		std::string str); //Converts a hex value to a float

double Round( const double n, unsigned int decimalPlaces); //Round to a specific decimal place
float Map(long x, long in_min, long in_max, long out_min, long out_max); 

std::string TurnString(std::string str); //Turnes the string around. 
std::string PadLeft(   std::string str, int newSize, char insert); //Makes the size of the string to new size by adding insert infront of the string

int ReturnBiggerInt(int x, int y); //Sets num to newNum if newNum is bigger than num

void ImGuiToolTip(const std::string str);
const char* GetConvertedIMGuiStyleColorName(ImGuiCol idx); //Returns converted imgui style color name.

void Copy(fs::path from, fs::path to); //Copy file
void CopyDir(fs::path from, fs::path to); //Copy directory
void RemoveDir(std::string path); //Remove directory

std::string GetWorkingDir(); //Returns the current working directory

DWORD GetProcessID(const wchar_t* name); //Returns the process name by ID
uintptr_t GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName); //Returns the base address of a process module
uintptr_t GetOffsetAddress(HANDLE process, uintptr_t moduleAddress, std::vector<unsigned int> offsets); //Returns the address with offset