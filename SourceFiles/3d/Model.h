#pragma once
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include "Sprite.h"
#include "WorldTransform.h"

class Model
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

	// 頂点データ構造体
	struct VertexData
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
		Sprite::Color color; // 色
	};

	// マテリアル
	struct Material
	{
		string name;
		Vector3 ambient = { 0.3f,0.3f,0.3f };
		Vector3 diffuse;
		Vector3 specular;
		float alpha = 1.0f;
		string textureFilename;
	};

	struct ConstBufferData
	{
		Vector3 ambient;
		float pad1;
		Vector3 diffuse;
		float pad2;
		Vector3 specular;
		float alpha;
	};
private:
	VertexData* vertMap = nullptr;
	Sprite* sprite = nullptr;
	string name;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// 頂点データ配列
	vector<VertexData> vertices;
	// 頂点インデックス配列
	vector<unsigned short> indices;
	// マテリアル
	Material material;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	void LoadFromOBJInternal(const string& modelName);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME);

	void CreateBuffers();

	static vector<Model*> models;
public:
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw() {}

	/// <summary>
	/// モデル作成
	/// </summary>
	static Model* LoadFromOBJ(const string& modelName);

	void Draw(const WorldTransform& worldTransform);
};