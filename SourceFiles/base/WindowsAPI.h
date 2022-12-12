#pragma once
#include <Windows.h>
#include "Vector.h"

class WindowsAPI final
{
private:
	HWND hwnd;
	WNDCLASSEX w{};
	WindowsAPI() = default;
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	// ウィンドウサイズ
	static const Vector2 WIN_SIZE;

	static WindowsAPI* GetInstance();
	WindowsAPI(const WindowsAPI& obj) = delete;
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }
	bool ProcessMessage();
	void Initialize();
	void Finalize();
};