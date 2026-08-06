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

/// <summary>
 /// リソースバリアを設定
 /// </summary>
 /// <param name="resource"></param>
 /// <param name="from"></param>
 /// <param name="to"></param>
 /// <returns></returns>
D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource;
	barrier.Transition.StateBefore = from;
	barrier.Transition.StateAfter = to;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	return barrier;
}

void Application::loop() noexcept
{
	while (windowInstance_->messageLoop())
	{
		// 現在のバックバッファインデックスを取得
		const auto backBufferIndex = swaoChainInstance_.get()->GetCurrentBackBufferIndex();

		// 以前のフレームの GPU の処理が完了しているか確認して待機する
		if (frameFenceValue_[backBufferIndex] != 0) {
			fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
		}

		// コマンドアロケータリセット
		commandAllocatorInstance_[backBufferIndex].reset();
		// コマンドリストリセット
		commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);
		
		//リソースバリアでレンダーターゲットをPresentからRenderTargetへ変更
		auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandListInstance_.get()->ResourceBarrier(1, &pToRT);

		//レンダーターゲットの設定
		D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getDescriptorHandle(deviceInstance_,descriptorHeapInstance_,backBufferIndex) };
		commandListInstance_.get()->OMSetRenderTargets(1, handles, false, nullptr);

		//レンダーターゲットのクリア
		const float	 cleaColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };  // 赤色でクリア
		commandListInstance_.get()->ClearRenderTargetView(handles[0], cleaColor, 0, nullptr);

		// リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
		auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandListInstance_.get()->ResourceBarrier(1, &rtToP);

		//コマンドリストをクローズ
		commandListInstance_.get()->Close();

		//コマンドキューにコマンドリストを送信
		ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
		commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		//プレゼント
		swaoChainInstance_.get()->Present(1, 0);

		//フェンスにフェンス値を設定
		commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
		frameFenceValue_[backBufferIndex] = nextFenceValue_;
		nextFenceValue_++;
	}

	commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
	fenceInstance_.wait(nextFenceValue_);
}