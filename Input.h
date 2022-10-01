#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>
#include <string>
#include "WindowsAPI.h"

class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	std::string key, oldkey;
	ComPtr<IDirectInputDevice8> keyboard;
public:
	void Initialize(WindowsAPI& wAPI);
	void Update();
	bool IsInput(const int KEY);
	bool IsTrigger(const int KEY);
	float Move(const int KEY1, const int KEY2, const float spd);
};