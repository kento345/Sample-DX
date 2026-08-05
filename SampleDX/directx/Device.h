#pragma once

#include"DXGI.h"
#include<d3d12.h>

class Device final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Device() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Device();

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

