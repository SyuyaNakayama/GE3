#include "Model.h"
#include <fstream>
#include <sstream>
#include <DirectXTex.h>
#include "Functions.h"
#include "Vector3.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void LoadVector3Stream(istringstream& stream, XMFLOAT3& vec)
{
	stream >> vec.x;
	stream >> vec.y;
	stream >> vec.z;
}

void Model::LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME)
{
	ifstream file;
	file.open(DIRECTORY_PATH + FILENAME);
	assert(!file.fail());

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') { key.erase(key.begin()); }
		if (key == "newmtl") { line_stream >> material.name; }
		if (key == "Ka") { LoadVector3Stream(line_stream, material.ambient); }
		if (key == "Kd") { LoadVector3Stream(line_stream, material.diffuse); }
		if (key == "Ks") { LoadVector3Stream(line_stream, material.specular); }
		if (key == "map_Kd")
		{
			line_stream >> material.textureFilename;
			//LoadTexture(DIRECTORY_PATH, material.textureFilename);
			string path = DIRECTORY_PATH;
			path.erase(path.begin(), path.begin() + 10);
			sprite = Sprite::Create(path + material.textureFilename);
		}
	}
	file.close();
}

void Model::CreateBuffers()
{
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	// 頂点バッファ生成
	BufferMapping(&vertBuff, &vertMap, sizeVB);
	// 全頂点に対して
	copy(vertices.begin(), vertices.end(), vertMap); // 座標をコピー
	vertBuff->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexPosNormalUv);

	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i].color = { 1,1,1,1 };
	}

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

	ConstBufferDataB1* constMap1 = nullptr;
	// 定数バッファ生成
	BufferMapping(&constBuffB1, &constMap1, (sizeof(ConstBufferDataB1) + 0xff) & ~0xff);

	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);
}

Model* Model::LoadFromOBJ(const std::string& modelName)
{
	Model* model = new Model();
	model->LoadFromOBJInternal(modelName);
	model->CreateBuffers();

	return model;
}

void Model::LoadFromOBJInternal(const std::string& modelName)
{
	ifstream file;
	const string FILENAME = modelName + ".obj",
		DIRECTORY_PATH = "Resources/" + modelName + "/";
	file.open(DIRECTORY_PATH + FILENAME);
	assert(!file.fail());

	vector<XMFLOAT3> positions, normals;
	vector<XMFLOAT2> texcoords;

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		// マテリアル読み込み
		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;
			LoadMaterial(DIRECTORY_PATH, filename);
		}
		// 頂点座標読み込み
		if (key == "v")
		{
			XMFLOAT3 position{};
			LoadVector3Stream(line_stream, position);
			positions.emplace_back(position);
		}
		// テクスチャ座標読み込み
		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}
		// 法線ベクトル読み込み
		if (key == "vn")
		{
			XMFLOAT3 normal{};
			LoadVector3Stream(line_stream, normal);
			normals.emplace_back(normal);
		}
		// ポリゴンデータ読み込み
		if (key == "f")
		{
			string index_string;
			vector<VertexPosNormalUv> vert;
			while (getline(line_stream, index_string, ' '))
			{
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				VertexPosNormalUv vertex{};
				vertex.pos = positions[(size_t)indexPosition - 1];
				vertex.normal = normals[(size_t)indexNormal - 1];
				vertex.uv = texcoords[(size_t)indexTexcoord - 1];
				vertices.emplace_back(vertex);
				vert.emplace_back(vertex);
				indices.emplace_back(indices.size());

				if (vert.size() != 4) { continue; }
				vertices.pop_back();
				for (size_t j = 0; j < 4; j++)
				{
					if (j == 1) { continue; }
					vertices.push_back(vert[j]);
					indices.emplace_back(indices.size());
				}
			}
		}
	}
	file.close();

	sprite->SetTextureLeftTop({ 200,200 });
	sprite->Update();
}

#include "SpriteCommon.h"

void Model::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	// デスクリプタヒープの配列
	SpriteCommon* spCommon = SpriteCommon::GetInstance();
	ID3D12DescriptorHeap* ppHeaps[] = { spCommon->GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (!material.textureFilename.empty())
	{
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(2, spCommon->GetGpuHandle(sprite->GetTextureIndex()));
	}
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}