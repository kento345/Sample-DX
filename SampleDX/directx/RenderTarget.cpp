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

[[nodiscard]] ID3D12Resource* RenderTarget::get(uint32_t index) const noexcept
{
	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲットです\0");
		return nullptr;
	}
	return renderTargets_[index].Get();
}

/// <summary>
/// テクスチャサイズを取得
/// </summary>
/// <returns></returns>
std::pair<UINT, UINT> RenderTarget::size() const noexcept
{
	if (renderTargets_.empty()) {
		assert(false && "不正なレンダーターゲットです\0");
		return {};
	}
	//サイズは同じなので最初の要素から取得
	const auto desc = renderTargets_[0]->GetDesc();

	return { static_cast<UINT>(desc.Width),static_cast<UINT>(desc.Height) };
}

bool RenderTarget::createRenderTargetView() noexcept
{
	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	for (auto i = 0;i < renderTargets_.size();i++) {
		const auto descruptorIndex = DescriptorHeapContainer::instance().allocateDescriptor(heapType);
		if (!descruptorIndex.has_value()) {
			assert(false && "レンダーターゲットのディスクリプタ確保に失敗しました");
			return false;
		}

		auto index = descruptorIndex.value();
		//ディスクリプタヒープの取得
		auto heap = DescriptorHeapContainer::instance().get(heapType);
		//ディスクリプタのサイズ取得
		const auto cbvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);

		//ディスクリプタヒープのハンドル取得
		auto cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += index * cbvDescriptorSize;

		//レンダーターゲットビューを作成してディスクリプタヒープのハンドルと関連付ける
		Device::instance().get()->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, cpuHandle);

		rtvDescriptorIndex_.emplace_back(index);
	}
	return true;
}

bool RenderTarget::createShaderResourceView() noexcept
{
	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//シェーダリソースビューの作成
	for (auto i = 0; i < renderTargets_.size();i++) {
		//ディスクリプタの確保
		const auto descripotorIndex = DescriptorHeapContainer::instance().allocateDescriptor(heapType);
		if (!descripotorIndex.has_value()) {
			assert(false && "シェーダーリソースのディスクリプタ確保に失敗しました");
			return false;
		}

		auto index = descripotorIndex.value();

		//シェーダーリソースビューの設定
		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Format = renderTargets_[i]->GetDesc().Format;
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = renderTargets_[i]->GetDesc().MipLevels;
		// ディスクリプターヒープの取得
		auto heap = DescriptorHeapContainer::instance().get(heapType);
		// ディスクリプタのサイズを取得
		const auto cbvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);
		// ディスクリプターヒープのハンドルを取得
		auto cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += index * cbvDescriptorSize;
		// シェーダーリソースビューを作成してディスクリプタヒープのハンドルと関連付ける
		Device::instance().get()->CreateShaderResourceView(renderTargets_[i].Get(), &desc, cpuHandle);
		srvDescriptorIndex_.emplace_back(index);
	}
	return true;
}


