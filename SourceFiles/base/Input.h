#pragma once
#include <dinput.h>
#include <wrl.h>
#include <string>

class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<IDirectInput8> directInput;
	std::string key, oldkey;
	ComPtr<IDirectInputDevice8> keyboard;
	Input() = default;
	~Input() = default;
public:
	static Input* GetInstance();
	void Initialize();
	void Update();
	bool IsInput(const int KEY);
	bool IsTrigger(const int KEY);
	float Move(const int KEY1, const int KEY2, const float spd);
};