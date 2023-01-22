#include "Model.h"
#include <fstream>
#include <sstream>
#include "Functions.h"
#include "SpriteCommon.h"
#include "PipelineManager.h"

using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ComPtr<ID3D12PipelineState> Model::pipelinestate = nullptr;
ComPtr<ID3D12RootSignature> Model::rootsignature = nullptr;
vector<Model*> Model::models;

void LoadVector3Stream(istringstream& stream, Vector3& vec)
{
	stream >> vec.x;
	stream >> vec.y;
	stream >> vec.z;
}

void Model::InitializeGraphicsPipeline()
{
	PipelineManager pipelineManager;
	pipelineManager.LoadShaders(L"OBJVertexShader", L"OBJPixelShader");
	pipelineManager.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineManager.AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	pipelineManager.InitDepthStencilState();
	pipelineManager.InitDSVFormat();
	pipelineManager.SetBlendDesc(D3D12_BLEND_OP_ADD, D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA);
	pipelineManager.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::CBV);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::CBV);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::DescriptorTable);
	pipelineManager.CreatePipeline(pipelinestate, rootsignature);
}

std::unique_ptr<Model> Model::Create(const string& modelName)
{
	unique_ptr<Model> newModel = make_unique<Model>();

	for (Model* model : models)
	{
		if (model->name == modelName)
		{
			newModel->name = model->name;
			newModel->vertices = model->vertices;
			newModel->material = model->material;
			newModel->indices = model->indices;
			newModel->CreateBuffers();
			return newModel;
		}
	}

	newModel->LoadFromOBJInternal(modelName);
	newModel->CreateBuffers();
	models.push_back(newModel.get());
	return newModel;
}

void Model::SetSprite(Sprite* sprite_)
{
	sprite.reset(sprite_);
}

void Model::TextureUpdate()
{
	assert(sprite);
	TextureUpdate(sprite.get());
}

void Model::TextureUpdate(Sprite* sprite)
{
	sprite->Update();
	Vector2 spriteSizeRate =
	{
		sprite->GetTextureSize().x / sprite->GetSize().x,
		sprite->GetTextureSize().y / sprite->GetSize().y
	};

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

void Model::LoadFromOBJInternal(const std::string& modelName)
{
	ifstream file;
	name = modelName;
	const string FILENAME = modelName + ".obj",
		DIRECTORY_PATH = "Resources/models/" + modelName + "/";
	file.open(DIRECTORY_PATH + FILENAME);
	assert(!file.fail());

	vector<Vector3> positions, normals;
	vector<Vector2> texcoords;

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
			Vector3 position{};
			LoadVector3Stream(line_stream, position);
			positions.emplace_back(position);
		}
		// テクスチャ座標読み込み
		if (key == "vt")
		{
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}
		// 法線ベクトル読み込み
		if (key == "vn")
		{
			Vector3 normal{};
			LoadVector3Stream(line_stream, normal);
			normals.emplace_back(normal);
		}
		// ポリゴンデータ読み込み
		if (key == "f")
		{
			string index_string;
			vector<VertexData> vert;
			while (getline(line_stream, index_string, ' '))
			{
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				VertexData vertex{};
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
			string path = DIRECTORY_PATH;
			path.erase(path.begin(), path.begin() + 10);
			sprite = Sprite::Create(path + material.textureFilename);
		}
	}
	file.close();
}

void Model::CreateBuffers()
{
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

	ConstBufferData* constMap = nullptr;
	// 定数バッファ生成
	BufferMapping(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	constMap->ambient = material.ambient;
	constMap->diffuse = material.diffuse;
	constMap->specular = material.specular;
	constMap->alpha = material.alpha;
	constBuffer->Unmap(0, nullptr);
}

void Model::PreDraw()
{
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Draw(const WorldTransform& worldTransform)
{
	Draw(worldTransform, sprite.get());
}

void Model::Draw(const WorldTransform& worldTransform, Sprite* sprite)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	cmdList->SetGraphicsRootConstantBufferView(0, worldTransform.constBuffer->GetGPUVirtualAddress());

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffer->GetGPUVirtualAddress());

	// デスクリプタヒープの配列
	SpriteCommon* spCommon = SpriteCommon::GetInstance();
	ID3D12DescriptorHeap* ppHeaps[] = { spCommon->GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (sprite)
	{
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(2, spCommon->GetGpuHandle(sprite->GetTextureIndex()));
	}
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}