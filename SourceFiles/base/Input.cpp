#include "Input.h"
#include "WindowsAPI.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input* input = new Input;
	return input;
}

void Input::Initialize()
{
	HRESULT result;
	WindowsAPI* wAPI = WindowsAPI::GetInstance();

	result = DirectInput8Create(wAPI->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	
	result = keyboard->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	key.resize(256);
	oldkey.resize(256);
}

void Input::Update()
{
	oldkey = key;
	keyboard->Acquire();
	keyboard->GetDeviceState((DWORD)key.size(), (LPVOID)key.c_str());
}

bool Input::IsInput(const int KEY) { return key[KEY]; }

bool Input::IsTrigger(const int KEY) { return (!oldkey[KEY] && key[KEY]); }

float Input::Move(const int KEY1, const int KEY2, const float spd) { return (IsInput(KEY1) - IsInput(KEY2)) * spd; }