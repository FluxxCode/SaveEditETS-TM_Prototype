#pragma once

#include "ETS2/Profile/Profile.h"
#include "Application/Config/Config.h"
#include "ETS2/Profile/Truck/Truck.h"

class VehicleTab
{
private:
	//Variables
	Profile* profile = nullptr;
	Config* config = nullptr;
	ConsoleWindow* consoleWindow = nullptr;
	Utility::ResourceManager* resourceManager = nullptr;

	Truck* truck = nullptr;
	std::map<std::string, Truck*>* trucks = { };
	std::string selectedTruck = "";

	Trailer* trailer = nullptr;
	std::map<std::string, Trailer*>* trailers = { };
	std::string selectedTrailer = "";

	//Functions
	ImVec4 NumberToColor(unsigned int num); //Converts the number (0 - 100) to a color between green and red
	void ColorRangeRectangle(unsigned int num); //Draws a rectangle with a color between red and green based on the number 

public:
	//Constructors/Destructors
	VehicleTab();
	virtual ~VehicleTab();

	//Functions
	void Update(Profile* profile);

	void Load(Profile* profile, Config* config, Utility::ResourceManager* resourceManager, ConsoleWindow* consoleWindow);

	//Update the values to the objects
	void Save();

	//Getter/Setter
};