#include "Framework.h"

void Framework::Initialize()
{
	wAPI->Initialize();
	dxCommon->Initialize();
	spriteCommon->Initialize();
	input->Initialize();
	sceneFactory_ = new SceneFactory();
	sceneManager_ = SceneManager::GetInstance();
	sceneManager_->SetSceneFactory(sceneFactory_);
}

void Framework::Finalize()
{
	delete sceneFactory_;
	wAPI->Finalize();
}

void Framework::Update()
{
	input->Update();
	sceneManager_->Update();
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