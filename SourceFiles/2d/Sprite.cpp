#include "Sprite.h"
#include "SpriteCommon.h"
#include "WindowsAPI.h"
#include <cassert>
using namespace Microsoft::WRL;

// 平行投影行列
const Matrix4 Sprite::matProj = OrthoGraphic(WindowsAPI::GetInstance()->WIN_SIZE);

template<class T> void BufferMapping(ID3D12Resource** buff, T** map, UINT64 width)
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = width;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// バッファの生成
	HRESULT result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(buff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = (*buff)->Map(0, nullptr, (void**)map);
	assert(SUCCEEDED(result));
}

void Sprite::Initialize(uint32_t textureIndex)
{
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	ID3D12Resource* vertBuff = nullptr;
	BufferMapping<Vertex>(&vertBuff, &vertMap, sizeVB);

	// 全頂点に対して座標をコピー
	for (int i = 0; i < vertices.size(); i++) { vertMap[i] = vertices[i]; }

	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);

	// 定数バッファ
	BufferMapping<ConstBufferDataMaterial>(constBuffMaterial.GetAddressOf(),
		&constMapMaterial, (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff);

	BufferMapping<ConstBufferDataTransform>(constBuffTransform.GetAddressOf(),
		&constMapTransform, (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	textureIndex_ = textureIndex;
	AdjustTextureSize();
	size_ = textureSize_;
}

void Sprite::AdjustTextureSize()
{
	ID3D12Resource* textureBuffer = SpriteCommon::GetInstance()->GetTextureBuffer(textureIndex_);
	assert(textureBuffer);

	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}

void Sprite::Update()
{
	static bool flag = false;
	if (!flag)
	{
		flag = true;

		float left = (0.0f - anchorPoint_.x) * size_.x;
		float right = (1.0f - anchorPoint_.x) * size_.x;
		float top = (0.0f - anchorPoint_.y) * size_.y;
		float bottom = (1.0f - anchorPoint_.y) * size_.y;

		if (isFlipX_) { left = -left; right = -right; }
		if (isFlipY_) { top = -top; bottom = -bottom; }

		vertices[LB].pos = { left, bottom };
		vertices[LT].pos = { left, top };
		vertices[RB].pos = { right, bottom };
		vertices[RT].pos = { right, top };

		ID3D12Resource* textureBuffer = SpriteCommon::GetInstance()->GetTextureBuffer(textureIndex_);
		D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom };
		vertices[LT].uv = { tex_left, tex_top };
		vertices[RB].uv = { tex_right, tex_bottom };
		vertices[RT].uv = { tex_right, tex_top };

	}
		Matrix4 matRot, matTrans;
		matRot = RotateZ(rotation_);
		matTrans = Translate(VectorChange(position_));

		matWorld = matRot * matTrans;

	// GPU転送
	constMapTransform->mat = matWorld * matProj;
	constMapMaterial->color = color_;
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
}

void Sprite::Draw()
{
	if (isInvisible_) { return; }

	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	spriteCommon->SetTextureCommands(textureIndex_);

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	// 描画コマンド
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // 全ての頂点を使って描画
}