#include "tmpch.h"
#include "SiiFileManager.h"
#include <SII_Decrypt/SII_Decrypt.h>

int SiiFileManager::Decrypt(std::string inputPath, std::string outputPath, bool consoleOutput)
{
	Utility::AddLog("[FileManager] Decrypt: " + inputPath);

	//InputFile
	std::string iPath = inputPath;
	char* cstrI = &iPath[0];

	//OutputFile
	std::string oPath;
	if (outputPath == "")
	{
		oPath = fs::path(inputPath).parent_path().string() + "\\temp.sii";
	}
	else
		oPath = outputPath;
	char* cstsrO = &oPath[0];

	//Decypt
	int res = DecryptFile(cstrI, cstsrO);

	Utility::AddLog("[FileManager] Result: " + std::to_string(res));

	//Error occured
	if (res != SIIDEC_RESULT_SUCCESS)
		return res;

	//Copy temp file 
	if (outputPath == "")
	{
		try
		{
			fs::copy(oPath, inputPath, fs::copy_options::overwrite_existing);
			fs::remove(oPath);
		}
		catch (fs::filesystem_error err)
		{
			Utility::AddLog("[FileManager] ERROR: " + std::string(err.what()));
		}
	}

	return res;
}

//Load functoins
std::string SiiFileManager::LoadAttribut(const std::string path, const std::string attribute, const char prefix, const short prefixChars, bool consoleOutput)
{
	//Read profile.sii
	std::string line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.find(attribute) != std::string::npos && line.find(prefix) != std::string::npos)
			{
				if (line.find(" ") != std::string::npos) //Delete white spaces
				{
					if (line.find(":"))
					{
						std::string att = line.substr(0, line.find(":"));
						std::string::iterator end_pos = std::remove(att.begin(), att.end(), ' ');
						att.erase(end_pos, att.end());
						line = att + ":" + line.substr(line.find(":") + 2);
					}
					else
					{
						std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
						line.erase(end_pos, line.end());
					}
				}

				if (line.find(prefix) == attribute.size())
				{
					std::string str = line.substr(line.find(prefix) + prefixChars);
					if (str[0] == '\"' && str[str.size() - 1] == '\"' && str.size() > 1 && str.find(" ") != std::string::npos) //Remove the " at the beginning and end if exists
					{
						str = str.erase(0, 1);
						str = str.erase(str.size() - 1, str.size());
					}

					return str; //Return the string
				}
			}
		}
		file.close(); //Close the file
	}
	else
	{
		if (consoleOutput)	std::cerr << "ERROR: SiiFileManager: Unable to open file \"" << path << "\"" << std::endl; //Unable to open file, return -1 TODO: Logging
		return NULL;
	}
	return NULL;
}

std::map<std::string, std::string> SiiFileManager::LoadAttributes(const std::string path, const std::vector<std::string> attributes, const char prefix, const short prefixChars, bool consoleOutput)
{
	std::map<std::string, std::string> att;
	std::string line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (att.size() == attributes.size())	break; //Attributes finished loading, return
			if (line.find(" ") != std::string::npos) //Delete white spaces
			{
				if (line.find(":"))
				{
					std::string att = line.substr(0, line.find(":"));
					std::string::iterator end_pos = std::remove(att.begin(), att.end(), ' ');
					att.erase(end_pos, att.end());
					line = att + ":" + line.substr(line.find(":") + 2);
				}
				else
				{
					std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
					line.erase(end_pos, line.end());
				}
			}

			for (unsigned int i = 0; i < attributes.size(); i++)
				if (line.find(attributes[i]) != std::string::npos && line.find(prefix) != std::string::npos)
					if (line.find(prefix) == attributes[i].size())
					{
						std::string str = line.substr(line.find(prefix) + prefixChars);
						if (str[0] == '\"' && str[str.size() - 1] == '\"' && str.size() > 1 && str.find(" ") != std::string::npos) //Remove the " at the beginning and end if exists
						{
							str = str.erase(0, 1);
							str = str.erase(str.size() - 1, str.size());
						}
						att[attributes[i]] = str;
					}
		}
		file.close(); //Close the file
	}
	else
	{
		if (consoleOutput)	std::cerr << "ERROR: SiiFileManager: Unable to open file \"" << path << "\"" << std::endl; //Unable to open file, return -1 TODO: Logging
		return att;
	}

	return att;
}

enum class InObject { Yes, No };
bool SiiFileManager::LoadObjects(const std::string path, const std::vector<SiiObject*> objects, const char prefix, const short prefixChars, bool consoleOutput)
{
	std::vector<SiiObject*> objs = objects;
	int objsReady = 0;
	int arraysReady = 0;

	InObject inObj = InObject::No;
	int currentObj = -1;

	std::string currentArray = "";

	std::string line;
	std::ifstream file(path);

	//Set none

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (objsReady == objs.size())										break; //If every object loaded the attribute values, break and return
			if (line.find("{") == std::string::npos && inObj == InObject::No)	continue; //Continue if the object is not found and the line is not an object start
			if (line.find("}") != std::string::npos && inObj == InObject::Yes) //Continue at end of object
			{
				Utility::AddLog("[FileManager] Object loaded: " + objs[currentObj]->name + " - " + objs[currentObj]->id);
				inObj = InObject::No;
				currentObj = -1;
				objsReady++;
				continue;
			}
			if (line.find(" ") != std::string::npos) //Delete white spaces
			{
				if (line.find(":"))
				{
					std::string att = line.substr(0, line.find(":"));
					std::string::iterator end_pos = std::remove(att.begin(), att.end(), ' ');
					att.erase(end_pos, att.end());
					line = att + ":" + line.substr(line.find(":") + 2);
				}
				else
				{
					std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
					line.erase(end_pos, line.end());
				}
			}

			/* Get the attributes */
			if (inObj == InObject::Yes)
			{
				if (line.find("[") == std::string::npos && line.find("]") == std::string::npos)
				{
					std::string str = line.substr(line.find(prefix) + prefixChars);
					if (str[0] == '\"' && str[str.size() - 1] == '\"' && str.size() > 1 && str.find(" ") != std::string::npos) //Remove the " at the beginning and end if exists
					{
						str = str.erase(0, 1);
						str = str.erase(str.size() - 1, str.size());
					}
					objs[currentObj]->attributes[line.substr(0, line.find(prefix))] = str;
				}
				else //Load array
				{
					std::string att = line.substr(0, line.find("["));

					std::string str = line.substr(line.find(":") + 1);
					if (str[0] == '\"' && str[str.size() - 1] == '\"' && str.size() > 1 && str.find(" ") != std::string::npos) //Remove the " at the beginning and end if exists
					{
						str = str.erase(0, 1);
						str = str.erase(str.size() - 1, str.size());
					}

					objs[currentObj]->arrayAttributes[att].push_back(str);
					objs[currentObj]->arraySizes[att]++;
					objs[currentObj]->arrayCount = objs[currentObj]->arrayAttributes.size();
					if (objs[currentObj]->attributes.find(att) != objs[currentObj]->attributes.end()) objs[currentObj]->attributes.erase(att);
				}
			}

			/* Check if line is at the start of a object */
			if (inObj == InObject::No && line.find("{") != std::string::npos)
			{
				//Go trough every object
				for (unsigned int i = 0; i < objs.size(); i++)
				{
					if (objs[i]->name != "" && objs[i]->id != "") //Name and ID of object is defined
					{
						if (line.find(objs[i]->name) != std::string::npos && line.find(objs[i]->id) != std::string::npos)
						{
							if (line.find(":") == objs[i]->name.size() && line.size() == objs[i]->name.size() + objs[i]->id.size() + 3)
							{
								inObj = InObject::Yes;
								currentObj = i;
								continue;
							}
						}
					}
					else if (objs[i]->name != "") //Only name is defined
					{
						if (line.find(objs[i]->name) != std::string::npos && line.find("{") != std::string::npos && line.find(":") != std::string::npos)
						{
							if (line.find(":") == objs[i]->name.size())
							{
								inObj = InObject::Yes;
								currentObj = i;

								line.erase(line.size() - 1);
								objs[currentObj]->id = line.substr(line.find(":") + 1);

								continue;
							}
						}
					}
					else //Only id is defined
					{
						if (line.find(objs[i]->id) != std::string::npos && line.find("{") != std::string::npos && line.find(":") != std::string::npos)
						{
							inObj = InObject::Yes;
							currentObj = i;

							objs[currentObj]->name = line.substr(0, line.find(":"));

							continue;
						}
					}
				}
			}
		}
		file.close();
	}
	else
	{
		if (consoleOutput)	Utility::AddLog("[FileManager] ERROR: Unable to open file \"" + path + "\""); //Unable to open file, return -1 TODO: Logging
		return false;
	}

	return true;
}

bool SiiFileManager::LoadObject(const std::string path, SiiObject* object, const char prefix, const short prefixChars, const bool consoleOutput)
{
	if (!SiiFileManager::LoadObjects(path, std::vector<SiiFileManager::SiiObject*> { object }))
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR: Unable to load object: \"" + path + "\"");
		return false;
	}

	return true;
}

//Save functions
bool SiiFileManager::OverwriteObject(const std::string path, const SiiObject* object, const bool consoleOutput)
{
	InObject inObj = InObject::No;
	bool writeObj = true;

	Decrypt(path);

	//File
	std::string lineFile;
	std::ifstream file(path);

	//Temp file
	fs::path p = fs::path(path);
	std::string lineTemp;
	std::ofstream fileTemp(p.parent_path().string() + "\\temp.txt");

	if (file.is_open() && fileTemp.is_open())
	{
		while (getline(file, lineFile))
		{
			if (lineFile.find(object->id) != std::string::npos && lineFile.find(object->name) != std::string::npos && lineFile.find("{") != std::string::npos) inObj = InObject::Yes; //Check if line is in object
			//Write new object in file
			if (inObj == InObject::Yes && writeObj)
			{
				writeObj = false;
				fileTemp << object->name << " : " << object->id << " {" << std::endl;

				//Write values
				for (auto const& [key, val] : object->attributes)
				{
					std::string str = val;
					if (str.find(" ") != std::string::npos && 
						str[0] != '('  && str[str.size() - 1] != ')'  &&  //Check if str is not a position
						str.size() > 1)
					{
						str.insert(0, "\"");
						str.push_back('\"');
					}
					fileTemp << " " << key << ": " << str << std::endl;
				}
				//Write arrays
				for (auto const& [key, val] : object->arrayAttributes)
				{
					int i = object->arraySizes.at(key);
					fileTemp << " " << key << ": " << i << std::endl;
					for (int i = 0; i < object->arraySizes.at(key); i++)
					{
						std::string str = val[i];
						if (str.find(" ") != std::string::npos && str[0] != '\"' && str[str.size() - 1] != '\"' && str.size() > 1)
						{
							str.insert(0, "\"");
							str.push_back('\"');
						}
						fileTemp << " " << key << "[" << i << "]: " << val[i] << std::endl;
					}
				}

				fileTemp << "}" << std::endl;
			}

			if (inObj == InObject::No) fileTemp << lineFile << "\n"; //Write line in file temp
			if (inObj == InObject::Yes && lineFile.find("}") != std::string::npos) inObj = InObject::No; //Check if line is out of object
		}

		if (writeObj)
		{
			if (consoleOutput) Utility::AddLog("[FileManager] WARNING:: Object: \"" + object->name + ", " + object->id + "\" in \"" + path + "\" not found. Creating new object");
			WriteObject(path, object, consoleOutput);
		}

		//Close the files
		file.close();
		fileTemp.close();
	}
	//Errors
	else if (!file.is_open())
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR: Unable to open file: \"" + path + "\"");
		return false;
	}
	else
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR::SiiFileManager: Unable to open file: \"" + p.parent_path().string() + "\\temp.txt" + "\"");
		return false;
	}

	remove(path.c_str());
	rename((p.parent_path().string() + "\\temp.txt").c_str(), path.c_str());

	return false;
}

bool SiiFileManager::OverwriteObjects(const std::string path, const std::vector<SiiObject*> objs, const bool consoleOutput)
{
	InObject inObj = InObject::No;
	std::vector<SiiObject*> objects = objs;

	Decrypt(path);

	//File
	std::string lineFile;
	std::ifstream file(path);

	//Temp file
	fs::path p = fs::path(path);
	std::string lineTemp;
	std::ofstream fileTemp(p.parent_path().string() + "\\temp.txt");

	if (file.is_open() && fileTemp.is_open())
	{
		while (getline(file, lineFile))
		{
			if (lineFile.find("{") == std::string::npos && inObj == InObject::No)
			{
				fileTemp << lineFile << std::endl;
				continue;
			}
			for (int x = 0; x < objects.size(); x++)
			{
				if (lineFile.size() < 3 || inObj == InObject::Yes)
					break; 
				if (lineFile.find(objects.at(x)->id) != std::string::npos && lineFile.find(objects.at(x)->name) != std::string::npos && lineFile.find("{") != std::string::npos)
					inObj = InObject::Yes; //Check if line is in object
				//Write new object in file
				if (inObj == InObject::Yes)
				{
					fileTemp << objects.at(x)->name << " : " << objects.at(x)->id << " {" << std::endl;

					//Write values
					for (auto const& [key, val] : objects.at(x)->attributes)
					{
						std::string str = val;
						if (str.find(" ") != std::string::npos &&
							str[0] != '(' && str[str.size() - 1] != ')' &&  //Check if str is not a position
							str.size() > 1)
						{
							str.insert(0, "\"");
							str.push_back('\"');
						}
						fileTemp << " " << key << ": " << str << std::endl;
					}
					//Write arrays
					for (auto const& [key, val] : objects.at(x)->arrayAttributes)
					{
						int size = objects.at(x)->arraySizes.at(key); //Array size
						fileTemp << " " << key << ": " << size << std::endl;
						for (int i = 0; i < objects.at(x)->arraySizes.at(key); i++)
						{
							std::string str = val[i];
							if (str.find(" ") != std::string::npos && str[0] != '\"' && str[str.size() - 1] != '\"' && str.size() > 1)
							{
								str.insert(0, "\"");
								str.push_back('\"');
							}
							fileTemp << " " << key << "[" << i << "]: " << val[i] << std::endl;
						}
					}

					fileTemp << "}" << std::endl;
					Utility::AddLog("[SiiFileManager] Overwrote object: " + objects[x]->name + " - " + objects[x]->id);
					objects.erase(objects.begin() + x);
					break;
				}
			}

			if (inObj == InObject::No) 
				fileTemp << lineFile << std::endl; //Write line in file temp
			if (inObj == InObject::Yes && lineFile.find("}") != std::string::npos) 
				inObj = InObject::No; //Check if line is out of object
		}

		//Close the files
		file.close();
		fileTemp.close();
	}
	//Errors
	else if (!file.is_open())
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR: Unable to open file: \"" + path + "\"");
		return false;
	}
	else
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR::SiiFileManager: Unable to open file: \"" + p.parent_path().string() + "\\temp.txt" + "\"");
		return false;
	}

	remove(path.c_str());
	rename((p.parent_path().string() + "\\temp.txt").c_str(), path.c_str());

	return false;
}

bool SiiFileManager::WriteObject(const std::string path, const SiiObject* object, const bool consoleOutput)
{
	Decrypt(path);

	return false;
}

bool SiiFileManager::RemoveObject(const std::string path, const SiiObject* object, const bool consoleOutput)
{
	Decrypt(path);

	InObject inObj = InObject::No;

	//File
	std::string lineFile;
	std::ifstream file(path);

	//Temp file
	fs::path p = fs::path(path);
	std::string lineTemp;
	std::ofstream fileTemp(p.parent_path().string() + "\\temp.txt");

	if (file.is_open() && fileTemp.is_open())
	{
		while (getline(file, lineFile))
		{
			if (lineFile.find(object->id) != std::string::npos && lineFile.find(object->name) != std::string::npos && lineFile.find("{") != std::string::npos) inObj = InObject::Yes; //Check if line is in object
			if (inObj == InObject::No) fileTemp << lineFile << "\n"; //Write line in file temp
			if (inObj == InObject::Yes && lineFile.find("}") != std::string::npos) inObj = InObject::No; //Check if line is out of object
		}

		//Close the files
		file.close();
		fileTemp.close();
	}
	//Errors
	else if (!file.is_open())
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR: Unable to open file: \"" + path + "\"");
		return false;
	}
	else
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR: Unable to open file: \"" + p.parent_path().string() + "\\temp.txt" + "\"");
		return false;
	}

	remove(path.c_str());
	rename((p.parent_path().string() + "\\temp.txt").c_str(), path.c_str());

	return false;
}

bool SiiFileManager::RemoveObject(const std::string path, std::string id, const bool consoleOutput)
{
	Decrypt(path);

	InObject inObj = InObject::No;

	//File
	std::string lineFile;
	std::ifstream file(path);

	//Temp file
	fs::path p = fs::path(path);
	std::string lineTemp;
	std::ofstream fileTemp(p.parent_path().string() + "\\temp.txt");

	if (file.is_open() && fileTemp.is_open())
	{
		while (getline(file, lineFile))
		{
			if (lineFile.find(id) != std::string::npos && lineFile.find("{") != std::string::npos) inObj = InObject::Yes; //Check if line is in object
			if (inObj == InObject::No) fileTemp << lineFile << "\n"; //Write line in file temp
			if (inObj == InObject::Yes && lineFile.find("}") != std::string::npos) inObj = InObject::No; //Check if line is out of object
		}

		//Close the files
		file.close();
		fileTemp.close();
	}
	//Errors
	else if (!file.is_open())
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR: Unable to open file: \"" + path + "\"");
		return false;
	}
	else
	{
		if (consoleOutput) Utility::AddLog("[FileManager] ERROR: Unable to open file: \"" + p.parent_path().string() + "\\temp.txt" + "\"");
		return false;
	}

	remove(path.c_str());
	rename((p.parent_path().string() + "\\temp.txt").c_str(), path.c_str());

	return false;
}