#pragma once

#include "ETS2/SiiFileManager/SiiFileManager.h"
#include "SlaveTrailer.h"

class Trailer
{
private:
	//Varaibles
	SiiFileManager::SiiObject trailer;
	SiiFileManager::SiiObject trailerDef;
	std::string savePath = "";

	std::vector<SiiFileManager::SiiObject*> accessories = { };
	std::vector<SlaveTrailer*> slaveTrailers;

	int bodyDamage    = 0;
	int chassisDamage = 0;
	int	tiresDamage	  = 0;
	int cargoDamage   = 0;
	int dollyCount    = 0;
	float cargoMass	  = 0;
	float trailerMass = 0;

	std::string name = "";

	//Functions
	void GetTrailerName();

public:
	//Constructors/Destructors
	Trailer(std::string savePath);
	virtual ~Trailer();

	//Functions
	void Load();
	void Save();

	//Getter/Setter
	SiiFileManager::SiiObject* GetTrailer() { return &this->trailer; }
	SiiFileManager::SiiObject* GetTrailerDef() { return &this->trailerDef; }

	std::vector<SiiFileManager::SiiObject*>* GetAccessories() { return &this->accessories; }
	std::vector<SlaveTrailer*>* GetSlaveTrailers() { return &this->slaveTrailers; }

	int* GetBodyDamage() { return &this->bodyDamage; }
	int* GetChassisDamage() { return &this->chassisDamage; }
	int* GetTiresDamage() { return &this->tiresDamage; }
	int* GetCargoDamage() { return &this->cargoDamage; }
	float  GetCargoMass() { return this->cargoMass; }
	float GetTrailerMass() { return this->trailerMass; }

	std::string GetName() { return this->name; }
};