#pragma once
#include <string>
#include <wrl.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfapi.h>
#include <vector>
#include <dshow.h>
#include <memory>
#include <wrl.h>

class Audio final
{
private:
	Microsoft::WRL::ComPtr<IGraphBuilder> pGraphBuilder;
	Microsoft::WRL::ComPtr<IMediaControl> pMediaControl;
	Microsoft::WRL::ComPtr<IMediaPosition> pMediaPosition;
public:
	static void StaticInitialize();
	static std::unique_ptr<Audio> Create(const std::wstring& fileName);
	void Play() { pMediaControl->Run(); SetPlayPosition(0); }
	void Stop() { pMediaControl->Stop(); }
	void SetSpeed(double playSpd) { pMediaPosition->put_Rate(playSpd); }
	void SetPlayPosition(double playPosition) { pMediaPosition->put_CurrentPosition(playPosition); }
	static void Finalize();
};