#pragma once

#include"Device.h"
#include"RootSignature.h"
#include "Shader.h"

#include<functional>

using VertexLayout = std::pair<D3D12_INPUT_ELEMENT_DESC*, size_t>;

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
	[[nodiscard]] bool create(const Shader& shader, const RootSignature& rootSignature,std::function<VertexLayout()> layout,int blendType,bool depth)noexcept;

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12PipelineState* get()const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> piplineState_{};  //パイプラインステート
};

