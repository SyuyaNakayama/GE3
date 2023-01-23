#include "ParticleManager.h"
#include "Functions.h"
#include "SpriteCommon.h"
#include "PipelineManager.h"
#include <random>
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;
ComPtr<ID3D12Resource> ParticleManager::vertBuff;
ParticleManager::VertexPos* ParticleManager::vertMap = nullptr;
ComPtr<ID3D12Resource> ParticleManager::constBuff;
ParticleManager::ConstBufferData* ParticleManager::constMap = nullptr;
Matrix4 ParticleManager::matBillboard;
D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView{};
ViewProjection* ParticleManager::viewProjection = nullptr;
uint32_t ParticleManager::textureIndex = 0;
std::list<ParticleManager::Particle> ParticleManager::particles;

void ParticleManager::Initialize()
{
	ParticleManager::viewProjection = WorldTransform::GetViewProjection();
	assert(viewProjection);
	// パイプライン初期化
	InitializeGraphicsPipeline();
	// テクスチャ読み込み
	textureIndex = SpriteCommon::GetInstance()->LoadTexture("Particle.png", 1);
	// モデル生成
	CreateBuffers();
}

void ParticleManager::InitializeGraphicsPipeline()
{
	PipelineManager pipelineManager;
	pipelineManager.LoadShaders(L"ParticleVS", L"ParticlePS", L"ParticleGS");
	pipelineManager.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineManager.InitDepthStencilState();
	pipelineManager.SetBlendDesc(D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ONE); // 光パーティクル
	//pipelineManager.SetBlendDesc(D3D12_BLEND_OP_REV_SUBTRACT, D3D12_BLEND_ONE, D3D12_BLEND_ONE); // 闇パーティクル
	pipelineManager.InitDSVFormat();
	pipelineManager.SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK_ZERO);
	pipelineManager.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::CBV);
	pipelineManager.AddRootParameter(PipelineManager::RootParamType::DescriptorTable);
	pipelineManager.CreatePipeline(pipelinestate, rootsignature);
}

void ParticleManager::CreateBuffers()
{
	BufferMapping(&vertBuff, &vertMap, vertexCount);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = vertexCount;
	vbView.StrideInBytes = sizeof(VertexPos);

	BufferMapping(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	if (!particles.empty()) { particles.clear(); }
}

void ParticleManager::UpdateViewMatrix()
{
	Vector3 cameraAxisZ = viewProjection->target - viewProjection->eye;
	// 0ベクトルの時
	assert(!(cameraAxisZ == Vector3(0, 0, 0)));
	assert(!(viewProjection->up == Vector3(0, 0, 0)));

	cameraAxisZ.Normalize();

	Vector3 cameraAxisX = Normalize(Cross(viewProjection->up, cameraAxisZ));
	Vector3 cameraAxisY = Normalize(Cross(cameraAxisZ, cameraAxisX));
	matBillboard = Matrix4::CreateFromVector(cameraAxisX, cameraAxisY, cameraAxisZ);
}

void ParticleManager::Update()
{
	particles.remove_if([](Particle& particle) { return particle.frame.CountDown(); });

	int i = 0;
	for (Particle& particle : particles)
	{
		particle.velocity += particle.accel;
		particle.position += particle.velocity;
		float f = 1.0f / particle.frame.GetRemainTimeRate();
		particle.scale = particle.s_scale + (particle.e_scale - particle.s_scale) / f;
		// 定数バッファへデータ転送
		vertMap[i].pos = particle.position;
		vertMap[i++].scale = particle.scale;
	}

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
	cmdList->DrawInstanced((UINT)particles.size(), 1, 0, 0);
}

void ParticleManager::Add(const AddParticleProp& particleProp)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<float>
		randPos(-particleProp.posRange, particleProp.posRange), 
		randVel(-particleProp.velRange, particleProp.velRange),
		randAcc(-particleProp.accRange, particleProp.accRange);

	for (UINT16 i = 0; i < particleProp.addNum; i++)
	{
		if (particles.size() >= particleMax) { return; }
		particles.emplace_front();
		Particle& p = particles.front();
		p.position = Vector3(randPos(mt), randPos(mt), randPos(mt)) + particleProp.position;
		p.velocity = Vector3(randVel(mt), randVel(mt), randVel(mt));
		p.accel = Vector3(randAcc(mt), randAcc(mt), 0);
		p.frame = particleProp.lifeTime;
		p.scale = p.s_scale = particleProp.start_scale;
		p.e_scale = particleProp.end_scale;
	}
}