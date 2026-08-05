#include<Windows.h>
#include<cassert>

#include "Application.h"

#include"../../window/FullWindow.h"
#include"../../window/ScaleWindow.h"

bool a = true;

/// <summary>
/// 初期化
/// </summary>
/// <returns></returns>
bool Application::initialize(HINSTANCE hInstance)
{
	//フルスクリーンウィンドウの生成
	if (a) {
		auto wi = std::make_unique<FullWindow>();
		if (S_OK != wi->create(hInstance, L"SampleDX"))
		{
			assert(false && "ウィンドウの作成に失敗しました");
			return false;
		}
		windowInstance_ = std::move(wi);
	}
	//サイズ変更可ウィンドウの生成
	else {
		auto wi = std::make_unique<ScaleWindow>();

		if (S_OK != wi->create(hInstance, 1280, 720, L"SampleDX"))
		{
			assert(false && "ウィンドウの作成に失敗しました");
			return false;
		}
		windowInstance_ = std::move(wi);
	}

	//DXGI の生成
	if (!dxgiInstance_.setDisplayAdapter()) {
		assert(false && "DXGIのアダプタの設定に失敗しました");
		return false;
	}

	//デバイスの生成
	if (!deviceInstance_.create(dxgiInstance_)) {
		assert(false && "デバイスの作成に失敗しました");
		return false;
	}

	//コマンドキューの生成
	if (!commandQueueInstance_.create(deviceInstance_)) {
		assert(false && "コマンドキューの作成に失敗しました");
		return false;
	}

	//スワップチェインの生成
	if (!swaoChainInstance_.create(dxgiInstance_, windowInstance_, commandQueueInstance_)) {
		assert(false && "スワップチェインの作成に失敗しました");
		return false;
	}

	//ディスクリプタヒープの生成
	if (!descriptorHeapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swaoChainInstance_.getDesc().BufferCount)) {
		assert(false && "ディスクリプタヒープの作成に失敗しました");
		return false;
	}

	//レンダーターゲットの生成
	if (!renderTargetInstance_.createBackBuffer(deviceInstance_, swaoChainInstance_, descriptorHeapInstance_)) {
		assert(false && "レンダーターゲットの作成に失敗しました");
		return false;
	}

	//コマンドアロケータの生成
	if (!commandAllocatorInstance_[0].create(deviceInstance_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
		assert(false && "コマンドアロケータの作成に失敗しました");
		return false;
	}

	if (!commandAllocatorInstance_[1].create(deviceInstance_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
		assert(false && "コマンドアロケータの作成に失敗しました");
		return false;
	}

	//コマンドリストの生成
	if (!commandListInstance_.create(deviceInstance_, commandAllocatorInstance_[0])) {
		assert(false && "コマンドリストの作成に失敗しました");
		return false;
	}

	//フェンスの生成
	if (!fenceInstance_.create(deviceInstance_)) {
		assert(false && "フェンスの作成に失敗しました");
		return false;
	}
	return true;
}

void Application::loop() noexcept
{
	while (windowInstance_->messageLoop())
	{

	}
}
