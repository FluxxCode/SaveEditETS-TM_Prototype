#include "tmpch.h"
#include "Profile.h"
#include "ETS2/SiiFileManager/SiiFileManager.h"

//Functions
void Profile::LoadImages()
{
	//Face
	this->driverImg = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\driver\\" + this->obj.attributes["face"] + ".dds");
	if (this->driverImg == 0)		Utility::AddLog("[Profile] ERROR: Soil loading error: \"Resources\\ets\\material\\ui\\driver\\" + this->obj.attributes["face"] + ".dds\"");  //Error occured

	//Brand
	this->brandImg = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\accessory\\" + this->obj.attributes["brand"] + ".png");
	if (this->brandImg == 0)		Utility::AddLog("[Profile] ERROR: Soil loading error: \"Resources\\ets\\material\\ui\\accessory\\" + this->obj.attributes["brand"] + ".png\"");  //Error occured

	//Logo
	this->logoImg = this->resourceManager->LoadTexture("Resources\\ets\\material\\ui\\player_logo\\" + this->obj.attributes["logo"] + ".png");
	if (this->logoImg == 0)			Utility::AddLog("[Profile] ERROR: Soil loading error: \"Resources\\ets\\material\\ui\\player_logo\\" + this->obj.attributes["logo"] + ".png\"");  //Error occured
}

void Profile::LoadSave()
{
	this->savePath = this->path + "\\save\\" + this->saveFolderName;
	this->save = new Save(this->savePath);
}

void Profile::SaveConfig()
{
	std::ofstream file(this->path + "\\config.cfg");
	if (file.is_open())
	{
		file << "# prism3d variable config data" << "\n\n";
		//Write values
		for (auto const& [key, val] : this->profileConfig)
		{
			file << "uset " << key << " \"" << val << "\"" << std::endl;
		}
		file.close();
	}
	else
	{
		Utility::AddLog("[Profile] ERROR: Unable to open: \"config.ini\"");
	}
	Utility::AddLog("[Profile] Saved profile config.");
}

void Profile::OnEnd()
{
	if (this->save) delete this->save;
}

//Constructors/Destructors
Profile::Profile(Config* config, Utility::ResourceManager* resourceManager, std::string path)
	: config(config), resourceManager(resourceManager)
{
	if (path.size() > 0)
	{
		this->path = path;
		this->LoadProfileData(path);
	}
}

Profile::~Profile()
{

}

//Functions
void Profile::LoadProfileData(std::string path)
{
	fs::path p(path);
	fs::path profPath(path + "\\profile.sii"); //Path of the profile.sii file

	this->folder = p.filename().string(); //Set file name
	this->path = path; //Set file path

	Utility::AddLog("[Profile] Loading profile: " + profPath.string() + "... ");
	SiiFileManager::Decrypt(profPath.string()); //Decrypt profile and return if error occurred

	if (!SiiFileManager::LoadObject(profPath.string(), &this->obj)) //Load the obj
	{
		Utility::AddLog("[Profile] ERROR: Unable to load user_profile: \"" + profPath.string() + "\"");
		return;
	}

	this->name = this->obj.attributes["profile_name"];
}

void Profile::Load()
{
	this->LoadImages();
	this->LoadSave();
	this->LoadConfig();
}

void Profile::LoadConfig()
{
	std::string line;
	std::ifstream file(this->path + "\\config.cfg");
	if (file.is_open())
	{
		while (getline(file, line))
			if (line.find("uset") != std::string::npos)
			{
				std::string str = line.substr(line.find(" ") + 1);
				std::string conf = str.substr(0, str.find(" "));
				std::string val = str.substr(str.find(" ") + 2);
				val.erase(val.size() - 1);
				this->profileConfig[conf] = val; //Update attributes
			}

		file.close();
	}
	else
	{
		Utility::AddLog("[Profile] ERROR: Unable to open file: \"" + path + "\"");
	}
}

void Profile::SaveData(bool config)
{
	if (config) //Create backup
	{
		//Create backup from config.cfg
		fs::remove(this->path + "\\config_save_edit_backup.cfg");
		fs::copy(this->path + "\\config.cfg", this->path + "\\config_save_edit_backup.cfg");
	}
	this->SaveConfig();

	std::string hex = Utility::StringToHex(this->GetAttribute("profile_name"));


	Utility::AddLog("[Profile] Saving profile...");

	SiiFileManager::OverwriteObject(this->path + "\\profile.sii", &this->obj); //Save profile

	this->save->SaveData(); //Save save

	//Save the name
	int result = Utility::RenameFolder(this->path, hex); //Rename the folder

	if (result != 2)
	{
		this->changedName = true;
		this->OnEnd();
	}
}