#include "Model.h"
#include <fstream>
#include <sstream>
#include <DirectXTex.h>
#include <d3dx12.h>
#include "Functions.h"
#include "SpriteCommon.h"

using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ComPtr<ID3D12PipelineState> Model::pipelinestate = nullptr;
ComPtr<ID3D12RootSignature> Model::rootsignature = nullptr;
vector<Model*> Model::models = {};

void LoadVector3Stream(istringstream& stream, Vector3& vec)
{
	stream >> vec.x;
	stream >> vec.y;
	stream >> vec.z;
}

void Model::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	LoadShader(&vsBlob, L"OBJVertexShader", "vs_5_0");
	// ピクセルシェーダの読み込みとコンパイル
	LoadShader(&psBlob, L"OBJPixelShader", "ps_5_0");

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		SetInputLayout("POSITION",DXGI_FORMAT_R32G32B32_FLOAT),
		SetInputLayout("NORMAL",DXGI_FORMAT_R32G32B32_FLOAT),
		SetInputLayout("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT),
		SetInputLayout("COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT)
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));
}

Model* Model::LoadFromOBJ(const string& modelName)
{
	for (Model* model : models)
	{
		if (model->name == modelName) { return model; }
	}

	Model* model = new Model();
	model->LoadFromOBJInternal(modelName);
	model->CreateBuffers();
	models.push_back(model);
	return model;
}

void Model::SetSprite(Sprite* sprite_)
{
	isSpriteChange = sprite != sprite_;
	sprite = sprite_;
}

void Model::TextureUpdate()
{
	if (!isSpriteChange) { return; }
	sprite->Update();
	for (size_t i = 0; i < vertices.size(); i++)
	{
		Vector2 spriteUvLT = sprite->GetVerticesUv(Sprite::VertexNumber::LT);
		Vector2 spriteUvRB = sprite->GetVerticesUv(Sprite::VertexNumber::RB);
		Vector2 uv = vertices[i].uv;
		uv.x *= spriteUvRB.x;
		uv.y *= spriteUvRB.y;
		uv += spriteUvLT;
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
	Draw(worldTransform, sprite);
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

	if (!material.textureFilename.empty())
	{
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(2, spCommon->GetGpuHandle(sprite->GetTextureIndex()));
	}
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}