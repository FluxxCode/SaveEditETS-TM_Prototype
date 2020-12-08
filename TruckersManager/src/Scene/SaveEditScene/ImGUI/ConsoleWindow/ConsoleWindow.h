#pragma once

/*
	Window that show debug informations
*/

class ConsoleWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

	std::vector<std::string> infos; //Infos / white
	std::vector<std::string> warnings; //Warning / yellow
	std::vector<std::string> errors; //Errors / red
	unsigned int maxInfos = 100; //The max amount of infos

	bool showInfos = true;
	bool showWarnings = true;
	bool showErrors = true;
	bool showInput = true;

	//Functions
	void Print(); //Print the messages
	void ExecuteCommand(std::string command);

public:
	//Constrcutors/Destructors
	ConsoleWindow(ImVec2 pos = ImVec2(-5.0f, -5.0f), ImVec2 size = ImVec2(-5.0f, -5.0f));
	virtual ~ConsoleWindow();

	//Functions
	void Update();
	void PushMessage (std::string message); //Write new info to the window

	//Getter/Setter
	//Pos
	ImVec2 GetPos() { return this->pos; }
	void SetPos(ImVec2 pos) { this->pos = pos; }

	//Size
	ImVec2 GetSize() { return this->size; }
	void SetSize(ImVec2 size) { this->size = size; }
};