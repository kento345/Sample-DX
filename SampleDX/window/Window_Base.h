#pragma once
#include <Windows.h>
#include <string>

#include "../MoveProhibited/Nomove.h"

class Window_Base : public Nomove 
{
public:
	/// <summary>
    /// コンストラクタ
    /// </summary>
	Window_Base() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Window_Base() = default;

public:
	/// <summary>
	/// メッセージループ
	/// </summary>
	/// <returns></returns>
	virtual bool messageLoop() noexcept;
	/// <summary>
	/// ハンドル取得
	/// </summary>
	/// <returns></returns>
	virtual HWND handle() const noexcept;
	/// <summary>
	/// サイズ取得
	/// </summary>
	/// <returns></returns>
	virtual std::pair<int, int> size() const noexcept;


protected:
	HWND handle_{}; // ウィンドウハンドル
	int  width_{};  //横幅
	int  height_{};	//縦幅
};