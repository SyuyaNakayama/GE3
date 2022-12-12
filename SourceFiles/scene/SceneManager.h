#pragma once
#include "BaseScene.h"
#include "FadeManager.h"

class SceneManager final
{
private:
	BaseScene* scene_ = nullptr, * nextScene_ = nullptr;
	FadeManager fadeManager_;

	SceneManager() = default;
public:
	static SceneManager* GetInstance();
	SceneManager(const SceneManager& obj) = delete;
	~SceneManager() { scene_->Finalize(); delete scene_; }
	void Initialize();
	void Update();
	void Draw();
	void SetNextScene(BaseScene* nextScene, bool isUseFade = true);
};