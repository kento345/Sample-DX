#include "RootSignature.h"

#include"Device.h"
#include<cassert>

RootSignature::~RootSignature()
{
	//ルートシグネーチャの解放
	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}
}

bool RootSignature::create() noexcept
{
	//描画に必要なリソースをシェーダに伝える
	
	//コンスタントバッファ(スロット b0)
	// 今回の場合はカメラビュー行列や射影行列が入る想定
	D3D12_DESCRIPTOR_RANGE r0{};
	r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	r0.NumDescriptors = 1;
	r0.BaseShaderRegister = 0;
	r0.RegisterSpace = 0;
	r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//コンスタントバッファ(スロット b1)
	// 今回の場合はポリゴンのワールド行列や色が入る想定
	D3D12_DESCRIPTOR_RANGE r1{};
	r1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	r1.NumDescriptors = 1;
	r1.BaseShaderRegister = 1;
	r1.RegisterSpace = 0;
	r1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	constexpr auto paramNum = 2;
	D3D12_ROOT_PARAMETER rootParameters[paramNum]{};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[0].DescriptorTable.pDescriptorRanges = &r0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &r1;

	//今回は特にリソースを利用せず空で用意
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDessc{};
	rootSignatureDessc.NumParameters = paramNum;
	rootSignatureDessc.pParameters = rootParameters;
	rootSignatureDessc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートシグネチャのシリアライズ
	ID3DBlob* signature{};
	ID3DBlob* error{};
	auto res = D3D12SerializeRootSignature(
		&rootSignatureDessc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature,
		&error);

	bool success = SUCCEEDED(res);
	if (!success) {
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ルートシグネチャのシリアライズに失敗\0");
	}
	else
	{
		//ルートシグネチャの生成
		res = Device::instance().get()->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature_));

		success &= SUCCEEDED(res);
		if (!success) {
			assert(false && "ルートシグネチャの生成に失敗\0");
		}
	}
	if (error) {
		error->Release();
	}

	if (signature) {
		signature->Release();
	}


	return success;
}

ID3D12RootSignature* RootSignature::get() const noexcept
{
	if (!rootSignature_) {
		assert(false && "ルートシグネチャが生成されていません");
	}
	return rootSignature_;
}
