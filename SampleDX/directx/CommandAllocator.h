#pragma once
#include"Device.h"

class CommandAllocator final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommandAllocator() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CommandAllocator();

	/// <summary>
	/// コマンドアロケータの作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="type"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const D3D12_COMMAND_LIST_TYPE type)noexcept;

	/// <summary>
	/// コマンドアロケータのリセット
	/// </summary>
	void reset()noexcept;

	/// <summary>
	/// コマンドアロケータの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12CommandAllocator* get()const noexcept;

	/// <summary>
	/// コマンドリストタイプの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] D3D12_COMMAND_LIST_TYPE getType()const noexcept;

private:
	ID3D12CommandAllocator* commandAllocator_{}; //コマンドアロケータ
	D3D12_COMMAND_LIST_TYPE type_{};			 //コマンドリストタイプ
};

