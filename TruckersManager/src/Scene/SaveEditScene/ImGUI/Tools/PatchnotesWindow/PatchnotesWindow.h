#pragma once

#include "Application/Config/Config.h"

namespace fs = std::filesystem;

class PatchnotesWindow
{
private:
	//Variables
	ImVec2 pos;
	ImVec2 size;

	std::string patchnotes = "";

public:
	//Constructors/Destructors
	PatchnotesWindow(ImVec2 pos, ImVec2 size);
	virtual ~PatchnotesWindow();

	//Functions
	void Update(Config* config, bool* bOpen);

	//void GetPatchnotes(bool download);
	//void DownloadPatchnotes();
};