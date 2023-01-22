#include "ParticleManager.h"
#include "Functions.h"
#include "SpriteCommon.h"
#include "PipelineManager.h"
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
std::forward_list<ParticleManager::Particle> ParticleManager::particles;

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

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Update()
{
	particles.remove_if([](Particle& x) { return x.frame >= x.num_frame; });

	for (Particle& particle: particles)
	{
		particle.frame++;
		particle.velocity += particle.accel;
		particle.position += particle.velocity;
		float f = (float)particle.num_frame / particle.frame;
		particle.scale = (particle.e_scale - particle.s_scale) / f;
		particle.scale += particle.s_scale;
	}

	// 定数バッファへデータ転送
	int i = 0;
	for (const Particle& particle : particles)
	{
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
	cmdList->DrawInstanced((UINT)GetParticleNum(), 1, 0, 0);
}

void ParticleManager::Add(Vector3 position, int life, float start_scale, float end_scale)
{
	for (size_t i = 0; i < 1; i++)
	{
		if (GetParticleNum() >= particleMax) { return; }
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
		const float md_vel = 0.2f;
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
		acc.x = (float)rand() / RAND_MAX * md_acc - md_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * md_acc - md_acc / 2.0f;
		p.accel = acc;

		p.num_frame = life;
		p.scale = p.s_scale = start_scale;
		p.e_scale = end_scale;
	}
}