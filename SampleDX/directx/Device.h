#pragma once

#include"DXGI.h"
#include<d3d12.h>
#include<wrl/client.h>
#include"../util.h"
#include"../MoveProhibited/Nomove.h"


class Device : public Nomove
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
	[[nodiscard]] bool create() noexcept;

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12Device* get() const noexcept;

	[[nodiscard]] const DXGI& dxgi() const noexcept;

private:

	Device() = default;
	
	~Device() = default;

private:
	DXGI dxgiInstance_{};
	Microsoft::WRL::ComPtr<ID3D12Device> device_{}; //デバイス	
};

