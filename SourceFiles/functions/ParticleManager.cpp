#include "ParticleManager.h"
#include "Functions.h"
#include "SpriteCommon.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;
ComPtr<ID3D12Resource> ParticleManager::vertBuff;
Matrix4 ParticleManager::matBillboard = Matrix4::Identity();
D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView{};
ViewProjection* ParticleManager::viewProjection = nullptr;
uint32_t ParticleManager::textureIndex = 0;

void ParticleManager::StaticInitialize(ViewProjection* viewProjection)
{
	ParticleManager::viewProjection = viewProjection;

	// nullptrチェック
	assert(viewProjection);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// テクスチャ読み込み
	textureIndex = SpriteCommon::GetInstance()->LoadTexture("Particle.png", 1);
	// モデル生成
	CreateModel();
}

void ParticleManager::InitializeGraphicsPipeline()
{
	Result result;
	ComPtr<ID3DBlob> vsBlob;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	LoadShader(&vsBlob, L"ParticleVS", "vs_5_0");

	// ピクセルシェーダの読み込みとコンパイル
	LoadShader(&psBlob, L"ParticlePS", "ps_5_0");

	// ジオメトリシェーダの読み込みとコンパイル
	LoadShader(&gsBlob, L"ParticleGS", "gs_5_0");

	// 頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		SetInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT),
		SetInputLayout("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT),
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc[2]{};
	for (size_t i = 0; i < 2; i++)
	{
		blenddesc[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
		blenddesc[i].BlendEnable = true;
		blenddesc[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc[i].SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc[i].DestBlendAlpha = D3D12_BLEND_ZERO;
		blenddesc[i].SrcBlend = D3D12_BLEND_ONE;
		blenddesc[i].DestBlend = D3D12_BLEND_ONE;
	}
	// 光パーティクル
	blenddesc[0].BlendOp = D3D12_BLEND_OP_ADD;
	// 闇パーティクル
	blenddesc[1].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc[0];

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout.data();
	gpipeline.InputLayout.NumElements = inputLayout.size();

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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
	assert(device);
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
}

void ParticleManager::CreateModel()
{
	VertexPos* vertMap = nullptr;
	BufferMapping(&vertBuff, &vertMap, vertexCount);
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = vertexCount;
	vbView.StrideInBytes = sizeof(VertexPos);
}

void Vector3ToArray(Vector3 vec, float* array)
{
	array[0] = vec.x;
	array[1] = vec.y;
	array[2] = vec.z;
	array[3] = 0;
}

void ParticleManager::UpdateViewMatrix()
{
	Vector3 cameraAxisZ = viewProjection->target - viewProjection->eye;
	// 0ベクトルの時
	assert(!(cameraAxisZ == Vector3(0, 0, 0)));
	assert(!(viewProjection->up == Vector3(0, 0, 0)));

	cameraAxisZ.Normalize();

	Vector3 cameraAxisX = Cross(viewProjection->up, cameraAxisZ);
	cameraAxisX = cameraAxisX.Normalize();

	Vector3 cameraAxisY = Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = cameraAxisY.Normalize();

	Matrix4 matCameraRot{};

	Vector3ToArray(cameraAxisX, matCameraRot.m[0]);
	Vector3ToArray(cameraAxisY, matCameraRot.m[1]);
	Vector3ToArray(cameraAxisZ, matCameraRot.m[2]);

	matBillboard = matCameraRot;
}

void ParticleManager::Initialize()
{
	if (!particles.empty()) { particles.clear(); }
	BufferMapping(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Update()
{
	HRESULT result;

	particles.remove_if([](Particle& x) { return x.frame >= x.num_frame; });

	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end(); it++)
	{
		it->frame++;
		it->velocity += it->accel;
		it->position += it->velocity;
		float f = (float)it->num_frame / it->frame;
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
	}

	// 定数バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end(); it++)
		{
			vertMap->pos = it->position;
			vertMap->scale = it->scale;
			vertMap++;
		}
	}
	vertBuff->Unmap(0, nullptr);

	UpdateViewMatrix();

	// 定数バッファへデータ転送
	constMap->mat = viewProjection->GetViewProjectionMatrix();	// 行列の合成
	constMap->matBillboard = matBillboard;
}

void ParticleManager::Draw()
{
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// デスクリプタヒープの配列
	SpriteCommon* spCommon = SpriteCommon::GetInstance();
	ID3D12DescriptorHeap* ppHeaps[] = { spCommon->GetDescriptorHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, spCommon->GetGpuHandle(textureIndex));
	// 描画コマンド
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void ParticleManager::Add(Vector3 position, int life, float start_scale, float end_scale)
{
	if (std::distance(particles.begin(), particles.end()) >= vertexCount) { return; }
	for (size_t i = 0; i < 10; i++)
	{
		particles.emplace_front();
		Particle& p = particles.front();
		const float md_pos = 3.0f;
		// -5.0f~+5.0f:xyz
		Vector3 pos =
		{
			(float)rand() / RAND_MAX * md_pos - md_pos / 2.0f,
			(float)rand() / RAND_MAX * md_pos - md_pos / 2.0f,
			(float)rand() / RAND_MAX * md_pos - md_pos / 2.0f
		};
		p.position = pos + position;
		// -0.05f~+0.05f:xyz
		const float md_vel = 0.1f;
		Vector3 vel =
		{
			(float)rand() / RAND_MAX * md_vel - md_vel / 2.0f,
			(float)rand() / RAND_MAX * md_vel - md_vel / 2.0f,
			(float)rand() / RAND_MAX * md_vel - md_vel / 2.0f
		};
		p.velocity = vel;
		// -0.001f~0.001:xy
		Vector3 acc{};
		const float md_acc = 0.001f;
		acc.x = (float)rand() / RAND_MAX * md_acc - md_acc / 2.0f,
			acc.y = (float)rand() / RAND_MAX * md_acc - md_acc / 2.0f,
			p.accel = acc;

		p.num_frame = life;
		p.scale = p.s_scale = start_scale;
		p.e_scale = end_scale;
	}
}