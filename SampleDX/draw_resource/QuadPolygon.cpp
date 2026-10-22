#include "QuadPolygon.h"
#include<cassert>


bool QuadPolygon::create() noexcept 
{
	//頂点バッファの生成
	if (!TempletePolygon<QVertex>::createVerttexBuffer(quadData.vertices))
	{
		assert(false && "頂点バッファの作成に失敗\0");
		return false;
	}
	//インデックスバッファの生成
	if (!TempletePolygon<QVertex>::createIndexBuffer(quadData.indices))
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