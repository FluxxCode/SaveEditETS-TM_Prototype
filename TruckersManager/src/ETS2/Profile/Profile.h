#pragma once

#include "Save/Save.h"

/*
	Class for every ETS2 Profile
*/

namespace fs = std::filesystem;

class Profile
{
private:
	//Variables
	std::string folder = ""; //Profile folder name
	std::string path = ""; //Profile folder path
	std::string name = ""; //Profile name

	SiiFileManager::SiiObject obj = SiiFileManager::SiiObject("", "user_profile"); //Profile obj

	GLuint* driverImg = NULL; //Image of the driver
	GLuint* brandImg = NULL; //Image of the brand
	GLuint* logoImg = NULL; //Logo 

	std::string saveFolderName = ""; //Save folder name
	std::string saveDisplayStr = "";  //Save display string
	std::string savePath = ""; //Path of the save

	Save* save = NULL; //Save that gets loaded
	Config* config;
	Utility::ResourceManager* resourceManager;

	std::map<std::string, std::string> profileConfig; //Profile config (config.cfg in profile folder)

	//Callbacks
	std::function<void()> changeProfileCallback; //Change profile

	//Functions
	void LoadProfileData(std::string path); //Only the folder, path, name and companyName
	void LoadImages();
	void LoadSave();

	void SaveConfig();

	void OnEnd();

public:
	//Variables
	bool changedName = false;

	//Constructors/Destructors
	Profile(Config* config, Utility::ResourceManager* resourceManager, std::string path = NULL);
	virtual ~Profile();

	//Functions
	void Load(); //Load everything
	void LoadConfig();

	void SaveData(bool backup); //Save everything

	//Getter/Setter
	std::string GetFolder() { return this->folder; }
	void SetFolder(std::string str) { this->folder = str; }

	std::string GetPath() { return this->path; }
	void SetPath(std::string str) { this->path = str; }

	std::string GetName() { return this->name; }
	void SetName(std::string str) { this->name = str; }

	std::string GetAttribute(std::string attribute) { return this->obj.attributes[attribute]; }
	void SetAttribute(std::string attribute, std::string value) { this->obj.attributes[attribute] = value; }

	GLuint* GetDriverImg() { return this->driverImg; }
	void SetDriverImg(GLuint* img) { this->driverImg = img; }
	GLuint* GetBrandImg() { return this->brandImg; }
	void SetBrandImg(GLuint* img) { this->brandImg = img; }
	GLuint* GetLogoImg() { return this->logoImg; }
	void SetLogoImg(GLuint* img) { this->logoImg = img; }

	std::string GetSaveFolderName() { return this->saveFolderName; }
	void SetSaveFolderName(std::string name) { this->saveFolderName = name; }
	std::string GetSaveDisplayStr() { return this->saveDisplayStr; }
	void SetSaveDisplayStr(std::string str) { this->saveDisplayStr = str; }

	Save* GetSave() { return this->save; }

	std::string GetConfig(std::string conf) { return this->profileConfig[conf]; }
	void SetConfig(std::string conf, std::string val) { this->profileConfig[conf] = val; }
};