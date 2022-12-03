#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

class SceneManager final
{
private:
	BaseScene* scene_ = nullptr, * nextScene_ = nullptr;
	AbstractSceneFactory* sceneFactory_ = nullptr;

	SceneManager() = default;
public:
	static SceneManager* GetInstance();
	SceneManager(const SceneManager& obj) = delete;
	~SceneManager() { scene_->Finalize(); delete scene_; }
	void Update();
	void Draw();
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
};