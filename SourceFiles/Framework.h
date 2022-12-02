#pragma once
#include "WindowsAPI.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "scene/BaseScene.h"

class Framework
{
protected:
	// WindowsAPI初期化処理
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	// DirectX初期化処理
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// スプライトシステム初期化処理
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	// 入力の初期化処理
	Input* input = Input::GetInstance();
	// シーン基底クラス
	BaseScene* baseScene_ = nullptr;

public:
	virtual ~Framework() = default;
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw() = 0;
	virtual bool IsEndRequest();

	void Run();
};