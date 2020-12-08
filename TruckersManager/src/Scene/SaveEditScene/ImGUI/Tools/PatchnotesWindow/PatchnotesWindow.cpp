#include "tmpch.h"
#include "PatchnotesWindow.h"

//Constructors/Destructors
PatchnotesWindow::PatchnotesWindow(ImVec2 pos, ImVec2 size)
	: pos(pos), size(size)
{
	//this->GetPatchnotes(false);
}

PatchnotesWindow::~PatchnotesWindow()
{

}

//Functions
void PatchnotesWindow::Update(Config* config, bool* bOpen)
{
	//Window pos + size
	if (this->pos.x == 0 && this->pos.y == 0) ImGui::SetNextWindowPos(this->pos);
	ImGui::SetNextWindowSize(this->size);
	ImGui::SetNextWindowBgAlpha(1.0f);

	ImGui::Begin(config->GetLabel("mmb_patchnotes").c_str(), bOpen, ImGuiWindowFlags_NoResize);

	if (this->patchnotes == "")
	{
		//if (ImGui::Button(config->GetLabel("patchnotes_btn_download").c_str()))
			//this->GetPatchnotes(true);
	}
	else
	{
		ImGui::Separator();
		ImGui::BeginChild("##patchnotestext", ImVec2(580.0f, 555.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Text(this->patchnotes.c_str());
		ImGui::EndChild();
		ImGui::Separator();
	}

	ImGui::End();
}

//void PatchnotesWindow::GetPatchnotes(bool download)
//{
//	//Variables
//	std::string patchnotesPath = "patchnotes.txt";
//
//	this->patchnotes = "";
//
//	//Patchnotes got allready downloaded
//	if (fs::exists("patchnotes.txt"))
//	{
//		//Read file
//		std::string line;
//		std::fstream file(patchnotesPath);
//		if (file.is_open())
//		{
//			while (std::getline(file, line))
//			{
//				if (line == "#patchnotes")
//					continue;
//
//				this->patchnotes += line + "\n";
//			}
//
//			file.close();
//		}
//		else
//		{
//			Utility::AddLog("[PatchnotesWindow] ERROR: Unable to open file: \"" + patchnotesPath + "\"");
//			this->patchnotes = "Unable to load patchnotes. Error: 3";
//		}
//		return;
//	}
//	//Download patchnotes file
//	else if (download)
//	{
//		Utility::AddLog("[PatchnotesWindow] Downloading patchnotes...");
//
//		//Download the patchnotes file
//		//https://drive.google.com/uc?export=download&id=ID
//
//		//URL to the file
//		std::string URL = "https://drive.google.com/uc?export=download&id=1WYd6OdFn3CfEZWgZFr1b79XEyaIsuIHW";
//		std::wstring wstr = std::wstring(URL.begin(), URL.end());
//		LPCWSTR lpw = wstr.c_str();
//
//		//File in that the data from the url gets written
//		std::string savePath = "patchnotes.txt";
//		std::wstring wSavePath = std::wstring(savePath.begin(), savePath.end());
//		LPCWSTR lSavePath = wSavePath.c_str();
//
//		fs::remove(savePath);
//
//		//Count time
//		Utility::Timer timer = Utility::Timer("");
//
//		//Get data from the URL and write it into a savePath
//		HRESULT result = URLDownloadToFile(NULL, lpw, lSavePath, 0, NULL);
//		if (result != S_OK) //Error when downloading the file
//		{
//			if (result == E_OUTOFMEMORY)
//			{
//				Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The buffer length is invalid, or there is insufficient memory to complete the operation.");
//				this->patchnotes = "Unable to load patchnotes.Error: 1";
//			}
//			else
//			{
//				Utility::AddLog("[UpdaterAPP] ERROR: Unable to download file: \"" + URL + "\". The specified resource or callback interface was invalid.");
//				this->patchnotes = "Unable to load patchnotes.Error: 2";
//			}
//			return;
//		}
//		timer.Stop();
//
//		uintmax_t size = fs::file_size(savePath);
//		Utility::AddLog("[UpdaterAPP] Downloaded file: \"" + URL + "\".(" + std::to_string(size) + " Bytes) Time used: " + std::to_string(timer.GetTimeS()) + "s");
//
//		//Read data from the patchnotes file
//		bool error = true;
//
//		std::string line;
//		std::fstream file(savePath);
//		if (file.is_open())
//		{
//			while (std::getline(file, line))
//			{
//				if (line.find("#patchnotes") != std::string::npos)
//				{
//					error = false;
//					continue;
//				}
//
//				this->patchnotes += line + "\n";
//			}
//
//			file.close();
//		}
//		else
//		{
//			Utility::AddLog("[PatchnotesWindow] ERROR: Unable to open file: \"" + savePath + "\"");
//			return;
//		}
//
//		if (error) this->patchnotes = "Unable to load patchnotes.Error: 4";
//	}
//}