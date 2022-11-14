#pragma once
#include "SpriteCommon.h"
#include <DirectXMath.h>

class Sprite
{
	SpriteCommon* spriteCommon;
	DirectXCommon* dxCommon;
	// 頂点データ
	std::vector<DirectX::XMFLOAT3> vertices;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
public:
	void Initialize(SpriteCommon* spriteCommon);
	void Draw();
};