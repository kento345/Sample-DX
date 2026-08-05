#include "RenderTarget.h"
#include<cassert>

RenderTarget::~RenderTarget()
{
	//レンダーターゲットリソースの開放
	for (auto& rt : rendertargets_) {
		if (rt) {
			rt->Release();
			rt = nullptr;
		}
	}
	rendertargets_.clear();
}

[[nodiscard]] bool RenderTarget::createBackBuffer(const Device & device, const SwapChain & swapChain, const DescriptorHeap & heap) noexcept
{
	//スワップチェインの設定を取得
	const auto& desc = swapChain.getDesc();

	//レンダーターゲットリソースのサイズ設定
	rendertargets_.resize(desc.BufferCount);

	//ディスクリプタヒープのハンドルを取得
	auto handle_ = heap.get()->GetCPUDescriptorHandleForHeapStart();

	//ディスクリプタヒープのタイプを取得
	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプがRTVではありません");

	//バックバッファの生成
	for (uint8_t i = 0;i < desc.BufferCount; ++i) {
		const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&rendertargets_[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗しました");
			return false;
		}

		//レンダーターゲットビューを作成してディスクリプタヒープのハンドルを関連付ける
		device.get()->CreateRenderTargetView(rendertargets_[i], nullptr, handle_);

		//次のハンドルへ移動
		handle_.ptr += device.get()->GetDescriptorHandleIncrementSize(heapType);
	}
	return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::getDescriptorHandle(const Device& device, const DescriptorHeap& heap, UINT index) const noexcept
{
	if (index >= rendertargets_.size() || !rendertargets_[index]) {
		assert(false && "不正なレンダーターゲットです\0");
	}

	//ディスクリプタヒープのタイプを取得
	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();

	//ディスクリプタヒープのタイプを取得
	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプがRTVではありません");

	//インデックスに応じてハンドルを移動
	handle.ptr += index * device.get()->GetDescriptorHandleIncrementSize(heapType);
	return handle;
}

[[nodiscard]] ID3D12Resource* RenderTarget::get(uint32_t index) const noexcept
{
	if (index >= rendertargets_.size() || !rendertargets_[index]) {
		assert(false && "不正なレンダーターゲットです\0");
		return nullptr;
	}
	return rendertargets_[index];
}
