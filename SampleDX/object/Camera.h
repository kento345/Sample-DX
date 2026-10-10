#pragma once

#include"Object.h"
#include<DirectXMath.h>
namespace game {

	class Camera final : public Object
	{
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Camera() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Camera() = default;

	public:

		/// <summary>
		/// カメラの初期化
		/// </summary>
		void initialize()noexcept;

		/// <summary>
		/// カメラを更新
		/// </summary>
		void update()noexcept;

	public:

		/// <summary>
		/// 描画バッファの作成
		/// </summary>
		virtual void createDrawBuffer()noexcept override;

		/// <summary>
		/// 描画バッファの更新
		/// </summary>
		virtual void updateDrawBuffer()noexcept override;

	public:
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
}


