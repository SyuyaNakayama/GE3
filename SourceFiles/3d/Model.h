#pragma once
#include <Windows.h>
#include <wrl.h>
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
		Sprite::Color color; // �F
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
	Sprite* sprite = nullptr;
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
	vector<unsigned short> indices;
	// �}�e���A��
	Material material;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

	void LoadFromOBJInternal(const string& modelName);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const string& DIRECTORY_PATH, const string& FILENAME);

	void CreateBuffers();

	static vector<Model*> models;
public:
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw() {}

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static Model* LoadFromOBJ(const string& modelName);

	void Draw(const WorldTransform& worldTransform);
};