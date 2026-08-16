#pragma once

#include"Device.h"
#include"DescriptorHeap.h"


class ConstantBuffer final
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ConstantBuffer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ConstantBuffer();

	/// <summary>
	/// コンスタントバッファの作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="heap"></param>
	/// <param name="bufferSize"></param>
	/// <param name="descriptorIndex"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const Device& device, const DescriptorHeap& heap, UINT bufferSize, UINT descriptorIndex) noexcept;

	/// <summary>
	/// コンスタントバッファの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12Resource* constantBuffer() const noexcept;

	/// <summary>
	/// GPU用ディスクリプタハンドルを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle() const noexcept;

private:
	ID3D12Resource* constantBuffer_{};  /// コンスタントバッファ
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_{};       /// GPU 用ディスクリプタハンドル
};

