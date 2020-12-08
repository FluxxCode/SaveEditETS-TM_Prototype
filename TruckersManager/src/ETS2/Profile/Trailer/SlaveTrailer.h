#pragma once

#include "ETS2/SiiFileManager/SiiFileManager.h"

class SlaveTrailer
{
private:
	//Variables
	std::string savePath;
	std::vector<SlaveTrailer*>* slaveTrailers;
	std::vector<SiiFileManager::SiiObject*> accessories = { };
	SiiFileManager::SiiObject obj;

public:
	//Variables
	int bodyDamage    = 0;
	int chassisDamage = 0;
	int	tiresDamage   = 0;
	int cargoDamage	  = 0;
	float cargoMass	  = 0;

	bool isDolly = false;

	//Constructors/Destructors
	SlaveTrailer(std::string savePath, std::vector<SlaveTrailer*>* slaveTrailers, std::string id);
	virtual ~SlaveTrailer();

	//Functions
	void Load();
	void Save(int bodyDamage, int chassisDamage, int tiresDamage, int cargoDamage);

	//Getter/Setter
	std::vector<SiiFileManager::SiiObject*>* GetAccessories() { return &this->accessories; }
};