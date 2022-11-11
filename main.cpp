#include "MyClass.h"
#include "Buffer.h"
#include "Input.h"
#include "DirectXCommon.h"

using namespace DirectX;
using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region WindowsAPI初期化処理
	// ウィンドウクラスの設定
	WindowsAPI* wAPI = nullptr;
	wAPI = new WindowsAPI();
	wAPI->Initialize();
#pragma endregion 
#pragma region DirectX初期化処理
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(wAPI);
#pragma endregion
	Input* input = nullptr;
	input = new Input();
	input->Initialize(*wAPI);
#pragma region ゲームループで使う変数の定義
	
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
		

		dxCommon->PostDraw();
#pragma endregion
#pragma endregion
#pragma endregion
	}

	// ウィンドウクラスを登録解除
	delete input;
	delete dxCommon;
	wAPI->Finalize();
	delete wAPI;

	return 0;
}