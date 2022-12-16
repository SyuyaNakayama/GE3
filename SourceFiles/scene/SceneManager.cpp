#include "SceneManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Update()
{
	if (nextScene_ != Scene::Null)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}

		scene_ = sceneFactory_->CreateScene(nextScene_);
		nextScene_ = Scene::Null;
		scene_->SetSceneManager(this);
		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}
