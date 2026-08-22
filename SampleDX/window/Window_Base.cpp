#include "Window_Base.h"
#include"Input.h"

/// <summary>
/// メッセージループ
/// </summary>
/// <returns></returns>
bool Window_Base::messageLoop() noexcept
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;  // WM_QUITメッセージが来たらループを抜ける
        }

        // メッセージ処理
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        //キー情報の取得
        static byte keyState[256]{};
        if (GetKeyboardState(keyState)) {
            Input::instance().updateKeyState(keyState);
        }
    }

    return true;
}

HWND Window_Base::handle() const noexcept
{
    return handle_;
}

std::pair<int, int> Window_Base::size() const noexcept
{
    return { width_, height_ };
}
