#pragma once
#include "Input.h"
#include "ImGuiManager.h"
class SceneManager;

class BaseScene
{
protected:
	SceneManager* sceneManager_ = nullptr;
	Input* input = Input::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

public:
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
};