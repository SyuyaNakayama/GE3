#pragma once
#include <Windows.h>
#include <vector>
#include "Sprite.h"
#include "WorldTransform.h"

class Model
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

	// ���_�f�[�^�\����
	struct VertexData
	{
		Vector3 pos; // xyz���W
		Vector3 normal; // �@���x�N�g��
		Vector2 uv;  // uv���W
		Color color; // �F
	};

	// �}�e���A��
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
	// �e�N�X�`��
	std::unique_ptr<Sprite> sprite;
	// ���f����
	string name;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// ���_�f�[�^�z��
	vector<VertexData> vertices;
	// ���_�C���f�b�N�X�z��
	vector<uint16_t> indices;
	// �}�e���A��
	Material material;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �X�v���C�g�ύX�t���O(�X�v���C�g�ύX�𓾂�)
	bool isSpriteChange = false;

	void LoadFromOBJInternal(const string& modelName);

	// �}�e���A���ǂݍ���
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME);

	void CreateBuffers();

	static vector<Model*> models;
public:
	// �O���t�B�b�N�p�C�v���C������
	static void InitializeGraphicsPipeline();
	// �`��O����
	static void PreDraw();
	// �`��㏈��
	static void PostDraw() {}
	// ���f���쐬
	static Model* LoadFromOBJ(const string& modelName);
	Sprite* GetSprite() { return sprite.get(); }
	void SetSprite(Sprite* sprite_);
	void TextureUpdate();
	void Draw(const WorldTransform& worldTransform);
	void Draw(const WorldTransform& worldTransform, Sprite* sprite); // �e�N�X�`���̕ύX
};