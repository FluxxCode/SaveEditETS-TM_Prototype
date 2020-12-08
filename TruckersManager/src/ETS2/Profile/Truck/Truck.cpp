#include "tmpch.h"
#include "Truck.h"

//Functions
std::string Truck::ConvertTruckName(std::string str)
{
	std::map<std::string, std::string> trucks = { { "daf", "DAF" }, { "daf_xf_euro6", "DAF XF Euro 6" }, { "iveco", "Iveco" }, { "iveco_h", "Iveco H" }, { "man", "MAN" }, { "man_tgx_euro6", "MAN TGX Euro 6" }, { "mercedes", "Mercedes" },
												  { "mercedes_actros_2014", "Mercedes Actros 2014" }, { "renault_m", "Renault M" }, { "renault_p", "Renault P" }, { "renault_t", "Renault T" }, { "scania_r", "Scania R" },
												  { "scania_r_2016", "Scania R 2016" }, { "scania_s_2016", "Scania S 2016" }, { "scania_streamline", "Scania Streamline" }, { "volvo_fh16", "Volvo FH16" }, { "volvo_fh16_2012", "Volvo FH16 2012" } };
	if (trucks.find(str) != trucks.end())
		return trucks[str];

	Utility::AddLog("[Truck] WARNING: Truck: \"" + str + "\" is undefined");

	return str;
}

//Constructors/Destructors
Truck::Truck()
{

}

Truck::~Truck()
{
	for (unsigned int i = 0; i < this->accessories.size(); i++)
		delete this->accessories[i];
}

//Functions
void Truck::Load()
{
	int tiresDamage = 0;
	int tiresCount  = 0;

	/* Load truck damage */
	//Truck fuel
	this->fuel = Utility::HexToFloat(this->truck.attributes["fuel_relative"]) * 100;
	//Truck damage
	for (int i = 0; i < this->accessories.size(); i++)
	{
		std::string dataPath = this->accessories[i]->attributes["data_path"];
		//Chassis
		if (dataPath.find("chassis") != std::string::npos)
			this->chassisDamage = Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;
		//Cabin
		if (dataPath.find("cabin") != std::string::npos)
			this->cabinDamage = Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;

		//Engine
		if (dataPath.find("engine") != std::string::npos)
			this->engineDamage = Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;

		//Transmission
		if (dataPath.find("transmission") != std::string::npos)
			this->transmissionDamage = Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;

		//Tires
		if (dataPath.find("tire") != std::string::npos)
		{
			tiresDamage += Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100;
			tiresCount++;
		}
			/*this->tiresDamage += (Utility::HexToFloat(this->accessories[i]->attributes["wear"]) * 100) / 4;*/
	}
	this->tiresDamage = tiresDamage / tiresCount;

	//Load name and file name
	fs::path dataPath(this->accessories[0]->attributes.at("data_path"));
	std::string truckFileName = dataPath.parent_path().filename().string();	//Get the truck file name
	std::replace(truckFileName.begin(), truckFileName.end(), '.', '_');

	this->fileName = truckFileName;
	this->name = this->ConvertTruckName(truckFileName);
}

void Truck::Save()
{
	this->truck.attributes["fuel_relative"] = std::to_string((float)this->fuel / 100);
	for (int i = 0; i < this->accessories.size(); i++)
	{
		float data;

		std::string dataPath = this->accessories.at(i)->attributes["data_path"];
		//Chassis
		if (dataPath.find("chassis") != std::string::npos)
		{
			data = this->chassisDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		//Cabin
		else if (dataPath.find("cabin") != std::string::npos)
		{
			data = this->cabinDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		else //Engine
		if (dataPath.find("engine") != std::string::npos)
		{
			data = this->engineDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		//Transmission
		else if (dataPath.find("transmission") != std::string::npos)
		{
			data = this->transmissionDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		//Tyres
		else if (dataPath.find("tire") != std::string::npos)
		{
			data = this->tiresDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
	}
}