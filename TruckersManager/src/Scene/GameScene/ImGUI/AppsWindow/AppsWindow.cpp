#include "tmpch.h"
#include "AppsWindow.h"

void AppsWindow::LoadApps()
{
	this->appsConfig.clear();
	this->apps.clear();

	Utility::AddLog("[AppsWindow] Loading apps config...");

	std::string line;
	std::ifstream file("Apps\\config.ini");
	if (file.is_open())
	{
		while (getline(file, line))
		{
			Utility::AddLog("[AppsWindow] Active app: \"" + line + "\"");
			this->appsConfig.push_back(line);
		}

		file.close();
	}
	else
		Utility::AddLog("[AppsWindow] ERROR: Unable to open file: \"Apps\\config.ini\"");

	Utility::AddLog("[AppsWindow] Loading apps...");

	for (const auto& p : fs::directory_iterator("Apps"))
	{
		if (p.is_directory())
		{
			Utility::AddLog("[AppsWindow] Added app: \"" + p.path().string() + "\"");
			App app;

			app.name = p.path().filename().string();
			for (unsigned int i = 0; i < this->appsConfig.size(); i++)
			{
				if (this->appsConfig[i] == app.name)
				{
					app.active = true;
					break;
				}
			}

			this->apps.push_back(app);
		}
	}
}

void AppsWindow::SaveApps()
{
	Utility::AddLog("[AppsWindow] Saving apps...");

	std::ofstream file("Apps\\config.ini", std::ios::trunc);
	if (file.is_open())
	{
		for (int i = 0; i < this->apps.size(); i++)
		{
			if (this->apps[i].active)
			{
				Utility::AddLog("[AppsWindow] Saved: \"" + this->apps[i].name + "\"");
				file << this->apps[i].name << std::endl;
			}
		}

		file.close();
	}
	else
		Utility::AddLog("[AppsWindow] ERROR: Unable to open file: \"Apps\\config.ini\"");
}

void AppsWindow::Inject()
{
	Utility::AddLog("[AppsWindow] Injecting DLL...");
	this->SaveApps();

	Utility::Timer timer("Time to inject: ");
	if (!fs::exists(this->dllName))
	{
		Utility::AddLog("[AppsWindow] ERROR: Unable to inject. \"" + this->dllName + "\" does not exists!");
		this->info = this->config->GetLabel("grew_injected_error");
		return;
	}

	if (!(this->ets2ProcessID = Utility::GetProcessID(L"eurotrucks2.exe")))
	{
		Utility::AddLog("[AppsWindow] ERROR: Unable to inject. Unable to get proc_id from: \"eurotrucks2.exe\"");
		this->info = this->config->GetLabel("grew_injected_error");
		return;
	}

	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, NULL, this->ets2ProcessID);
	if (!proc)
	{
		Utility::AddLog("[AppsWindow] ERROR: Unable to inject. Unable to open process: " + std::to_string(GetLastError()));
		this->info = this->config->GetLabel("grew_injected_error");
		return;
	}

	void* allocatedMemory = VirtualAllocEx(proc, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!allocatedMemory)
	{
		Utility::AddLog("[AppsWindow] ERROR: Unable to allocate memory: " + std::to_string(GetLastError()));
		this->info = this->config->GetLabel("grew_injected_error");
		return;
	}

	if (!WriteProcessMemory(proc, allocatedMemory, fs::absolute(this->dllName).string().c_str(), MAX_PATH, nullptr))
	{
		Utility::AddLog("[AppsWindow] ERROR: Unable to write process memory: " + std::to_string(GetLastError()));
		this->info = this->config->GetLabel("grew_injected_error");
		return;
	}

	HANDLE thread = CreateRemoteThread(proc, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocatedMemory, NULL, nullptr);
	if (!thread)
	{
		Utility::AddLog("[AppsWindow] ERROR: Unable to create remote thread: " + std::to_string(GetLastError()));
		this->info = this->config->GetLabel("grew_injected_error");
		return;
	}

	CloseHandle(proc);
	VirtualFreeEx(proc, allocatedMemory, NULL, MEM_RELEASE);
	Utility::AddLog("[AppsWindow] Successfully injected!");
	this->info = this->config->GetLabel("grew_injected_successfully");
}

//Constructors/Destructors
AppsWindow::AppsWindow()
{
	this->LoadApps();
}

AppsWindow::~AppsWindow()
{

}

//Functions
void AppsWindow::Update(Config* config, Utility::ResourceManager* resourceManager, ImVec2 pos, ImVec2 size)
{
	this->config = config;
	this->resourceManager = resourceManager;
	this->pos  = pos;
	this->size = size;

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);

	ImGui::Begin(config->GetLabel("gaw_title").c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

	GLuint* img = this->resourceManager->LoadTexture("Resources\\Icons\\Freepik\\refresh-button.png");
	if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(16.0f, 16.0f)))
		this->LoadApps();
	Utility::ImGuiToolTip(this->config->GetLabel("grew_btn_reload_tt"));

	ImGui::SameLine();
	if (ImGui::Button(this->config->GetLabel("grew_btn_save").c_str()))
		this->SaveApps();

	ImGui::SameLine();
	if (ImGui::Button(this->config->GetLabel("grew_btn_inject").c_str()))
		this->Inject();

	ImGui::SameLine();
	ImGui::Text(this->info.c_str());

	ImGui::BeginChild("##appselection", ImVec2(695.0f, 612.0f), true);	

	for (unsigned int i = 0; i < this->apps.size(); i++)
	{
		if (i % 3 != 0)
			ImGui::SameLine();

		ImGui::BeginChild(("##app" + std::to_string(i)).c_str(), ImVec2(217.5f, 155.0f), true, ImGuiWindowFlags_NoScrollbar);
		ImGui::SetWindowFontScale(1.0f);

		img = this->resourceManager->LoadTexture("Resources\\Icons\\Freepik\\gear.png");
		ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(16.0f, 16.0f));
		Utility::ImGuiToolTip(this->config->GetLabel("grew_btn_edit_tt"));
		
		ImGui::SameLine();
		
		if (this->apps[i].active)
		{
			img = this->resourceManager->LoadTexture("Resources\\Icons\\Circle_active.png");
			if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(16.0f, 16.0f)))
				this->apps[i].active = false;
			Utility::ImGuiToolTip(this->config->GetLabel("grew_btn_active_tt"));
		}
		else
		{
			img = this->resourceManager->LoadTexture("Resources\\Icons\\Circle_disabled.png");
			if (ImGui::ImageButton((void*)(intptr_t)*img, ImVec2(16.0f, 16.0f)))
				this->apps[i].active = true;
			Utility::ImGuiToolTip(this->config->GetLabel("grew_btn_disabled_tt"));
		}

		ImGui::SameLine();
		ImGui::TextColored(Utility::Colors::Blue, this->apps[i].name.c_str());
		img = this->resourceManager->LoadTexture("Apps\\" + this->apps[i].name + "\\preview.png");
		ImGui::Image((void*)(intptr_t)*img, ImVec2(200.0f, 113.0f));

		ImGui::EndChild();
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("##appproperties", ImVec2(398.5f, 612.0f), true);

	ImGui::EndChild();

	ImGui::End();
}