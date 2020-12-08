#pragma once

#include "ETS2/SiiFileManager/SiiFileManager.h"

class Truck
{
private:
	//Varaibles
	SiiFileManager::SiiObject truck;

	std::vector<SiiFileManager::SiiObject*> accessories = { };

	int fuel = 0;
	int chassisDamage = 0;
	int cabinDamage = 0;
	int engineDamage = 0;
	int transmissionDamage = 0;
	int tiresDamage = 0;

	std::string name = "";
	std::string fileName = "";
	std::string licensePlate = "";

	//Functions
	std::string ConvertTruckName(std::string str); //Convert the truckFileName to the truck name

public:
	//Constructors/Destructors
	Truck();
	virtual ~Truck();

	//Functions
	void Load();
	void Save();

	//Getter/Setter
	SiiFileManager::SiiObject* GetTruck() { return &this->truck; }
	
	std::vector<SiiFileManager::SiiObject*>* GetAccessories() { return &this->accessories; }

	int* GetFuel() { return &this->fuel; }
	int* GetChassisDamage() { return &this->chassisDamage; }
	int* GetCabinDamage() { return &this->cabinDamage; }
	int* GetEngineDamage() { return &this->engineDamage; }
	int* GetTransmissionDamage() { return &this->transmissionDamage; }
	int* GetTiresDamage() { return &this->tiresDamage; }

	std::string GetName() { return this->name; }
	std::string GetFileName() { return this->fileName; }
	std::string GetLicensePlate() { return this->licensePlate; }
};