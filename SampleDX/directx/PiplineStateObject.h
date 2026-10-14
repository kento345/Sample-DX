#pragma once

#include"Device.h"
#include"RootSignature.h"
#include "Shader.h"

class PiplineStateObject final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PiplineStateObject() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PiplineStateObject() = default;

	/// <summary>
	/// パイプラインステートオブジェクト生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shader"></param>
	/// <param name="rootSignature"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const Shader& shader, const RootSignature& rootSignature)noexcept;

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12PipelineState* get()const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> piplineState_{};  //パイプラインステート
};

