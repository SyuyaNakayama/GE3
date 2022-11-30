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
#pragma region WindowsAPI初期化処理
	// ウィンドウクラスの設定
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	wAPI->Initialize();
#pragma endregion 
#pragma region DirectX初期化処理
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize();
#pragma endregion
#pragma region 基盤システム初期化処理
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize();

	Input* input = Input::GetInstance();
	input->Initialize();
#pragma endregion
#pragma region ゲームループで使う変数の定義
	unique_ptr<Sprite> sprites[4];
	for (int i = 0; i < 4; i++)
	{
		sprites[i] = make_unique<Sprite>();
		sprites[i]->Initialize();
		Vector2 pos = sprites[i]->GetPosition();
		pos = { 200.0f * (float)i,200.0f * (float)i };
		sprites[i]->SetPosition(pos);
	}
	sprites[2]->SetIsInvisible();
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
		for (unique_ptr<Sprite>& sprite : sprites) { sprite->Draw(); }
		dxCommon->PostDraw();
	}

	// ウィンドウクラスを登録解除
	wAPI->Finalize();

	return 0;
}