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
	struct Data
	{
		std::wstring fileName;
		IMediaControl* pMediaControl = nullptr;
		IMediaPosition* pMediaPosition = nullptr;
	};

	IGraphBuilder* pGraphBuilder = nullptr;
	IMediaControl* pMediaControl = nullptr;
	IMediaPosition* pMediaPosition = nullptr;
	Audio() = default;
public:
	static Audio* GetInatance();
	Audio(const Audio& obj) = delete;

	void Initialize();
	void Load(const std::wstring& fileName);
	void Play();
	void SetSpeed(double playSpd) { pMediaPosition->put_Rate(playSpd); }
	void SetPlayPosition(double playPosition) { pMediaPosition->put_CurrentPosition(playPosition); }
	void Finalize();
};