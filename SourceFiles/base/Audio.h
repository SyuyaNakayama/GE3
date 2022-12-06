#pragma once
#include <xaudio2.h>
#include <string>
#include <wrl.h>
#include <array>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfapi.h>
#include <vector>
#include <dshow.h>

class Audio final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4]{}; // �`�����N����ID
		int size = 0; // �`�����N�T�C�Y
	};

	// RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk; // "RIFF"
		char type[4]{}; // "WAVE"
	};

	// FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk; // "fmt "
		WAVEFORMATEX fmt; // �g�`�t�H�[�}�b�g
	};

	// �����f�[�^
	struct SoundData
	{
		WAVEFORMATEX wfex; // �g�`�t�H�[�}�b�g
		BYTE* pBuffer; // �o�b�t�@�̐擪�A�h���X
		UINT bufferSize; // �o�b�t�@�̃T�C�Y
	};

	HRESULT result = S_OK;
	ComPtr<IXAudio2> xAudio2;
	SoundData soundData{};
	IMFMediaType* pMFMediaType{ nullptr };
	std::vector<BYTE> mediaData;
	IGraphBuilder* pGraphBuilder = nullptr;
	IMediaControl* pMediaControl = nullptr;
	IMediaPosition* pMediaPosition = nullptr;

	Audio() = default;
public:
	static Audio* GetInatance();
	Audio(const Audio& obj) = delete;

	void Initialize();
	void LoadWave(const std::string& fileName);
	void PlayWave();
	void SoundUnload();
	void Finalize();
};