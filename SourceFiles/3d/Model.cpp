#include "Model.h"
#include <fstream>
#include <sstream>
#include "SpriteCommon.h"
#include "PipelineManager.h"
#include "Functions.h"

using namespace Microsoft::WRL;
using namespace std;

// 静的メンバ変数の実体
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
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::CBV);
	pipelineManager.CreatePipeline(pipelinestate, rootsignature);
}

std::unique_ptr<Model> Model::Create(const string& modelName, bool smoothing)
{
	unique_ptr<Model> newModel = make_unique<Model>();

	for (Model* model : models)
	{
		if (model->name == modelName)
		{
			newModel->name = model->name;
			newModel->mesh = model->mesh;
			newModel->material = model->material;
			unique_ptr<Sprite> newSprite = make_unique<Sprite>();
			*newSprite = *model->sprite;
			newModel->sprite = move(newSprite);
			newModel->CreateBuffers();
			return newModel;
		}
	}

	newModel->LoadFromOBJInternal(modelName, smoothing);
	newModel->CreateBuffers();
	models.push_back(newModel.get());
	return newModel;
}

void Model::TextureUpdate(Sprite* sprite)
{
	sprite->Update();
	Vector2 spriteSizeRate =
	{
		sprite->GetTextureSize().x / sprite->GetSize().x,
		sprite->GetTextureSize().y / sprite->GetSize().y
	};
	mesh.Update(sprite, spriteSizeRate);
}

void Model::LoadFromOBJInternal(const std::string& modelName, bool smoothing)
{
	const string FILENAME = modelName + ".obj";
	const string DIRECTORY_PATH = "Resources/models/" + modelName + "/";

	ifstream file;
	name = modelName;
	file.open(DIRECTORY_PATH + FILENAME);
	assert(!file.fail());

	vector<Vector3> positions, normals;
	vector<Vector2> texcoords;
	int indexCountTex = 0;
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
			int faceIndexCount = 0;

			while (getline(line_stream, index_string, ' '))
			{
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				// 頂点データの追加
				Mesh::VertexData vertex;
				vertex.pos = positions[(size_t)indexPosition - 1];
				vertex.normal = normals[(size_t)indexNormal - 1];
				vertex.uv = texcoords[(size_t)indexTexcoord - 1];
				mesh.AddVertex(vertex);
				if (smoothing) { mesh.AddSmoothData(indexPosition, (UINT16)mesh.GetVertexCount() - 1); }

				// インデックスデータの追加
				if (faceIndexCount >= 3)
				{
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
					mesh.AddIndex(indexCountTex - 1);
					mesh.AddIndex(indexCountTex);
					mesh.AddIndex(indexCountTex - 3);
				}
				else { mesh.AddIndex(indexCountTex); }

				indexCountTex++;
				faceIndexCount++;
			}
		}
	}
	file.close();

	if (smoothing) { mesh.CalculateSmoothedVertexNormals(); }
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
	mesh.CreateBuffers();

	ConstBufferData* constMap = nullptr;
	// 定数バッファ生成
	CreateBuffer(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	constMap->ambient = material.ambient;
	constMap->diffuse = material.diffuse;
	constMap->specular = material.specular;
	constMap->alpha = material.alpha;
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
	// ライトの描画
	WorldTransform::GetLightGroup()->Draw(3);
}

void Model::Draw(const WorldTransform& worldTransform, Sprite* sprite)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	cmdList->SetGraphicsRootConstantBufferView(0, worldTransform.constBuffer->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffer->GetGPUVirtualAddress());

	// デスクリプタヒープの配列
	SpriteCommon* spCommon = SpriteCommon::GetInstance();
	ID3D12DescriptorHeap* ppHeaps[] = { spCommon->GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダリソースビューをセット
	assert(sprite);
	cmdList->SetGraphicsRootDescriptorTable(2, spCommon->GetGpuHandle(sprite->GetTextureIndex()));

	mesh.Draw();
}