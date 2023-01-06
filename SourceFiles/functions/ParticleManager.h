#pragma once

#include <wrl.h>
#include <d3dx12.h>
#include <forward_list>
#include "WorldTransform.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
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
		int frame = 0; // 現在フレーム
		int	num_frame = 0; // 終了フレーム
		float scale = 0.10f, // スケール
			s_scale = 0.10f, // 初期値
			e_scale = 0.0f;	// 最終値
	};

	static const int vertexCount = 1024; // 頂点数

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// ビルボード行列
	static Matrix4 matBillboard;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff; 
	ConstBufferData* constMap = nullptr;
	// パーティクル配列
	std::forward_list<Particle> particles;
	static uint32_t textureIndex;
	// ビュープロジェクション
	static ViewProjection* viewProjection;

	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();

	// モデル作成
	static void CreateModel();

	// ビュー行列を更新
	void UpdateViewMatrix();

	ParticleManager() = default;
public: // メンバ関数
	static ParticleManager* GetInstance();
	ParticleManager(const ParticleManager& obj) = delete;

	// 静的初期化
	static void StaticInitialize();
	
	void Initialize();

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// パーティクル追加
	void Add(Vector3 position,int life, float start_scale, float end_scale);

	size_t GetParticleNum() { return std::distance(particles.begin(), particles.end()); }
};

