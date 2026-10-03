#include "RenderTarget.h"
#include<cassert>


namespace {
	constexpr auto heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
}

RenderTarget::~RenderTarget()
{
	rendertargets_.clear();
}

[[nodiscard]] bool RenderTarget::createBackBuffer(const SwapChain & swapChain) noexcept
{
	//スワップチェインの設定を取得
	const auto& desc = swapChain.getDesc();

	//レンダーターゲットリソースのサイズ設定
	rendertargets_.resize(desc.BufferCount);

	//ディスクリプタヒープの取得
	auto heap = DescriptorHeapContainer::instance().get(heapType_);

	//ディスクリプタヒープのハンドルを取得
	auto handle_ = heap->GetCPUDescriptorHandleForHeapStart();

	//バックバッファの生成
	for (uint8_t i = 0;i < desc.BufferCount; ++i) {
		const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&rendertargets_[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗しました");
			return false;
		}

		//レンダーターゲットビューを作成してディスクリプタヒープのハンドルを関連付ける
		Device::instance().get()->CreateRenderTargetView(rendertargets_[i].Get(), nullptr, handle_);

		//次のハンドルへ移動
		handle_.ptr += Device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);
	}
	return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::getDescriptorHandle(UINT index) const noexcept
{
	if (index >= rendertargets_.size() || !rendertargets_[index]) {
		assert(false && "不正なレンダーターゲットです\0");
	}

	auto heap = DescriptorHeapContainer::instance().get(heapType_);

	//ディスクリプタヒープのタイプを取得
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();

	//インデックスに応じてハンドルを移動
	handle.ptr += index * Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);
	return handle;
}

[[nodiscard]] ID3D12Resource* RenderTarget::get(uint32_t index) const noexcept
{
	if (index >= rendertargets_.size() || !rendertargets_[index]) {
		assert(false && "不正なレンダーターゲットです\0");
		return nullptr;
	}
	return rendertargets_[index].Get();
}
