#pragma once

#include"../directx/Device.h"
#include"../directx/CommandList.h"
#include<DirectXMath.h>

class TrianglePolygon final
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
	[[nodiscard]] bool create(const Device& device)noexcept;

	/// <summary>
	/// ポリゴン描画
	/// </summary>
	/// <param name="commandList"></param>
	[[nodiscard]] void draw(const CommandList& commandList)noexcept;

private:

	/// <summary>
	/// 頂点バッファの生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] bool createVerttexBuffer(const Device& device)noexcept;

	/// <summary>
	/// インデクスバッファの生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] bool createIndexBuffer(const Device& device)noexcept;


private:
	ID3D12Resource* vertexBuffer_{}; //頂点バッファ
	ID3D12Resource* indexBuffer_{};	 //インデックスバッファ

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{}; //頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};	   //インデックスバッファビュー
};

