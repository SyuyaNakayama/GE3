#include "Framework.h"

void Framework::Initialize()
{
	wAPI->Initialize();
	dxCommon->Initialize();
	spriteCommon->Initialize();
	input->Initialize();
	imguiManager->Initialize();
	audio->Initialize();
	sceneManager_->Initialize();
}

void Framework::Finalize()
{
	sceneManager_->Finalize();
	audio->Finalize();
	imguiManager->Finalize();
	wAPI->Finalize();
}

void Framework::Update()
{
	input->Update();
	imguiManager->Begin();
	sceneManager_->Update();
	imguiManager->End();
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