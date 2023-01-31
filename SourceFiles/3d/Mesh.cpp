#include "Mesh.h"
#include "Functions.h"

void Mesh::CreateBuffers()
{
	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices.size());
	// 頂点バッファ生成
	CreateBuffer(&vertBuff, &vertMap, sizeVB);
	// 全頂点に対して
	copy(vertices.begin(), vertices.end(), vertMap); // 座標をコピー
	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexData);

	UINT16* indexMap = nullptr;
	UINT sizeIB = static_cast<UINT>(sizeof(UINT16) * indices.size());
	// インデックスバッファ生成
	CreateBuffer(&indexBuff, &indexMap, sizeIB);
	// 全インデックスに対して
	copy(indices.begin(), indices.end(), indexMap);	// インデックスをコピー
	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Mesh::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		// 各面用の共通頂点コンテナ
		std::vector<UINT16>& v = itr->second;
		// 全頂点の法線を平均する
		Vector3 normal{};
		for (UINT16 index : v) { normal += vertices[index].normal; }
		normal = Normalize(normal / (float)v.size());
		// 共通法線を使用する全ての頂点データに書き込む
		for (UINT16 index : v) { vertices[index].normal = normal; }
	}
}

void Mesh::Update(Sprite* sprite, Vector2 spriteSizeRate)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		Vector2 uv = vertices[i].uv;
		uv.x *= spriteSizeRate.x;
		uv.y *= spriteSizeRate.y;
		uv += sprite->GetVerticesUv(Sprite::VertexNumber::LT);
		vertMap[i].uv = uv;
		vertMap[i].color = sprite->GetColor();
	}
}

void Mesh::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}