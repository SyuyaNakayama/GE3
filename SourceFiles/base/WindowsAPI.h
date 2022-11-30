#pragma once
#include <Windows.h>
#include "Vector2.h"

class WindowsAPI
{
private:
	HWND hwnd;
	WNDCLASSEX w{};
	WindowsAPI() = default;
	~WindowsAPI() = default;
public:
	// ウィンドウサイズ
	const Vector2 WIN_SIZE = { 1280.0f,720.0f };

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static WindowsAPI* GetInstance();
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }
	bool ProcessMessage();
	void Initialize();
	void Finalize();
};