#pragma once

#include"DXGI.h"
#include<d3d12.h>
#include<wrl/client.h>


class Device final
{
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static Device& instance() noexcept {
		static Device instance;
		return instance;
	}

	/// <summary>
	/// デバイスの作成
	/// </summary>
	/// <param name="adapter"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const DXGI& dxgi) noexcept;

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12Device* get() const noexcept;

private:
	ID3D12Device* device_{}; //デバイス	
};

