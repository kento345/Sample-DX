#pragma once

#include"Device.h"
#include"SwapChain.h"
#include"DescriptorHeap.h"
#include<vector>



class RenderTarget final
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
	[[nodiscard]] bool createBackBuffer(const SwapChain& swapChain)noexcept;

	/// <summary>
	/// レンダーターゲットを作成
	/// </summary>
	/// <param name="w"></param>
	/// <param name="h"></param>
	/// <param name="format"></param>
	/// <param name="num"></param>
	/// <returns></returns>
	[[nodiscard]] bool createRendertTarget(UINT w, UINT h, DXGI_FORMAT format, UINT num)noexcept;

	/// <summary>
	/// ビューを取得
	/// </summary>
	/// <param name="device"></param>
	/// <param name="heap"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle(UINT index)const noexcept;

	/// <summary>
	/// GPU用ディスクリプタハンドルの取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle(UINT index)const noexcept;

	/// <summary>
	/// レンダーターゲットを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	[[nodiscard]] ID3D12Resource* get(uint32_t index)const noexcept;

	/// <summary>
	/// テクスチャサイズを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] std::pair<UINT, UINT>size()const noexcept;

private:
	/// <summary>
	/// レンダーリソースビューを作成
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool createRenderTargetView()noexcept;

	/// <summary>
	/// シェーダリソースビューを作成
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool createShaderResourceView()noexcept;

private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets_{};       /// レンダーターゲットリソースの配列
	std::vector<UINT> rtvDescriptorIndex_{};  /// ディスクリプタインデックス
	std::vector<UINT> srvDescriptorIndex_{};  /// ディスクリプタインデックス
};

