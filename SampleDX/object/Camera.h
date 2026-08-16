#pragma once

#include<DirectXMath.h>

class Camera final
{
public:

	/// <summary>
	/// コンスタントバッファ用データ構造体
	/// </summary>
	struct ConstBufferData
	{
		DirectX::XMMATRIX view_{};		 //ビュー行列
		DirectX::XMMATRIX projection_{}; //射影行列
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera() = default;

public:

	/// <summary>
	/// カメラの初期化
	/// </summary>
	void initialize()noexcept;

	/// <summary>
	/// カメラを更新
	/// </summary>
	void update()noexcept;

	/// <summary>
	/// カメラのビュー行列を取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV viewMatrix() const noexcept;

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	/// <returns></returns>
	[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV projection() const noexcept;

private:
	DirectX::XMMATRIX view_{};       //ビュー行列
	DirectX::XMMATRIX projection_{}; //射影行列

	//カメラのパラメータ
	DirectX::XMFLOAT3 position_{};	 //カメラの位置
	DirectX::XMFLOAT3 target_{};	 //カメラの注視点
	DirectX::XMFLOAT3 up_{};		 //カメラの上方向
};

