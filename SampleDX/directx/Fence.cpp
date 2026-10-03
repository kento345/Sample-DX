#include "Fence.h"
#include<cassert>

Fence::~Fence()
{
	if (fence_) {
		fence_->Release();
		fence_ = nullptr;
	}
}

[[nodiscard]] bool Fence::create() noexcept
{
	//フェンスの作成
	HRESULT hr = Device::instance().get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	if (FAILED(hr))
	{
		assert(false && "フェンスの作成に失敗\0");
		return false;
	}
	//GPU 同期用イベントハンドル作成
	waitGpuEvent_ = CreateEvent(nullptr, false, false, L"WAIT_GPU");
	if (!waitGpuEvent_) {
		assert(false && "GPU 同期用イベントハンドルの作成に失敗\0");
		return false;
	}
	return true;
}

void Fence::wait(UINT64 fenceValue) const noexcept
{
	if (!fence_) {
		assert(false && "フェンスが未作成です\0");
		return;
	}

	//フェンスの値が指定された値に到達するまで待機
	if (fence_->GetCompletedValue() < fenceValue) {
		//GPUがフェンスの値に到達するまで待機
		fence_->SetEventOnCompletion(fenceValue, waitGpuEvent_);
		WaitForSingleObject(waitGpuEvent_, INFINITE);
	}
}

[[nodiscard]] ID3D12Fence* Fence::get() const noexcept
{
	if (!fence_) {
		assert(false && "フェンスが未作成です\0");
		return nullptr;
	}
	return fence_;
}
