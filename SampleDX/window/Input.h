#pragma once

#include"../MoveProhibited/Nomove.h"

#include<Windows.h>
#include<cstdint>
#include<array>

class Input final : public Nomove
{
public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static Input& instance()noexcept {
		static Input instance;
		return instance;
	}

	/// <summary>
	/// 入力されていればtrue
	/// </summary>
	/// <param name="sKey"></param>
	/// <returns></returns>
	[[nodiscard]] bool getKey(uint16_t sKey)const noexcept;

	/// <summary>
	/// 入力したらtrue
	/// </summary>
	/// <param name="sKey"></param>
	/// <returns></returns>
	[[nodiscard]] bool getTrigger(uint16_t sKey)const noexcept;

	/// <summary>
	/// キー情報の更新
	/// </summary>
	/// <param name="pState"></param>
	void updateKeyState(void* pState)noexcept;

	void updatePrevKeyState() noexcept;

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Input() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Input() = default;

private:
	std::array<byte, 256> keyState_{};			/// ハードから設定されたキー情報配列
	std::array<byte, 256> prevKeyState_{};		/// ハードから設定されたキー情報配列
};

