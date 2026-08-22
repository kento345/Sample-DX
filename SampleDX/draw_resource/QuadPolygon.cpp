#include "QuadPolygon.h"
#include<cassert>


QuadPolygon::~QuadPolygon()
{
	//頂点バッファの開放
	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
	//インデックスバッファの開放
	if (indexBuffer_) {
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}

bool QuadPolygon::create(const Device& device) noexcept
{
	//頂点バッファの生成
	if (!createVerttexBuffer(device))
	{
		assert(false && "頂点バッファの作成に失敗\0");
		return false;
	}
	//インデックスバッファの生成
	if (!createIndexBuffer(device))
	{
		assert(false && "インデックスバッファの作成に失敗\0");
		return false;
	}
	return true;
}

void QuadPolygon::draw(const CommandList& commandList) noexcept
{
	//頂点バッファの設定
	commandList.get()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//インデックスバッファの設定
	commandList.get()->IASetIndexBuffer(&indexBufferView_);
	//プリミティブ形状の設定(三角形)
	commandList.get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画コマンド
	commandList.get()->DrawIndexedInstanced(quadData.indices.size(), 1, 0, 0, 0);
}

bool QuadPolygon::createVerttexBuffer(const Device& device) noexcept
{
	return TempletePolygon::createVerttexBuffer(device,quadData.vertices);
}

bool QuadPolygon::createIndexBuffer(const Device& device) noexcept
{
	return TempletePolygon::createIndexBuffer(device,quadData.indices);
}
