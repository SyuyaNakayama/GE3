#include "Framework.h"
#include "Audio.h"
#include "ImGuiManager.h"

void Framework::Initialize()
{
	wAPI->Initialize();
	dxCommon->Initialize();
	spriteCommon->Initialize();
	input->Initialize();
	ImGuiManager::Initialize();
	Audio::StaticInitialize();
	sceneManager_->Initialize();
}

void Framework::Finalize()
{
	sceneManager_->Finalize();
	Audio::Finalize();
	ImGuiManager::Finalize();
	wAPI->Finalize();
}

void Framework::Update()
{
	input->Update();
	ImGuiManager::Begin();
	sceneManager_->Update();
	ImGuiManager::End();
}

bool Framework::IsEndRequest()
{
	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (wAPI->ProcessMessage()) { return true; }
	if (input->IsTrigger(DIK_ESCAPE)) { return true; }
	return false;
}

void Framework::Run()
{
	Initialize();

	while (true)
	{
		Update();
		if (IsEndRequest()) { break; }
		Draw();
	}

	Finalize();
}