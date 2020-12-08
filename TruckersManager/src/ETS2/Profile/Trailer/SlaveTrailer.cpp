#include "tmpch.h"
#include "SlaveTrailer.h"

//Constructors/Destructors
SlaveTrailer::SlaveTrailer(std::string savePath, std::vector<SlaveTrailer*>* slaveTrailers, std::string id)
	: savePath(savePath), slaveTrailers(slaveTrailers)
{
	this->obj.id = id;
	SiiFileManager::LoadObject(savePath, &this->obj);
	if (this->obj.attributes["slave_trailer"] != "null")
		slaveTrailers->push_back(new SlaveTrailer(savePath, slaveTrailers, this->obj.attributes["slave_trailer"]));
	for (int i = 0; i < this->obj.arraySizes["accessories"]; i++)
		this->accessories.push_back(new SiiFileManager::SiiObject(this->obj.arrayAttributes["accessories"][i], ""));
}

SlaveTrailer::~SlaveTrailer()
{
	for (int i = 0; i < this->accessories.size(); i++)
		delete this->accessories[i];
}

//Functions
void SlaveTrailer::Load()
{
	this->bodyDamage    = 0;
	this->chassisDamage = 0;
	this->tiresDamage   = 0;
	this->cargoDamage   = 0;

	int tiresDamage = 0;
	int tiresCount  = 0;

	/* Load trailer damage */
	this->cargoDamage = Utility::HexToFloat(this->obj.attributes["cargo_damage"]) * 100;
	for (int i = 0; i < this->accessories.size(); i++)
	{
		std::string dataPath = this->accessories[i]->attributes["data_path"];
		//Body
		if (dataPath.find("/body/") != std::string::npos)
			this->bodyDamage = Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;

		//Chassis
		if (dataPath.find("chassis") != std::string::npos)
			this->chassisDamage = Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;

		//Tyres
		if (dataPath.find("tire") != std::string::npos)
		{
			tiresDamage += Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;
			tiresCount++;
		}
		//Is dolly
		else if (dataPath.find("dolly") != std::string::npos)
			this->isDolly = true;
	}
	this->tiresDamage = tiresDamage / tiresCount;
	this->cargoMass = Utility::StringToFloat(this->obj.attributes["cargo_mass"]);
}

void SlaveTrailer::Save(int bodyDamage, int chassisDamage, int tiresDamage, int cargoDamage)
{
	this->obj.attributes["cargo_damage"] = std::to_string((float)cargoDamage / 100);
	for (int i = 0; i < this->accessories.size(); i++)
	{
		float data;

		std::string dataPath = this->accessories.at(i)->attributes["data_path"];
		//Body
		if (dataPath.find("/body/") != std::string::npos)
		{
			data = bodyDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		//Chassis
		else if (dataPath.find("chassis") != std::string::npos)
		{
			data = chassisDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		//Tyres
		else if (dataPath.find("tire") != std::string::npos)
		{
			data = tiresDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
	}
}