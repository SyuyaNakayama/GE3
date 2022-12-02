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
	// WindowsAPI����������
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	// DirectX����������
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// ��ՃV�X�e������������
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	// ���͂̏���������
	Input* input = Input::GetInstance();
	// �Q�[�����[�v�Ŏg���ϐ��̒�`
	std::vector<Sprite*> sprites{};

public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	bool IsEndRequest();
};