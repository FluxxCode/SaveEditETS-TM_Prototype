#include "tmpch.h"
#include "ResourceManager.h"

//Constructors/Destructors
Utility::ResourceManager::ResourceManager()
{
	this->textures.clear();
}

Utility::ResourceManager::~ResourceManager()
{

}

//Functions
GLuint* Utility::ResourceManager::LoadTexture(std::string path)
{
	if (!fs::exists(path))
	{
		this->noTexture = SOIL_load_OGL_texture("Resources\\NoTexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);

		if (this->errors.find(path) == this->errors.end())
			AddLog("[ResourceManager] ERROR: File: \"" + path + "\" does not exists!");

		this->errors[path] = 1;
		return &this->noTexture;
	}
	if (this->textures.find(path) != this->textures.end())		return &this->textures[path]; 	//Check if texture was already loaded

	GLuint texture = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL); //Load the texture
	if (texture == 0)
	{
		std::string s = SOIL_last_result();
		AddLog("[ResourceManager] Soil error when loading: \"" + path + "\" | " + s);  //Error occured
		this->noTexture = SOIL_load_OGL_texture("Resources\\NoTexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
		return &this->noTexture;
	}

	this->textures[path] = texture;

	return &this->textures[path];
}