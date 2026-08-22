#include "TrianglePolygon.h"
#include<cassert>
#include<DirectXMath.h>

namespace {
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}

TrianglePolygon::~TrianglePolygon()
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

bool TrianglePolygon::create(const Device& device) noexcept
{
	//頂点バッファの生成
	if (!createVerttexBuffer(device)) {
		assert(false && "頂点バッファの作成に失敗\0");
		return false;
	}
	//インデックスバッファの生成
	if (!createIndexBuffer(device)) {
		assert(false && "インデックスバッファの作成に失敗\0");
		return false;
	}
	return true;
}

bool TrianglePolygon::createVerttexBuffer(const Device& device) noexcept
{
	//三角形の頂点データ
	Vertex triangleVertices[] = {
		// 上
        {{ 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        
        // 前
        {{ 0.0f, -0.5f,  0.8f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        
        // 左
        {{-0.8f, -0.5f, -0.4f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        
        // 右
        {{ 0.8f, -0.5f, -0.4f}, {1.0f, 1.0f, 0.0f, 1.0f}},
	};

	//頂点データサイズ
	const auto vertexBufferSize = sizeof(triangleVertices);

	//ヒープの設定を指定
	//CPUからアクセス可能なメモリーを利用するための設定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	//どんなリソースを作成するのか設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = vertexBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//頂点バッファの生成
	auto res = device.get()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer_));
	if (FAILED(res)) {
		assert(false && "頂点バッファの作成に失敗\0");
		return false;
	}

	//頂点バッファにデータを転送
	//CPUからアクセス可能なアドレスを取得
	Vertex* data{};

	//バッファをマップ(CPUからアクセス可能にする)
	//vertexBuffer_ を直接利用するのではなく,dataを介して更新する
	res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "頂点バッファのマップに失敗\0");
		return false;
	}

	//頂点データをコピー
	memcpy_s(data, vertexBufferSize, triangleVertices, vertexBufferSize);

	//コピーが終わったのでマップ削除(CPUからアクセス不可にする)
	//ここまで来たらGPUが利用するメモリ領域にコピー済みなのでtriangleVertices は不要になる
	vertexBuffer_->Unmap(0, nullptr);

	//頂点バッファビューの設定
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = vertexBufferSize;
	vertexBufferView_.StrideInBytes = sizeof(Vertex);

	return true;
}

[[nodiscard]]bool TrianglePolygon::createIndexBuffer(const Device& device) noexcept
{
	//三角形の頂点データ
	uint16_t triangleIndeces[] = {
		 0, 1, 2,  // 前面
	     0, 3, 1,  // 右面
	     0, 2, 3,  // 左面
	     1, 3, 2   // 底面
	};

	//インデックスデータのサイズ
	const auto indexBufferSize = sizeof(triangleIndeces);/*sizeof(uint16_t) * 3;*/
		

	//ヒープの設定を指定
	//CPUからアクセス可能なメモリーを利用するための設定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	//どんなリソースを作成するのか設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = indexBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//頂点バッファの生成
	auto res = device.get()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer_));
	if (FAILED(res)) {
		assert(false && "インデックスバッファの作成に失敗\0");
		return false;
	}

	//頂点バッファにデータを転送
	uint16_t* data{};

	//バッファをマップ(CPUからアクセス可能にする)
	//vertexBuffer_ を直接利用するのではなく,dataを介して更新する
	res = indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "インデックスバッファのマップに失敗\0");
		return false;
	}

	//頂点データをコピー
	//memcpy(data, triangleIndeces, indexBufferSize);

	memcpy_s(data, indexBufferSize, triangleIndeces, indexBufferSize);

	//コピーが終わったのでマップ削除(CPUからアクセス不可にする)
	//ここまで来たらGPUが利用するメモリ領域にコピー済みなのでtriangleVertices は不要になる
	indexBuffer_->Unmap(0, nullptr);

	//頂点バッファビューの設定
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = indexBufferSize;
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;

	return true;
}

void TrianglePolygon::draw(const CommandList& commandList) noexcept
{
	//頂点バッファの設定
	commandList.get()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//インデックスバッファの設定
	commandList.get()->IASetIndexBuffer(&indexBufferView_);
	//プリミティブ形状の設定(三角形)
	commandList.get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画コマンド
	commandList.get()->DrawIndexedInstanced(12, 1, 0, 0, 0);
}