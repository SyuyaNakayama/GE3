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
	// COM��������
	result = CoInitialize(NULL);
	assert(SUCCEEDED(result));

	// FilterGraph�𐶐�
	result = CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&pGraphBuilder);
	assert(SUCCEEDED(result));
	
	// MediaControl�C���^�[�t�F�[�X�擾
	result = pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID*)&pMediaControl);
	assert(SUCCEEDED(result));
	
	result = pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID*)&pMediaPosition);
	assert(SUCCEEDED(result));
}

void Audio::Load(const wstring& fileName)
{
	// Graph�𐶐�
	result = pMediaControl->RenderFile((wchar_t*)fileName.c_str());
	assert(SUCCEEDED(result));
}

void Audio::Play()
{
	pMediaControl->Run();
}

void Audio::Finalize()
{
	// ���������
	pMediaPosition->Release();
	pMediaControl->Release();
	pGraphBuilder->Release();

	// COM�I��
	CoUninitialize();
}