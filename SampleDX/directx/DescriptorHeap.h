#pragma once

#include"Device.h"


class DescriptorHeap final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DescriptorHeap() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DescriptorHeap();

	/// <summary>
	/// ディスクリプタヒープを生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="type"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaferVisible"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaferVisible = false)noexcept;

	/// <summary>
	/// ディスクリプタヒープを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12DescriptorHeap* get()const noexcept;

	/// <summary>
	/// ディスクリプタヒープのタイプを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE getType()const noexcept;

private:
	ID3D12DescriptorHeap* heap_{};	   //ディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_TYPE type_{}; //ヒープのタイプ
};

