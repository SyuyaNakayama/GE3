#pragma once
#include <dinput.h>
#include <wrl.h>
#include <string>

class Input final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	ComPtr<IDirectInput8> directInput;
	std::string key, oldkey;
	ComPtr<IDirectInputDevice8> keyboard;
	Input() = default;
public:
	static Input* GetInstance();
	Input(const Input& obj) = delete;
	void Initialize();
	void Update();
	bool IsInput(const int KEY);
	bool IsTrigger(const int KEY);
	float Move(const int KEY1, const int KEY2, const float spd);
};