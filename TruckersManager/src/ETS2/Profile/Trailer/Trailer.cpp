#include "tmpch.h"
#include "Trailer.h"

//Functions
void Trailer::GetTrailerName()
{
	std::string sourceName = this->trailerDef.attributes["source_name"];

	if (sourceName.find("flatbed") != std::string::npos)
		this->name = "Flatbed ";
	else if (sourceName.find("schwmuller") != std::string::npos)
		this->name = "Schwarzmueller ";
	else if (sourceName.find("krone") != std::string::npos)
		this->name = "Krone ";
	else if (sourceName.find("foodtank") != std::string::npos)
		this->name = "Foodtank ";

	if (sourceName.find("container") != std::string::npos)
		this->name += "Container carrier ";
	else if (sourceName.find("curtain") != std::string::npos)
		this->name += "Curtainsider ";
	else if (sourceName.find("dryvan") != std::string::npos)
		this->name += "Dry Freighter ";
	else if (sourceName.find("insulated") != std::string::npos)
		this->name += "Insulated ";
	else if (sourceName.find("reefer"))
		this->name += "Refrigerated ";

	if (sourceName.find("double") != std::string::npos)
		this->name += "Double ";
	else if (sourceName.find("bdouble ") != std::string::npos)
		this->name += "B-double ";
	else if (sourceName.find("hct") != std::string::npos)
		this->name += "HCT ";

	if (sourceName.find("crane") != std::string::npos)
		this->name += "Crane";

	if (this->name == "" && sourceName.find('.') != std::string::npos)
		this->name = sourceName.substr(sourceName.find_first_of('.'), sourceName.find_last_of('.') - sourceName.find_first_of('.'));
	if (this->name == "")
		this->name = this->trailer.id;
}

//Constructors/Destructors
Trailer::Trailer(std::string savePath)
	: savePath(savePath)
{

}

Trailer::~Trailer()
{
	for (unsigned int i = 0; i < this->accessories.size(); i++)
		delete this->accessories[i];

	for (unsigned int i = 0; i < this->slaveTrailers.size(); i++)
		delete this->slaveTrailers[i];
}

//Functions
void Trailer::Load()
{
	this->bodyDamage    = 0;
	this->chassisDamage = 0;
	this->tiresDamage   = 0;
	this->cargoDamage   = 0;
	this->dollyCount	= 0;
	this->cargoMass		= 0;

	int tiresDamage = 0;
	int tiresCount  = 0;

	/* Load trailer damage */
	this->cargoDamage = Utility::HexToFloat(this->trailer.attributes["cargo_damage"]) * 100;
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
	}
	this->tiresDamage = tiresDamage / tiresCount;

	/* Load slave trailer objects */
	if (this->trailer.attributes["slave_trailer"] != "null")
	{
		this->slaveTrailers.push_back(new SlaveTrailer(this->savePath, &this->slaveTrailers, this->trailer.attributes["slave_trailer"]));

		std::vector<SiiFileManager::SiiObject*> objs;
		for (int i = 0; i < this->slaveTrailers.size(); i++)
			for (int x = 0; x < this->slaveTrailers[i]->GetAccessories()->size(); x++)
				objs.push_back(this->slaveTrailers[i]->GetAccessories()->at(x));
		SiiFileManager::LoadObjects(this->savePath, objs);

		for (int i = 0; i < this->slaveTrailers.size(); i++)
		{
			this->slaveTrailers[i]->Load();
			this->bodyDamage    += this->slaveTrailers[i]->bodyDamage;
			this->chassisDamage += this->slaveTrailers[i]->chassisDamage;
			this->tiresDamage += this->slaveTrailers[i]->tiresDamage;
			this->cargoDamage   += this->slaveTrailers[i]->cargoDamage;
			if (slaveTrailers[i]->isDolly)
				this->dollyCount++;
		}
		this->bodyDamage    /= this->slaveTrailers.size() + 1;
		this->chassisDamage /= this->slaveTrailers.size() + 1;
		this->tiresDamage /= this->slaveTrailers.size() + 1;
		this->cargoDamage   /= this->slaveTrailers.size() + 1;
	}

	for (int i = 0; i < this->slaveTrailers.size(); i++)
		this->cargoMass += this->slaveTrailers[i]->cargoMass;
	this->cargoMass += Utility::StringToFloat(this->trailer.attributes["cargo_mass"]);

	this->trailerMass = Utility::HexToFloat(this->trailerDef.attributes["chassis_mass"]) + Utility::HexToFloat(this->trailerDef.attributes["body_mass"]);
	if (this->trailerMass < 0)
		this->trailerMass = 0;

	this->GetTrailerName();
}

void Trailer::Save()
{
	if (this->dollyCount > 0)
		this->bodyDamage = (this->bodyDamage * (this->slaveTrailers.size() + 1)) / (this->slaveTrailers.size() + 1 - this->dollyCount);

	this->trailer.attributes["cargo_damage"] = std::to_string((float)this->cargoDamage / 100);
	for (int i = 0; i < this->accessories.size(); i++)
	{
		float data;

		std::string dataPath = this->accessories.at(i)->attributes["data_path"];
		//Body
		if (dataPath.find("/body/") != std::string::npos)
		{
			data = this->bodyDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		//Chassis
		else if (dataPath.find("chassis") != std::string::npos)
		{
			data = this->chassisDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
		//Tyres
		else if (dataPath.find("tire") != std::string::npos)
		{
			data = this->tiresDamage;
			this->accessories.at(i)->attributes["wear"] = std::to_string(data / 100);
		}
	}
	for (int i = 0; i < this->slaveTrailers.size(); i++)
		this->slaveTrailers[i]->Save(this->bodyDamage, this->chassisDamage, this->tiresDamage, this->cargoDamage);
}