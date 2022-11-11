#pragma once
#include <Windows.h>

class WindowsAPI
{
private:
	HWND hwnd;
	WNDCLASSEX w{};
public:
	// ウィンドウサイズ
	static const int WIN_WIDTH = 1280;
	static const int WIN_HEIGHT = 720;

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }
	bool ProcessMessage();
	void Initialize();
	void Finalize();
};