#pragma once
#include "BaseScene.h"
#include "FadeManager.h"
#include "SceneFactory.h"

class SceneManager final
{
private:
	BaseScene* scene_ = nullptr;
	Scene nextScene_ = Scene::Null;
	FadeManager fadeManager_;
	SceneFactory* sceneFactory_ = SceneFactory::GetInstance();

	SceneManager() = default;
public:
	static SceneManager* GetInstance();
	SceneManager(const SceneManager& obj) = delete;
	~SceneManager() { scene_->Finalize(); delete scene_; }
	void Initialize();
	void Update();
	void Draw();
	void SetNextScene(Scene nextScene, bool isUseFade = true);
};