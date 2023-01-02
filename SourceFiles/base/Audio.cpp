#include "Audio.h"
#include <fstream>
#include <cassert>
using namespace std;

Audio* Audio::GetInatance()
{
	static Audio instance;
	return &instance;
}

void Audio::Initialize()
{
	// COMを初期化
	HRESULT result = CoInitialize(NULL);
	assert(SUCCEEDED(result));

	// FilterGraphを生成
	result = CoCreateInstance(CLSID_FilterGraph,
		NULL, CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&pGraphBuilder);
	assert(SUCCEEDED(result));

	// MediaControlインターフェース取得
	result = pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID*)&pMediaControl);
	assert(SUCCEEDED(result));

	result = pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID*)&pMediaPosition);
	assert(SUCCEEDED(result));
}

void Audio::Load(const wstring& fileName)
{
	wstring fullpath = L"Resources/audios/" + fileName;
	// Graphを生成
	HRESULT result = pMediaControl->RenderFile((BSTR)fullpath.c_str());
	assert(SUCCEEDED(result));
}

void Audio::Play()
{
	pMediaControl->Run();
}

void Audio::Finalize()
{
	// 資源を解放
	pMediaPosition->Release();
	pMediaControl->Release();
	pGraphBuilder->Release();

	// COM終了
	CoUninitialize();
}