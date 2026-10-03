#pragma once
#include <Windows.h>
#include <string>

#include "../MoveProhibited/Nomove.h"

class Window_Base : public Nomove 
{
public:

	static Window_Base& instance() noexcept {
		static Window_Base instance;
		return instance;
	}

	/// <summary>
	/// メッセージループ
	/// </summary>
	/// <returns></returns>
	virtual bool messageLoop() noexcept;
	/// <summary>
	/// ハンドル取得
	/// </summary>
	/// <returns></returns>
	virtual HWND handle() const noexcept = 0;
	/// <summary>
	/// サイズ取得
	/// </summary>
	/// <returns></returns>
	virtual std::pair<int, int> size() const noexcept = 0;

private:
	/// <summary>
    /// コンストラクタ
    /// </summary>
	Window_Base() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Window_Base() = default;

protected:
	HWND handle_{}; // ウィンドウハンドル
	int  width_{};  //横幅
	int  height_{};	//縦幅
};