#include "Window_Base.h"

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
    }

    return true;
}