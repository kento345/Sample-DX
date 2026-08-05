#pragma once
#include<memory>

#include"DXGI.h"
#include"CommandQueue.h"
#include"../window/Window_Base.h"

class SwapChain final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SwapChain() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SwapChain();
	
	/// <summary>
	/// スワップチェインの生成
	/// </summary>
	/// <param name="dxgi"></param>
	/// <param name="window"></param>
	/// <param name="commandQueue"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const DXGI& dxgi, const std::unique_ptr<Window_Base>& window, const CommandQueue& commandQueue) noexcept;
	
	/// <summary>
	/// スワップチェインを取得する
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] IDXGISwapChain3* get() const noexcept;

	/// <summary>
	/// スワップチェインの設定を取得する
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1&  getDesc() const noexcept;

private:
	IDXGISwapChain3* swapChain_{};          //スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{}; //スワップチェーンの設定
};

