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

	// チャンクヘッダ
	struct ChunkHeader
	{
		char id[4]{}; // チャンク毎のID
		int size = 0; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk; // "RIFF"
		char type[4]{}; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk; // "fmt "
		WAVEFORMATEX fmt; // 波形フォーマット
	};

	// 音声データ
	struct SoundData
	{
		WAVEFORMATEX wfex; // 波形フォーマット
		BYTE* pBuffer; // バッファの先頭アドレス
		UINT bufferSize; // バッファのサイズ
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