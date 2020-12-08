#pragma once

/*
	Class that handels sii files
*/

namespace fs = std::filesystem;

class SiiFileManager
{
private:
	//Functions
	SiiFileManager() {};

public:
	//Structs
	struct SiiObject
	{
		SiiObject() 
			: id(""), name(""), arrayCount(0) { }
		SiiObject(std::string id, std::string name)
			: id(id), name(name), arrayCount(0) { }

		std::string id = "";
		std::string name = "";
		std::map<std::string, std::string> attributes;
		std::map<std::string, std::vector<std::string>> arrayAttributes;
		std::map<std::string, int> arraySizes; //Sizes of the arrayAttributeValues
		unsigned int arrayCount = 0; //Number of arrays
	};

	//Functions
	static int Decrypt(const std::string inputPath, std::string outputPath = "", const bool consoleOutput = true); //Decrypts the file

	//Load functions
	static std::string LoadAttribut(const std::string path, const std::string attribute, const char prefix = ':', const short prefixChars = 1, const bool consoleOutput = true); //Returns the value of the attribut. prefix = string after prefix + prefiChars will get returned
	static std::map<std::string, std::string> LoadAttributes(const std::string path, const std::vector<std::string> attributes, const char prefix = ':', const short prefixChars = 1, const bool consoleOutput = true); //Returns a struct attributes with the values of the attrbiutes. prefix = string after prefix + prefiChars will get returned	
	static bool LoadObjects(const std::string path, const std::vector<SiiObject*> objects, const char prefix = ':', const short prefixChars = 1, const bool consoleOutput = true); //Returns a map with the attributes
	static bool LoadObject(const std::string path, SiiObject* object, const char prefix = ':', const short prefixChars = 1, const bool consoleOutput = true); //Load one object

	//Save functions
	static bool OverwriteObject(const std::string path, const SiiObject* object, const bool consoleOutput = true); //Overwrites an object
	static bool OverwriteObjects(const std::string path, const std::vector<SiiObject*> objs, const bool consoleOutput = true); //Overwrites objects
	static bool WriteObject(const std::string path, const SiiObject* object, const bool consoleOutput = true); //Write a new object to the file, objectInFront = Obj gets saved after the objectInFornt
	static bool RemoveObject(const std::string path, const SiiObject* object, const bool consoleOutput = true);
	static bool RemoveObject(const std::string path, std::string id, const bool consoleOutput = true);
};