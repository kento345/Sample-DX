#include "RenderTarget.h"
#include<cassert>

RenderTarget::~RenderTarget()
{
	renderTargets_.clear();
}

[[nodiscard]] bool RenderTarget::createBackBuffer(const SwapChain & swapChain) noexcept
{
	//スワップチェインの設定を取得
	const auto& desc = swapChain.getDesc();

	//レンダーターゲットリソースのサイズ設定
	renderTargets_.resize(desc.BufferCount);

	//バックバッファの生成
	for (uint8_t i = 0;i < desc.BufferCount; ++i) {
		const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗しました");
			return false;
		}
	}

	//ビューの作成
	if (!createRenderTargetView()) {
		return false;
	}

	//ビューの作成
	if (!createShaderResourceView()) {
		return false;
	}

	return true;
}

[[nodiscard]] bool RenderTarget::createRendertTarget(UINT w, UINT h, DXGI_FORMAT format, UINT num) noexcept
{
	//descの作成
	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width = w;
	desc.Height = h;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//レンダーターゲット用ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	//レンダーターゲット用クリア値
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 1.0f;

	for (UINT i = 0; i < num; i++) {
		//レンダーターゲットリソースの作成

		Microsoft::WRL::ComPtr<ID3D12Resource> renderTarget{};

		const auto hr = Device::instance().get()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clearValue,
			IID_PPV_ARGS(&renderTarget));
		if (FAILED(hr)) {
			assert(false && "レンダーターゲットの作成に失敗しました");
			return false;
		}

		renderTargets_.emplace_back(std::move(renderTarget));
	}

	//ビューの作成
	if (!createRenderTargetView()) {
		return false;
	}

	//ビューの作成
	if (!createShaderResourceView()) {
		return false;
	}

	return true;
}



[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::getCpuDescriptorHandle(UINT index) const noexcept
{
	if (index >= renderTargets_.size() || !renderTargets_[index] || rtvDescriptorIndex_.size() != renderTargets_.size()) {
		assert(false && "不正なレンダーターゲットです\0");
	}
	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//ディスクリプタヒープの取得
	auto heap = DescriptorHeapContainer::instance().get(heapType);

	//ディスクリプタヒープのタイプを取得
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();

	//ディスクリプタのサイズ取得
	const auto rtvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);

	//インデックスに応じてハンドルを移動
	handle.ptr += rtvDescriptorIndex_[index] * rtvDescriptorSize;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE RenderTarget::getGpuDescriptorHandle(UINT index) const noexcept
{
	if (index >= renderTargets_.size() || !renderTargets_[index] || srvDescriptorIndex_.size() != renderTargets_.size()) {
		assert(false && "不正なレンダーターゲットです");
	}

	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//ディスクリプタヒープの取得
	auto heap = DescriptorHeapContainer::instance().get(heapType);

	//GPU用ディスクリプタヒープのハンドルを取得
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	//ディスクリプタのサイズの取得
	const auto srvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);

	handle.ptr += srvDescriptorIndex_[index] * srvDescriptorSize;
	return handle;
}

//-------------------
// ここから変更
//-------------------

[[nodiscard]] ID3D12Resource* RenderTarget::get(uint32_t index) const noexcept
{
	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲットです\0");
		return nullptr;
	}
	return renderTargets_[index].Get();
}
