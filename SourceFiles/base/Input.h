#pragma once
#include <dinput.h>
#include <wrl.h>
#include <array>

enum class Key
{
	_1,
};

enum class Mouse
{
	Left, Right, Middle,
	B_0, B_1, B_2, B_3, B_4, B_5 // Bは"Buttion"の略
};

class Input final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Input() = default;
	ComPtr<IDirectInput8> directInput;
	ComPtr<IDirectInputDevice8> keyboard;
	std::array<BYTE, 256> key, oldkey;
	ComPtr<IDirectInputDevice8> mouse;
	DIMOUSESTATE2 mouseState{}, mouseStatePre{};

public:
	struct MouseMove
	{
		LONG lX;
		LONG lY;
		LONG lZ;
	};

	static Input* GetInstance();
	Input(const Input& obj) = delete;
	void Initialize();
	void Update();
	bool IsInput(const int KEY) { return key[KEY]; }
	bool IsTrigger(const int KEY) { return !oldkey[KEY] && key[KEY]; }
	bool IsInputMouse(Mouse KEY) { return mouseState.rgbButtons[(int)KEY]; }
	bool IsTriggerMouse(Mouse KEY);
	MouseMove GetMouseMove();
	// KEY1が押されてたらプラス、KEY2が押されてたらマイナス
	float Move(const int KEY1, const int KEY2, const float spd);
};