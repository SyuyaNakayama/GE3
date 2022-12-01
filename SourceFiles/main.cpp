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
	// リソース読み込み
	spriteCommon->LoadTexture(0, "Map.png");
	spriteCommon->LoadTexture(1, "reimu.png");

	// ゲームループで使う変数の定義
	unique_ptr<Sprite> sprites[4];
	for (int i = 0; i < 4; i++)
	{
		sprites[i] = make_unique<Sprite>();
		sprites[i]->Initialize();
		Vector2 pos = sprites[i]->GetPosition();
		pos = { 200.0f * (float)i,0 };
		sprites[i]->SetPosition(pos);
		sprites[i]->SetTextureIndex(i % 2);
	}
	
#pragma endregion
	// ゲームループ
	while (!wAPI->ProcessMessage())
	{
		// ✖ボタンで終了メッセージが来たらゲームループを抜ける
		if (wAPI->ProcessMessage()) { break; }
		// 更新処理
		input->Update();
		for (unique_ptr<Sprite>& sprite : sprites) { sprite->Update(); }
		// 描画処理
		dxCommon->PreDraw();

		spriteCommon->PreDraw();
		for (unique_ptr<Sprite>& sprite : sprites) { sprite->Draw(); }
		spriteCommon->PostDraw();
		
		dxCommon->PostDraw();
	}

	// ウィンドウクラスを登録解除
	wAPI->Finalize();

	return 0;
}