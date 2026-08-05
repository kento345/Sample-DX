#pragma once
#include<Windows.h>
#include<string_view>

#include"Window_Base.h"

class ScaleWindow : public Window_Base
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ScaleWindow() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScaleWindow() = default;

	/// <summary>
	/// Window生成
	/// </summary>
	/// <param name="instance"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	[[nodiscard]]  HRESULT create(HINSTANCE instance, int width,int height,std::wstring_view name)noexcept;

	/// <summary>
	/// メッセージループ
	/// </summary>
	/// <returns></returns>
	[[nodiscard]]  bool messageLoop()noexcept override;

	/// <summary>
	/// Windowハンドルを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]]  HWND handle()const noexcept override;

	/// <summary>
	/// サイズの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]]  std::pair<int, int> size()const noexcept override;
};

