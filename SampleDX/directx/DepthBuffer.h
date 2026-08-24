#pragma once

#include"Device.h"
#include"../window/Window_Base.h"
#include<memory>



class DepthBuffer final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DepthBuffer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DepthBuffer();

	/// <summary>
	/// デプスバッファを生成
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool create(const std::unique_ptr<Window_Base>& window)noexcept;

	/// <summary>
	/// デプスバッファを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12Resource* depthBuffer()const noexcept;

	/// <summary>
	/// ディスクリプタハンドル取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle()const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;  /// レンダーターゲットリソースの配列
	D3D12_CPU_DESCRIPTOR_HANDLE            handle_{};     /// ディスクリプタハンドル
};

