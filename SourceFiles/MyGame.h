#pragma once
#include "MyClass.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include <memory>
#include "WindowsAPI.h"
#include "DirectXCommon.h"

class MyGame
{
private:
	// WindowsAPI初期化処理
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	// DirectX初期化処理
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 基盤システム初期化処理
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	// 入力の初期化処理
	Input* input = Input::GetInstance();
	// ゲームループで使う変数の定義
	std::vector<Sprite*> sprites{};

public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	bool IsEndRequest();
};