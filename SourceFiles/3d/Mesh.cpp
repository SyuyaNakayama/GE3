#include "Mesh.h"
#include "Functions.h"

void Mesh::CreateBuffers() {
	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices.size());
	// 頂点バッファ生成
	BufferMapping(&vertBuff, &vertMap, sizeVB);
	// 全頂点に対して
	copy(vertices.begin(), vertices.end(), vertMap); // 座標をコピー
	vertBuff->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexData);

	unsigned short* indexMap = nullptr;
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// インデックスバッファ生成
	BufferMapping(&indexBuff, &indexMap, sizeIB);
	// 全インデックスに対して
	copy(indices.begin(), indices.end(), indexMap);	// インデックスをコピー
	indexBuff->Unmap(0, nullptr);
	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList) {
	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(2, material->GetGpuHandle());

	// マテリアルの定数バッファをセット
	ID3D12Resource* constBuff = material->GetConstantBuffer();
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Mesh::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		// 各面用の共通頂点コンテナ
		std::vector<UINT16>& v = itr->second;
		// 全頂点の法線を平均する
		XMVECTOR normal{};
		for (UINT16 index : v)
		{
			normal += XMVectorSet(vertices[index].normal.x,
				vertices[index].normal.y, vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		// 共通法線を使用する全ての頂点データに書き込む
		for (UINT16 index : v)
		{
			vertices[index].normal = { normal.m128_f32[0],
				normal.m128_f32[1],normal.m128_f32[2] };
		}
	}
}