#pragma once

#include"../directx/CommandList.h"

#include"TempletePolygon.h"

#include<DirectXMath.h>

namespace {
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}

class TrianglePolygon : public TempletePolygon<Vertex>
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
	TrianglePolygon() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TrianglePolygon();

	/// <summary>
	/// ポリゴン生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] bool create()noexcept;

	/// <summary>
	/// ポリゴン描画
	/// </summary>
	/// <param name="commandList"></param>
	[[nodiscard]] void draw(const CommandList& commandList)noexcept;

private:
	PolygonData triangleData_{
		{
		   { { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },//上
		   { { 0.0f, -0.5f,  0.8f}, {0.0f, 1.0f, 0.0f, 1.0f} },//下
		   { {-0.8f, -0.5f, -0.4f}, {0.0f, 0.0f, 1.0f, 1.0f} },//左
		   { { 0.8f, -0.5f, -0.4f}, {1.0f, 1.0f, 0.0f, 1.0f} },//右
	    },
	    {
	       0, 1, 2,
	       0, 3, 1,
	       0, 2, 3,
	       1, 3, 2
	    }
    };
};

