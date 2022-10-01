#include "Input.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(WindowsAPI& wAPI)
{
	HRESULT result;
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(wAPI.w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	result=keyboard->SetDataFormat(&c_dfDIKeyboard);// •W€Œ`Ž®
	assert(SUCCEEDED(result));
	result=keyboard->SetCooperativeLevel(wAPI.hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::GetDeviceState()
{
	keyboard->GetDeviceState(sizeof(key), key);
}
void Input::TransferOldkey()
{
	for (size_t i = 0; i < sizeof(oldkey); i++) { oldkey[i] = key[i]; }
}
bool Input::IsInput(const int KEY)
{
	if (key[KEY]) { return true; }
	return false;
}
bool Input::IsTrigger(const int KEY)
{
	return (!oldkey[KEY] && key[KEY]);
	return false;
}
float Input::Move(const int KEY1, const int KEY2, const float spd)
{
	return (IsInput(KEY1) - IsInput(KEY2)) * spd;
}