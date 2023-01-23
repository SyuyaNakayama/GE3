#pragma once
#include <wrl.h>
#include <d3dx12.h>
#include <list>
#include "WorldTransform.h"
#include "Timer.h"

struct AddParticleProp
{
	Vector3 position;
	float posRange = 1.0f;
	float velRange = 0.1f;
	float accRange = 0.001f;
	int lifeTime = 60;
	float start_scale = 1.0f;
	float end_scale = 0.0f;
	UINT16 addNum = 1;
};

class ParticleManager final
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale; // スケール
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;	// ３Ｄ変換行列
		Matrix4 matBillboard; // ビルボード行列
	};

	// パーティクル1粒
	struct Particle
	{
		Vector3 position{}; // 座標
		Vector3 velocity{}; // 速度
		Vector3 accel{}; // 加速度
		Timer frame = 0;
		float scale = 1.0f; // スケール
		float s_scale = 1.0f; // 初期値
		float e_scale = 0.0f;	// 最終値
	};

	static const int vertexCount = 1024; // 頂点数
	static const int particleMax = 64;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	static VertexPos* vertMap;
	// ビルボード行列
	static Matrix4 matBillboard;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	static ComPtr<ID3D12Resource> constBuff;
	static ConstBufferData* constMap;
	// パーティクル配列
	static std::list<Particle> particles;
	static uint32_t textureIndex;
	// ビュープロジェクション
	static ViewProjection* viewProjection;

	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

	// モデル作成
	static void CreateBuffers();

	// ビュー行列を更新
	static void UpdateViewMatrix();

	ParticleManager() = delete;
	ParticleManager(const ParticleManager& obj) = delete;
public: // メンバ関数

	// 静的初期化
	static void Initialize();

	// 毎フレーム処理
	static void Update();

	// 描画
	static void Draw();

	// パーティクル追加
	static void Add(const AddParticleProp& particleProp);
};

