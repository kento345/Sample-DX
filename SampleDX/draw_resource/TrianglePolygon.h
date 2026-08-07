#pragma once

#include"../directx/Device.h"
#include"../directx/CommandList.h"

class TrianglePolygon final
{
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

	D3D12_VERTEX_BUFFER_VIEW verttexBufferView_{}; //頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};	   //インデックスバッファビュー
};

