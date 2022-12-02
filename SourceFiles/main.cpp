#include "MyClass.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include <memory>
#include "WindowsAPI.h"
#include "DirectXCommon.h"
using namespace DirectX;
using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region 汎用機能初期化
	// WindowsAPI初期化処理
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	wAPI->Initialize();
	// DirectX初期化処理
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize();
	// 基盤システム初期化処理
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize();
	// 入力の初期化処理
	Input* input = Input::GetInstance();
	input->Initialize();
#pragma endregion
	// ゲームループで使う変数の定義
	vector<Sprite*> sprites{};
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
#pragma endregion
	// ゲームループ
	while (!wAPI->ProcessMessage())
	{
		// ✖ボタンで終了メッセージが来たらゲームループを抜ける
		if (wAPI->ProcessMessage()) { break; }
		// 更新処理
		input->Update();
		for (Sprite*& sprite : sprites) { sprite->Update(); }
		// 描画処理
		dxCommon->PreDraw();

		spriteCommon->PreDraw();
		for (Sprite*& sprite : sprites) { sprite->Draw(); }
		spriteCommon->PostDraw();
		
		dxCommon->PostDraw();
	}

	for (Sprite*& sprite : sprites) { delete sprite; }
	
	// ウィンドウクラスを登録解除
	wAPI->Finalize();

	return 0;
}