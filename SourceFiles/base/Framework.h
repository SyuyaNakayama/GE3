#pragma once
#include "WindowsAPI.h"
#include "SpriteCommon.h"
#include "SceneManager.h"
#include "Audio.h"
#include "ImGuiManager.h"

class Framework
{
protected:
	// WindowsAPI����������
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	// DirectX����������
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// �X�v���C�g�V�X�e������������
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	// ���͂̏���������
	Input* input = Input::GetInstance();
	SceneManager* sceneManager_ = SceneManager::GetInstance();
	Audio* audio = Audio::GetInatance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

public:
	virtual ~Framework() = default;
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw() = 0;
	virtual bool IsEndRequest();

	void Run();
};