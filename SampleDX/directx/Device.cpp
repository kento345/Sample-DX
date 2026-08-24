#include "Device.h"
#include<cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxguid.lib")

bool Device::create(const DXGI& dxgi) noexcept
{
	const auto hr = D3D12CreateDevice(dxgi.displayAdapter(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_));
	if (FAILED(hr))
	{
		assert(false && "デバイスの作成に失敗\0");
		return false;
	}
	return true;
}

ID3D12Device* Device::get() const noexcept
{
	if (!device_) {
		assert(false && "デバイスが未作成です\0");
		return nullptr;
	}
	return device_;
}
