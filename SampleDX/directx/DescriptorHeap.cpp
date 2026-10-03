#include "DescriptorHeap.h"
#include"Device.h"
#include<cassert>
#include<wrl/client.h>

class DescriptorHeap final {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DescriptorHeap() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DescriptorHeap() = default;

public:
	/// <summary>
	/// ディスクリプタヒープを生成
	/// </summary>
	/// <param name="type"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaferVisible"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaferVisible) noexcept {
		//ヒープの設定
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = type;
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Flags = shaferVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		type_ = type; //ヒープタイプの保存

		//ディスクリプタヒープの生成
		HRESULT hr = Device::instance().get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
		if (FAILED(hr)) {
			assert(false && "ディスクリプタヒープの生成に失敗しました");
			return false;
		}

		//全ディスクリプタをフリーリストに登録
		maxDescriptorCount_ = numDescriptors;
		for (UINT i = 0; i < maxDescriptorCount_;i++) {
			freeIndices_.push_back(i);
		}

		return true;
	}

	/// <summary>
	/// 解放予約されているディスクリプタを解放
	/// </summary>
	/// <returns></returns>
	void applyPendingFree() noexcept {
		if (pendingFreeIndices_.empty()) {
			return;
		}
		for (auto i : pendingFreeIndices_) {
			freeIndices_.push_back(i);
		}
		pendingFreeIndices_.clear();
	}

	/// <summary>
	/// ディスクリプタヒープを取得する
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12DescriptorHeap* get() const noexcept {
		if (!heap_) {
			assert(false && "ディスクリプタヒープが未作成です");
		}
		return heap_.Get();
	}

	/// <summary>
	/// ディスクリプタヒープのタイプを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE getType() const noexcept {
		if (!heap_) {
			assert(false && "ディスクリプタヒープが未作成です");
		}
		return type_;
	}

	/// <summary>
	/// ディスクリプタを確保
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] std::optional<UINT> allocateDescriptor() noexcept {
		if (freeIndices_.empty()) {
			assert(false && "ディスクリプタヒープの空きがありません");
			return std::nullopt;
		}
		UINT index = freeIndices_.back();
		freeIndices_.pop_back();
		return index;
	}

	/// <summary>
	/// 解放予定のディスクリプタを登録する
	/// </summary>
	/// <param name="descriptorIndex"></param>
	void releaseDescriptor(UINT descriptorIndex) noexcept {
		pendingFreeIndices_.push_back(descriptorIndex);
	}

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_; //ディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_TYPE type_{}; //ヒープタイプ
	UINT maxDescriptorCount_{}; //最大ディスクリプタ数
	std::vector<UINT> freeIndices_; //解放済みディスクリプタのインデックス
	std::vector<UINT> pendingFreeIndices_; //解放予定のディスクリプタのインデックス
};

DescriptorHeapContainer::DescriptorHeapContainer() = default;


DescriptorHeapContainer::~DescriptorHeapContainer() {
	map_.clear();
}


bool DescriptorHeapContainer::create( D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaferVisible) noexcept
{
	if (map_.find(type) != map_.end()) {
		return false;
	}

	auto p = std::make_unique<DescriptorHeap>();
	if (p->create(type, numDescriptors, shaferVisible)) {
		map_.emplace(type, std::move(p));
	}
	else {
		assert(false && "ディスクリプタヒープの作成に失敗しました\0");
		return false;
	}
	return true;
}

void DescriptorHeapContainer::applyPendingFree() noexcept
{
	for (auto& [key, p] : map_) {
		p->applyPendingFree();
	}
}

ID3D12DescriptorHeap* DescriptorHeapContainer::get(D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept
{
	const auto it = map_.find(type);
	if (it == map_.end()) {
		assert(false && "ディスクリプタヒープがありません\0");
		return nullptr;
	}
	return it->second->get();
}

std::optional<UINT> DescriptorHeapContainer::allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept
{
	const auto it = map_.find(type);
	if (it == map_.end()) {
		assert(false && "ディスクリプタヒープがありません\0");
		return std::nullopt;
	}
	return it->second->allocateDescriptor();
}

void DescriptorHeapContainer::releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex) noexcept
{
	const auto it = map_.find(type);
	if (it == map_.end()) {
		assert(false && "ディスクリプタヒープがありません\0");
		return;
	}
	it->second->releaseDescriptor(descriptorIndex);
}
