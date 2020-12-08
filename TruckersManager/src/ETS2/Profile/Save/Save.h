#pragma once

#include "ETS2/SiiFileManager/SiiFileManager.h"
#include "Application/Config/Config.h"
#include "Application/Application.h"
#include "ETS2/Profile/Truck/Truck.h"
#include "ETS2/Profile/Trailer/Trailer.h"

/*
	Class for every save
*/

namespace fs = std::filesystem;

class Save
{
private:
	//Variables
	std::string path = ""; //Path of the save folder

	//Objects
	std::vector<SiiFileManager::SiiObject*> objs = { };
	SiiFileManager::SiiObject economyObj  = SiiFileManager::SiiObject();
	SiiFileManager::SiiObject playerObj   = SiiFileManager::SiiObject();
	SiiFileManager::SiiObject bankObj     = SiiFileManager::SiiObject();

	std::string playerTruck = "0";
	std::map<std::string, Truck*> trucks = { };

	std::string playerTrailer = "0";
	std::map<std::string, Trailer*> trailers = { };

	Config* config = config;

	//Functions
	void Init();
	void InitObjects();

public:
	//Constructors/Destructors
	Save(std::string path = ""); //Path = path of the save folder
	virtual ~Save();

	//Functions
	void Load(); //Load the save data
	void SaveData(); //Save the save data in the save file

	//Getter/setter
	SiiFileManager::SiiObject* GetEconomyObj() { return &this->economyObj; }
	SiiFileManager::SiiObject* GetPlayerObj()  { return &this->playerObj; }
	SiiFileManager::SiiObject* GetBankObj()    { return &this->bankObj; }

	std::string GetPlayerTruckID() { return this->playerTruck; }
	std::map<std::string, Truck*>* GetTrucks() { return &this->trucks; }

	std::string GetPlayerTrailerID() { return this->playerTrailer; }
	std::map<std::string, Trailer*>* GetTrailers() { return &this->trailers; }

	std::string GetPath() { return this->path; }
};