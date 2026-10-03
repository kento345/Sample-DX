#pragma once

#include"Device.h"

class Fence final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Fence() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fence();
	/// <summary>
	/// フェンスの作成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] bool create() noexcept;

	/// <summary>
	/// 同期待ちを行う
	/// </summary>
	/// <param name="fenceValue"></param>
	void wait(UINT64 fenceValue) const noexcept; 

	/// <summary>
	/// フェンスの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12Fence* get() const noexcept;

private:
	ID3D12Fence* fence_{}; //フェンス
    HANDLE waitGpuEvent_{};  //GPUとCPU　同期用イベントハンドル
};

