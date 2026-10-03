#pragma once

#include"../MoveProhibited/Nomove.h"

#include<d3d12.h>
#include<unordered_map>
#include<optional>
#include<memory>

class DescriptorHeap; //前方宣言


class DescriptorHeapContainer : public Nomove{
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static DescriptorHeapContainer& instance() noexcept {
		static DescriptorHeapContainer instance;
		return instance;
	}

	/// <summary>
	/// ディスクリプタヒープを生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="type"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaferVisible"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaferVisible = false)noexcept;

	/// <summary>
	/// 解放予約されているディスクリプターを解放する
	/// </summary>
	void applyPendingFree() noexcept;;

	/// <summary>
	/// ディスクリプタヒープを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12DescriptorHeap* get(D3D12_DESCRIPTOR_HEAP_TYPE type)const noexcept;

	/// <summary>
	/// ディスクリプタを確保
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	[[nodiscard]] std::optional<UINT> allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept;

	/// <summary>
	/// 解放予定のディスクリプタを登録
	/// </summary>
	/// <param name="type"></param>
	/// <param name="descriptorIndex"></param>
	void releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex)noexcept;

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DescriptorHeapContainer();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DescriptorHeapContainer();

private:
	std::unordered_map<UINT, std::unique_ptr<DescriptorHeap>> map_; //種類毎のデスクリプタマップ
};

