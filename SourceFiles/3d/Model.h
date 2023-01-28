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
	// �e�N�X�`��
	std::unique_ptr<Sprite> sprite;
	// ���f����
	string name;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer;
	// �}�e���A��
	Material material;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

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
	static std::unique_ptr<Model> Create(const string& modelName);
	Sprite* GetSprite() { return sprite.get(); }
	void SetSprite(Sprite* sprite_);
	void TextureUpdate();
	void TextureUpdate(Sprite* sprite);
	void Draw(const WorldTransform& worldTransform);
	void Draw(const WorldTransform& worldTransform, Sprite* sprite); // �e�N�X�`���̕ύX
};