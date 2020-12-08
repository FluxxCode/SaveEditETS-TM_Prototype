#include "tmpch.h"
#include "Save.h"
#include <SII_Decrypt/SII_Decrypt.h>

//Functions
void Save::Init()
{
	this->InitObjects();
}

void Save::InitObjects()
{
	fs::path saveFile(this->path + "\\game.sii");
	if (!fs::exists(saveFile))	Utility::AddLog("[Save] ERROR: \"" + saveFile.string() + "\" does not existst");
	SiiFileManager::Decrypt(this->path + "\\game.sii"); //Decrypt game file

	//Init the objects
	//Economy
	this->economyObj.name = "economy";
	//Player
	this->playerObj.name = "player";
	this->objs.push_back(&this->playerObj);
	//Bank
	this->bankObj.name = "bank";
	this->objs.push_back(&this->bankObj);
}

//Constructors/Destructors
Save::Save(std::string path)
	: config(config), path(path)
{
	this->Init();
	if (path != "") this->Load();
}

Save::~Save()
{
	for (auto const& [key, val] : this->trucks)
		delete this->trucks[key];
	for (auto const& [key, val] : this->trailers)
		delete this->trailers[key];
}

//Functoins
void Save::Load()
{
	Utility::Timer timer("[Save] Load time: ");

	fs::path saveFile(this->path + "\\game.sii");
	if (!fs::exists(saveFile))
	{
		std::cerr << "ERROR: \"" << saveFile.string() << "\" does not existst" << std::endl;
		return;
	}

	//Economy opject
	SiiFileManager::LoadObject(saveFile.string(), &this->economyObj);

	//Player + bank object
	this->playerObj.id = this->economyObj.attributes["player"];
	this->bankObj.id = this->economyObj.attributes["bank"];
	SiiFileManager::LoadObjects(saveFile.string(), this->objs);
	this->objs.clear();

	this->playerTruck = this->playerObj.attributes["my_truck"];
	this->playerTrailer = this->playerObj.attributes["my_trailer"];

	//Trucks
	for (int i = 0; i < this->playerObj.arraySizes["trucks"]; i++)
	{
		this->trucks[this->playerObj.arrayAttributes["trucks"][i]] = new Truck();
		this->trucks[this->playerObj.arrayAttributes["trucks"][i]]->GetTruck()->id = this->playerObj.arrayAttributes["trucks"][i];
		this->objs.push_back(this->trucks[this->playerObj.arrayAttributes["trucks"][i]]->GetTruck());
	}
	//Trailers
	for (int i = 0; i < this->playerObj.arraySizes["trailers"]; i++)
	{
		this->trailers[this->playerObj.arrayAttributes["trailers"][i]] = new Trailer(saveFile.string());
		this->trailers[this->playerObj.arrayAttributes["trailers"][i]]->GetTrailer()->id = this->playerObj.arrayAttributes["trailers"][i];
		this->objs.push_back(this->trailers[this->playerObj.arrayAttributes["trailers"][i]]->GetTrailer());
	}
	SiiFileManager::LoadObjects(saveFile.string(), this->objs);
	this->objs.clear();

	//Truck accessories
	for (auto const& [key, val] : this->trucks)
	{
		for (int x = 0; x < this->trucks[key]->GetTruck()->arraySizes["accessories"]; x++)
		{
			this->trucks[key]->GetAccessories()->push_back(new SiiFileManager::SiiObject(this->trucks[key]->GetTruck()->arrayAttributes["accessories"][x], ""));
			this->objs.push_back(this->trucks[key]->GetAccessories()->at(x));
		}
	}
	//Trailer accessories and defs
	for (auto const& [key, val] : this->trailers)
	{
		for (int x = 0; x < this->trailers[key]->GetTrailer()->arraySizes["accessories"]; x++)
		{
			this->trailers[key]->GetAccessories()->push_back(new SiiFileManager::SiiObject(this->trailers[key]->GetTrailer()->arrayAttributes["accessories"][x], ""));
			this->objs.push_back(this->trailers[key]->GetAccessories()->at(x));
		}

		this->trailers[key]->GetTrailerDef()->id = this->trailers[key]->GetTrailer()->attributes["trailer_definition"];
		if (this->trailers[key]->GetTrailerDef()->id != "null")
			this->objs.push_back(this->trailers[key]->GetTrailerDef());
	}
	SiiFileManager::LoadObjects(saveFile.string(), this->objs);
	this->objs.clear();

	for (auto const& [key, val] : this->trucks)
		this->trucks[key]->Load();
	for (auto const& [key, val] : this->trailers)
		this->trailers[key]->Load();
}

void Save::SaveData()
{
	Utility::Timer timer("[Save] Save time: ");
	Utility::AddLog("[Save] Saving save...");

	std::vector<SiiFileManager::SiiObject*> obj = { &this->economyObj, &this->playerObj, &this->bankObj };
	for (auto const& [key, val] : this->trucks)
	{
		obj.push_back(this->trucks[key]->GetTruck());
		for (int i = 0; i < this->trucks[key]->GetAccessories()->size(); i++)
			obj.push_back(this->trucks[key]->GetAccessories()->at(i));
	}
	for (auto const& [key, val] : this->trailers)
	{
		obj.push_back(this->trailers[key]->GetTrailer());
		for (int i = 0; i < this->trailers[key]->GetAccessories()->size(); i++)
			obj.push_back(this->trailers[key]->GetAccessories()->at(i));
		for (int i = 0; i < this->trailers[key]->GetSlaveTrailers()->size(); i++)
			for (int x = 0; x < this->trailers[key]->GetSlaveTrailers()->at(i)->GetAccessories()->size(); x++)
				obj.push_back(this->trailers[key]->GetSlaveTrailers()->at(i)->GetAccessories()->at(x));
	}
	SiiFileManager::OverwriteObjects(this->path + "\\game.sii", obj);
}