#include "SceneManager.h"
#include "SpriteCommon.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Initialize()
{
	fadeManager_.Initialize();
}

void SceneManager::Update()
{
	fadeManager_.Update();
	if (fadeManager_.IsChange() || !scene_)
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
	}

	if (!fadeManager_.IsFade()) { scene_->Update(); }
}

void SceneManager::Draw()
{
	scene_->Draw();
	SpriteCommon::GetInstance()->PreDraw();
	fadeManager_.Draw();
	SpriteCommon::GetInstance()->PostDraw();
}

void SceneManager::SetNextScene(BaseScene* nextScene, bool isUseFade)
{
	nextScene_ = nextScene;
	if (isUseFade) { fadeManager_.FadeScene(); }
}
