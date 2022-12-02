#include "MyGame.h"
using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	wAPI->Initialize();
	dxCommon->Initialize();
	spriteCommon->Initialize();
	input->Initialize();

	sprites.push_back(Sprite::Create("Map.png"));
	sprites.push_back(Sprite::Create("reimu.png"));
	sprites.push_back(Sprite::Create("Map.png"));
	sprites.push_back(Sprite::Create("reimu.png"));

	for (size_t i = 0; i < 4; i++)
	{
		Vector2 pos = sprites[i]->GetPosition();
		pos = { 200.0f * (float)i,0 };
		sprites[i]->SetPosition(pos);
	}
}

void MyGame::Finalize()
{
	for (Sprite*& sprite : sprites) { delete sprite; }

	// ウィンドウクラスを登録解除
	wAPI->Finalize();
}

void MyGame::Update()
{
	input->Update();
	for (Sprite*& sprite : sprites) { sprite->Update(); }
}

void MyGame::Draw()
{
	dxCommon->PreDraw();

	spriteCommon->PreDraw();
	for (Sprite*& sprite : sprites) { sprite->Draw(); }
	spriteCommon->PostDraw();

	dxCommon->PostDraw();
}

bool MyGame::IsEndRequest()
{
	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (wAPI->ProcessMessage()) { return true; }
	if (input->IsTrigger(DIK_ESCAPE)) { return true; }
	return false;
}