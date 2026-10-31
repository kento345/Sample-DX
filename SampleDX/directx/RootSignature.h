#pragma once

#include"Device.h"
#include<wrl.h>

struct Drae3DRootParameters {
	Drae3DRootParameters() {
		//コンスタントバッファ(スロット b0)
	// 今回の場合はカメラビュー行列や射影行列が入る想定
		r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		r0.NumDescriptors = 1;
		r0.BaseShaderRegister = 0;
		r0.RegisterSpace = 0;
		r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// コンスタントバッファ( スロット b1 )
		// 今回の場合はポリゴンのワールド行列や色が入る想定
		r1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		r1.NumDescriptors = 1;
		r1.BaseShaderRegister = 1;  // スロット番号が 1 に変更
		r1.RegisterSpace = 0;
		r1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//ルートパラメータの設定
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // 頂点シェーダーのみで利用する
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[0].DescriptorTable.pDescriptorRanges = &r0;
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // 全てのシェーダーで利用する
		rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[1].DescriptorTable.pDescriptorRanges = &r1;


		// ルートシグネチャの設定
		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	D3D12_DESCRIPTOR_RANGE r0{};
	D3D12_DESCRIPTOR_RANGE r1{};
	D3D12_ROOT_PARAMETER rootParameters[2]{};
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
};
//-----------
// ここから変更
//-----------


/*struct DrawScreenTextureRootParameters {
	DrawScreenTextureRootParameters() {
		//コンスタントバッファ(スロット b0)
	// 今回の場合はカメラビュー行列や射影行列が入る想定
		r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		r0.NumDescriptors = 1;
		r0.BaseShaderRegister = 0;
		r0.RegisterSpace = 0;
		r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//スタティックサンプラ
		samplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].MipLODBias = 0;
		samplers[0].MaxAnisotropy = 0;
		samplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplers[0].BorderColor = 

		//ルートパラメータの設定
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // 頂点シェーダーのみで利用する
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[0].DescriptorTable.pDescriptorRanges = &r0;
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // 全てのシェーダーで利用する
		rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[1].DescriptorTable.pDescriptorRanges = &r1;


		// ルートシグネチャの設定
		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	D3D12_DESCRIPTOR_RANGE r0{};
	D3D12_STATIC_SAMPLER_DESC samplers[1]{};
	D3D12_ROOT_PARAMETER rootParameters[1]{};
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
};*/


class RootSignature final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RootSignature() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~RootSignature() = default;

	/// <summary>
	/// ルートシグネチャを作成する
	/// </summary>
	/// <param name="devicenoe"></param>
	/// <returns></returns>
	[[nodiscard]] bool create() noexcept;

	/// <summary>
	/// ルートシグネチャを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3D12RootSignature* get() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_{};  /// ルートシグネチャ
};

