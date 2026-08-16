#pragma once

#include<DirectXMath.h>

class Object final
{
public:
	struct ConstBufferData
	{
		DirectX::XMMATRIX world_{}; //ワールド行列
		DirectX::XMFLOAT4 color_{};	//カラー(RGBA)
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object() = default;

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void update() noexcept;

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] DirectX::XMMATRIX world()const noexcept;

	/// <summary>
	/// カラーの取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] DirectX::XMFLOAT4 color()const noexcept;

private:
	DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();				  //ワールド行列
	DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //カラー(RGBA)

	float move_{}; //移動速度
};

