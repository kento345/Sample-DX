#pragma once

#include"../directx/Device.h"
#include"../directx/CommandList.h"
#include"PolygonBase.h"
#include<DirectXMath.h>
#include<vector>

template <class T>
class TempletePolygon : public PolygonBase
{
public:
	
	TempletePolygon() = default;

	~TempletePolygon()  = default;

protected:

	struct PolygonData
	{
		std::vector<T> vertices;
		std::vector<uint16_t> indices;
	};

	/// <summary>
	/// 頂点バッファの生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] virtual bool createVerttexBuffer(const std::vector<T>& Vertices)noexcept {
		//三角形の頂点データ

		//頂点データサイズ
		const auto vertexBufferSize = sizeof(T) * Vertices.size();

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
		auto res = Device::instance().get()->CreateCommittedResource(
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
		T* data{};

		//バッファをマップ(CPUからアクセス可能にする)
		//vertexBuffer_ を直接利用するのではなく,dataを介して更新する
		res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
		if (FAILED(res)) {
			assert(false && "頂点バッファのマップに失敗\0");
			return false;
		}

		//頂点データをコピー
		memcpy_s(data, vertexBufferSize, Vertices.data(), vertexBufferSize);

		//コピーが終わったのでマップ削除(CPUからアクセス不可にする)
		//ここまで来たらGPUが利用するメモリ領域にコピー済みなのでtriangleVertices は不要になる
		vertexBuffer_->Unmap(0, nullptr);

		//頂点バッファビューの設定
		vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
		vertexBufferView_.SizeInBytes = vertexBufferSize;
		vertexBufferView_.StrideInBytes = sizeof(T);

		return true;
	};

	/// <summary>
	/// インデクスバッファの生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] virtual bool createIndexBuffer(const std::vector<uint16_t>& Indeces) noexcept
	{
		//三角形の頂点データ

		//インデックスデータのサイズ
		const auto indexBufferSize = sizeof(uint16_t) * Indeces.size();/*sizeof(uint16_t) * 3;*/


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
		auto res = Device::instance().get()->CreateCommittedResource(
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

		memcpy_s(data, indexBufferSize, Indeces.data(), indexBufferSize);

		//コピーが終わったのでマップ削除(CPUからアクセス不可にする)
		//ここまで来たらGPUが利用するメモリ領域にコピー済みなのでtriangleVertices は不要になる
		indexBuffer_->Unmap(0, nullptr);

		//頂点バッファビューの設定
		indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
		indexBufferView_.SizeInBytes = indexBufferSize;
		indexBufferView_.Format = DXGI_FORMAT_R16_UINT;

		return true;
	}

	protected:
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_{}; //頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_{};	 //インデックスバッファ

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{}; //頂点バッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView_{};	   //インデックスバッファビュー
};

