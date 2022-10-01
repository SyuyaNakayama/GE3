#pragma once
#include <dinput.h>
#include <wrl.h>
#include "WindowsAPI.h"

using namespace Microsoft::WRL;

class Input
{
private:
	BYTE key[256]{};
	BYTE oldkey[256]{};
public:
	ComPtr<IDirectInputDevice8> keyboard;

	void Initialize(WindowsAPI& wAPI);
	void GetDeviceState();
	void TransferOldkey();
	bool IsInput(const int KEY);
	bool IsTrigger(const int KEY);
	float Move(const int KEY1, const int KEY2, const float spd);
};