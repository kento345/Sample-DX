#pragma once

#include"Device.h"
#include"SwapChain.h"
#include"DescriptorHeap.h"
#include<vector>

class RenderTarget
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RenderTarget() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RenderTarget();

	/// <summary>
	/// バックバッファを生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="swapChain"></param>
	/// <param name="heap"></param>
	/// <returns></returns>
	[[nodiscard]] bool createBackBuffer(const Device& device, const SwapChain& swapChain, const DescriptorHeap& heap)noexcept;

	/// <summary>
	/// ビューを取得
	/// </summary>
	/// <param name="device"></param>
	/// <param name="heap"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const Device& device, const DescriptorHeap& heap, UINT index)const noexcept;

	/// <summary>
	/// レンダーターゲットを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	[[nodiscard]] ID3D12Resource* get(uint32_t index)const noexcept;

private:
	std::vector<ID3D12Resource*> rendertargets_; //レンダーターゲットリソースの配列
};

