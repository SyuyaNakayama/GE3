#include "WindowsAPI.h"
#pragma comment(lib,"winmm.lib")

// ウィンドウプロシージャ
LRESULT WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	case WM_PAINT:
		//MyPaint();
		break;
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsAPI* WindowsAPI::GetInstance()
{
	static WindowsAPI* wAPI = new WindowsAPI;
	return wAPI;
}

bool WindowsAPI::ProcessMessage()
{
	MSG msg{}; // メッセージ

	// メッセージがある?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	return msg.message == WM_QUIT;
}

void WindowsAPI::Initialize()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	RECT wrc = { 0, 0, (int)WIN_SIZE.x, (int)WIN_SIZE.y };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(w.lpszClassName, // クラス名
		L"LE2C_18_ナカヤマ_シュウヤ_GE3", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		CW_USEDEFAULT, // 表示Y座標(OSに任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション

	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

void WindowsAPI::Finalize()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

void WindowsAPI::MyPaint()
{
	HDC hdc;
	PAINTSTRUCT ps;

	const char* szStr = "Hello, World!!";
	hdc = BeginPaint(hwnd, &ps);

	SetBkColor(hdc, RGB(10, 10, 10));  //文字の背景色を設定
	SetTextColor(hdc, RGB(200, 200, 200));  //文字の色を設定
	TextOutA(hdc, 10, 10, szStr, (int)strlen(szStr));//文字を書く
	EndPaint(hwnd, &ps);
}