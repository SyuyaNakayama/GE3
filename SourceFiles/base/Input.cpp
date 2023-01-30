#include "Input.h"
#include "WindowsAPI.h"
#include "Functions.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input input;
	return &input;
}

void Input::Initialize()
{
	Result result;
	WindowsAPI* wAPI = WindowsAPI::GetInstance();

	result = DirectInput8Create(wAPI->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	// キーボード
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	result = keyboard->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	// マウス
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	result = mouse->SetDataFormat(&c_dfDIMouse2);
	result = mouse->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	keyboard->Acquire();
	oldkey = key;
	DIK_0;
	keyboard->GetDeviceState((DWORD)key.size(), (LPVOID)key.data());

	mouse->Acquire();
	mouseStatePre = mouseState;
	mouse->GetDeviceState(sizeof(mouseState), &mouseState);
}

bool Input::IsTriggerMouse(Mouse KEY)
{
	return !mouseStatePre.rgbButtons[(int)KEY] && mouseState.rgbButtons[(int)KEY]; 
}

Input::MouseMove Input::GetMouseMove() 
{
	MouseMove tmp{};
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}

float Input::Move(const int KEY1, const int KEY2, const float spd) { return (IsInput(KEY1) - IsInput(KEY2)) * spd; }