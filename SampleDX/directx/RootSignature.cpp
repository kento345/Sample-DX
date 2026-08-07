#include "RootSignature.h"
#include<cassert>

RootSignature::~RootSignature()
{
	//ルートシグネーチャの解放
	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}
}

bool RootSignature::create(const Device& device) noexcept
{
	//描画に必要なリソースをシェーダに伝える
	//今回は特にリソースを利用せず空で用意
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDessc{};
	rootSignatureDessc.NumParameters = 0;
	rootSignatureDessc.pParameters = nullptr;
	rootSignatureDessc.NumStaticSamplers = 0;
	rootSignatureDessc.pStaticSamplers = nullptr;
	rootSignatureDessc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートシグネチャのシリアライズ
	ID3DBlob* signature{};
	auto res = D3D12SerializeRootSignature(
		&rootSignatureDessc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature,
		nullptr);

	bool success = SUCCEEDED(res);
	if (!success) {
		assert(false && "ルートシグネチャのシリアライズに失敗\0");
	}
	else
	{
		//ルートシグネチャの生成
		res = device.get()->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature_));

		success &= SUCCEEDED(res);
		if (!success) {
			assert(false && "ルートシグネチャの生成に失敗\0");
		}
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
