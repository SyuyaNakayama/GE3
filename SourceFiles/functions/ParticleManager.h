#pragma once

#include <wrl.h>
#include <d3dx12.h>
#include <forward_list>
#include "ViewProjection.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleManager final
{
private: // エイリアス
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

private: // 定数

	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = 1624;		// 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize(ViewProjection* viewProjection);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビルボード行列
	static Matrix4 matBillboard;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;

private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static void LoadTexture();

	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	void UpdateViewMatrix();

	ParticleManager() = default;
public: // メンバ関数
	static ParticleManager* GetInstance();
	ParticleManager(const ParticleManager& obj) = delete;

	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// パーティクル追加
	void Add(Vector3 position,int life, float start_scale, float end_scale);

	size_t GetParticleNum() { return std::distance(particles.begin(), particles.end()); }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	ConstBufferData* constMap = nullptr;
	// パーティクル配列
	std::forward_list<Particle> particles;
	// ビュープロジェクション
	static ViewProjection* viewProjection;
};

