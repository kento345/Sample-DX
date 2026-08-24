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
	if (!swapChainInstance_.create(dxgiInstance_, windowInstance_, commandQueueInstance_)) {
		assert(false && "スワップチェインの作成に失敗しました");
		return false;
	}

	//ディスクリプタヒープの生成
	if (!descriptorHeapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapChainInstance_.getDesc().BufferCount)) {
		assert(false && "ディスクリプタヒープの作成に失敗しました");
		return false;
	}

	//レンダーターゲットの生成
	if (!renderTargetInstance_.createBackBuffer(deviceInstance_, swapChainInstance_, descriptorHeapInstance_)) {
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


	//三角形ポリゴンの生成
	if (!trianglePolygonInstance_.create(deviceInstance_)) {
		assert(false && "三角形ポリゴンの作成に失敗しました");
		return false;
	}

	triangleObjectInstance_.initialize({0.f,0.f,0.f}, {0.1f,0.1f,0.1f,1.0f});

	//四角形ポリゴン生成
	if (!quadPolygonInstance_.create(deviceInstance_)){
		assert(false && "四角形ポリゴンの作成に失敗しました");
		return false;
	}
	quadObjectInstance_.initialize({ 2.f,0.f,1.f }, { 1.f,0.f,0.f,1.f });

	//ルートシグネチャの生成
	if (!rootSignatureInstance_.create(deviceInstance_)) {
		assert(false && "ルートシグネチャの作成に失敗しました");
		return false;
	}
	//シェーダの生成
	if (!shaderInstance_.create(deviceInstance_)) {
		assert(false && "シェーダーの作成に失敗しました");
		return false;
	}
	//パイプラインステートオブジェクトの生成
	if (!piplineStateObjectInstance_.create(deviceInstance_, shaderInstance_, rootSignatureInstance_)) {
		assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
		return false;
	}

	//カメラの生成
	cameraInstance_.initialize();

	//定数バッファ用ディスクリプタヒープの生成
	//定数バッファ用はCBV_SRV_UAV タイプで2つ分確保
	if (!constantBufferDescriptorHeapInstance_.create(deviceInstance_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3, true)) {
		assert(false && "定数バッファ用ディスクリプタヒープのの作成に失敗しました");
		return false;
	}

	//カメラ用コンスタントバッファの作成
	//カメラ用はディスクリプタヒープの先頭に作成
	if (!cameraConstantBufferInstance_.create(deviceInstance_, constantBufferDescriptorHeapInstance_, sizeof(Camera::ConstBufferData), 0)) {
		assert(false && "カメラ用コンスタントバッファの作成に失敗しました");
		return false;
	}

	//三角形ポリゴン用コンスタントバッファの作成
	//三角形ポリゴン用はディスクリプタヒープの2番目に作成
	if (!trianglePolygonConstantBufferInstance_.create(deviceInstance_, constantBufferDescriptorHeapInstance_, sizeof(TrianglePolygon::ConstBufferData), 1)) {
		assert(false && "三角形ポリゴン用コンスタントバッファの作成に失敗しました");
		return false;
	}

	//四角形ポリゴン用コンスタントバッファの作成
	if (!quadPolygonConstanBufferInstance_.create(deviceInstance_, constantBufferDescriptorHeapInstance_, sizeof(QuadPolygon::ConstBufferData), 2)) {
		assert(false && "四角形ポリゴン用コンスタントバッファの作成に失敗しました");
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
		//カメラの更新
		cameraInstance_.update();
		//三角形オブジェクトの更新
		triangleObjectInstance_.update();
		//四角形オブジェクトの更新
		quadObjectInstance_.update();
		

		// 現在のバックバッファインデックスを取得
		const auto backBufferIndex = swapChainInstance_.get()->GetCurrentBackBufferIndex();

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
		const float	 cleaColor[] = { 0.2f, 0.2f, 0.2f, 1.2f };  // 赤色でクリア
		commandListInstance_.get()->ClearRenderTargetView(handles[0], cleaColor, 0, nullptr);

		//-----------------
		//パイプラインステートの設定
		commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());
		//ルートシグネチャの設定
		commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

		//ビューポートの設定
		const auto [w, h] = windowInstance_->size();
		D3D12_VIEWPORT viewport{};
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(w);
		viewport.Height = static_cast<float>(h);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		commandListInstance_.get()->RSSetViewports(1, &viewport);

		//シザー短形の設定
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = w;
		scissorRect.bottom = h;
		commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

		//コンスタントバッファ用ディスクリプタヒープの設定
		ID3D12DescriptorHeap* p[] = { constantBufferDescriptorHeapInstance_.get() };
		commandListInstance_.get()->SetDescriptorHeaps(1, p);

		{
			// カメラのコンスタントバッファへデータ転送
			Camera::ConstBufferData cameraData{
				DirectX::XMMatrixTranspose(cameraInstance_.viewMatrix()),
				DirectX::XMMatrixTranspose(cameraInstance_.projection()),
			};
			UINT8* pCameraData{};
			cameraConstantBufferInstance_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pCameraData));
			memcpy_s(pCameraData, sizeof(cameraData), &cameraData, sizeof(cameraData));
			cameraConstantBufferInstance_.constantBuffer()->Unmap(0, nullptr);
			commandListInstance_.get()->SetGraphicsRootDescriptorTable(0, cameraConstantBufferInstance_.getGpuDescriptorHandle());

		}
	
		{
			// 三角形ポリゴンのコンスタントバッファへデータ転送
			TrianglePolygon::ConstBufferData triangleData{
				DirectX::XMMatrixTranspose(triangleObjectInstance_.world()),
				triangleObjectInstance_.color() };
			UINT8* pTriangleData{};
			trianglePolygonConstantBufferInstance_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pTriangleData));
			memcpy_s(pTriangleData, sizeof(triangleData), &triangleData, sizeof(triangleData));
			trianglePolygonConstantBufferInstance_.constantBuffer()->Unmap(0, nullptr);
			commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, trianglePolygonConstantBufferInstance_.getGpuDescriptorHandle());
			// ポリゴンの描画
			trianglePolygonInstance_.draw(commandListInstance_);
		}

		{
			QuadPolygon::ConstBufferData quadData{
				DirectX::XMMatrixTranspose(quadObjectInstance_.world()),
				quadObjectInstance_.color() };
			UINT8* pQuadData{};
			quadPolygonConstanBufferInstance_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pQuadData));
			memcpy_s(pQuadData, sizeof(quadData), &quadData, sizeof(quadData));
			quadPolygonConstanBufferInstance_.constantBuffer()->Unmap(0, nullptr);
			commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, quadPolygonConstanBufferInstance_.getGpuDescriptorHandle());

			quadPolygonInstance_.draw(commandListInstance_);
		}
		//-------------------------------------------------

		// リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
		auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandListInstance_.get()->ResourceBarrier(1, &rtToP);

		//コマンドリストをクローズ
		commandListInstance_.get()->Close();

		//コマンドキューにコマンドリストを送信
		ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
		commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		//プレゼント
		swapChainInstance_.get()->Present(1, 0);

		//フェンスにフェンス値を設定
		commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
		frameFenceValue_[backBufferIndex] = nextFenceValue_;
		nextFenceValue_++;
	}

	commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
	fenceInstance_.wait(nextFenceValue_);
}