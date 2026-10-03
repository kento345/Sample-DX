#pragma once

class RootSignature final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RootSignature() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RootSignature();

	/// <summary>
	/// ルートシグネチャを作成する
	/// </summary>
	/// <param name="devicenoe"></param>
	/// <returns></returns>
	[[nodiscard]] bool create()noexcept;

	/// <summary>
	/// ルートシグネチャを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12RootSignature* get() const noexcept;

private:
	ID3D12RootSignature* rootSignature_{}; //ルートシグネチャ
};

