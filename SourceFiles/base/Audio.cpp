#include "Audio.h"
#include <fstream>
#include <cassert>
using namespace std;

void Audio::StaticInitialize()
{
	// COMを初期化
	HRESULT result = CoInitialize(NULL);
	assert(SUCCEEDED(result));
}

std::unique_ptr<Audio> Audio::Create(const wstring& fileName)
{
	std::unique_ptr<Audio> newAudio = std::make_unique<Audio>();

	// FilterGraphを生成
	HRESULT result = CoCreateInstance(CLSID_FilterGraph,
		NULL, CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&newAudio->pGraphBuilder);
	assert(SUCCEEDED(result));

	// MediaControlインターフェース取得
	result = newAudio->pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID*)&newAudio->pMediaControl);
	assert(SUCCEEDED(result));

	result = newAudio->pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID*)&newAudio->pMediaPosition);
	assert(SUCCEEDED(result));

	wstring fullpath = L"Resources/audios/" + fileName;
	// Graphを生成
	result = newAudio->pMediaControl->RenderFile((BSTR)fullpath.c_str());
	assert(SUCCEEDED(result));

	return newAudio;
}

void Audio::Finalize()
{
	CoUninitialize(); // COM終了
}