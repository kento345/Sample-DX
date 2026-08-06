#include "ScaleWindow.h"

namespace
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) {
				DestroyWindow(hwnd);
				return 0;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

HRESULT ScaleWindow::create(HINSTANCE instance, int width, int height, std::wstring_view name) noexcept
{
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = name.data();
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//ウィンドウの登録
	RegisterClass(&wc);

	//ウィンドウの作成
	handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, instance, nullptr);
	if (!handle_)
	{
		return E_FAIL;
	}

	//ウィンドウの表示
	ShowWindow(handle_, SW_SHOW);

	//ウィンドウを更新
	UpdateWindow(handle_);

	//ウィンドウのサイズを保存
	width_ = width;
	height_ = height;

	return S_OK;
}

bool ScaleWindow::messageLoop() noexcept
{
	return Window_Base::messageLoop();
}

HWND ScaleWindow::handle() const noexcept
{
	return Window_Base::handle();
}

std::pair<int, int> ScaleWindow::size() const noexcept
{
	return Window_Base::size();
}

