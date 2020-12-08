#pragma once

/*
	Class to load and handle resources
*/

namespace fs = std::filesystem;

class ResourceManager
{
private:
	//Variables
	std::map<std::string, GLuint> textures;
	GLuint noTexture = -1;

	std::map<std::string, int> errors;

public:
	//Constructors/Destructors
	ResourceManager();
	virtual ~ResourceManager();

	//Functions
	GLuint* LoadTexture(std::string path);
};