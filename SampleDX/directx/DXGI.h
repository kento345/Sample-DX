#pragma once

#include<d3d12.h>
#include<dxgi1_4.h>

class DXGI final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DXGI() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DXGI();

	/// <summary>
	/// ディスプレイアダプター
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] bool setDisplayAdapter() noexcept;

	/// <summary>
	/// ファクトリーを取得
	/// </summary>
	[[nodiscard]] IDXGIFactory4* factory() const noexcept;

	/// <summary>
	/// ディスプレイアダプターを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] IDXGIAdapter1* displayAdapter() const noexcept;

private:
	IDXGIFactory4* dxgiFactory_{}; //DXGIを作成するファクトリー
	IDXGIAdapter1* dxgiAdapter_{}; //ディスプレイアダプター
};

