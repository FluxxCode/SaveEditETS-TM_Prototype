#include "tmpch.h"
#include "Scene.h"

#include "Application/Application.h"

//Constructors/Destructors
Scene::Scene(Application* app)
	: app(app)
{
	this->config = this->app->GetConfig();
	this->resourceManager = this->app->GetResourceManager();
}

Scene::~Scene()
{

}

//Functions
void Scene::LoadScene(Scene* scene)
{
	if (scene != nullptr && this->app != nullptr)
	{
		//End current scene if exists
		if (this->app->GetScene())
			this->app->GetScene()->OnEnd();

		//Set current scene
		this->app->SetScene(scene);
		this->app->GetScene()->OnLoad();
	}
}