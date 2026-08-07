#pragma once
#include"Device.h"

class Shader final
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Shader() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Shader();

	/// <summary>
	/// シェーダの作成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	[[nodiscard]] bool create(const Device& device)noexcept;

	/// <summary>
	/// 頂点シェーダを取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3DBlob* vertexShader()const noexcept;

	/// <summary>
	/// ピクセルシェーダの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] ID3DBlob* pixelShader()const noexcept;

private:
	ID3DBlob* vertexShader_{}; //頂点シェーダ
	ID3DBlob* pixelShader_{};  //ピクセルシェーダ
};

