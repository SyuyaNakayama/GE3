#pragma once
#include "BaseScene.h"
#include "SceneFactory.h"

class SceneManager final
{
private:
	BaseScene* scene_ = nullptr;
	Scene nextScene_ = Scene::Null;
	AbstractSceneFactory* sceneFactory_ = SceneFactory::GetInstance();

	SceneManager() = default;
public:
	static SceneManager* GetInstance();
	SceneManager(const SceneManager& obj) = delete;
	~SceneManager() { scene_->Finalize(); delete scene_; }
	void Update();
	void Draw();
	void SetNextScene(Scene nextScene) { nextScene_ = nextScene; }
};