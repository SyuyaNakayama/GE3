#include "SceneManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Update()
{
	if (nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}

		scene_ = nextScene_;
		nextScene_ = nullptr;
		scene_->SetSceneManager(this);
		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}
