#pragma once
#include "SpriteCommon.h"
#include <DirectXMath.h>

class Sprite
{
	SpriteCommon* spriteCommon;
	DirectXCommon* dxCommon;
	// ���_�f�[�^
	std::vector<DirectX::XMFLOAT3> vertices;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
public:
	void Initialize(SpriteCommon* spriteCommon);
	void Draw();
};