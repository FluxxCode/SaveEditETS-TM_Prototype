#pragma once

#include "Application/Config/Config.h"

/*
	Credits window class
*/

class CreditsWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

public:
	//Constructors/Destructors
	CreditsWindow(ImVec2 pos = ImVec2(-5.0f, -5.0f), ImVec2 size = ImVec2(-5.0f, -5.0f));
	virtual ~CreditsWindow();

	//Functions
	void Udpate(Config* config, bool* bOpen);

	//Getter/Setter
	ImVec2 GetPos() { return this->pos; }
	void SetPos(ImVec2 pos) { this->pos = pos; }

	ImVec2 GetSize() { return this->size; }
	void SetSize(ImVec2 size) { this->size = size; }
};