#include "tmpch.h"
#include "Utility.h"

namespace fs = std::filesystem;

const ImVec4 Utility::Colors::Blue				   = ImVec4(0.2f,   0.4f,   0.9f,   1.0f);
const ImVec4 Utility::Colors::Green				   = ImVec4(0.2f,   0.9f,   0.4f,   1.0f);
const ImVec4 Utility::Colors::White				   = ImVec4(1.0f,   1.0f,   1.0f,   1.0f);
const ImVec4 Utility::Colors::Black				   = ImVec4(0.0f,   0.0f,   0.0f,   1.0f);
const ImVec4 Utility::Colors::Grey				   = ImVec4(0.85f,  0.85f,  0.85f,  1.0f);
const ImVec4 Utility::Colors::DarkOrange		   = ImVec4(0.9f,   0.3f,   0.2f,   1.0f);
const ImVec4 Utility::Colors::Yellow			   = ImVec4(1.0f,   0.8f,   0.0f,   1.0f);

const ImVec4 Utility::Colors::btnHovered		   = ImVec4(0.101f, 0.152f, 0.239f, 0.6f);
const ImVec4 Utility::Colors::btnActive			   = ImVec4(0.101f, 0.152f, 0.239f, 0.3f);
const ImVec4 Utility::Colors::btnDeselected		   = ImVec4(0.0f,   0.0f,   0.0f,   0.0f);
const ImVec4 Utility::Colors::btnSelected		   = ImVec4(0.101f, 0.152f, 0.3f,   0.9f);

const ImVec4 Utility::Colors::btnDeactive		   = ImVec4(0.101f, 0.152f, 0.239f, 0.3f);
const ImVec4 Utility::Colors::btnDeactiveHovered   = ImVec4(0.101f, 0.152f, 0.239f, 0.3f);
const ImVec4 Utility::Colors::btnDeactivePressed   = ImVec4(0.101f, 0.152f, 0.239f, 0.3f);

const ImVec4 Utility::Colors::btnDeselected2       = ImVec4(0.26f,  0.59f,  0.98f,  0.20f);

const ImVec4 Utility::Colors::btnSelectedHovered   = ImVec4(0.26f,  0.59f,  0.98f,  0.20f);
const ImVec4 Utility::Colors::btnSelectedPressed   = ImVec4();

const ImVec4 Utility::Colors::transperant		   = ImVec4(0.0f,   0.0f,   0.0f,   0.0f);

std::string Utility::GetUsername()
{
	//Get the current username
	TCHAR username[UNLEN + 1];
	DWORD bufSize = UNLEN + 1;
	if (GetUserName((TCHAR*)username, &bufSize)) //Get the username
	{
		std::wstring ws(username); //Convert the username into a wstring
		std::string s(ws.begin(), ws.end()); //Convert the wstring to string
		return s;
	}
	return " ";
}

void Utility::AddLog(std::string log)
{
	std::string path = "log.log";
	std::string str = log;
	if (str.find("ERROR") != std::string::npos || str.find("Error") != std::string::npos)
		str.insert(0, "[ERR] ");
	else if (str.find("WARNING") != std::string::npos || str.find("Warning") != std::string::npos)
		str.insert(0, "[WAR] ");
	else
		str.insert(0, "[INF] ");

	std::ofstream file(path, std::ios::app);
	if (file.is_open())
	{
		std::cout << Utility::GetTime() << ": " << str << std::endl;
		file << Utility::GetTime() << ": " << str << std::endl;
		file.close();
	}
	else
	{
		std::cout << Utility::GetTime() << ": " << str << std::endl;
		std::cerr << Utility::GetTime() << ": [Utility] ERROR: Unable to open file: " << path << std::endl;
	}
}

void Utility::Print(std::string str)
{
	std::cout << Utility::GetTime() << ": " << str << std::endl;
}

std::string Utility::GetTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	std::string str = buf;

	return str;
}

int Utility::RenameFolder(std::string path, std::string newName)
{
	fs::path oldPath(path);
	fs::path newPath(oldPath.parent_path().string() + "\\" + newName);

	if (oldPath.filename().string() == newName)
		return 2;

	try
	{
		fs::rename(oldPath, newPath);
	}
	catch (fs::filesystem_error err) { AddLog("[Utility] ERROR: RenameForlder: " + (std::string)err.what()); return -1; }

	return 1;
}

std::string Utility::StringToHex(const std::string str)
{
	static const char hex_digits[] = "0123456789ABCDEF";

	std::string output;
	output.reserve(str.length() * 2);
	for (unsigned char c : str)
	{
		output.push_back(hex_digits[c >> 4]);
		output.push_back(hex_digits[c & 15]);
	}
	return output;
}

int Utility::StringToInt(const std::string str)
{
	try { return std::stoi(str); }
	catch (std::invalid_argument const& e) { return -1; }
	catch (std::out_of_range const& e)     { return -1; }
}

long Utility::StrinToLong(const std::string str)
{
	try { return std::stol(str); }
	catch (std::invalid_argument const& e) { return -1; }
	catch (std::out_of_range const& e)     { return -1; }
}

long long Utility::StrinToLongLong(const std::string str)
{
	try { return std::stoll(str); }
	catch (std::invalid_argument const& e) { return -1; }
	catch (std::out_of_range const& e)     { return -1; }
}

ImU64 Utility::StringToImU64(const std::string str)
{
	try { return std::stoll(str); }
	catch (std::invalid_argument const& e) { return -1; }
	catch (std::out_of_range const& e)     { return -1; }
}

float Utility::StringToFloat(const std::string str)
{
	try { return std::stof(str); }
	catch (std::invalid_argument const& e) { return -1; }
	catch (std::out_of_range const& e)	   { return -1; }
}

bool Utility::StringToBool(const std::string str)
{
	if (str == "true" || str == "True" || str == "1")		 return true;
	else if (str == "false" || str == "False" || str == "0") return false;

	return false;
}

bool Utility::CharToBool(const char c)
{
	if (c == '1') return true;

	return false;
}

std::string Utility::IntToBinary(int i)
{
	std::string r;
	while (i != 0) { r = (i % 2 == 0 ? "0" : "1") + r; i /= 2; }
	return r;
}

std::string Utility::BoolToString(const bool b)
{
	if (b)
		return "true";

	return "false";
}

double Utility::Round(const double n, unsigned int decimalPlaces)
{
	double scale = 1.0;
	double accu = 10.0;
	unsigned power = 1;

	while (decimalPlaces > 0)
	{
		if (decimalPlaces >= power * 2)
		{
			power *= 2;
			accu *= accu;
		}
		else
		{
			scale *= accu;
			decimalPlaces -= power;
			accu = 10.0;
			power = 1;
		}
	}

	return floor(n * scale + 0.5) / scale;

	return 0.0;
}

std::string Utility::TurnString(std::string str)
{
	std::string s = "";

	for (unsigned int i = 0; i < str.size(); i++)
		s.insert(s.begin(), str[i]);

	return s;
}

std::string Utility::PadLeft(std::string str, int newSize, char insert)
{
	std::string s = str;

	s.insert(s.begin(), newSize - s.size(), insert);

	return s;
}

int Utility::ReturnBiggerInt(int x, int y)
{
	if (x > y) return x;
	if (y > x) return y;
	if (x == y) return x;
}


void Utility::ImGuiToolTip(const std::string str)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 100.0f);
		ImGui::TextUnformatted(str.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

const char* Utility::GetConvertedIMGuiStyleColorName(ImGuiCol idx)
{
	switch (idx)
	{
	case ImGuiCol_Text: return "text";
	case ImGuiCol_TextDisabled: return "text_disabled";
	case ImGuiCol_WindowBg: return "window_bg";
	case ImGuiCol_ChildBg: return "child_bg";
	case ImGuiCol_PopupBg: return "popup_bg";
	case ImGuiCol_Border: return "border";
	case ImGuiCol_BorderShadow: return "border_shadow";
	case ImGuiCol_FrameBg: return "frame_bg";
	case ImGuiCol_FrameBgHovered: return "frame_bg_hovered";
	case ImGuiCol_FrameBgActive: return "frame_bg_active";
	case ImGuiCol_TitleBg: return "title_bg";
	case ImGuiCol_TitleBgActive: return "title_bg_active";
	case ImGuiCol_TitleBgCollapsed: return "title_bg_collapsed";
	case ImGuiCol_MenuBarBg: return "menu_bar_bg";
	case ImGuiCol_ScrollbarBg: return "scrollbar_bg";
	case ImGuiCol_ScrollbarGrab: return "scrollbar_grab";
	case ImGuiCol_ScrollbarGrabHovered: return "scrollbar_grab_hovered";
	case ImGuiCol_ScrollbarGrabActive: return "scrollbar_grab_active";
	case ImGuiCol_CheckMark: return "check_mark";
	case ImGuiCol_SliderGrab: return "slider_grab";
	case ImGuiCol_SliderGrabActive: return "slider_grab_active";
	case ImGuiCol_Button: return "button";
	case ImGuiCol_ButtonHovered: return "button_hovered";
	case ImGuiCol_ButtonActive: return "button_active";
	case ImGuiCol_Header: return "header";
	case ImGuiCol_HeaderHovered: return "header_hovered";
	case ImGuiCol_HeaderActive: return "header_active";
	case ImGuiCol_Separator: return "separator";
	case ImGuiCol_SeparatorHovered: return "separator_hovered";
	case ImGuiCol_SeparatorActive: return "separator_active";
	case ImGuiCol_ResizeGrip: return "resize_grip";
	case ImGuiCol_ResizeGripHovered: return "resize_grip_hovered";
	case ImGuiCol_ResizeGripActive: return "resize_grip_active";
	case ImGuiCol_Tab: return "tab";
	case ImGuiCol_TabHovered: return "tab_hovered";
	case ImGuiCol_TabActive: return "tab_active";
	case ImGuiCol_TabUnfocused: return "tab_unfocused";
	case ImGuiCol_TabUnfocusedActive: return "tab_unfocused_active";
	case ImGuiCol_PlotLines: return "plot_lines";
	case ImGuiCol_PlotLinesHovered: return "plot_lines_hovered";
	case ImGuiCol_PlotHistogram: return "plot_histogram";
	case ImGuiCol_PlotHistogramHovered: return "plot_histogram_hovered";
	case ImGuiCol_TextSelectedBg: return "text_selected_bg";
	case ImGuiCol_DragDropTarget: return "drag_drop_target";
	case ImGuiCol_NavHighlight: return "nav_highlight";
	case ImGuiCol_NavWindowingHighlight: return "nav_windowing_highlight";
	case ImGuiCol_NavWindowingDimBg: return "nav_windowing_dim_bg";
	case ImGuiCol_ModalWindowDimBg: return "modal_window_dim_bg";
	}
	IM_ASSERT(0);
	return "Unknown";
}

void Utility::Copy(fs::path from, fs::path to)
{
	Utility::AddLog("[Updater] Copy: \"" + from.string() + "\" to: \"" + to.string() + "\"");
	try
	{
		fs::copy(from, to);
	}
	catch (fs::filesystem_error err)
	{
		Utility::AddLog("[Updater] ERROR: " + std::string(err.what()));
	}
}

void Utility::CopyDir(fs::path from, fs::path to)
{
	Utility::AddLog("[Updater] Copy directory: \"" + from.string() + "\" to: \"" + to.string() + "\"");
	try
	{
		fs::copy(from, to, fs::copy_options::recursive);
	}
	catch (std::exception& err)
	{
		AddLog("[Updater] ERROR: " + std::string(err.what()));
	}
}

void Utility::RemoveDir(std::string path)
{
	if (fs::exists(path))
		fs::remove_all(path);
	else
		Utility::AddLog("[Updater] WARNING: \"" + path + "\" does not exists");
}

std::string Utility::GetWorkingDir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

float Utility::Map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Utility::HexToFloat(std::string str)
{
	if (str[0] != '&')
		return Utility::StringToFloat(str);

	str = str.erase(0, 1);

	uint32_t num;
	float f;
	sscanf_s(str.c_str(), "%x", &num);  // assuming you checked input
	f = *((float*)&num);

	return f;
}

DWORD Utility::GetProcessID(const wchar_t* name)
{
	DWORD id = 0;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(entry);

		if (Process32First(snap, &entry))
		{
			do
			{
				if (!_wcsicmp(entry.szExeFile, name))
				{
					id = entry.th32ProcessID;
					break;
				}
			} while (Process32Next(snap, &entry));
		}
	}
	CloseHandle(snap);

	return id;
}

uintptr_t Utility::GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName)
{
	uintptr_t moduleBaseAddress = 0;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
	
	if (snap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(entry);
		if (Module32First(snap, &entry))
		{
			do
			{
				if (!_wcsicmp(entry.szModule, moduleName))
				{
					moduleBaseAddress = (uintptr_t)entry.modBaseAddr;
					break;
				}
			} while (Module32Next(snap, &entry));
		}
	}
	else
	{
		std::wstring ws(moduleName);
		Utility::AddLog("[ERR] [Utility] ERROR: Unable to create snapshot. ProcID: " + std::to_string(processID) + ", ModuleName: " + std::string(ws.begin(), ws.end()) + " Code: " + std::to_string(GetLastError()));
	}
	CloseHandle(snap);

	return moduleBaseAddress;
}

uintptr_t Utility::GetOffsetAddress(HANDLE process, uintptr_t moduleAddress, std::vector<unsigned int> offsets)
{
	uintptr_t addr = moduleAddress + offsets[0];
	for (unsigned int i = 1; i < offsets.size(); i++)
	{
		ReadProcessMemory(process, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}