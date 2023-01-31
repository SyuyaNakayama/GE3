#pragma once
#include "WorldTransform.h"
#include "Mesh.h"

class Model
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

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
	// テクスチャ
	std::unique_ptr<Sprite> sprite;
	// モデル名
	string name;
	// メッシュ
	Mesh mesh;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer;
	// マテリアル
	Material material;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	void LoadFromOBJInternal(const string& modelName, bool smoothing);

	// マテリアル読み込み
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME);

	void CreateBuffers();

	static vector<Model*> models;
public:
	// グラフィックパイプライン生成
	static void InitializeGraphicsPipeline();
	// 描画前処理
	static void PreDraw();
	// 描画後処理
	static void PostDraw() {}
	// モデル作成
	static std::unique_ptr<Model> Create(const string& modelName, bool smoothing = false);
	Sprite* GetSprite() { return sprite.get(); }
	void SetSprite(Sprite* sprite_) { sprite.reset(sprite_); }
	void TextureUpdate() { TextureUpdate(sprite.get()); }
	void TextureUpdate(Sprite* sprite);
	void Draw(const WorldTransform& worldTransform){ Draw(worldTransform, sprite.get()); }
	void Draw(const WorldTransform& worldTransform, Sprite* sprite); // テクスチャの変更
};