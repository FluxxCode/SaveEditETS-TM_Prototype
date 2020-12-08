#include "tmpch.h"
#include "Config.h"

//Functoins
bool Config::LoadSystemConfig(std::string path, ConsoleWindow* consoleWindow)
{
	std::string line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.find("]") == line.find(":") - 1)
			{
				//Array attribute
				this->arrayConfigs[line.substr(0, line.find("["))].push_back(line.substr(line.find(":") + 1, line.size() - 1)); //Set array attribute
				Utility::AddLog("[Config] " + line);
				continue;
			}
			if (line.find(":") != std::string::npos)
			{
				this->configs[line.substr(0, line.find(":"))] = line.substr(line.find(":") + 1, line.size() - 1); //Set attributes
				Utility::AddLog("[Config] " + line);
			}
		}

		file.close();
	}
	else
	{
		Utility::AddLog("[Config] ERROR: Unable to open file: \"" + path + "\"");
		if (consoleWindow)	consoleWindow->PushMessage("(#err#)ERROR::Config::LoadSystemConfig: Unable to open file: \"" + path + "\"");
		return false;
	}

	return true;
}

bool Config::LoadStyle(std::string styleName, ConsoleWindow* consoleWindow)
{
	std::string path = "Styles\\" + styleName + ".ini";
	if (styleName == "") path = "Styles\\" + this->GetConfig("style") + ".ini";

	std::string line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line))
			if (line.find(":") != std::string::npos)	this->style[line.substr(0, line.find(":"))] = line.substr(line.find(":") + 1, line.size() - 1); //Update attributes

		file.close();
	}
	else
	{
		Utility::AddLog("[Config] ERROR: Unable to open file: \"" + path + "\"");
		if (consoleWindow)	consoleWindow->PushMessage("(#err#)ERROR::Config::LoadSystemConfig: Unable to open file: \"" + path + "\"");
		return false;
	}

	//Load style
	ImGuiStyle& style = ImGui::GetStyle();
	//Sizes
	style.ScrollbarSize = Utility::StringToFloat(this->style["imgui_scrollbar_size"]);
	style.GrabMinSize   = Utility::StringToFloat(this->style["imgui_grab_size"]);

	//Borders
	style.WindowBorderSize = Utility::StringToFloat(this->style["imgui_window_border_size"]);
	style.ChildBorderSize  = Utility::StringToFloat(this->style["imgui_child_border_size"]);
	style.FrameBorderSize  = Utility::StringToFloat(this->style["imgui_frame_border_size"]);
	style.TabBorderSize    = Utility::StringToFloat(this->style["imgui_tab_border_size"]);

	//Roundings
	style.WindowRounding    = Utility::StringToFloat(this->style["imgui_window_rounding"]);
	style.ChildRounding     = Utility::StringToFloat(this->style["imgui_child_rounding"]);
	style.FrameRounding     = Utility::StringToFloat(this->style["imgui_frame_rounding"]);
	style.ScrollbarRounding = Utility::StringToFloat(this->style["imgui_scrollbar_rounding"]);
	style.GrabRounding      = Utility::StringToFloat(this->style["imgui_grab_rounding"]);
	style.TabRounding       = Utility::StringToFloat(this->style["imgui_tab_rounding"]);

	//Alignment
	style.WindowTitleAlign.x = Utility::StringToFloat(this->style["imgui_window_title_align_x"]);
	style.WindowTitleAlign.y = Utility::StringToFloat(this->style["imgui_window_title_align_y"]);
	style.ButtonTextAlign.x  = Utility::StringToFloat(this->style["imgui_button_text_align_x"]);
	style.ButtonTextAlign.y  = Utility::StringToFloat(this->style["imgui_button_text_align_y"]);

	//Colors
	/* Load custom style */
	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		std::string name = Utility::GetConvertedIMGuiStyleColorName(i);
		//R
		std::string convertedName = "imgui_color_" + name + "_r";
		style.Colors[i].x = Utility::StringToFloat(this->style[convertedName]);

		//G
		convertedName = "imgui_color_" + name + "_g";
		style.Colors[i].y = Utility::StringToFloat(this->style[convertedName]);

		//B
		convertedName = "imgui_color_" + name + "_b";
		style.Colors[i].z = Utility::StringToFloat(this->style[convertedName]);

		//A
		convertedName = "imgui_color_" + name + "_a";
		style.Colors[i].w = Utility::StringToFloat(this->style[convertedName]);
	}

	return true;
}

void Config::SaveStyle(std::string styleName, ConsoleWindow* consoleWindow)
{
	ImGuiStyle& style = ImGui::GetStyle();

	//Sizes
	this->style["imgui_scrollbar_size"] = std::to_string(style.ScrollbarSize);
	this->style["imgui_grab_size"] = std::to_string(style.GrabMinSize);

	//Borders
	this->style["imgui_window_border_size"] = std::to_string((int)style.WindowBorderSize);
	this->style["imgui_child_border_size"] = std::to_string((int)style.ChildBorderSize);
	this->style["imgui_frame_border_size"] = std::to_string((int)style.FrameBorderSize);
	this->style["imgui_tab_border_size"] = std::to_string((int)style.TabBorderSize);

	//Rouning
	this->style["imgui_window_rounding"] = std::to_string(style.WindowRounding);
	this->style["imgui_child_rounding"] = std::to_string(style.ChildRounding);
	this->style["imgui_frame_rounding"] = std::to_string(style.FrameRounding);
	this->style["imgui_scrollbar_rounding"] = std::to_string(style.ScrollbarRounding);
	this->style["imgui_grab_rounding"] = std::to_string(style.GrabRounding);
	this->style["imgui_tab_rounding"] = std::to_string(style.TabRounding);

	//Align
	this->style["imgui_window_title_align_x"] = std::to_string(style.WindowTitleAlign.x);
	this->style["imgui_window_title_align_y"] = std::to_string(style.WindowTitleAlign.y);
	this->style["imgui_button_text_align_x"] = std::to_string(style.ButtonTextAlign.x);
	this->style["imgui_button_text_align_y"] = std::to_string(style.ButtonTextAlign.y);

	this->SetConfig("style", styleName);
	//Save own style
	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		std::string name = Utility::GetConvertedIMGuiStyleColorName(i);
		//R
		std::string convertedName = "imgui_color_" + name + "_r";
		this->style[convertedName] = std::to_string(style.Colors[i].x);

		//G
		convertedName = "imgui_color_" + name + "_g";
		this->style[convertedName] = std::to_string(style.Colors[i].y);

		//B
		convertedName = "imgui_color_" + name + "_b";
		this->style[convertedName] = std::to_string(style.Colors[i].z);

		//A
		convertedName = "imgui_color_" + name + "_a";
		this->style[convertedName] = std::to_string(style.Colors[i].w);
	}

	std::string path = "Styles\\" + styleName + ".ini";
	std::ofstream file(path);
	if (file.is_open())
	{
		//Write values
		for (auto const& [key, val] : this->style)
		{
			file << key << ":" << val << std::endl;
		}
		file.close();
		Utility::AddLog("[Config] Saved style in: \"" + path + "\"");
		if (consoleWindow) consoleWindow->PushMessage("Saved style in: \"" + path + "\"");
	}
	else
	{
		Utility::AddLog("[Config] ERROR: Unable to open: \"" + path + "\"");
		if (consoleWindow) consoleWindow->PushMessage("(#err#)ERROR::Config::SaveStyle: Unable to open: \"" + path + "\"");
	}
}

enum class InLabel { YES, NO};
bool Config::LoadLanguages(std::string language)
{
	this->labels.clear();

	InLabel inLabel = InLabel::NO;
	std::string currentLabel = "";
	std::string path = "";

	if (language == "")		path = "Languages\\" + this->configs["language"] + ".txt";
	else					path = "Languages\\" + language + ".txt";

	std::string   line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			//Check if line is currently in an attribute
			if (inLabel == InLabel::NO && line.find(":") != std::string::npos)
			{
				currentLabel = line.substr(0, line.find(":"));
				if (currentLabel.find(" ") == std::string::npos) inLabel = InLabel::YES;
			}

			if (inLabel == InLabel::YES && line.find(";") != std::string::npos)
			{
				if (this->labels[currentLabel].size() != 0) //Get label text
					this->labels[currentLabel] += "\n" + line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1);
				else this->labels[currentLabel] += line.substr(line.find(":") + 1, line.find(";") - line.find(":") - 1);
				inLabel = InLabel::NO;
				//Get argument if exists
				if (line.find(";:") != std::string::npos) //Check if the label has an argument
					this->labelArguments[currentLabel] = line.substr(line.find(";") + 2); //Get argument
			}
			else if (inLabel == InLabel::YES)
			{
				if (this->labels[currentLabel].size() != 0)
					this->labels[currentLabel] += "\n" + line.substr(line.find(":") + 1);
				else this->labels[currentLabel] += line.substr(line.find(":") + 1);
			}
		}

		file.close();
	}
	else
	{
		Utility::AddLog("[Config] ERROR: Unable to open file: \"" + path + "\"");
		return false;
	}

	return true;
}

void Config::SaveSystemConfig()
{
	std::ofstream file("config.ini");
	if (file.is_open())
	{
		//Write attribtues
		for (auto const& [key, val] : this->configs)
			file << key << ":" << val << std::endl;
		//Write array attributes
		for (auto const& [key, val] : this->arrayConfigs)
		{
			for (unsigned int i = 0; i < val.size(); i++)
				file << key << "[" << i << "]:" << val[i] << std::endl;
		}
		file.close();
	}
	else
	{
		Utility::AddLog("[Config] ERROR: Unable to open: \"config.ini\"");
	}
	Utility::AddLog("[Config] Saved system config.");
}

//Cosntructors/Destructors
Config::Config()
{

}

Config::~Config()
{

}

//Functions
bool Config::Load()
{
	bool ret = true; //Return value

	if (!this->LoadSystemConfig())	ret = false;
	if (!this->LoadLanguages())     ret = false;
	if (!this->LoadStyle())		ret = false;

	return ret;
}

void Config::Save()
{
	this->SaveSystemConfig();
}

int Config::GetBiggestLabelSize(std::string arg)
{
	int i = 0;

	for (auto const& [key, val] : *this->GetLabelMap())
	{
		if (this->GetLabelArg(key) == arg)
			i = Utility::ReturnBiggerInt(i, ImGui::CalcTextSize(val.c_str()).x);
	}

	return i;
}

void Config::UpdateLanguage(std::string language, bool save)
{
	this->SetConfig("language", language);
	this->LoadLanguages(language);
	if (save) this->Save();
}