#include "Audio.h"
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <cassert>
#include <thread>
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
using namespace std;

Audio* Audio::GetInatance()
{
	static Audio instance;
	return &instance;
}

void Audio::Initialize()
{
	// XAudio2エンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイスを生成
	IXAudio2MasteringVoice* masterVoice = nullptr;
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

	// Media Foundationの初期化
	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	result = CoInitialize(NULL);
	assert(SUCCEEDED(result));

	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&pGraphBuilder);
	
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID*)&pMediaControl);
	
	pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID*)&pMediaPosition);
}

void Audio::LoadWave(const std::string& fileName)
{
	//ifstream file;
	//file.open(fileName, ios_base::binary);
	//assert(file.is_open());

	// ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, nullptr, 0);
	// ワイド文字列に変換
	vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilePath.data(), filePathBufferSize);
	//if (fileName.find(".wav") != string::npos)
	//{
	//	RiffHeader riff;
	//	file.read((char*)&riff, sizeof(riff));
	//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) { assert(0); }
	//	if (strncmp(riff.type, "WAVE", 4) != 0) { assert(0); }

	//	FormatChunk format{};
	//	file.read((char*)&format, sizeof(ChunkHeader));
	//	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }

	//	assert(format.chunk.size <= sizeof(format.fmt));
	//	file.read((char*)&format.fmt, format.chunk.size);

	//	ChunkHeader data;
	//	file.read((char*)&data, sizeof(data));
	//	if (strncmp(data.id, "JUNK", 4) == 0)
	//	{
	//		file.seekg(data.size, ios_base::cur);
	//		file.read((char*)&data, sizeof(data));
	//	}

	//	if (strncmp(data.id, "data", 4) != 0) { assert(0); }
	//	char* pBuffer = new char[data.size];
	//	file.read(pBuffer, data.size);

	//	soundData.wfex = format.fmt;
	//	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	//	soundData.bufferSize = data.size;
	//}
	//else if (fileName.find(".mp3") != string::npos)
	//{


	//	IMFSourceReader* pMFSourceReader{ nullptr };
	//	MFCreateSourceReaderFromURL(wfilePath.data(), NULL, &pMFSourceReader);

	//	MFCreateMediaType(&pMFMediaType);
	//	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	//	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	//	pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

	//	pMFMediaType->Release();
	//	pMFMediaType = nullptr;
	//	pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

	//	WAVEFORMATEX* waveFormat{ nullptr };
	//	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);

	//	while (true)
	//	{
	//		IMFSample* pMFSample{ nullptr };
	//		DWORD dwStreamFlags{ 0 };
	//		pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

	//		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
	//		{
	//			break;
	//		}

	//		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
	//		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

	//		BYTE* pBuffer{ nullptr };
	//		DWORD cbCurrentLength{ 0 };
	//		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

	//		mediaData.resize(mediaData.size() + cbCurrentLength);
	//		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

	//		pMFMediaBuffer->Unlock();

	//		pMFMediaBuffer->Release();
	//		pMFSample->Release();
	//	}
	//	soundData.wfex = *waveFormat;
	//	soundData.pBuffer = mediaData.data();
	//	soundData.bufferSize = static_cast<UINT32>(mediaData.size());
	//	mediaData.clear();
	//}

	//file.close();

	pMediaControl->RenderFile(wfilePath.data());
}

void Audio::PlayWave()
{
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	//result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	//assert(SUCCEEDED(result));

	//XAUDIO2_BUFFER buf{};
	//buf.pAudioData = soundData.pBuffer;
	//buf.AudioBytes = soundData.bufferSize;
	//buf.Flags = XAUDIO2_END_OF_STREAM;

	//result = pSourceVoice->SubmitSourceBuffer(&buf);
	//assert(SUCCEEDED(result));
	////result = pSourceVoice->Start();
	//assert(SUCCEEDED(result));

	pMediaPosition->put_Rate(1.0);

	pMediaControl->Run();
}

void Audio::SoundUnload()
{
	// バッファのメモリを解放
	//delete[] soundData.pBuffer;
	soundData.pBuffer = nullptr;
	soundData.bufferSize = 0;
	soundData.wfex = {};
}

void Audio::Finalize()
{
	xAudio2.Reset();
	SoundUnload();
}