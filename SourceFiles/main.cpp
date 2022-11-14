#include "MyClass.h"
#include "Buffer.h"
#include "Input.h"
#include "Sprite.h"
#include <memory>
#include "WindowsAPI.h"
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
	unique_ptr<SpriteCommon> spriteCommon = make_unique<SpriteCommon>();
	spriteCommon->Initialize();
	
	unique_ptr<Input> input = make_unique<Input>();
	input->Initialize();
#pragma endregion
#pragma region ゲームループで使う変数の定義
	unique_ptr<Sprite> sprite = make_unique<Sprite>();
	sprite->Initialize(spriteCommon.get());
#pragma endregion
	// ゲームループ
	while (1)
	{
#pragma region ウィンドウメッセージ処理
		// ✖ボタンで終了メッセージが来たらゲームループを抜ける
		if (wAPI->ProcessMessage()) { break; }
#pragma endregion
#pragma region DirectX毎フレーム処理
#pragma region 更新処理
		input->Update();
#pragma endregion
#pragma region 描画コマンド
		dxCommon->PreDraw();
		sprite->Draw();
		dxCommon->PostDraw();
#pragma endregion
#pragma endregion
	}

	// ウィンドウクラスを登録解除
	wAPI->Finalize();

	return 0;
}