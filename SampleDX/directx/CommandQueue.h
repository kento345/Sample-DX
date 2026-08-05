#pragma once

#include"Device.h"

class CommandQueue final	
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommandQueue() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandQueue();

	/// <summary>
	/// コマンドキューの作成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const Device& device) noexcept;

	/// <summary>
	/// コマンドキューの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12CommandQueue* get() const noexcept;

private:
	ID3D12CommandQueue* commandQueue_{}; //コマンドキュー
};

