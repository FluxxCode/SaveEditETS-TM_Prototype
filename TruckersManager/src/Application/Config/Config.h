#pragma once

#include "Scene/SaveEditScene/ImGUI/ConsoleWindow/ConsoleWindow.h"

/*
	Class that contains every config
*/

class Config
{
private:
	//Variables
	std::map<std::string, std::string> configs; //All configurations
	std::map<std::string, std::vector<std::string>> arrayConfigs; //All array configs
	std::map<std::string, std::string> style; //Current style
	std::map<std::string, std::string> labels; //Labels
	std::map<std::string, std::string> labelArguments; //Text/Argument that is behind the ;

	//Functions
	bool LoadLanguages(std::string language = "");

	void SaveSystemConfig();

public:
	//Cosntructors/Destructors
	Config();
	virtual ~Config();

	//Functions
	void UpdateLanguage(std::string language, bool save = true); //Save = save the config if its true

	bool Load();
	bool LoadSystemConfig(std::string path = "config.ini", ConsoleWindow* consoleWindow = NULL); //Load system config
	bool LoadStyle(std::string styleName = "", ConsoleWindow* consoleWindow = NULL);
	void SaveStyle(std::string styleName, ConsoleWindow* consoleWindow = NULL);
	void Save();

	int GetBiggestLabelSize(std::string arg = ""); //Returns the size of the biggest label. arg = only labels with the argument arg will be used

	//Getter/Setter
	std::string GetConfig(std::string config) { return this->configs[config]; }
	void SetConfig(std::string conf, std::string arg) { this->configs[conf] = arg; }

	std::vector<std::string> GetArrayConfig(std::string config) { return this->arrayConfigs[config]; }
	void SetArrayConfig(std::string key, std::vector<std::string> config) { this->arrayConfigs[key] = config; }

	std::string GetLabel(std::string label) { return this->labels[label]; }
	std::map<std::string, std::string>* GetLabelMap() { return &this->labels; } //Get the labels map
	std::string GetLabelArg(std::string label) { return this->labelArguments[label]; }  //Get argument of a label

	std::string GetStyleVal(std::string val) { return this->style[val]; } //Get a style value
	void SetStyleVal(std::string key, std::string val) { this->style[key] = val; } //Set a style value
};