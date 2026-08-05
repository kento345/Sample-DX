#include "FullWindow.h"

namespace
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		}
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

HRESULT FullWindow::create(HINSTANCE instance, std::wstring_view name) noexcept
{
	MONITORINFO mi{};
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(MonitorFromPoint({ 0,0 }, MONITOR_DEFAULTTOPRIMARY), &mi);

	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = name.data();
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//ウィンドウの登録
	RegisterClass(&wc);

	//フルスクリーンのサイズを取得
	width_ = mi.rcMonitor.right - mi.rcMonitor.left;
	height_ = mi.rcMonitor.bottom - mi.rcMonitor.top;

	//ウィンドウの作成
	handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_POPUP, mi.rcMonitor.left, mi.rcMonitor.top, width_, height_, nullptr, nullptr, instance, nullptr);
	if (!handle_)
	{
		return E_FAIL;
	}

	//ウィンドウの表示
	ShowWindow(handle_, SW_SHOW);

	//ウィンドウを更新
	UpdateWindow(handle_);


	return S_OK;
}

bool FullWindow::messageLoop() noexcept {
	return Window_Base::messageLoop();
}

HWND FullWindow::handle() const noexcept{
	return handle_;
}

std::pair<int, int> FullWindow::size() const noexcept{
	return {width_, height_};
}