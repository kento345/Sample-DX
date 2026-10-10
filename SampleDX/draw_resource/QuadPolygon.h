#pragma once

#include"TempletePolygon.h"
#include<DirectXMath.h>

namespace {
	struct QVertex
	{
		DirectX::XMFLOAT3 position{};
		DirectX::XMFLOAT4 color{};
	};
}

class QuadPolygon : public TempletePolygon<QVertex>
{
public:
	/// <summary>
	/// コンスタントバッファ用データ構造
	/// </summary>
	struct ConstBufferData {
		DirectX::XMMATRIX world_{};  /// ワールド行列
		DirectX::XMFLOAT4 color_{};  /// カラー(RGBA)
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	QuadPolygon() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~QuadPolygon() override;

	/// <summary>
	/// ポリゴン生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] bool create()noexcept override;

	/// <summary>
	/// ポリゴン描画
	/// </summary>
	/// <param name="commandList"></param>
	[[nodiscard]] void draw(const CommandList& commandList)noexcept override;

private:
	PolygonData quadData{
		{
			{ { -1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },// 左上
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },// 左下
			{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },// 右上
			{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },// 右下
		},
		{
		   0, 2, 1,
	       2, 3, 1
		}
	};
};

