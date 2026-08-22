#pragma once

#include<Windows.h>
#include<cstdint>

class Input final
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

	[[nodiscard]] bool getKey(uint16_t sKey)const noexcept;

	[[nodiscard]] bool isTriggerKey(uint16_t sKey)const noexcept;

	void updateKeyState(void* pState)noexcept;

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
	byte keyState_[256];
};

