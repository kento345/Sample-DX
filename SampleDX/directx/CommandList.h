#pragma once
#include"Device.h"
#include"CommandAllocator.h"

class CommandList final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommandList() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandList();

	/// <summary>
	/// コマンドアロケータの作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="commandAllocator"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const Device& device, const CommandAllocator& commandAllocator)noexcept;

	/// <summary>
	/// コマンドアロケータのリセット
	/// </summary>
	/// <param name="commandAllocator"></param>
	void reset(const CommandAllocator& commandAllocator)noexcept;

	/// <summary>
	/// コマンドアロケータの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12GraphicsCommandList* get()const noexcept;

private:
	ID3D12GraphicsCommandList* commandList_{};  //コマンドアロケータ
};

