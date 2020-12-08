#include "tmpch.h"
#include "RealtimeEditingWindow.h"

#include "ETS2/MemoryAddresses.h"

#ifndef LLONG_MIN
ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
#endif

//Functions
void RealtimeEditingWindow::Init()
{
	this->InitValues();
}

void RealtimeEditingWindow::InitValues()
{
	this->ets2ProcessID = Utility::GetProcessID(L"eurotrucks2.exe"); //Get process ID
	this->ets2ModuleBaseAddress = Utility::GetModuleBaseAddress(this->ets2ProcessID, L"eurotrucks2.exe"); //Get the eurotrucks2.exe base address
	this->ets2Process = OpenProcess(PROCESS_ALL_ACCESS, NULL, this->ets2ProcessID); //Get the process

	this->playerMoneyAdress = Utility::GetOffsetAddress(this->ets2Process, this->ets2ModuleBaseAddress, MemoryAddresses::playerMoney);
	ReadProcessMemory(this->ets2Process, (BYTE*)this->playerMoneyAdress, &this->playerMoney, sizeof(this->playerMoney), nullptr);
}

//Constructors/Destructors
RealtimeEditingWindow::RealtimeEditingWindow()
{
	this->Init();
}

RealtimeEditingWindow::~RealtimeEditingWindow()
{

}

//Functions
void RealtimeEditingWindow::Update(Config* config, Utility::ResourceManager* resourceManager, ImVec2 pos, ImVec2 size)
{
	this->config = config;
	this->resourceManager = resourceManager;
	this->pos  = pos;
	this->size = size;

	ImGui::SetNextWindowPos(this->pos);
	ImGui::SetNextWindowSize(this->size);
	ImGui::Begin(this->config->GetLabel("grew_title").c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

	ImGui::Text((this->config->GetLabel("grew_supported_version") + " " + this->supportedETSVersion).c_str());

	this->ets2ProcessID = Utility::GetProcessID(L"eurotrucks2.exe");
	if (this->ets2ProcessID == 0)
		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("grew_run_ets").c_str());
	else
	{
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, this->config->GetBiggestLabelSize("gwupd1") + 15.0f);

		const ImU64 step = 1000;
		static bool inputs_step = true;
		ImGui::TextColored(Utility::Colors::Blue, this->config->GetLabel("grew_player_money").c_str());
		ImGui::NextColumn();
		ReadProcessMemory(this->ets2Process, (BYTE*)this->playerMoneyAdress, &this->playerMoney, sizeof(this->playerMoney), nullptr);
		if (ImGui::InputScalar("##inputmoney", ImGuiDataType_U64, &this->playerMoney, inputs_step ? &step : NULL))
			WriteProcessMemory(this->ets2Process, (BYTE*)this->playerMoneyAdress, &this->playerMoney, sizeof(this->playerMoney), nullptr);

		if (this->playerMoney > ULLONG_MAX)	this->playerMoney = ULLONG_MAX;
		if (this->playerMoney < 0)			this->playerMoney = 0;
	}

	ImGui::End();
}