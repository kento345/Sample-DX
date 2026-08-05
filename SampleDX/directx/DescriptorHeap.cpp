#include "DescriptorHeap.h"
#include<cassert>

DescriptorHeap::~DescriptorHeap()
{
	if (heap_) {
		heap_->Release();
		heap_ = nullptr;
	}
}

bool DescriptorHeap::create(const Device & device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaferVisible) noexcept
{
	//ヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = type;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Flags = shaferVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	type_ = type; //ヒープタイプの保存

	//ディスクリプタヒープの生成
	HRESULT hr = device.get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
	if (FAILED(hr)) {
		assert(false && "ディスクリプタヒープの生成に失敗しました");
		return false;
	}

	return true;
}

ID3D12DescriptorHeap* DescriptorHeap::get() const noexcept
{
	if (!heap_) {
		assert(false && "ディスクリプタヒープが未作成です\0");
		return nullptr;
	}
	return heap_;
}

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::getType() const noexcept
{
	if (!heap_) {
		assert(false && "ディスクリプタヒープが未作成です\0");
	}
	return type_;
}
