#pragma once

#include<DirectXMath.h>

class GameObject 
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
	GameObject() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameObject() = default;

	virtual void initialize(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT4 color)noexcept;

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	virtual void update() noexcept;

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

protected:
	DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();				  //ワールド行列
	DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //カラー(RGBA)

	float move_{}; //移動速度
	DirectX::XMFLOAT3 position_{};
};

