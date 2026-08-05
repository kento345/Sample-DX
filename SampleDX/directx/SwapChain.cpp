#include "SwapChain.h"
#include<cassert>

SwapChain::~SwapChain()
{
	if (swapChain_) {
		swapChain_->Release();
		swapChain_ = nullptr;
	}
}

bool SwapChain::create(const DXGI& dxgi, const std::unique_ptr<Window_Base>& window, const CommandQueue& commandQueue) noexcept
{
	//ウィンドウサイズを取得
	const auto [w, h] = window->size();

	swapChainDesc_ = {};
	swapChainDesc_.BufferCount = 2;
	swapChainDesc_.Width = w;
	swapChainDesc_.Height = h;
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc_.SampleDesc.Count = 1;

	//一時的なスワップチェインの作成
	//スワップチェインのアップグレートが必要
	IDXGISwapChain1* tempSwapChain{};
	{
		const auto hr = dxgi.factory()->CreateSwapChainForHwnd(commandQueue.get(), window->handle(), &swapChainDesc_, nullptr, nullptr, &tempSwapChain);

		if (FAILED(hr)) {
			assert(false && "スワップチェインの作成に失敗\0");
			return false;
		}
	}

	//スワップチェインのバージョンをアップグレート
	{
		//一時的なスワップチェインを IDXGISwapChain3 に変換
		const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));
		//一時的なスワップチェインは解放
		tempSwapChain->Release();

		if (FAILED(hr)) {
			assert(false && "スワップチェインのアップグレートに失敗\0");
			return false;
		}
	}

	return true;
}

IDXGISwapChain3* SwapChain::get() const noexcept
{
	if (!swapChain_) {
		assert(false && "スワップチェインが未作成です\0");
		return nullptr;
	}
	return swapChain_;
}

const DXGI_SWAP_CHAIN_DESC1& SwapChain::getDesc() const noexcept
{
	if (!swapChain_) {
		assert(false && "スワップチェインが未作成です\0");
	}
	return swapChainDesc_;
}
