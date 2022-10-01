#pragma once
#include <Windows.h>
#include "struct.h"

class WindowsAPI
{
private:
	RECT wrc;
public:
	WNDCLASSEX w;
	HWND hwnd;

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	WindowsAPI(Int2 WIN_SIZE);
	void MyUnregisterClass();
};